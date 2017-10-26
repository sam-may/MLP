// -*- C++ -*-
// This is a header file generated with the command:
// makeCMS3ClassFiles("/hadoop/cms/store/user/phchang/mlp/output_MLP_muon_25ktrain_25ktest_1000epoch_julianOriginal.root", "t", "IsoMLTree", "IsoML", "isoml")

#ifndef IsoMLTree_H
#define IsoMLTree_H

#include "Math/LorentzVector.h"
#include "Math/Point3D.h"
#include "TMath.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TBits.h"
#include <vector> 
#include <unistd.h> 
typedef ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > LorentzVector;

using namespace std; 
class IsoMLTree {
 private: 
 protected: 
  unsigned int index;
  int      run_;
  TBranch *run_branch;
  bool     run_isLoaded;
  int      lumiBlock_;
  TBranch *lumiBlock_branch;
  bool     lumiBlock_isLoaded;
  int      event_;
  TBranch *event_branch;
  bool     event_isLoaded;
  int      nvtx_;
  TBranch *nvtx_branch;
  bool     nvtx_isLoaded;
  float    lepton_eta_;
  TBranch *lepton_eta_branch;
  bool     lepton_eta_isLoaded;
  float    lepton_phi_;
  TBranch *lepton_phi_branch;
  bool     lepton_phi_isLoaded;
  float    lepton_pt_;
  TBranch *lepton_pt_branch;
  bool     lepton_pt_isLoaded;
  int      lepton_flavor_;
  TBranch *lepton_flavor_branch;
  bool     lepton_flavor_isLoaded;
  int      lepton_isFromW_;
  TBranch *lepton_isFromW_branch;
  bool     lepton_isFromW_isLoaded;
  int      lepton_isFromB_;
  TBranch *lepton_isFromB_branch;
  bool     lepton_isFromB_isLoaded;
  int      lepton_isFromC_;
  TBranch *lepton_isFromC_branch;
  bool     lepton_isFromC_isLoaded;
  int      lepton_isFromL_;
  TBranch *lepton_isFromL_branch;
  bool     lepton_isFromL_isLoaded;
  int      lepton_isFromLF_;
  TBranch *lepton_isFromLF_branch;
  bool     lepton_isFromLF_isLoaded;
  float    lepton_relIso03EA_;
  TBranch *lepton_relIso03EA_branch;
  bool     lepton_relIso03EA_isLoaded;
  float    lepton_chiso_;
  TBranch *lepton_chiso_branch;
  bool     lepton_chiso_isLoaded;
  float    lepton_nhiso_;
  TBranch *lepton_nhiso_branch;
  bool     lepton_nhiso_isLoaded;
  float    lepton_emiso_;
  TBranch *lepton_emiso_branch;
  bool     lepton_emiso_isLoaded;
  float    lepton_ncorriso_;
  TBranch *lepton_ncorriso_branch;
  bool     lepton_ncorriso_isLoaded;
  float    lepton_dxy_;
  TBranch *lepton_dxy_branch;
  bool     lepton_dxy_isLoaded;
  float    lepton_dz_;
  TBranch *lepton_dz_branch;
  bool     lepton_dz_isLoaded;
  float    lepton_ip3d_;
  TBranch *lepton_ip3d_branch;
  bool     lepton_ip3d_isLoaded;
  vector<float> *pf_eta_;
  TBranch *pf_eta_branch;
  bool     pf_eta_isLoaded;
  vector<float> *pf_phi_;
  TBranch *pf_phi_branch;
  bool     pf_phi_isLoaded;
  vector<float> *pf_pt_;
  TBranch *pf_pt_branch;
  bool     pf_pt_isLoaded;
  vector<int> *pf_charge_;
  TBranch *pf_charge_branch;
  bool     pf_charge_isLoaded;
  vector<int> *pf_el_;
  TBranch *pf_el_branch;
  bool     pf_el_isLoaded;
  vector<int> *pf_mu_;
  TBranch *pf_mu_branch;
  bool     pf_mu_isLoaded;
  vector<int> *pf_chHad_;
  TBranch *pf_chHad_branch;
  bool     pf_chHad_isLoaded;
  vector<int> *pf_nEM_;
  TBranch *pf_nEM_branch;
  bool     pf_nEM_isLoaded;
  vector<int> *pf_nHad_;
  TBranch *pf_nHad_branch;
  bool     pf_nHad_isLoaded;
  vector<int> *pf_HFHad_;
  TBranch *pf_HFHad_branch;
  bool     pf_HFHad_isLoaded;
  vector<int> *pf_HFEM_;
  TBranch *pf_HFEM_branch;
  bool     pf_HFEM_isLoaded;
  float    mlp_;
  TBranch *mlp_branch;
  bool     mlp_isLoaded;
public: 
void Init(TTree *tree);
void GetEntry(unsigned int idx); 
void LoadAllBranches(); 
  const int &run();
  const int &lumiBlock();
  const int &event();
  const int &nvtx();
  const float &lepton_eta();
  const float &lepton_phi();
  const float &lepton_pt();
  const int &lepton_flavor();
  const int &lepton_isFromW();
  const int &lepton_isFromB();
  const int &lepton_isFromC();
  const int &lepton_isFromL();
  const int &lepton_isFromLF();
  const float &lepton_relIso03EA();
  const float &lepton_chiso();
  const float &lepton_nhiso();
  const float &lepton_emiso();
  const float &lepton_ncorriso();
  const float &lepton_dxy();
  const float &lepton_dz();
  const float &lepton_ip3d();
  const vector<float> &pf_eta();
  const vector<float> &pf_phi();
  const vector<float> &pf_pt();
  const vector<int> &pf_charge();
  const vector<int> &pf_el();
  const vector<int> &pf_mu();
  const vector<int> &pf_chHad();
  const vector<int> &pf_nEM();
  const vector<int> &pf_nHad();
  const vector<int> &pf_HFHad();
  const vector<int> &pf_HFEM();
  const float &mlp();

  static void progress(int nEventsTotal, int nEventsChain);
};

#ifndef __CINT__
extern IsoMLTree isoml;
#endif

namespace IsoML {
  const int &run();
  const int &lumiBlock();
  const int &event();
  const int &nvtx();
  const float &lepton_eta();
  const float &lepton_phi();
  const float &lepton_pt();
  const int &lepton_flavor();
  const int &lepton_isFromW();
  const int &lepton_isFromB();
  const int &lepton_isFromC();
  const int &lepton_isFromL();
  const int &lepton_isFromLF();
  const float &lepton_relIso03EA();
  const float &lepton_chiso();
  const float &lepton_nhiso();
  const float &lepton_emiso();
  const float &lepton_ncorriso();
  const float &lepton_dxy();
  const float &lepton_dz();
  const float &lepton_ip3d();
  const vector<float> &pf_eta();
  const vector<float> &pf_phi();
  const vector<float> &pf_pt();
  const vector<int> &pf_charge();
  const vector<int> &pf_el();
  const vector<int> &pf_mu();
  const vector<int> &pf_chHad();
  const vector<int> &pf_nEM();
  const vector<int> &pf_nHad();
  const vector<int> &pf_HFHad();
  const vector<int> &pf_HFEM();
  const float &mlp();
}
#endif
