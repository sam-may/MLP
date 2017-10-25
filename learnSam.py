import tensorflow as tf # Needs python 3.5.2
import gzip
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# Standard imports
import numpy
import urllib
import random
from math import exp
from math import log
import math

X = [] # Per-row features
XX = [] # Per-instance features
XXX = [] # Per-row features concatenated to per-instance features
y = [] # label
re = [] # relIso field used for baseline predictor

lepton_pt = []
lepton_phi = []
lepton_eta = []
nvtx = []

#pf_eta = []
#pf_phi = []
#pf_pt = [] 
pf_energyIn03 = []
pf_energyOut03 = []

row = []


### Parse json-formatted data
def parseData(fname):
  for l in gzip.open(fname):
    yield eval(l)

def deltaR(phi1, eta1, phi2, eta2):
    dphi = math.acos(math.cos(phi1-phi2))
    deta = eta1-eta2
    return math.sqrt( dphi**2 + deta**2)

index = 0
#for d in parseData("/home/jmcauley/datasets/leptons/parsed_50000entries.json.gz"):
#for d in parseData("parsed_IsoML.json.gz"):
#for d in parseData("parsed_50000entries.json.gz"):
#for d in parseData("parsed_100k.json.gz"):
for d in parseData("babymaker/jsons/output_100k_v0.0.1.json.gz"):
  if d['lepton_flavor'] == 0: # To skip either muons or electrons
    continue
  # TODO: Not sure if the feature encoding the number of instances should be represented differently or is of any use?

  # FIXME
  #lepVec = d['lepVec'][:7]
  lepVec = d['lepVec']
  x = [1,1.0/len(d['X'])] + lepVec # Per-row feature and constant feature
  xx = d['X'] # Matrix of per-instance features
  XX.append(numpy.array(xx, dtype = numpy.float32))
  X.append(numpy.array(x, dtype = numpy.float32))
  re.append(d['lepton_relIso03EA'])
  y.append(d['lepton_isFromW'])
  enIn = 0
  enOut = 0
  for i in range(len(d['X'])):
      dR = deltaR(d['lepVec'][1], d['lepVec'][0], d['X'][i][1], d['X'][i][0])   
      if dR < 0.3:
          if d['X'][i][5] == 1 and dR < 0.05: # this is the lepton in question, don't put it in the pf cand sum
            continue
          else:
            enIn += d['X'][i][2]
      else:
      #elif dR >= 0.3 and dR <= 1.0:
          enOut +=  d['X'][i][2]
  pf_energyIn03.append(enIn)
  pf_energyOut03.append(enOut)
  lepton_pt.append(d['lepVec'][2])
  lepton_phi.append(d['lepVec'][1])
  lepton_eta.append(d['lepVec'][0])
  row.append(d['Row'])
  if (len(y) % 1000 == 0 and len(y)):
    print(len(y))
  index += 1


# This code pads each row with zeros to have the same number of instances. This allows for efficient representation of the whole dataset as a 3-d tensor. It is fairly wasteful in terms of memory but is necessary for efficient computation
XXX = []
lengths = [len(x) for x in XX]
longest = max(lengths)

for x,xx in zip(X,XX):
  mat = numpy.concatenate([numpy.tile(x, (len(xx),1)), xx],axis=1)
  add = longest - len(xx)
  pad = numpy.pad(mat, [[0,add],[0,0]], 'constant')
  for i in range(len(xx),len(pad)):
    pad[i][0] = 0
  XXX.append(pad)

XXX = numpy.array(XXX)

y = numpy.array(y, dtype = numpy.float32)

# Controls number of hidden dimensions
n_input = len(XXX[0][0])
n_hidden_1 = 16
n_hidden_2 = 16
n_hidden_3 = 16

