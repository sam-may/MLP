//  .
// ..: P. Chang, philip@physics.ucsd.edu

// RooUtil tool
#include "CORE/CMS3.h"
#include "COREHelper/corehelper.h"
#include "rooutil/looper.h"
#include "rooutil/ttreex.h"

#include "Math/VectorUtil.h"

using namespace std;
using namespace RooUtil;

void ScanChain(TChain* chain, TString output_name, TString optstr, int nevents=-1); // the default nevents=-1 option means loop over all events.

std::vector<unsigned int> goodMuonIdx();
std::vector<unsigned int> goodElecIdx();

void fill( RooUtil::TTreeX* ttree, std::vector<unsigned int >& idx, int pdgid );


