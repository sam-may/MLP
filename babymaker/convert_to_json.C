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

    // lepton counters
    unsigned int n_prompt_mu = 0;
    unsigned int n_prompt_el = 0;
    unsigned int n_fake_mu = 0;
    unsigned int n_fake_el = 0;

    // output file
    ofstream jsonfile;
    jsonfile.open(output_name.Data());

    // Main event loop
    while (looper.nextEvent())
    {
        if (isoml.lepton_flavor() == 0 && isoml.lepton_isFromW() == 0) n_fake_el++;
        if (isoml.lepton_flavor() == 1 && isoml.lepton_isFromW() == 0) n_fake_mu++;
        if (isoml.lepton_flavor() == 0 && isoml.lepton_isFromW() == 1) n_prompt_el++;
        if (isoml.lepton_flavor() == 1 && isoml.lepton_isFromW() == 1) n_prompt_mu++;

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

        jsonfile << "'nvtx' : ";
        jsonfile << isoml.nvtx();
        jsonfile << ",";

        jsonfile << "'rewgt' : ";
        jsonfile << rewgtfine(isoml.lepton_pt(), isoml.lepton_eta());
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

    std::cout << n_fake_el++ << std::endl;
    std::cout << n_fake_mu++ << std::endl;
    std::cout << n_prompt_el++ << std::endl;
    std::cout << n_prompt_mu++ << std::endl;
}

// eof
