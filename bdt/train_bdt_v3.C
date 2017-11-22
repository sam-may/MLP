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
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"

void train_bdt_v3(int nR, int nAlpha, int nSummaryVariables, int nTrainSig, int nTrainBkg)
{
    // Initialize TMVA
    TMVA::Tools::Instance();

    TFile* outputFile = 0;
    outputFile = TFile::Open("BDT.root", "RECREATE");

    TMVA::Factory *factory = new TMVA::Factory("TMVA", outputFile, "V:DrawProgressBar=True:Transformations=I;D;P;G:AnalysisType=Classification");

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
   
   
    const int nCandTypes = 7;
    for (int i = 0; i < nR; i++) {
      for (int j = 0; j < nAlpha; j++) {
	if (nSummaryVariables == 1) {
	  TString name = "SumLabel" + to_string(i) + to_string(j) + ":= ";
	  for (int k = 0; k < nCandTypes; k++) {
	    name += "summaryVar_R" + to_string(i) + "_Alpha" + to_string(j) + "_Cand" + to_string(k);
	    if (k != nCandTypes - 1) name += "+";
	  }
	  factory->AddVariable(name, 'F');
	}
	else {
	  for (int k = 0; k < nSummaryVariables; k++) {
	    TString name = "summaryVar_R" + to_string(i) + "_Alpha" + to_string(j) + "_Cand" + to_string(k);
	    factory->AddVariable(name, 'F');
	  }
	}
      }
    }
    

    // -----------------------------
    //  Input File & Tree
    // -----------------------------
    TString rootFile = "../babymaker/MLP_BDT_outputs/IsoML_output_" + to_string(nR) + "annuli_" + to_string(nAlpha) + "alpha_" + to_string(nSummaryVariables) + "cands.root"; 
    TFile* inputSignal = TFile::Open(rootFile);
    TFile* inputBkg    = TFile::Open(rootFile);
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
    TString prepare_nevents = "nTrain_Signal=" + to_string(nTrainSig) + ":nTrain_Background=" + to_string(nTrainBkg) + ":nTest_Signal=88168:nTest_Background=11780:SplitMode=Alternate:NormMode=NumEvents:!V";
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
