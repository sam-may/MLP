import tensorflow as tf # Needs python 3.5.2
import gzip
import matplotlib.pyplot as plt

# Standard imports
import numpy
import urllib
import random
from math import exp
from math import log
from math import pi
from math import atan2

import sys

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

def calcSummaryVariables(nSumVars, nR, nAlpha, lepVec, pfCandMatrix):
  image = numpy.zeros((nR, nAlpha, nSumVars)) # represent summary variables as nR x nAlpha image with 6 "color" values for each of 6 summary variables
  for i in range(len(pfCandMatrix)):
    pfCandVec = pfCandMatrix[i]
    r, alpha = calcRAlpha(lepVec, pfCandVec)
    rIdx, alphaIdx = calcGridLocation(nR, nAlpha, r, alpha)
    if candIsLepton(r, pfCandVec): # the lepton itself is included in the pf cand list, so we skip it when we encounter it
      continue
    # Now determine the type of pf candidate, and add its energy to the corresponding "color" in the corresponding bin
    candIdx = -1
    if nSumVars == 1:
      candIdx = 0
    else:
      candIdx = calcCandIdx(pfCandVec, nSumVars)
    image[rIdx][alphaIdx][candIdx] += pfCandVec[2]
  return image

#############################################
### Read the data                         ###
#############################################

X1 = [] # Per-row features
X2 = [] # Per-instance features

y = [] # label
re = [] # relIso field used for baseline predictor
row = [] # eventID for writing to txt file


nR = int(sys.argv[1])
nAlpha = int(sys.argv[2])
nSumVars = int(sys.argv[3])

### Parse json-formatted data
def parseData(fname):
  for l in gzip.open(fname):
    yield eval(l)

for d in parseData("/home/users/sjmay/ML/convertJson/parsed_1000k_nvtx.json.gz"):
  if d['lepton_flavor'] != 1: # To skip either muons or electrons
    continue
  x1 = [1] + d['lepVec'] + [d['nvtx']]
  x2 = list(calcSummaryVariables(nR,nAlpha,nSumVars, d['lepVec'], d['X']).flatten())
  X1.append(numpy.array(x1, dtype = numpy.float32))
  X2.append(numpy.array(x2, dtype = numpy.float32))
  re.append(d['lepton_relIso03EA'])
  y.append(d['lepton_isFromW'])
  row.append(d['Row'])
  if (len(y) % 1000 == 0 and len(y)):
    print(len(y))
  if len(y) >= 400000:
    break

### A few quick lines to write the parsed data to a file to read it more quickly without re-parsing it

# f = open("muons400k_8.json", 'w')
# for i in range(len(y)):
#   f.write(str({"X1": list(X1[i]), "X2": list(X2[i]), "y": y[i]}) + '\n')

# f.close()

# for l in gzip.open("muons400k_8.json.gz", 'r'):
#   d = eval(l)
#   X1.append(d["X1"])
#   X2.append(d["X2"])
#   y.append(d["y"])

X1 = numpy.array(X1)
X2 = numpy.array(X2)
y = numpy.array(y, dtype = numpy.float32)

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
  #print("N_lepton_____isFromW_passing:" + str(N_lepton_____isFromW_passing))
  #print("N_lepton_not_isFromW_passing:" + str(N_lepton_not_isFromW_passing))
  #print("N_lepton_____isFromW_failing:" + str(N_lepton_____isFromW_failing))
  #print("N_lepton_not_isFromW_failing:" + str(N_lepton_not_isFromW_failing))
  # True positive rate and false positive rate
  tpr = float( N_lepton_____isFromW_passing ) / float( N_lepton_____isFromW_passing + N_lepton_____isFromW_failing )
  fpr = float( N_lepton_not_isFromW_passing ) / float( N_lepton_not_isFromW_passing + N_lepton_not_isFromW_failing )
  #print("fraction of     isFromW lepton passing " + str(tpr))
  #print("fraction of not isFromW lepton passing " + str(fpr))
  return tpr, fpr

