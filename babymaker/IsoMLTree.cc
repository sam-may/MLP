#include "IsoMLTree.h"
IsoMLTree isoml;

void IsoMLTree::Init(TTree *tree) {

  tree->SetMakeClass(1);

  run_branch = tree->GetBranch("run");
  if (run_branch) run_branch->SetAddress(&run_);
  lumiBlock_branch = tree->GetBranch("lumiBlock");
  if (lumiBlock_branch) lumiBlock_branch->SetAddress(&lumiBlock_);
  event_branch = tree->GetBranch("event");
  if (event_branch) event_branch->SetAddress(&event_);
  nvtx_branch = tree->GetBranch("nvtx");
  if (nvtx_branch) nvtx_branch->SetAddress(&nvtx_);
  lepton_eta_branch = tree->GetBranch("lepton_eta");
  if (lepton_eta_branch) lepton_eta_branch->SetAddress(&lepton_eta_);
  lepton_phi_branch = tree->GetBranch("lepton_phi");
  if (lepton_phi_branch) lepton_phi_branch->SetAddress(&lepton_phi_);
  lepton_pt_branch = tree->GetBranch("lepton_pt");
  if (lepton_pt_branch) lepton_pt_branch->SetAddress(&lepton_pt_);
  lepton_flavor_branch = tree->GetBranch("lepton_flavor");
  if (lepton_flavor_branch) lepton_flavor_branch->SetAddress(&lepton_flavor_);
  lepton_isFromW_branch = tree->GetBranch("lepton_isFromW");
  if (lepton_isFromW_branch) lepton_isFromW_branch->SetAddress(&lepton_isFromW_);
  lepton_isFromB_branch = tree->GetBranch("lepton_isFromB");
  if (lepton_isFromB_branch) lepton_isFromB_branch->SetAddress(&lepton_isFromB_);
  lepton_isFromC_branch = tree->GetBranch("lepton_isFromC");
  if (lepton_isFromC_branch) lepton_isFromC_branch->SetAddress(&lepton_isFromC_);
  lepton_isFromL_branch = tree->GetBranch("lepton_isFromL");
  if (lepton_isFromL_branch) lepton_isFromL_branch->SetAddress(&lepton_isFromL_);
  lepton_isFromLF_branch = tree->GetBranch("lepton_isFromLF");
  if (lepton_isFromLF_branch) lepton_isFromLF_branch->SetAddress(&lepton_isFromLF_);
  lepton_relIso03EA_branch = tree->GetBranch("lepton_relIso03EA");
  if (lepton_relIso03EA_branch) lepton_relIso03EA_branch->SetAddress(&lepton_relIso03EA_);
  lepton_chiso_branch = tree->GetBranch("lepton_chiso");
  if (lepton_chiso_branch) lepton_chiso_branch->SetAddress(&lepton_chiso_);
  lepton_nhiso_branch = tree->GetBranch("lepton_nhiso");
  if (lepton_nhiso_branch) lepton_nhiso_branch->SetAddress(&lepton_nhiso_);
  lepton_emiso_branch = tree->GetBranch("lepton_emiso");
  if (lepton_emiso_branch) lepton_emiso_branch->SetAddress(&lepton_emiso_);
  lepton_ncorriso_branch = tree->GetBranch("lepton_ncorriso");
  if (lepton_ncorriso_branch) lepton_ncorriso_branch->SetAddress(&lepton_ncorriso_);
  lepton_dxy_branch = tree->GetBranch("lepton_dxy");
  if (lepton_dxy_branch) lepton_dxy_branch->SetAddress(&lepton_dxy_);
  lepton_dz_branch = tree->GetBranch("lepton_dz");
  if (lepton_dz_branch) lepton_dz_branch->SetAddress(&lepton_dz_);
  lepton_ip3d_branch = tree->GetBranch("lepton_ip3d");
  if (lepton_ip3d_branch) lepton_ip3d_branch->SetAddress(&lepton_ip3d_);
  pf_eta_branch = tree->GetBranch("pf_eta");
  if (pf_eta_branch) pf_eta_branch->SetAddress(&pf_eta_);
  pf_phi_branch = tree->GetBranch("pf_phi");
  if (pf_phi_branch) pf_phi_branch->SetAddress(&pf_phi_);
  pf_pt_branch = tree->GetBranch("pf_pt");
  if (pf_pt_branch) pf_pt_branch->SetAddress(&pf_pt_);
  pf_charge_branch = tree->GetBranch("pf_charge");
  if (pf_charge_branch) pf_charge_branch->SetAddress(&pf_charge_);
  pf_el_branch = tree->GetBranch("pf_el");
  if (pf_el_branch) pf_el_branch->SetAddress(&pf_el_);
  pf_mu_branch = tree->GetBranch("pf_mu");
  if (pf_mu_branch) pf_mu_branch->SetAddress(&pf_mu_);
  pf_chHad_branch = tree->GetBranch("pf_chHad");
  if (pf_chHad_branch) pf_chHad_branch->SetAddress(&pf_chHad_);
  pf_nEM_branch = tree->GetBranch("pf_nEM");
  if (pf_nEM_branch) pf_nEM_branch->SetAddress(&pf_nEM_);
  pf_nHad_branch = tree->GetBranch("pf_nHad");
  if (pf_nHad_branch) pf_nHad_branch->SetAddress(&pf_nHad_);
  pf_HFHad_branch = tree->GetBranch("pf_HFHad");
  if (pf_HFHad_branch) pf_HFHad_branch->SetAddress(&pf_HFHad_);
  pf_HFEM_branch = tree->GetBranch("pf_HFEM");
  if (pf_HFEM_branch) pf_HFEM_branch->SetAddress(&pf_HFEM_);

  tree->SetMakeClass(0);
}

