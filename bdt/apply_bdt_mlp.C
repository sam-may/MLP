//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include <map>

#include "REPLACEHEADER.h"
//#include "../babymaker/zip_mlpoutputs_to_baby.h"
#include "../babymaker/parse_tf_output.C"
#include "../babymaker/rooutil/looper.h"

//_________________________________________________________________________________________________
void ScanChain(TChain* chain, TString output_name, TString base_optstr, int nevents)
{
    // Event Looper
    Looper<REPLACEME> looper(chain, &isoml, nevents);
    chain->GetEntry(0);
    isoml.Init(chain->GetTree());

    Parser parser("../output_MLP.txt");

    // Set up output
    RooUtil::TTreeX tx;
    looper.setSkim(output_name);

    TMVA::Reader* reader = new TMVA::Reader("!Color:!Silent");
    //TMVA::Reader* reader2 = new TMVA::Reader("!Color:!Silent");
    float lepton_eta;
    float lepton_phi;
    float lepton_pt;
    float lepton_relIso03EA;
    float lepton_chiso;
    float lepton_nhiso;
    float lepton_emiso;
    float lepton_ncorriso;
    float lepton_dxy;
    float lepton_dz;
    float lepton_ip3d;
    float nvtx;

    reader->AddVariable("lepton_eta", &lepton_eta);
    reader->AddVariable("lepton_phi", &lepton_phi);
    reader->AddVariable("lepton_pt", &lepton_pt);
    reader->AddVariable("lepton_relIso03EA", &lepton_relIso03EA);
    reader->AddVariable("lepton_chiso", &lepton_chiso);
    reader->AddVariable("lepton_nhiso", &lepton_nhiso);
    reader->AddVariable("lepton_emiso", &lepton_emiso);
    reader->AddVariable("lepton_ncorriso", &lepton_ncorriso);
    reader->AddVariable("lepton_dxy", &lepton_dxy);
    reader->AddVariable("lepton_dz", &lepton_dz);
    reader->AddVariable("lepton_ip3d", &lepton_ip3d);
    reader->AddVariable("nvtx", &nvtx);
    
    // Add summary variables

    int nR = REPLACENR;
    int nAlpha = REPLACENALPHA;
    int nSummaryVariables = REPLACENSUMMARYVARIABLES;

    const int nCandTypes = 7;

    vector<vector<vector<float>>> vSumVars(nR, vector<vector<float>>(nAlpha, vector<float>(nSummaryVariables, 0.0)));
    for (int i = 0; i < nR; i++) {
      for (int j = 0; j < nAlpha; j++) {
	if (nSummaryVariables == 1) {
          TString name = "";
          for (int k = 0; k < nCandTypes; k++) {
            name += "summaryVar_R" + to_string(i) + "_Alpha" + to_string(j) + "_Cand" + to_string(k);
            if (k != nCandTypes - 1) name += "+";
          }
          reader->AddVariable("SumLabel" + to_string(i) + to_string(j) +  " := "+name, &vSumVars[i][j][0]);
        }
        else {
          for (int k = 0; k < nSummaryVariables; k++) {
            TString name = "summaryVar_R" + to_string(i) + "_Alpha" + to_string(j) + "_Cand" + to_string(k);
            reader->AddVariable(name, &vSumVars[i][j][k]);
	  }
	}
      }
    }

    reader->BookMVA("BDT", "/home/users/sjmay/ML/MLP/bdt/weights/TMVA_BDT.weights.xml");

    // Main event loop
    //std::map<TString, std::function<float()>> funcMap;
    typedef const float &(*FnPtr)();
    std::map<TString, FnPtr> funcMap;
    //funcMap["isoml.summaryVar_R0_Alpha0_Cand0"] = tas::summaryVar_R0_Alpha0_Cand0;
    funcMap = REPLACEMAP;

    while (looper.nextEvent())
    {
        if (!tx.getTree())
        {
            tx.setTree(looper.getSkimTree());
            tx.createBranch<Float_t>("bdt1");
            tx.createBranch<Float_t>("mlp");
        }
        lepton_eta           = isoml.lepton_eta();
        lepton_phi           = isoml.lepton_phi();
        lepton_pt            = isoml.lepton_pt();
        lepton_relIso03EA    = isoml.lepton_relIso03EA();
        lepton_chiso         = isoml.lepton_chiso();
        lepton_nhiso         = isoml.lepton_nhiso();
        lepton_emiso         = isoml.lepton_emiso();
        lepton_ncorriso      = isoml.lepton_ncorriso();
        lepton_dxy           = isoml.lepton_dxy();
        lepton_dz            = isoml.lepton_dz();
        lepton_ip3d          = isoml.lepton_ip3d();
        nvtx 		     = isoml.nvtx();

	for (int i = 0; i < nR; i++) {
	  for (int j = 0; j < nAlpha; j++) {	
	    if (nSummaryVariables == 1) {
	      for (int k = 0; k < 7; k++) {
                TString name = "summaryVar_R" + to_string(i) + "_Alpha" + to_string(j) + "_Cand" + to_string(k);
		//vSumVars[i][j][0] = 0;
		vSumVars[i][j][0] += funcMap[name]();
	      }	      
	    }
	    else {
	      for (int k = 0; k < nSummaryVariables; k++) {
		TString name = "summaryVar_R" + to_string(i) + "_Alpha" + to_string(j) + "_Cand" + to_string(k);
	        //vSumVars[i][j][k] = 0;
		vSumVars[i][j][k] += funcMap[name]();
	      }
	    }
	  }
	}

        tx.setBranch<Float_t>("bdt1", reader->EvaluateMVA("BDT"));
        //tx.setBranch<Float_t>("mlp", parser.mlp(looper.getCurrentEventIndex()));
        if (isoml.lepton_flavor() == 1) // only muons
            looper.fillSkim(); 
    }
    looper.saveSkim();
}

// eof
