import gzip
import numpy

import math

### Parse json-formatted data
def parseData(fname):
  for l in gzip.open(fname):
    yield eval(l)

def deltaR(phi1, eta1, phi2, eta2):
    dphi = math.acos(math.cos(phi1-phi2))
    deta = eta1-eta2
    return math.sqrt( dphi**2 + deta**2)

def prepLearn(jsonFile, options=0):
  # options = 11: 11-variable lepton vector 
  # options = 12: 11-variable lepton vector + nvtx
  # options = 13: 11-variable lepton vector + nvtx + 11(12?)-var BDT output 

  X = [] # Per-row features
  y = [] # label
  re = [] # relIso field used for baseline predictor
  row = []

  for d in parseData(jsonFile):
    if d['lepton_flavor'] == 0: # To skip either muons or electrons
      continue
    # TODO: Not sure if the feature encoding the number of instances should be represented differently or is of any use?
    x = [1,1.0/len(d['X'])] + d['lepVec'] # Per-row feature and constant features
    X.append(numpy.array(x, dtype = numpy.float32))
    re.append(d['lepton_relIso03EA'])
    y.append(d['lepton_isFromW'])
    row.append(d['Row'])
    if (len(y) % 1000 == 0 and len(y)):
      print(len(y))
      break #FIXME

  X = numpy.array(X)
  y = numpy.array(y, dtype = numpy.float32)

  return X, y, row
