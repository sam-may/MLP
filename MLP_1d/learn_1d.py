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

import prep_1d

# Parse args
# Arg1 = input variable options  
# Arg2 = number of training epochs
# Arg3 = number of layers
# Arg4 = save name

if len(sys.argv) == 5:
  nEpochs = int(sys.argv[2])
  options = int(sys.argv[1])
  nLayers = int(sys.argv[3])
  training_name = str(sys.argv[4])
else:
  print('Incorrect number of arguments')
  exit(1)

X, y, row = prep_1d.prepLearn("../convertJson/parsed_100k_nvtx.json.gz", options)

# Controls number of hidden dimensions
n_input = len(X[0])
n_hidden_1 = 16
n_hidden_2 = 16
n_hidden_3 = 16

# Fraction of data used for training
n_train = int(len(X) / 2)

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
def loss(X, y, weights):
  preds = tf.squeeze(predictor(X, weights))
  cross_entropy_f = y * tf.log(tf.clip_by_value(preds,1e-10,1.0))
  cross_entropy_f1 = (1 - y) * tf.log(tf.clip_by_value(1 - preds,1e-10,1.0))
  mce = tf.reduce_mean(cross_entropy_f) + tf.reduce_mean(cross_entropy_f1)
  return -mce

optimizer = tf.train.AdamOptimizer(0.1)
objective = loss(tf.constant(X[:n_train]), y[:n_train], weights)
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

train_details = "Opts" + str(options) + "_Epochs" + str(nEpochs) + "_Layers" + str(nLayers)
save_name = "checkpoints/weights_" + train_details + ".ckpt"
save_path = saver.save(sess, save_name)

# Evaluate the model's predictions
with sess.as_default():
  preds = tf.squeeze(predictor(tf.constant(XXX[n_train:]),  weights))
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

n_test = len(varsToWrite[1])

# Save to txt file
fileName = 'checkpoints/output_MLP_' + training_name + '.txt'
file = open(fileName, 'w')

for i in range(n_test):
  for j in range(len(varsToWrite)):
    if j == 0:
      file.write("%d " % varsToWrite[j][i])
    elif j != len(varsToWrite) - 1:
      file.write("%.9f " % varsToWrite[j][i])
    else:
      file.write("%.9f\n" % varsToWrite[j][i])
