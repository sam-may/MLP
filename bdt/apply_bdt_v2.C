//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "apply_bdt_v2.h"

//_________________________________________________________________________________________________
void ScanChain(TChain* chain, TString output_name, TString base_optstr, int nevents)
{
    // Event Looper
    Looper<IsoMLTree> looper(chain, &isoml, nevents);
    chain->GetEntry(0);
    isoml.Init(chain->GetTree());

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

    float summaryVar_R0_Alpha0_Cand0;
    float summaryVar_R1_Alpha0_Cand0;
    float summaryVar_R2_Alpha0_Cand0;
    float summaryVar_R3_Alpha0_Cand0;
    float summaryVar_R4_Alpha0_Cand0;
    float summaryVar_R5_Alpha0_Cand0;
    float summaryVar_R6_Alpha0_Cand0;
    float summaryVar_R7_Alpha0_Cand0;
    float summaryVar_R8_Alpha0_Cand0;
    float summaryVar_R9_Alpha0_Cand0;

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
    
    reader->AddVariable("summaryVar_R0_Alpha0_Cand0", &summaryVar_R0_Alpha0_Cand0);
    reader->AddVariable("summaryVar_R1_Alpha0_Cand0", &summaryVar_R1_Alpha0_Cand0);
    reader->AddVariable("summaryVar_R2_Alpha0_Cand0", &summaryVar_R2_Alpha0_Cand0);
    reader->AddVariable("summaryVar_R3_Alpha0_Cand0", &summaryVar_R3_Alpha0_Cand0);
    reader->AddVariable("summaryVar_R4_Alpha0_Cand0", &summaryVar_R4_Alpha0_Cand0);
    reader->AddVariable("summaryVar_R5_Alpha0_Cand0", &summaryVar_R5_Alpha0_Cand0);
    reader->AddVariable("summaryVar_R6_Alpha0_Cand0", &summaryVar_R6_Alpha0_Cand0);
    reader->AddVariable("summaryVar_R7_Alpha0_Cand0", &summaryVar_R7_Alpha0_Cand0);
    reader->AddVariable("summaryVar_R8_Alpha0_Cand0", &summaryVar_R8_Alpha0_Cand0);
    reader->AddVariable("summaryVar_R9_Alpha0_Cand0", &summaryVar_R9_Alpha0_Cand0);
    

    // Add summary variables

    /*
    bool addSummaryVariables = true;
    int nR = 10;
    int nAlpha = 1;
    int nSummaryVariables = 1;
    vector<vector<vector<float>>> vSumVars(nR, vector<vector<float>>(nAlpha, vector<float>(nSummaryVariables, 0.0)));
    if (addSummaryVariables) {
      for (int i = 0; i < nR; i++) {
        for (int j = 0; j < nAlpha; j++) {
          for (int k = 0; k < nSummaryVariables; k++) {
            TString name = "summaryVar_R" + to_string(i) + "_Alpha" + to_string(j) + "_Cand" + to_string(k);
	    float value = vSumVars[i][j][k];
            reader->AddVariable(name, &value);
          }
        }
      }
    }    
    */

    reader->BookMVA("BDT", "/home/users/sjmay/ML/MLP/bdt/weights/TMVA_BDT.weights.xml");

    /*
    reader->BookMVA("BDT", "/hadoop/cms/store/user/phchang/mlp/weights_BDTbaseline_v0.0.2__preliminary_11lepvec_1Msig_100Kbkg_events/TMVA_BDT.weights.xml");
    reader->BookMVA("BDT2", "/hadoop/cms/store/user/phchang/mlp/weights_BDTbaseline_v0.0.3__preliminary_11lepvec_50ksig_5kbkg_events/TMVA_BDT.weights.xml");
    reader2->AddVariable("lepton_relIso03EA", &lepton_relIso03EA);
    reader2->AddVariable("lepton_chiso", &lepton_chiso);
    reader2->AddVariable("lepton_nhiso", &lepton_nhiso);
    reader2->AddVariable("lepton_emiso", &lepton_emiso);
    reader2->AddVariable("lepton_ncorriso", &lepton_ncorriso);
    reader2->AddVariable("lepton_dxy", &lepton_dxy);
    reader2->AddVariable("lepton_dz", &lepton_dz);
    reader2->AddVariable("lepton_ip3d", &lepton_ip3d);
    reader2->BookMVA("BDT3", "/hadoop/cms/store/user/phchang/mlp/weights_BDTbaseline_v0.0.4__nolepp4/TMVA_BDT.weights.xml");
    */

    // Main event loop
    while (looper.nextEvent())
    {
        if (!tx.getTree())
        {
            tx.setTree(looper.getSkimTree());
            tx.createBranch<Float_t>("bdt1");
            //tx.createBranch<Float_t>("bdt2");
            //tx.createBranch<Float_t>("bdt3");
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

        // Hard code the 10 annuli for now, need a way to implement for arbitrary values of nR, nAlpha, nSummaryVariables
	summaryVar_R0_Alpha0_Cand0 = isoml.summaryVar_R0_Alpha0_Cand0();
	summaryVar_R1_Alpha0_Cand0 = isoml.summaryVar_R1_Alpha0_Cand0();
	summaryVar_R2_Alpha0_Cand0 = isoml.summaryVar_R2_Alpha0_Cand0();
	summaryVar_R3_Alpha0_Cand0 = isoml.summaryVar_R3_Alpha0_Cand0();
	summaryVar_R4_Alpha0_Cand0 = isoml.summaryVar_R4_Alpha0_Cand0();
	summaryVar_R5_Alpha0_Cand0 = isoml.summaryVar_R5_Alpha0_Cand0();
	summaryVar_R6_Alpha0_Cand0 = isoml.summaryVar_R6_Alpha0_Cand0();
	summaryVar_R7_Alpha0_Cand0 = isoml.summaryVar_R7_Alpha0_Cand0();
	summaryVar_R8_Alpha0_Cand0 = isoml.summaryVar_R8_Alpha0_Cand0();
	summaryVar_R9_Alpha0_Cand0 = isoml.summaryVar_R9_Alpha0_Cand0();

        tx.setBranch<Float_t>("bdt1", reader->EvaluateMVA("BDT"));
        //tx.setBranch<Float_t>("bdt2", reader->EvaluateMVA("BDT2"));
        //tx.setBranch<Float_t>("bdt3", reader2->EvaluateMVA("BDT3"));
        looper.fillSkim();
    }
    looper.saveSkim();
}

// eof
