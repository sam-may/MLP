//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "zip_mlpoutputs_to_baby.h"

#include "parse_tf_output.C"

//_________________________________________________________________________________________________
void ScanChain(TChain* chain, TString output_name, TString base_optstr, int nevents)
{
    // Event Looper
    Looper<IsoMLTree> looper(chain, &isoml, nevents);
    chain->GetEntry(0);
    isoml.Init(chain->GetTree());

    Parser parser("../output_MLP.txt");

    RooUtil::TTreeX tx;

    looper.setSkim(output_name);

    // Main event loop
    while (looper.nextEvent())
    {
        if (!tx.getTree())
        {
            tx.setTree(looper.getSkimTree());
            tx.createBranch<Float_t>("mlp");
        }
        tx.setBranch<Float_t>("mlp", parser.mlp(looper.getCurrentEventIndex()));
        looper.fillSkim();
    }
    looper.saveSkim();
}

// eof
