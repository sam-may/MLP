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
import sys

import prep

# Parse args
# Arg1 = options (0 = Julian's original MLP, 5 = add pT, deltaR, and type of top 5 pf Cands
# Arg2 = number of training epochs

if len(sys.argv) == 1:
  nEpochs = 1000
  options = 0
elif len(sys.argv) == 3:
  nEpochs = int(sys.argv[2])
  options = int(sys.argv[1])
else:
  print('You did not supply the correct number of arguments (0 or 2)') 
  sys.exit(0)

XXX, y = prep.prepLearn("../convertJson/parsed_100k_nvtx.json.gz", options)

# Controls number of hidden dimensions
n_input = len(XXX[0][0])
n_hidden_1 = 16
n_hidden_2 = 16
n_hidden_3 = 16

# Fraction of data used for training
n_train = int(len(XXX) / 2)

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
sess = tf.Session()
sess.run(init)

# Run several iterations of gradient descent
for iteration in range(10000):
  cvalues = sess.run([train, objective])
  print("objective = " + str(cvalues[1]))

save_path = saver.save(sess, "weights.ckpt")

# Evaluate the model's predictions
#with sess.as_default():
#  preds = tf.squeeze(predictor(tf.constant(XXX[n_train:]),  weights))
#  y_pred = preds.eval()

sess.close()