void IsoMLTree::GetEntry(unsigned int idx) {
  // this only marks branches as not loaded, saving a lot of time
  index = idx;
  run_isLoaded = false;
  lumiBlock_isLoaded = false;
  event_isLoaded = false;
  nvtx_isLoaded = false;
  lepton_eta_isLoaded = false;
  lepton_phi_isLoaded = false;
  lepton_pt_isLoaded = false;
  lepton_flavor_isLoaded = false;
  lepton_isFromW_isLoaded = false;
  lepton_isFromB_isLoaded = false;
  lepton_isFromC_isLoaded = false;
  lepton_isFromL_isLoaded = false;
  lepton_isFromLF_isLoaded = false;
  lepton_relIso03EA_isLoaded = false;
  lepton_chiso_isLoaded = false;
  lepton_nhiso_isLoaded = false;
  lepton_emiso_isLoaded = false;
  lepton_ncorriso_isLoaded = false;
  lepton_dxy_isLoaded = false;
  lepton_dz_isLoaded = false;
  lepton_ip3d_isLoaded = false;
  pf_eta_isLoaded = false;
  pf_phi_isLoaded = false;
  pf_pt_isLoaded = false;
  pf_charge_isLoaded = false;
  pf_el_isLoaded = false;
  pf_mu_isLoaded = false;
  pf_chHad_isLoaded = false;
  pf_nEM_isLoaded = false;
  pf_nHad_isLoaded = false;
  pf_HFHad_isLoaded = false;
  pf_HFEM_isLoaded = false;
}