# Quick modification of Philip's function
def testError(labels, preds, thresh):
  N_lepton_____isFromW_passing = 0
  N_lepton_not_isFromW_passing = 0
  N_lepton_____isFromW_failing = 0
  N_lepton_not_isFromW_failing = 0
  for y,p in zip(labels, preds):
    if y > 0:
      if p > thresh:
        N_lepton_____isFromW_passing += 1
      else:
        N_lepton_____isFromW_failing += 1
    else:
      if p > thresh:
        N_lepton_not_isFromW_passing += 1
      else:
        N_lepton_not_isFromW_failing += 1
  print("N_lepton_____isFromW_passing:" + str(N_lepton_____isFromW_passing))
  print("N_lepton_not_isFromW_passing:" + str(N_lepton_not_isFromW_passing))
  print("N_lepton_____isFromW_failing:" + str(N_lepton_____isFromW_failing))
  print("N_lepton_not_isFromW_failing:" + str(N_lepton_not_isFromW_failing))
  # True positive rate and false positive rate
  tpr = float( N_lepton_____isFromW_passing ) / float( N_lepton_____isFromW_passing + N_lepton_____isFromW_failing )
  fpr = float( N_lepton_not_isFromW_passing ) / float( N_lepton_not_isFromW_passing + N_lepton_not_isFromW_failing )
  print("fraction of     isFromW lepton passing " + str(tpr))
  print("fraction of not isFromW lepton passing " + str(fpr))
  return tpr, fpr

# Fraction of data used for training
n_train = int(len(XXX) / 2)

# Run the baseline method. Note the negation due to interpretation of being above vs. below a threshold
testError(y[n_train:], list(-numpy.array(re))[n_train:], -0.1)

weights = {
    'h1': tf.Variable(tf.random_normal([n_input, n_hidden_1])), # Weights
    'h2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
    'h3': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_3])),
    'out': tf.Variable(tf.random_normal([n_hidden_3, 1])),
    'b1': tf.Variable(tf.random_normal([n_hidden_1])), # Bias terms
    'b2': tf.Variable(tf.random_normal([n_hidden_2])),
    'b3': tf.Variable(tf.random_normal([n_hidden_3])),
    'bout': tf.Variable(tf.random_normal([1]))
}

# MLP layers applied to individual instances
def multilayer_perceptron1(x, weights):
  layer_1 = tf.einsum('ijk,kl->ijl', x, weights['h1']) + weights['b1']
  layer_1 = tf.nn.sigmoid(layer_1)
  layer_2 = tf.einsum('ijk,kl->ijl', layer_1, weights['h2']) + weights['b2']
  layer_2 = tf.nn.sigmoid(layer_2)
  return layer_2

# MLP layers applied to aggregated instances
def multilayer_perceptron2(x, weights):
  layer_1 = tf.einsum('ik,kl->il', x, weights['h3']) + weights['b3']
  layer_1 = tf.nn.sigmoid(layer_1)
  layer_out = tf.einsum('ik,kl->il', layer_1, weights['out']) + weights['bout']
  layer_out = tf.nn.sigmoid(layer_out)
  return layer_out

# Combine both layers
def predictor(mat, weights):
  responses = multilayer_perceptron1(mat, weights)
  responses = tf.reduce_mean(responses, reduction_indices = 1, keep_dims = False)
  return multilayer_perceptron2(responses, weights)

# Regular logistic loss
def loss(XXX, y, weights):
  preds = tf.squeeze(predictor(XXX, weights))
  cross_entropy_f = y * tf.log(tf.clip_by_value(preds,1e-10,1.0))
  cross_entropy_f1 = (1 - y) * tf.log(tf.clip_by_value(1 - preds,1e-10,1.0))
  mce = tf.reduce_mean(cross_entropy_f) + tf.reduce_mean(cross_entropy_f1)
  return -mce

optimizer = tf.train.AdamOptimizer(0.1)
objective = loss(tf.constant(XXX[:n_train]), y[:n_train], weights)
train = optimizer.minimize(objective)
init = tf.global_variables_initializer()

# Create a new optimization session
sess = tf.Session()
sess.run(init)

# Run several iterations of gradient descent
for iteration in range(1000):
  cvalues = sess.run([train, objective])
  print("objective = " + str(cvalues[1]))

# Evaluate the model's predictions
with sess.as_default():
  preds = tf.squeeze(predictor(tf.constant(XXX[n_train:]),  weights))
  y_pred = preds.eval()