# Fraction of data used for training
n_test = 50000
n_train = len(y) - n_test

# Run the baseline method. Note the negation due to interpretation of being above vs. below a threshold
(tpr_cut, fpr_cut) = testError(y[n_train:], list(-numpy.array(re))[n_train:], -0.06)

# Controls number of hidden dimensions
n_input1 = len(X1[0])
n_input2 = len(X2[0])
n_hidden_1 = 10
n_hidden_2 = 8
n_hidden_3 = 6

n_hidden_sv_1 = int((2*n_input2)/3)
n_hidden_sv_2 = int(n_hidden_sv_1/2)
n_hidden_sv_3 = int(n_hidden_sv_2/2)

weights = {
    'h11': tf.Variable(tf.random_normal([n_input1, n_hidden_1])), # Weights
    'h12': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
    'h13': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_3])),
    'h21': tf.Variable(tf.random_normal([n_input2, n_hidden_sv_1])), # Weights
    'h22': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_sv_2])),
    'h23': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_sv_3])),
    'h31': tf.Variable(tf.random_normal([n_hidden_3 + n_hidden_sv_3, n_hidden_1])), # Weights
    'h32': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
    'h33': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_3])),
    'out': tf.Variable(tf.random_normal([n_hidden_3, 1])),
    'b11': tf.Variable(tf.random_normal([n_hidden_1])), # Bias terms
    'b12': tf.Variable(tf.random_normal([n_hidden_2])),
    'b13': tf.Variable(tf.random_normal([n_hidden_3])),
    'b21': tf.Variable(tf.random_normal([n_hidden_sv_1])), # Bias terms
    'b22': tf.Variable(tf.random_normal([n_hidden_sv_2])),
    'b23': tf.Variable(tf.random_normal([n_hidden_sv_3])),
    'b31': tf.Variable(tf.random_normal([n_hidden_1])), # Bias terms
    'b32': tf.Variable(tf.random_normal([n_hidden_2])),
    'b33': tf.Variable(tf.random_normal([n_hidden_3])),
    'bout': tf.Variable(tf.random_normal([1]))
}

# MLP applied to the original lepton
def multilayer_perceptron1(x1, weights):
  layer_1 = tf.einsum('ij,jk->ik', x1, weights['h11']) + weights['b11']
  layer_1 = tf.nn.sigmoid(layer_1)
  layer_2 = tf.einsum('ij,jk->ik', layer_1, weights['h12']) + weights['b12']
  layer_2 = tf.nn.sigmoid(layer_2)
  layer_3 = tf.einsum('ij,jk->ik', layer_2, weights['h13']) + weights['b13']
  layer_3 = tf.nn.sigmoid(layer_3)
  return layer_3

# MLP applied to the radial representation
def multilayer_perceptron2(x2, weights):
  layer_1 = tf.einsum('ij,jk->ik', x2, weights['h21']) + weights['b21']
  layer_1 = tf.nn.sigmoid(layer_1)
  layer_2 = tf.einsum('ij,jk->ik', layer_1, weights['h22']) + weights['b22']
  layer_2 = tf.nn.sigmoid(layer_2)
  layer_3 = tf.einsum('ij,jk->ik', layer_2, weights['h23']) + weights['b23']
  layer_3 = tf.nn.sigmoid(layer_3)
  return layer_3

# MLP applied to the concatenation of the above two representations
def multilayer_perceptron3(l, weights):
  layer_1 = tf.einsum('ij,jk->ik', l, weights['h31']) + weights['b31']
  layer_1 = tf.nn.sigmoid(layer_1)
  layer_2 = tf.einsum('ij,jk->ik', layer_1, weights['h32']) + weights['b32']
  layer_2 = tf.nn.sigmoid(layer_2)
  layer_3 = tf.einsum('ij,jk->ik', layer_2, weights['h33']) + weights['b33']
  layer_3 = tf.nn.sigmoid(layer_3)
  layer_out = tf.einsum('ij,jk->ik', layer_3, weights['out']) + weights['bout']
  layer_out = tf.nn.sigmoid(layer_out)
  return layer_out

