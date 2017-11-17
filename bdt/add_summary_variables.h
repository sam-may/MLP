//  .
// ..: P. Chang, philip@physics.ucsd.edu

// C++
#include <iostream>
#include <fstream>

// RooUtil tool
#include "IsoMLTree.cc"
#include "rooutil/looper.h"
#include "rooutil/ttreex.h"

#include "Math/VectorUtil.h"
#include "TMath.h"

using namespace std;
using namespace RooUtil;

void ScanChain(TChain* chain, TString output_name, TString optstr, int nevents = -1); // the default nevents=-1 option means loop over all events.

const double pi = 3.1415926536;
void CalcRAlpha(double pfEta, double pfPhi, double lepEta, double lepPhi, double &r, double &alpha) {
  double phi = pfPhi - lepPhi;
  if (abs(phi) > pi)
    phi = pfPhi + lepPhi;
  double eta = pfEta - lepEta;
  r = sqrt( (phi*phi) + (eta*eta) );
  alpha = TMath::ATan2(eta, phi);
}

void CalcIndices(double r, double alpha, int nR, int nAlpha, vector<int> vCandId, int &rIdx, int &alphaIdx, int &candIdx) {
  TH1D* hAlpha = new TH1D("hAlpha", "", nAlpha, -pi, pi);
  TH1D* hR = new TH1D("hR", "", nR, 0.0, 1.0);
  
  alphaIdx = (hAlpha->FindBin(alpha)) - 1;
  rIdx = (hR->FindBin(r)) - 1;

  for (int i = 0; i<7; i++) {
    if (vCandId[i]) {
      candIdx = i;
      break;
    }
  }

  delete hAlpha;
  delete hR;
}

bool CandIsLepton(double r, vector<int> vCandId) {
  if (r > 0.05)
    return false;
  else if (vCandId[0] || vCandId[1])
    return true;
  else
    return false;
}