void IsoMLTree::LoadAllBranches() {
  // load all branches
  if (run_branch != 0) run();
  if (lumiBlock_branch != 0) lumiBlock();
  if (event_branch != 0) event();
  if (nvtx_branch != 0) nvtx();
  if (lepton_eta_branch != 0) lepton_eta();
  if (lepton_phi_branch != 0) lepton_phi();
  if (lepton_pt_branch != 0) lepton_pt();
  if (lepton_flavor_branch != 0) lepton_flavor();
  if (lepton_isFromW_branch != 0) lepton_isFromW();
  if (lepton_isFromB_branch != 0) lepton_isFromB();
  if (lepton_isFromC_branch != 0) lepton_isFromC();
  if (lepton_isFromL_branch != 0) lepton_isFromL();
  if (lepton_isFromLF_branch != 0) lepton_isFromLF();
  if (lepton_relIso03EA_branch != 0) lepton_relIso03EA();
  if (lepton_chiso_branch != 0) lepton_chiso();
  if (lepton_nhiso_branch != 0) lepton_nhiso();
  if (lepton_emiso_branch != 0) lepton_emiso();
  if (lepton_ncorriso_branch != 0) lepton_ncorriso();
  if (lepton_dxy_branch != 0) lepton_dxy();
  if (lepton_dz_branch != 0) lepton_dz();
  if (lepton_ip3d_branch != 0) lepton_ip3d();
  if (pf_eta_branch != 0) pf_eta();
  if (pf_phi_branch != 0) pf_phi();
  if (pf_pt_branch != 0) pf_pt();
  if (pf_charge_branch != 0) pf_charge();
  if (pf_el_branch != 0) pf_el();
  if (pf_mu_branch != 0) pf_mu();
  if (pf_chHad_branch != 0) pf_chHad();
  if (pf_nEM_branch != 0) pf_nEM();
  if (pf_nHad_branch != 0) pf_nHad();
  if (pf_HFHad_branch != 0) pf_HFHad();
  if (pf_HFEM_branch != 0) pf_HFEM();
}

const int &IsoMLTree::run() {
  if (not run_isLoaded) {
    if (run_branch != 0) {
      run_branch->GetEntry(index);
    } else {
      printf("branch run_branch does not exist!\n");
      exit(1);
    }
    run_isLoaded = true;
  }
  return run_;
}

const int &IsoMLTree::lumiBlock() {
  if (not lumiBlock_isLoaded) {
    if (lumiBlock_branch != 0) {
      lumiBlock_branch->GetEntry(index);
    } else {
      printf("branch lumiBlock_branch does not exist!\n");
      exit(1);
    }
    lumiBlock_isLoaded = true;
  }
  return lumiBlock_;
}

const int &IsoMLTree::event() {
  if (not event_isLoaded) {
    if (event_branch != 0) {
      event_branch->GetEntry(index);
    } else {
      printf("branch event_branch does not exist!\n");
      exit(1);
    }
    event_isLoaded = true;
  }
  return event_;
}

const int &IsoMLTree::nvtx() {
  if (not nvtx_isLoaded) {
    if (nvtx_branch != 0) {
      nvtx_branch->GetEntry(index);
    } else {
      printf("branch nvtx_branch does not exist!\n");
      exit(1);
    }
    nvtx_isLoaded = true;
  }
  return nvtx_;
}

const float &IsoMLTree::lepton_eta() {
  if (not lepton_eta_isLoaded) {
    if (lepton_eta_branch != 0) {
      lepton_eta_branch->GetEntry(index);
    } else {
      printf("branch lepton_eta_branch does not exist!\n");
      exit(1);
    }
    lepton_eta_isLoaded = true;
  }
  return lepton_eta_;
}

const float &IsoMLTree::lepton_phi() {
  if (not lepton_phi_isLoaded) {
    if (lepton_phi_branch != 0) {
      lepton_phi_branch->GetEntry(index);
    } else {
      printf("branch lepton_phi_branch does not exist!\n");
      exit(1);
    }
    lepton_phi_isLoaded = true;
  }
  return lepton_phi_;
}

const float &IsoMLTree::lepton_pt() {
  if (not lepton_pt_isLoaded) {
    if (lepton_pt_branch != 0) {
      lepton_pt_branch->GetEntry(index);
    } else {
      printf("branch lepton_pt_branch does not exist!\n");
      exit(1);
    }
    lepton_pt_isLoaded = true;
  }
  return lepton_pt_;
}

const int &IsoMLTree::lepton_flavor() {
  if (not lepton_flavor_isLoaded) {
    if (lepton_flavor_branch != 0) {
      lepton_flavor_branch->GetEntry(index);
    } else {
      printf("branch lepton_flavor_branch does not exist!\n");
      exit(1);
    }
    lepton_flavor_isLoaded = true;
  }
  return lepton_flavor_;
}

