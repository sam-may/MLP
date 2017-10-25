import gzip
import numpy

### Parse json-formatted data
def parseData(fname):
  for l in gzip.open(fname):
    yield eval(l)

def prepLearn():
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

  return XXX, y

def deltaR(phi1, eta1, phi2, eta2):
    dphi = math.acos(math.cos(phi1-phi2))
    deta = eta1-eta2
    return math.sqrt( dphi**2 + deta**2)

def calcPfEnergy(pfCands, lepVec):
  enIn03 = 0
  enOut03 = 0

  nCands = len(pfCands)
  for i in range(nCands):
    dR = deltaR(lepVec[1], lepVec[0], pfCands[i][1], pfCands[i][0])
    if dR >= 0.3:
      enOut03 += pfCands[i][2]
    else:
      if pfCands[i][5] == 1 and dR < 0.05: # check if pf cand is lepton itself
        contine # don't include
      else:
        enIn03 += pfCands[i][2]

  return enIn03, enOut03, nCands

def prepEval():
  X = [] # Per-row features
  XX = [] # Per-instance features
  XXX = [] # Per-row features concatenated to per-instance features
  y = [] # label
  re = [] # relIso field used for baseline predictor

  pt = []
  eta = []
  phi = []
  pf_energyIn03 = []
  pf_energyOut03 = []
  nCands = []
  nVtx = []
  ip3d = []

  vars = numpy.array([pt, eta, phi, pf_energyIn03, pf_energyOut03, nCands, nVtx, ip3d])

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

    lepVec = d['lepVec']
    pfCands = d['X']

    vars[0].append(lepVec[2]) # pt
    vars[1].append(lepVec[0]) # eta  
    vars[2].append(lepVec[1]) # phi

    enIn03, enOut03, nCands = calcPfEnergy(pfCands, lepVec)

    vars[3].append(enIn03)
    vars[4].append(enOut03)
    vars[5].append(nCands)
    vars[6].append(d['nvtx'])
    vars[7].append(lepVec[10]) # ip3d

    if (len(y) % 1000 == 0 and len(y)):
      print(len(y))

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

  return XXX, y, re, vars

