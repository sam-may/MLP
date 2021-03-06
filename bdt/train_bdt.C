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

//#include "summary_variables.h"

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

    // Original 11 variables
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


    // Add nvtx
    factory->AddVariable("nvtx", 'F');    

    // Add summary variables
    // just hardcode 10 annuli for now
    factory->AddVariable("summaryVar_R0_Alpha0_Cand0", 'F');
    factory->AddVariable("summaryVar_R1_Alpha0_Cand0", 'F');
    factory->AddVariable("summaryVar_R2_Alpha0_Cand0", 'F');
    factory->AddVariable("summaryVar_R3_Alpha0_Cand0", 'F');
    factory->AddVariable("summaryVar_R4_Alpha0_Cand0", 'F');
    factory->AddVariable("summaryVar_R5_Alpha0_Cand0", 'F');
    factory->AddVariable("summaryVar_R6_Alpha0_Cand0", 'F');
    factory->AddVariable("summaryVar_R7_Alpha0_Cand0", 'F');
    factory->AddVariable("summaryVar_R8_Alpha0_Cand0", 'F');
    factory->AddVariable("summaryVar_R9_Alpha0_Cand0", 'F');

    /*
    int nR = 10;
    int nAlpha = 1;
    int nSummaryVariables = 1;

    bool addSummaryVariables = true;
    if (addSummaryVariables) {
      for (int i = 0; i < nR; i++) {
        for (int j = 0; j < nAlpha; j++) {
          for (int k = 0; k < nSummaryVariables; k++) {
            TString name = "summaryVar_R" + to_string(i) + "_Alpha" + to_string(j) + "_Cand" + to_string(k);
            factory->AddVariable(name, 'F');
	  }
	}
      }
    }    
    */

    // -----------------------------
    //  Input File & Tree
    // -----------------------------
    //TFile* inputSignal = TFile::Open("/hadoop/cms/store/user/phchang/mlp/IsoML_output_3M.root");
    //TFile* inputBkg    = TFile::Open("/hadoop/cms/store/user/phchang/mlp/IsoML_output_3M.root");
    TFile* inputSignal = TFile::Open("/home/users/sjmay/ML/IsoML_output_SV.root");
    TFile* inputBkg    = TFile::Open("/home/users/sjmay/ML/IsoML_output_SV.root");
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

    //root [2] t->Draw("1", "lepton_isFromW==1&&lepton_flavor==1", "goff")
    //(Long64_t) 1439065
    //root [3] t->Draw("1", "lepton_isFromW==0&&lepton_flavor==1", "goff")
    //(Long64_t) 193401
    //(long long) 11780
    //root [3] t->Draw("1", "lepton_isFromW==1&&lepton_flavor==1")
    //(long long) 88168
    //TString prepare_nevents = "nTrain_Signal=44084:nTrain_Background=5890:nTest_Signal=44084:nTest_Background=5890:SplitMode=Alternate:NormMode=NumEvents:!V";
    //TString prepare_nevents = "nTrain_Signal=100000:nTrain_Background=96700:nTest_Signal=100000:nTest_Background=96700:SplitMode=Alternate:NormMode=NumEvents:!V";
    TString prepare_nevents = "nTrain_Signal=88168:nTrain_Background=11780:nTest_Signal=88168:nTest_Background=11780:SplitMode=Alternate:NormMode=NumEvents:!V";
    factory->PrepareTrainingAndTestTree("lepton_isFromW==1&&lepton_flavor==1", "lepton_isFromW==0&&lepton_flavor==1", prepare_nevents);
    factory->SetSignalWeightExpression("1");
    factory->SetBackgroundWeightExpression("1");

    TString option = "!H:V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=2000:MinNodeSize=0.1%:PruneStrength=5:PruneMethod=CostComplexity:MaxDepth=3:CreateMVAPdfs";
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