def predictor(mat1, mat2, weights):
  l1 = multilayer_perceptron1(mat1, weights)
  l2 = multilayer_perceptron2(mat2, weights)
  return multilayer_perceptron3(tf.concat([l1,l2],1), weights)

# Regular logistic loss
def loss(X1, X2, y, weights):
  preds = tf.squeeze(predictor(X1, X2, weights))
  cross_entropy_f = y * tf.log(tf.clip_by_value(preds,1e-10,1.0))
  cross_entropy_f1 = (1 - y) * tf.log(tf.clip_by_value(1 - preds,1e-10,1.0))
  mce = tf.reduce_mean(cross_entropy_f) + tf.reduce_mean(cross_entropy_f1)
  return -mce

optimizer = tf.train.AdamOptimizer(0.01)
objective = loss(tf.constant(X1[:n_train]), tf.constant(X2[:n_train]), y[:n_train], weights)
train = optimizer.minimize(objective)
init = tf.global_variables_initializer()

# Create a new optimization session
sess = tf.Session()
sess.run(init)

# Run several iterations of gradient descent
for iteration in range(10000):
  cvalues = sess.run([train, objective])
  print("objective = " + str(cvalues[1]))

# Evaluate the model's predictions
with sess.as_default():
  preds = tf.squeeze(predictor(tf.constant(X1[n_train:]), tf.constant(X2[n_train:]), weights))
  y_pred = preds.eval()

sess.close()

varsToWrite = []
varsToWrite.append(row)
varsToWrite.append(y_pred)

# Keep only non-training portion
for i in range(len(varsToWrite)):
  if i == 1:
    continue # y_preds were only calculated for second half of data
  varsToWrite[i] = list(numpy.array(varsToWrite[i]))[n_train:]

#n_test = len(varsToWrite[1])

# Save to txt file
fileName = 'checkpoints/output_MLP_' + str(nR) + 'annuli_' + str(nAlpha) + 'alpha_' + str(nSumVars) + 'cands.txt'
file = open(fileName, 'w')

for i in range(n_test):
  for j in range(len(varsToWrite)):
    if j == 0:
      file.write("%d " % varsToWrite[j][i])
    elif j != len(varsToWrite) - 1:
      file.write("%.9f " % varsToWrite[j][i])
    else:
      file.write("%.9f\n" % varsToWrite[j][i])

def curve(pred_base, pred, y):
  x_base = []
  y_base = []
  x_pred = []
  y_pred = []
  nPoints = 100000
  for i in range(nPoints):
    tpr, fpr = testError(y, pred, i * 1.0 / nPoints)
    y_pred.append(tpr)
    x_pred.append(fpr)
    tpr, fpr = testError(y, pred_base, - i * 1.0 / nPoints)
    y_base.append(tpr)
    x_base.append(fpr)
  fig, ax = plt.subplots()
  #z = open("BDT_ROC.txt", 'r').readlines()
  #z = [xy.split() for xy in z]
  x_bdt = [float(x) for (x,y) in z]
  y_bdt = [float(y) for (x,y) in z]
  plt.xlim((0.001,1.0))
  plt.ylim((0.5,1.0))
  ax.set_xscale("log", nonposx='clip')
  ax.plot(x_base, y_base, 'y', lw=1, label='relIso')
  #ax.plot(x_bdt, y_bdt, 'g', lw=1, label='bdt')
  ax.plot(x_pred, y_pred, 'b', lw=2, label='mlp')
  #ax.plot()
  ax.legend()
  plt.ylabel("True positive rate")
  plt.xlabel("False positive rate")
  plt.savefig("plot_" + str(nR) + 'annuli_' + str(nAlpha) + 'alpha_' + str(nSumVars) + "cands.pdf")

curve(list(-numpy.array(re))[n_train:], y_pred, y[n_train:])