const int &IsoMLTree::lepton_isFromW() {
  if (not lepton_isFromW_isLoaded) {
    if (lepton_isFromW_branch != 0) {
      lepton_isFromW_branch->GetEntry(index);
    } else {
      printf("branch lepton_isFromW_branch does not exist!\n");
      exit(1);
    }
    lepton_isFromW_isLoaded = true;
  }
  return lepton_isFromW_;
}

const int &IsoMLTree::lepton_isFromB() {
  if (not lepton_isFromB_isLoaded) {
    if (lepton_isFromB_branch != 0) {
      lepton_isFromB_branch->GetEntry(index);
    } else {
      printf("branch lepton_isFromB_branch does not exist!\n");
      exit(1);
    }
    lepton_isFromB_isLoaded = true;
  }
  return lepton_isFromB_;
}

const int &IsoMLTree::lepton_isFromC() {
  if (not lepton_isFromC_isLoaded) {
    if (lepton_isFromC_branch != 0) {
      lepton_isFromC_branch->GetEntry(index);
    } else {
      printf("branch lepton_isFromC_branch does not exist!\n");
      exit(1);
    }
    lepton_isFromC_isLoaded = true;
  }
  return lepton_isFromC_;
}

const int &IsoMLTree::lepton_isFromL() {
  if (not lepton_isFromL_isLoaded) {
    if (lepton_isFromL_branch != 0) {
      lepton_isFromL_branch->GetEntry(index);
    } else {
      printf("branch lepton_isFromL_branch does not exist!\n");
      exit(1);
    }
    lepton_isFromL_isLoaded = true;
  }
  return lepton_isFromL_;
}

const int &IsoMLTree::lepton_isFromLF() {
  if (not lepton_isFromLF_isLoaded) {
    if (lepton_isFromLF_branch != 0) {
      lepton_isFromLF_branch->GetEntry(index);
    } else {
      printf("branch lepton_isFromLF_branch does not exist!\n");
      exit(1);
    }
    lepton_isFromLF_isLoaded = true;
  }
  return lepton_isFromLF_;
}

const float &IsoMLTree::lepton_relIso03EA() {
  if (not lepton_relIso03EA_isLoaded) {
    if (lepton_relIso03EA_branch != 0) {
      lepton_relIso03EA_branch->GetEntry(index);
    } else {
      printf("branch lepton_relIso03EA_branch does not exist!\n");
      exit(1);
    }
    lepton_relIso03EA_isLoaded = true;
  }
  return lepton_relIso03EA_;
}

const float &IsoMLTree::lepton_chiso() {
  if (not lepton_chiso_isLoaded) {
    if (lepton_chiso_branch != 0) {
      lepton_chiso_branch->GetEntry(index);
    } else {
      printf("branch lepton_chiso_branch does not exist!\n");
      exit(1);
    }
    lepton_chiso_isLoaded = true;
  }
  return lepton_chiso_;
}

const float &IsoMLTree::lepton_nhiso() {
  if (not lepton_nhiso_isLoaded) {
    if (lepton_nhiso_branch != 0) {
      lepton_nhiso_branch->GetEntry(index);
    } else {
      printf("branch lepton_nhiso_branch does not exist!\n");
      exit(1);
    }
    lepton_nhiso_isLoaded = true;
  }
  return lepton_nhiso_;
}

const float &IsoMLTree::lepton_emiso() {
  if (not lepton_emiso_isLoaded) {
    if (lepton_emiso_branch != 0) {
      lepton_emiso_branch->GetEntry(index);
    } else {
      printf("branch lepton_emiso_branch does not exist!\n");
      exit(1);
    }
    lepton_emiso_isLoaded = true;
  }
  return lepton_emiso_;
}

const float &IsoMLTree::lepton_ncorriso() {
  if (not lepton_ncorriso_isLoaded) {
    if (lepton_ncorriso_branch != 0) {
      lepton_ncorriso_branch->GetEntry(index);
    } else {
      printf("branch lepton_ncorriso_branch does not exist!\n");
      exit(1);
    }
    lepton_ncorriso_isLoaded = true;
  }
  return lepton_ncorriso_;
}

