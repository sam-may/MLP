import tensorflow as tf # Needs python 3.5.2
import gzip
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import json

# Standard imports
import numpy
import urllib
import random
from math import exp
from math import log

X = [] # Per-row features
XX = [] # Per-instance features
XXX = [] # Per-row features concatenated to per-instance features
y = [] # label
re = [] # relIso field used for baseline predictor

### Parse json-formatted data
def parseData(fname):
  for l in gzip.open(fname):
    yield eval(l)

index = 0
#for d in parseData("/home/jmcauley/datasets/leptons/parsed_50000entries.json.gz"):
#for d in parseData("parsed_IsoML.json.gz"):
#for d in parseData("parsed_50000entries.json.gz"):
for d in parseData("../parsed_100k.json.gz"):
  if d['lepton_flavor'] == 0: # To skip either muons or electrons
    continue
  # TODO: Not sure if the feature encoding the number of instances should be represented differently or is of any use?
  x = [1,1.0/len(d['X'])] + d['lepVec'] # Per-row feature and constant feature
  xx = d['X'] # Matrix of per-instance features
  XX.append(numpy.array(xx, dtype = numpy.float32))
  X.append(numpy.array(x, dtype = numpy.float32))
  re.append(d['lepton_relIso03EA'])
  y.append(d['lepton_isFromW'])
  if (len(y) % 1000 == 0 and len(y)):
    print(len(y))
    break
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

# Fraction of data used for training
n_train = int(len(XXX) / 100)

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
saver = tf.train.Saver()

# Create a new optimization session

with tf.Session() as sess:
    saver.restore(sess, "weights.ckpt")
    preds = tf.squeeze(predictor(tf.constant(XXX[n_train:]),  weights))
    y_pred = preds.eval()

print(y_pred)


# Evaluate the model's predictions
#with sess.as_default():
#  preds = tf.squeeze(predictor(tf.constant(XXX[n_train:]),  weights))
#  y_pred = preds.eval()

sess.close()

