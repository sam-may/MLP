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
        continue # don't include
      else:
        enIn03 += pfCands[i][2]

  return enIn03, enOut03, nCands

def makePfCandVec(N, lepVec, pfCands): # returns vector of pT, deltaR, and type of the N highest energy N pf cands 
  nCands = len(pfCands)
  for i in range(N):
    if i < nCands:
      lepVec.append(pfCands[i][2]) # pT
      lepVec.append(deltaR(lepVec[1], lepVec[0], pfCands[i][1], pfCands[i][0]))
      for j in range(7):
          lepVec.append(pfCands[i][j+4])
    else:
      for j in range(9):
          lepVec.append(0)
  return lepVec 

def createLepVec(d, options):
  lepVec = d['lepVec']
  pfCands = d['X']
  if options == 5:
    lepVec = makePfCandVec(5, lepVec, pfCands)
  if options == 6: 
    lepVec = makePfCandVec(1, lepVec, pfCands)
  return lepVec

def prepLearn(jsonFile, options=0):
  # options = 0: Julian's original MLP
  # options = 5: Julian's original MLP + pT, DeltaR, and type of highest 5 pT pf cands

  X = [] # Per-row features
  XX = [] # Per-instance features
  XXX = [] # Per-row features concatenated to per-instance features
  y = [] # label
  re = [] # relIso field used for baseline predictor

  for d in parseData(jsonFile):
    if d['lepton_flavor'] == 0: # To skip either muons or electrons
      continue
    # TODO: Not sure if the feature encoding the number of instances should be represented differently or is of any use?
    lepVec = createLepVec(d, options)
    x = [1,1.0/len(d['X'])] + lepVec # Per-row feature and constant features
    xx = d['X'] # Matrix of per-instance features
    if options == 6:
      xx = numpy.array(xx, dtype = numpy.float32)
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

def prepEval(jsonFile, options=0): 
  X = [] # Per-row features
  XX = [] # Per-instance features
  XXX = [] # Per-row features concatenated to per-instance features
  y = [] # label
  re = [] # relIso field used for baseline predictor
  row = [] # event identifier

  pt = []
  eta = []
  phi = []
  pf_energyIn03 = []
  pf_energyOut03 = []
  nCands = []
  nVtx = []
  ip3d = []

  vars = numpy.array([pt, eta, phi, pf_energyIn03, pf_energyOut03, nCands, nVtx, ip3d])

  for d in parseData(jsonFile):
    if d['lepton_flavor'] == 0: # To skip either muons or electrons
      continue
    # TODO: Not sure if the feature encoding the number of instances should be represented differently or is of any use?
    lepVec = createLepVec(d, options) 
    x = [1,1.0/len(d['X'])] + lepVec # Per-row feature and constant feature
    xx = d['X'] # Matrix of per-instance features
    XX.append(numpy.array(xx, dtype = numpy.float32))
    X.append(numpy.array(x, dtype = numpy.float32))
    re.append(d['lepton_relIso03EA'])
    y.append(d['lepton_isFromW'])
    row.append(d['Row'])

    lepVec = d['lepVec']
    pfCands = d['X']

    pt.append(lepVec[2])
    eta.append(lepVec[0])
    phi.append(lepVec[1])

    enIn03, enOut03, nPfCands = calcPfEnergy(pfCands, lepVec)

    pf_energyIn03.append(enIn03)
    pf_energyOut03.append(enOut03)
    nCands.append(nPfCands)
    nVtx.append(d['nvtx'])
    ip3d.append(lepVec[10])

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

  vars = numpy.array([pt, eta, phi, pf_energyIn03, pf_energyOut03, nCands, nVtx, ip3d])

  return XXX, y, row, re, vars