const float &IsoMLTree::lepton_dxy() {
  if (not lepton_dxy_isLoaded) {
    if (lepton_dxy_branch != 0) {
      lepton_dxy_branch->GetEntry(index);
    } else {
      printf("branch lepton_dxy_branch does not exist!\n");
      exit(1);
    }
    lepton_dxy_isLoaded = true;
  }
  return lepton_dxy_;
}

const float &IsoMLTree::lepton_dz() {
  if (not lepton_dz_isLoaded) {
    if (lepton_dz_branch != 0) {
      lepton_dz_branch->GetEntry(index);
    } else {
      printf("branch lepton_dz_branch does not exist!\n");
      exit(1);
    }
    lepton_dz_isLoaded = true;
  }
  return lepton_dz_;
}

const float &IsoMLTree::lepton_ip3d() {
  if (not lepton_ip3d_isLoaded) {
    if (lepton_ip3d_branch != 0) {
      lepton_ip3d_branch->GetEntry(index);
    } else {
      printf("branch lepton_ip3d_branch does not exist!\n");
      exit(1);
    }
    lepton_ip3d_isLoaded = true;
  }
  return lepton_ip3d_;
}

const vector<float> &IsoMLTree::pf_eta() {
  if (not pf_eta_isLoaded) {
    if (pf_eta_branch != 0) {
      pf_eta_branch->GetEntry(index);
    } else {
      printf("branch pf_eta_branch does not exist!\n");
      exit(1);
    }
    pf_eta_isLoaded = true;
  }
  return *pf_eta_;
}

const vector<float> &IsoMLTree::pf_phi() {
  if (not pf_phi_isLoaded) {
    if (pf_phi_branch != 0) {
      pf_phi_branch->GetEntry(index);
    } else {
      printf("branch pf_phi_branch does not exist!\n");
      exit(1);
    }
    pf_phi_isLoaded = true;
  }
  return *pf_phi_;
}

const vector<float> &IsoMLTree::pf_pt() {
  if (not pf_pt_isLoaded) {
    if (pf_pt_branch != 0) {
      pf_pt_branch->GetEntry(index);
    } else {
      printf("branch pf_pt_branch does not exist!\n");
      exit(1);
    }
    pf_pt_isLoaded = true;
  }
  return *pf_pt_;
}

const vector<int> &IsoMLTree::pf_charge() {
  if (not pf_charge_isLoaded) {
    if (pf_charge_branch != 0) {
      pf_charge_branch->GetEntry(index);
    } else {
      printf("branch pf_charge_branch does not exist!\n");
      exit(1);
    }
    pf_charge_isLoaded = true;
  }
  return *pf_charge_;
}

const vector<int> &IsoMLTree::pf_el() {
  if (not pf_el_isLoaded) {
    if (pf_el_branch != 0) {
      pf_el_branch->GetEntry(index);
    } else {
      printf("branch pf_el_branch does not exist!\n");
      exit(1);
    }
    pf_el_isLoaded = true;
  }
  return *pf_el_;
}

const vector<int> &IsoMLTree::pf_mu() {
  if (not pf_mu_isLoaded) {
    if (pf_mu_branch != 0) {
      pf_mu_branch->GetEntry(index);
    } else {
      printf("branch pf_mu_branch does not exist!\n");
      exit(1);
    }
    pf_mu_isLoaded = true;
  }
  return *pf_mu_;
}

const vector<int> &IsoMLTree::pf_chHad() {
  if (not pf_chHad_isLoaded) {
    if (pf_chHad_branch != 0) {
      pf_chHad_branch->GetEntry(index);
    } else {
      printf("branch pf_chHad_branch does not exist!\n");
      exit(1);
    }
    pf_chHad_isLoaded = true;
  }
  return *pf_chHad_;
}

const vector<int> &IsoMLTree::pf_nEM() {
  if (not pf_nEM_isLoaded) {
    if (pf_nEM_branch != 0) {
      pf_nEM_branch->GetEntry(index);
    } else {
      printf("branch pf_nEM_branch does not exist!\n");
      exit(1);
    }
    pf_nEM_isLoaded = true;
  }
  return *pf_nEM_;
}

