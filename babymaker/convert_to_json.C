//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "convert_to_json.h"

//_________________________________________________________________________________________________
void ScanChain(TChain* chain, TString output_name, TString base_optstr, int nevents)
{
    // Event Looper
    Looper<IsoMLTree> looper(chain, &isoml, nevents);
    chain->GetEntry(0);
    isoml.Init(chain->GetTree());

    int ievent = 0;
    looper.setSilent();

    // output file
    ofstream jsonfile;
    jsonfile.open(output_name.Data());

    // Main event loop
    while (looper.nextEvent())
    {
        jsonfile << "{";

        jsonfile << "'Row' : ";
        jsonfile << ievent;
        jsonfile << ",";

        jsonfile << "'lepton_flavor' : ";
        jsonfile << isoml.lepton_flavor();
        jsonfile << ",";

        jsonfile << "'lepton_isFromW' : ";
        jsonfile << isoml.lepton_isFromW();
        jsonfile << ",";

        jsonfile << "'lepton_relIso03EA' : ";
        jsonfile << isoml.lepton_relIso03EA();
        jsonfile << ",";

        jsonfile << "'lepVec' : ";
        jsonfile << "[";
        jsonfile << isoml.lepton_eta();
        jsonfile << ",";
        jsonfile << isoml.lepton_phi();
        jsonfile << ",";
        jsonfile << isoml.lepton_pt();
        jsonfile << ",";
        jsonfile << isoml.lepton_relIso03EA();
        jsonfile << ",";
        jsonfile << isoml.lepton_chiso();
        jsonfile << ",";
        jsonfile << isoml.lepton_emiso();
        jsonfile << ",";
        jsonfile << isoml.lepton_nhiso();
        jsonfile << ",";
        jsonfile << isoml.lepton_ncorriso();
        jsonfile << ",";
        jsonfile << isoml.lepton_dxy();
        jsonfile << ",";
        jsonfile << isoml.lepton_dz();
        jsonfile << ",";
        jsonfile << isoml.lepton_ip3d();
        jsonfile << "]";
        jsonfile << ",";

        jsonfile << "'X' : ";
        jsonfile << "[";
        for (unsigned int ipf = 0; ipf < isoml.pf_pt().size(); ++ipf)
        {
            jsonfile << "[";
            jsonfile << isoml.pf_eta()[ipf];
            jsonfile << ",";
            jsonfile << isoml.pf_phi()[ipf];
            jsonfile << ",";
            jsonfile << isoml.pf_pt()[ipf];
            jsonfile << ",";
            jsonfile << isoml.pf_charge()[ipf];
            jsonfile << ",";
            jsonfile << isoml.pf_el()[ipf];
            jsonfile << ",";
            jsonfile << isoml.pf_mu()[ipf];
            jsonfile << ",";
            jsonfile << isoml.pf_chHad()[ipf];
            jsonfile << ",";
            jsonfile << isoml.pf_nEM()[ipf];
            jsonfile << ",";
            jsonfile << isoml.pf_nHad()[ipf];
            jsonfile << ",";
            jsonfile << isoml.pf_HFHad()[ipf];
            jsonfile << ",";
            jsonfile << isoml.pf_HFEM()[ipf];
            jsonfile << "]";
            jsonfile << ",";
        }
        jsonfile << "]";
        jsonfile << "}";
        jsonfile << std::endl;

        ievent++;
    }
}

// eof
