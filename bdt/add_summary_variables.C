//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "add_summary_variables.h"

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

    int nSummaryVariables = 7;
    int nR = REPLACENR;
    int nAlpha = REPLACENALPHA;

    vector<vector<vector<float>>> vSumVars(nR, vector<vector<float>>(nAlpha, vector<float>(nSummaryVariables, 0.0)));

    // Main event loop
    while (looper.nextEvent())
    {
        if (!tx.getTree())
        {
            tx.setTree(looper.getSkimTree());
     
            for (int i = 0; i < nR; i++) {
              for (int j = 0; j < nAlpha; j++) {
                for (int k = 0; k < nSummaryVariables; k++) {
	          TString name = "summaryVar_R" + to_string(i) + "_Alpha" + to_string(j) + "_Cand" + to_string(k);
                  tx.createBranch<Float_t>(name);	  
		}
	      }
	    }
        }

        vector<vector<vector<float>>> vSumVars(nR, vector<vector<float>>(nAlpha, vector<float>(nSummaryVariables, 0.0)));
        for (int i = 0; i < isoml.pf_pt().size(); i++) {
	  int rIdx(-1), alphaIdx(-1), candIdx;
          double r(-1), alpha(-1);
          CalcRAlpha(isoml.pf_eta()[i], isoml.pf_phi()[i], isoml.lepton_eta(), isoml.lepton_phi(), r, alpha);
          vector<int> vCandId = {isoml.pf_el()[i], isoml.pf_mu()[i], isoml.pf_chHad()[i], isoml.pf_nEM()[i], isoml.pf_nHad()[i], isoml.pf_HFHad()[i], isoml.pf_HFEM()[i]}; 
          CalcIndices(r, alpha, nR, nAlpha, vCandId, rIdx, alphaIdx, candIdx);
          if (CandIsLepton(r, vCandId))
	    continue; 
	  vSumVars[rIdx][alphaIdx][candIdx] += isoml.pf_pt()[i];
          //vSumVars[rIdx][alphaIdx][0] += isoml.pf_pt()[i]; // ignore the nSummaryVariables types of cands
	}

        for (int i = 0; i < nR; i++) {
	  for (int j = 0; j < nAlpha; j++) {
	    for (int k = 0; k < nSummaryVariables; k++) {
	      TString name = "summaryVar_R" + to_string(i) + "_Alpha" + to_string(j) + "_Cand" + to_string(k);
              tx.setBranch<Float_t>(name, vSumVars[i][j][k]);
	    }
	  }
	}
 
        
        //tx.setBranch<Float_t>("bdt2", reader->EvaluateMVA("BDT2"));
        //tx.setBranch<Float_t>("bdt3", reader2->EvaluateMVA("BDT3"));
        looper.fillSkim();
    }
    looper.saveSkim();
}

// eof