const vector<int> &IsoMLTree::pf_nHad() {
  if (not pf_nHad_isLoaded) {
    if (pf_nHad_branch != 0) {
      pf_nHad_branch->GetEntry(index);
    } else {
      printf("branch pf_nHad_branch does not exist!\n");
      exit(1);
    }
    pf_nHad_isLoaded = true;
  }
  return *pf_nHad_;
}

const vector<int> &IsoMLTree::pf_HFHad() {
  if (not pf_HFHad_isLoaded) {
    if (pf_HFHad_branch != 0) {
      pf_HFHad_branch->GetEntry(index);
    } else {
      printf("branch pf_HFHad_branch does not exist!\n");
      exit(1);
    }
    pf_HFHad_isLoaded = true;
  }
  return *pf_HFHad_;
}

const vector<int> &IsoMLTree::pf_HFEM() {
  if (not pf_HFEM_isLoaded) {
    if (pf_HFEM_branch != 0) {
      pf_HFEM_branch->GetEntry(index);
    } else {
      printf("branch pf_HFEM_branch does not exist!\n");
      exit(1);
    }
    pf_HFEM_isLoaded = true;
  }
  return *pf_HFEM_;
}


void IsoMLTree::progress( int nEventsTotal, int nEventsChain ){
  int period = 1000;
  if (nEventsTotal%1000 == 0) {
    // xterm magic from L. Vacavant and A. Cerri
    if (isatty(1)) {
      if ((nEventsChain - nEventsTotal) > period) {
        float frac = (float)nEventsTotal/(nEventsChain*0.01);
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
             "\033[0m\033[32m <---\033[0m\015", frac);
        fflush(stdout);
      }
      else {
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", 100.);
        cout << endl;
      }
    }
  }
}

namespace IsoML {

const int &run() { return isoml.run(); }
const int &lumiBlock() { return isoml.lumiBlock(); }
const int &event() { return isoml.event(); }
const int &nvtx() { return isoml.nvtx(); }
const float &lepton_eta() { return isoml.lepton_eta(); }
const float &lepton_phi() { return isoml.lepton_phi(); }
const float &lepton_pt() { return isoml.lepton_pt(); }
const int &lepton_flavor() { return isoml.lepton_flavor(); }
const int &lepton_isFromW() { return isoml.lepton_isFromW(); }
const int &lepton_isFromB() { return isoml.lepton_isFromB(); }
const int &lepton_isFromC() { return isoml.lepton_isFromC(); }
const int &lepton_isFromL() { return isoml.lepton_isFromL(); }
const int &lepton_isFromLF() { return isoml.lepton_isFromLF(); }
const float &lepton_relIso03EA() { return isoml.lepton_relIso03EA(); }
const float &lepton_chiso() { return isoml.lepton_chiso(); }
const float &lepton_nhiso() { return isoml.lepton_nhiso(); }
const float &lepton_emiso() { return isoml.lepton_emiso(); }
const float &lepton_ncorriso() { return isoml.lepton_ncorriso(); }
const float &lepton_dxy() { return isoml.lepton_dxy(); }
const float &lepton_dz() { return isoml.lepton_dz(); }
const float &lepton_ip3d() { return isoml.lepton_ip3d(); }
const vector<float> &pf_eta() { return isoml.pf_eta(); }
const vector<float> &pf_phi() { return isoml.pf_phi(); }
const vector<float> &pf_pt() { return isoml.pf_pt(); }
const vector<int> &pf_charge() { return isoml.pf_charge(); }
const vector<int> &pf_el() { return isoml.pf_el(); }
const vector<int> &pf_mu() { return isoml.pf_mu(); }
const vector<int> &pf_chHad() { return isoml.pf_chHad(); }
const vector<int> &pf_nEM() { return isoml.pf_nEM(); }
const vector<int> &pf_nHad() { return isoml.pf_nHad(); }
const vector<int> &pf_HFHad() { return isoml.pf_HFHad(); }
const vector<int> &pf_HFEM() { return isoml.pf_HFEM(); }

}
