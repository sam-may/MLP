#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

//#include "TMVAGui.C"

//#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
//#include "TMVA/Category.h"
//#endif

void train_bdt()
{
    // Initialize TMVA
    TMVA::Tools::Instance();
//    (TMVA::gConfig().GetVariablePlotting()).fNbinsXOfROCCurve = 400;

    TFile* outputFile = 0;
    outputFile = TFile::Open("BDT.root", "RECREATE");

    TMVA::Factory *factory = new TMVA::Factory("TMVA", outputFile, "V:DrawProgressBar=True:Transformations=I;D;P;G:AnalysisType=Classification");

    //run             = 1
    //lumiBlock       = 1971
    //event           = 315602
    //nvtx            = 15
    //lepton_eta      = 0.00140919
    //lepton_phi      = -1.00151
    //lepton_pt       = 95.0017
    //lepton_flavor   = 1
    //lepton_isFromW  = 1
    //lepton_isFromB  = 0
    //lepton_isFromC  = 0
    //lepton_isFromL  = 0
    //lepton_isFromLF = 0
    //lepton_relIso03EA = 0.0124158
    //lepton_chiso    = 1.17953
    //lepton_nhiso    = 0
    //lepton_emiso    = 0
    //lepton_ncorriso = -1.0716
    //lepton_dxy      = -0.000968322
    //lepton_dz       = -0.00364195
    //lepton_ip3d     = -0.00376861
    //pf_eta          = (vector<float>*)0x40caac0
    //pf_phi          = (vector<float>*)0x4701d20
    //pf_pt           = (vector<float>*)0x3ca1b50
    //pf_charge       = (vector<int>*)0x4059eb0
    //pf_el           = (vector<int>*)0x3551110
    //pf_mu           = (vector<int>*)0x495a590
    //pf_chHad        = (vector<int>*)0x569b690
    //pf_nEM          = (vector<int>*)0x3ce1880
    //pf_nHad         = (vector<int>*)0x569c200
    //pf_HFHad        = (vector<int>*)0x416b7b0
    //pf_HFEM         = (vector<int>*)0x3ce1800
    //mlp             = -999

    factory->AddVariable("lepton_eta", 'F');
    factory->AddVariable("lepton_phi", 'F');
    factory->AddVariable("lepton_pt", 'F');
    factory->AddVariable("lepton_relIso03EA", 'F');
    factory->AddVariable("lepton_chiso", 'F');
    factory->AddVariable("lepton_nhiso", 'F');
    factory->AddVariable("lepton_emiso", 'F');
    factory->AddVariable("lepton_ncorriso", 'F');
    factory->AddVariable("lepton_dxy", 'F');
    factory->AddVariable("lepton_dz", 'F');
    factory->AddVariable("lepton_ip3d", 'F');

    // -----------------------------
    //  Input File & Tree
    // -----------------------------
    TFile* inputSignal = TFile::Open("/hadoop/cms/store/user/phchang/mlp/output_MLP_muon_25ktrain_25ktest_1000epoch_julianOriginal.root");
    TFile* inputBkg    = TFile::Open("/hadoop/cms/store/user/phchang/mlp/output_MLP_muon_25ktrain_25ktest_1000epoch_julianOriginal.root");
    TTree *signal     = (TTree*)inputSignal->Get("t");
    TTree *background = (TTree*)inputBkg->Get("t");
    // global event weights per tree (see below for setting event-wise weights)
    Double_t signalWeight     = 1.0;
    Double_t backgroundWeight = 1.0;
    // You can add an arbitrary number of signal or background trees
    factory->AddSignalTree(signal);
    factory->AddBackgroundTree(background);
    // ---------------------------
    //  Training
    // ---------------------------
    // Apply additional cuts on the signal and background samples (can be different)
    TString prepare_nevents = "nTrain_Signal=1000:nTrain_Background=1000:nTest_Signal=1000:nTest_Background=1000:SplitMode=Alternate:NormMode=NumEvents:!V";
    factory->PrepareTrainingAndTestTree("lepton_isFromW==1", "lepton_isFromW==0", prepare_nevents);
    factory->SetSignalWeightExpression("1");
    factory->SetBackgroundWeightExpression("1");

    TString option = "!H:V:NTrees=200:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=2000:MinNodeSize=0.1%:PruneStrength=5:PruneMethod=CostComplexity:MaxDepth=3:CreateMVAPdfs";
    factory->BookMethod(TMVA::Types::kBDT, "BDT", option);
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();
    // Save the output
    outputFile->Close();
    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;
    delete factory;
}
