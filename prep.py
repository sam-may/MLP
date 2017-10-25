import gzip
import numpy

### Parse json-formatted data
def parseData(fname):
  for l in gzip.open(fname):
    yield eval(l)

def prepData():
  X = [] # Per-row features
  XX = [] # Per-instance features
  XXX = [] # Per-row features concatenated to per-instance features
  y = [] # label
  re = [] # relIso field used for baseline predictor

  for d in parseData("../parsed_100k.json.gz"):
    if d['lepton_flavor'] == 0: # To skip either muons or electrons
      continue
    # TODO: Not sure if the feature encoding the number of instances should be represented differently or is of any use?
    lepVec = d['lepVec']
    x = [1,1.0/len(d['X'])] + lepVec # Per-row feature and constant feature
    xx = d['X'] # Matrix of per-instance features
    XX.append(numpy.array(xx, dtype = numpy.float32))
    X.append(numpy.array(x, dtype = numpy.float32))
    re.append(d['lepton_relIso03EA'])
    y.append(d['lepton_isFromW'])
    if (len(y) % 1000 == 0 and len(y)):
      print(len(y))
      break

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

  return XXX, y, re

