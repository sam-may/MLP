//  .
// ..: P. Chang, philip@physics.ucsd.edu

// C++
#include <iostream>
#include <fstream>

// RooUtil tool
#include "REPLACEME.cc"
#include "rooutil/looper.h"
#include "rooutil/ttreex.h"

#include "Math/VectorUtil.h"

#include "TMVA/Reader.h"

using namespace std;
using namespace RooUtil;

void ScanChain(TChain* chain, TString output_name, TString optstr, int nevents = -1); // the default nevents=-1 option means loop over all events.