sess.close()

# Create a quick tpr vs. fpr plot
def curve(pred_base, pred, y):
  x_base = []
  y_base = []
  x_pred = []
  y_pred = []
  nThresholdVals = 500
  for i in range(nThresholdVals):
    tpr, fpr = testError(y, pred, i * 1.0 / nThresholdVals)
    y_pred.append(tpr)
    x_pred.append(fpr)
    tpr, fpr = testError(y, pred_base, - i * 1.0 / nThresholdVals)
    y_base.append(tpr)
    x_base.append(fpr)
  fig, ax = plt.subplots()
  ax.plot(x_base, y_base, 'y', lw=1, label='relIso')
  ax.plot(x_pred, y_pred, 'b', lw=2, label='mlp')
  ax.legend()
  plt.ylabel("True positive rate")
  plt.xlabel("False positive rate")
  plt.savefig("plot.pdf")

curve(list(-numpy.array(re))[n_train:], y_pred, y[n_train:])

plt.clf()
# Write floats to text file
discFileRI = open('discriminantValuesRelIso.txt', 'w')
discFileMLP = open('discriminantValuesMLPGPU.txt', 'w')
txtFiles = numpy.array([discFileRI, discFileMLP])
discriminants = numpy.array([list(-numpy.array(re))[n_train:], y_pred])
fromW = list(numpy.array(y))[n_train:]
pt = list(numpy.array(lepton_pt))[n_train:]
eta = list(numpy.array(lepton_eta))[n_train:]
phi = list(numpy.array(lepton_phi))[n_train:]
numVtx = list(numpy.array(nvtx))[n_train:]
row = list(numpy.array(row))[n_train:]

pf_energyIn03 = list(numpy.array(pf_energyIn03))[n_train:]
pf_energyOut03 = list(numpy.array(pf_energyOut03))[n_train:]

dataFile = open('eventData.txt', 'w')
for i in range(len(fromW)):
    dataFile.write("%d" % fromW[i])
    dataFile.write(" , ")
    dataFile.write("%.5f" % pt[i])
    dataFile.write(" , ")
    dataFile.write("%.5f" % eta[i])
    dataFile.write(" , ")
    dataFile.write("%.5f" % phi[i])
    dataFile.write(" , ")
    dataFile.write("%.5f" % pf_energyIn03[i])
    dataFile.write(" , ") 
    dataFile.write("%.5f" % pf_energyOut03[i])
    dataFile.write(" \n ")
dataFile.close()

idx1 = 0
for file in txtFiles:
    idx2 = 0
    for item in discriminants[idx1]:
        file.write("%.9f" % item) 
        #file.write(" , ")
        #file.write("%d" % fromW[idx2])
        #file.write(" , ")
        #file.write("%.3f" % pt[idx2])
        #file.write(" , ")
        #file.write("%.3f" % eta[idx2])
        #for i in range(maxPfCands):
        #    file.write("%.3f" % deltaR(phi[idx2], eta[idx2], pf_phi[idx2, i], pf_eta[idx2, i]))
        #    file.write(" , ")
        #    file.write("%.3f" % pf_pt[idx2, i])
        #    if i != maxPfCands - 1:
        #        file.write(" , ")
        #file.write(" , ")
        #file.write("%.3f" % nvtx[idx2])
        file.write("\n")
        idx2 += 1
    file.close()
    idx1 += 1

f = open('output_MLP.txt', 'w')
for r, y in zip(r, y_pred):
    f.write("{} {}\n".format(r, y))
f.close()


#for item in list(-numpy.array(re))[n_train:]:
#    discFileRI.write("%.3f" % item)
#    discFileRI.write("\n")
#discFileRI.close()

#discFileMLP = open('discriminantValuesMLP.txt', 'w')
#for item in y_pred:
#    discFileMLP.write("%.3f" % item)
#    discFileMLP.write("\n")
#discFileMLP.close()

# Make scatterplot of MLP floats vs. RelIso floats
#plt.figure(1)
#plt.scatter(list(-numpy.array(re))[n_train:],  y_pred)
#plt.savefig('scatterPlot.pdf')
