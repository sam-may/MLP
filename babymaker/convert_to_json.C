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
    // Main event loop
    while (looper.nextEvent())
    {
        std::cout << "{";

        std::cout << "'Row' : ";
        std::cout << ievent;
        std::cout << ",";

        std::cout << "'lepton_flavor' : ";
        std::cout << isoml.lepton_flavor();
        std::cout << ",";

        std::cout << "'lepton_isFromW' : ";
        std::cout << isoml.lepton_isFromW();
        std::cout << ",";

        std::cout << "'lepton_relIso03EA' : ";
        std::cout << isoml.lepton_relIso03EA();
        std::cout << ",";

        std::cout << "'lepVec' : ";
        std::cout << "[";
        std::cout << isoml.lepton_eta();
        std::cout << ",";
        std::cout << isoml.lepton_phi();
        std::cout << ",";
        std::cout << isoml.lepton_pt();
        std::cout << ",";
        std::cout << isoml.lepton_relIso03EA();
        std::cout << ",";
        std::cout << isoml.lepton_chiso();
        std::cout << ",";
        std::cout << isoml.lepton_emiso();
        std::cout << ",";
        std::cout << isoml.lepton_nhiso();
        std::cout << ",";
        std::cout << isoml.lepton_ncorriso();
        std::cout << ",";
        std::cout << isoml.lepton_dxy();
        std::cout << ",";
        std::cout << isoml.lepton_dz();
        std::cout << ",";
        std::cout << isoml.lepton_ip3d();
        std::cout << "]";
        std::cout << ",";

        std::cout << "'X' : ";
        std::cout << "[";
        for (unsigned int ipf = 0; ipf < isoml.pf_pt().size(); ++ipf)
        {
            std::cout << "[";
            std::cout << isoml.pf_eta()[ipf];
            std::cout << ",";
            std::cout << isoml.pf_phi()[ipf];
            std::cout << ",";
            std::cout << isoml.pf_pt()[ipf];
            std::cout << ",";
            std::cout << isoml.pf_charge()[ipf];
            std::cout << ",";
            std::cout << isoml.pf_el()[ipf];
            std::cout << ",";
            std::cout << isoml.pf_mu()[ipf];
            std::cout << ",";
            std::cout << isoml.pf_chHad()[ipf];
            std::cout << ",";
            std::cout << isoml.pf_nEM()[ipf];
            std::cout << ",";
            std::cout << isoml.pf_nHad()[ipf];
            std::cout << ",";
            std::cout << isoml.pf_HFHad()[ipf];
            std::cout << ",";
            std::cout << isoml.pf_HFEM()[ipf];
            std::cout << "]";
            std::cout << ",";
        }
        std::cout << "]";
        std::cout << "}";
        std::cout << std::endl;

        ievent++;
    }
}

// eof
