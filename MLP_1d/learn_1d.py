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
  nHiddenLayers = int(sys.argv[3])
  training_name = str(sys.argv[4])
else:
  print('Incorrect number of arguments')
  exit(1)

X, y, row = prep_1d.prepLearn("../../convertJson/parsed_200k_nvtx.json.gz", options)

# Controls number of hidden dimensions
n_input = len(X[0])
n_hidden = 50

# Fraction of data used for training
n_train = int(len(X) / 2)

weights = {
    'h1': tf.Variable(tf.random_normal([n_input, n_hidden])), # Weights
    'out': tf.Variable(tf.random_normal([n_hidden, 1])),
    'b1': tf.Variable(tf.random_normal([n_hidden])), # Bias terms
    'bout': tf.Variable(tf.random_normal([1]))
}

for i in range(nHiddenLayers):
  weights['h'+str(i+2)] = tf.Variable(tf.random_normal([n_hidden, n_hidden]))
  weights['b'+str(i+2)] = tf.Variable(tf.random_normal([n_hidden]))

def multilayer_perceptron(x, weights):
  layer_1 = tf.einsum('ij,jk->ik', x, weights['h1']) + weights['b1']
  layer_1 = tf.nn.sigmoid(layer_1)
  layer_prev = layer_1
  for i in range(nHiddenLayers):
    layer_n = tf.einsum('ij,jk->ik', layer_prev, weights['h'+str(i+2)]) + weights['b'+str(i+2)]
    layer_n = tf.nn.sigmoid(layer_n)
    layer_prev = layer_n
  layer_out = tf.einsum('ij,jk->ik', layer_prev, weights['out']) + weights['bout']
  layer_out = tf.nn.sigmoid(layer_out)
  return layer_out

def predictor(mat, weights):
  return multilayer_perceptron(mat, weights)

# Regular logistic loss
def loss(X, y, weights):
  preds = tf.squeeze(multilayer_perceptron(X, weights))
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
for iteration in range(nEpochs):
  cvalues = sess.run([train, objective])
  print("objective = " + str(cvalues[1]))

train_details = "Opts" + str(options) + "_Epochs" + str(nEpochs) + "_Layers" + str(nHiddenLayers)
save_name = "checkpoints/weights_" + train_details + ".ckpt"
save_path = saver.save(sess, save_name)

# Evaluate the model's predictions
with sess.as_default():
  preds = tf.squeeze(predictor(tf.constant(X[n_train:]),  weights))
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
