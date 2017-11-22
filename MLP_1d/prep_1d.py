import gzip
import numpy

from math import atan2, pi
import math

### Parse json-formatted data
def parseData(fname):
  for l in gzip.open(fname):
    yield eval(l)

def deltaR(phi1, eta1, phi2, eta2):
    dphi = math.acos(math.cos(phi1-phi2))
    deta = eta1-eta2
    return math.sqrt( dphi**2 + deta**2)

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
  nSumVars = 1
  image = numpy.zeros((nR, nAlpha, nSumVars)) # represent summary variables as nR x nAlpha image with 6 "color" values for each of 6 summary variables
  for i in range(len(pfCandMatrix)):
    pfCandVec = pfCandMatrix[i]
    r, alpha = calcRAlpha(lepVec, pfCandVec)
    rIdx, alphaIdx = calcGridLocation(nR, nAlpha, r, alpha)

    if candIsLepton(r, pfCandVec): # the lepton itself is included in the pf cand list, so we skip it when we encounter it
      continue

    # Now determine the type of pf candidate, and add its energy to the corresponding "color" in the corresponding bin
    candIdx = calcCandIdx(pfCandVec, nSumVars)
    #image[rIdx][alphaIdx][candIdx] += pfCandVec[2]
    image[rIdx][alphaIdx][0] += pfCandVec[2]
  return image

#########################
### Summary Variables ### 
#########################

#nSumVars = 7    # the 7 summary variables for each grid slice are the energy sums for the 6 types of pf candidates:
                # electrons, muons, charged hadrons, neutral EM particles, neutral hadronic particles, HF EM, and HF hadrons
# Granularity of grid
#nR = 2
#nAlpha = 1     # set nAlpha = 1 for a 1-d grid in increasing radii (i.e. annului)


def prepLearn(jsonFile, options=0):
  # options = 11: 11-variable lepton vector 
  # options = 12: 11-variable lepton vector + nvtx
  # options = 13: 11-variable lepton vector + nvtx + 11(12?)-var BDT output 

  X = [] # Per-row features
  y = [] # label
  re = [] # relIso field used for baseline predictor
  row = []

  nTrainSig = 0
  nTrainBkg = 0

  nR = 10
  nAlpha = 1

  for d in parseData(jsonFile):
    if d['lepton_flavor'] == 0: # To skip either muons or electrons
      continue
    # TODO: Not sure if the feature encoding the number of instances should be represented differently or is of any use?
    if options == 11:
      x = [1,1.0/len(d['X'])] + d['lepVec'] # Per-row feature and constant features
    elif options == 12:
      x = [1,1.0/len(d['X'])] + d['lepVec'] + [d['nvtx']]
    elif options == 14:
      xxx = calcSummaryVariables(nR, nAlpha, d['lepVec'], d['X'])
      xxx = numpy.ravel(xxx)
      x = [1,1.0/len(d['X'])] + d['lepVec'] + [d['nvtx']]
      for var in xxx:
        x += [var]
    X.append(numpy.array(x, dtype = numpy.float32))
    re.append(d['lepton_relIso03EA'])
    y.append(d['lepton_isFromW'])
    row.append(d['Row'])

    if d['lepton_isFromW']:
      nTrainSig += 1
    else:
      nTrainBkg += 1

    if (len(y) % 1000 == 0 and len(y)):
      print(len(y))

  X = numpy.array(X)
  y = numpy.array(y, dtype = numpy.float32)

  print("Signal training events: %d" % nTrainSig)
  print("Bkg training evnets: %d" %nTrainBkg)

  return X, y, row
