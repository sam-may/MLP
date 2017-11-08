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
from math import atan2, pi

X = [] # Per-row features
XX = [] # Per-instance features
XXX = [] # Per-row features concatenated to per-instance features
y = [] # label
re = [] # relIso field used for baseline predictor

### Parse json-formatted data
def parseData(fname):
  for l in gzip.open(fname):
    yield eval(l)

#############################################
### Functions to create summary variables ###
#############################################

def calcRAlpha(lepVec, pfCandVec):
  phi = pfCandVec[1] - lepVec[1]
  if abs(phi) > pi:
    phi = pfCandVec[1] + lepVec[1]
  eta = pfCandVec[0] - lepVec[0]
  r = ((phi**2)+(eta**2))**(0.5)
  alpha = atan2(eta, phi)
  return r, alpha

def calcGridLocation(nR, nAlpha, r, alpha): # places r, alpha coordinates into corresponding grid-bin
  rBins = numpy.linspace(0, 1, nR+1)
  alphaBins = numpy.linspace(-pi, pi, nAlpha+1)

  rIdx = -1 
  for i in range(nR):
    if r <= rBins[i+1]:
      rIdx = i
      break

  alphaIdx = -1
  for i in range(nAlpha):
    if alpha <= alphaBins[i+1]:
      alphaIdx = i
      break

  return rIdx, alphaIdx

def candIsLepton(r, pfCandVec):
  return (r < 0.05 and (pfCandVec[4] or pfCandVec[5]))

def calcCandIdx(pfCandVec, nSumVars):
  for j in range(nSumVars):
    if pfCandVec[j+4]:
      return j
  return -1 

def calcSummaryVariables(nR, nAlpha, lepVec, pfCandMatrix):
  nSumVars = 6
  image = numpy.zeros((nR, nAlpha, nSumVars)) # represent summary variables as nR x nAlpha image with 6 "color" values for each of 6 summary variables
  for i in range(len(pfCandMatrix)):
    pfCandVec = pfCandMatrix[i]
    r, alpha = calcRAlpha(lepVec, pfCandVec)
    rIdx, alphaIdx = calcGridLocation(nR, nAlpha, r, alpha)

    if candIsLepton(r, pfCandVec): # the lepton itself is included in the pf cand list, so we skip it when we encounter it
      continue

    # Now determine the type of pf candidate, and add its energy to the corresponding "color" in the corresponding bin
    candIdx = calcCandIdx(pfCandVec, nSumVars)
    image[rIdx][alphaIdx][candIdx] += pfCandVec[2]
  return image

#########################
### Summary Variables ### 
#########################
     
nSumVars = 6 	# the 6 summary variables for each grid slice are the energy sums for the 6 types of pf candidates:
	     	# electrons, muons, charged hadrons, neutral EM particles, neutral hadronic particles, HF EM, and HF hadrons
# Granularity of grid
nR = 20
nAlpha = 20 	# set nAlpha = 1 for a 1-d grid in increasing radii (i.e. annului)


#for d in parseData("/home/jmcauley/datasets/leptons/parsed_50000entries.json.gz"):
#for d in parseData("parsed_IsoML.json.gz"):
#for d in parseData("parsed_50000entries.json.gz"):


###############################################################################################
### For each event, grab lepton vector and summary variable "image" (nR x nAlpha x 6 array) ###
###############################################################################################

for d in parseData("../parsed_100k.json.gz"):
  if d['lepton_flavor'] == 0: # To skip either muons or electrons
    continue
  # TODO: Not sure if the feature encoding the number of instances should be represented differently or is of any use?
  x = [1,1.0/len(d['X'])] + d['lepVec'] # Per-row feature and constant feature
  xxx = calcSummaryVariables(nR, nAlpha, d['lepVec'], d['X']) 

  X.append(numpy.array(x, dtype = numpy.float32))
  XX.append(numpy.array(xxx, dtype = numpy.float32))
  re.append(d['lepton_relIso03EA'])
  y.append(d['lepton_isFromW'])

  if (len(y) % 1000 == 0 and len(y)):
    print(len(y))

###########################################
### Remainder of file is left unchanged ###
###########################################

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
for iteration in range(500):
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
  for i in range(500):
    tpr, fpr = testError(y, pred, i * 1.0 / 500)
    y_pred.append(tpr)
    x_pred.append(fpr)
    tpr, fpr = testError(y, pred_base, - i * 1.0 / 500)
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
