//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "compute_pt_rewgt.h"

#include "rewgt.C"

//_________________________________________________________________________________________________
void ScanChain(TChain* chain, TString output_name, TString base_optstr, int nevents)
{
    // Event Looper
    Looper<IsoMLTree> looper(chain, &isoml, nevents);
    chain->GetEntry(0);
    isoml.Init(chain->GetTree());

    // output file
    ofstream jsonfile;
    jsonfile.open(output_name.Data());

    // Histograms
    RooUtil::AutoHist hists;

    // Pt binning
    // https://github.com/aminnj/electronML/blob/c4a0f7bfb2157e156f447cbcf25f1b3b2644b6b4/Analysis/postprocessing/utils.py#L48
    // binedges = np.array(range(0,80,5)+range(80,200,20)+range(200,300,50)+range(300,500,100)+range(500,1000+1,500))
    int nptbins = 27;
    double ptbins[28] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 100, 120, 140, 160, 180, 200, 250, 300, 400, 500, 1000};

    int nptbinsfine = 67;
    double ptbinsfine[68];
    for (unsigned int i = 0; i < nptbinsfine + 1; ++i)
    {
        if (i <= 50) ptbinsfine[i] = i;
        else if (i <= 56) ptbinsfine[i] = 50 + 5 * (i - 50);
        else if (i <= 62) ptbinsfine[i] = 80 + 20 * (i - 56);
        else              ptbinsfine[i] =200 + 50 * (i - 62);
        std::cout << ptbinsfine[i] << std::endl;
    }

    int netabins = 3;
    double etabins[4] = {0, 1.0, 2.1, 2.4};

    // Main event loop
    while (looper.nextEvent())
    {
        if (isoml.lepton_flavor() == 0)
        {
            // Not implemented
        }
        else if (isoml.lepton_flavor() == 1)
        {
            if (isoml.lepton_isFromW() == 1)
            {
                hists.fill(isoml.lepton_pt(), "h_mu_pt_sig", 1., nptbins, ptbins);
                hists.fill(fabs(isoml.lepton_eta()), "h_mu_eta_sig", 1., netabins, etabins);
                hists.fill(isoml.lepton_pt(), "h_mu_rewgt_pt_sig", 1., 90, 0., 90.);
                hists.fill(fabs(isoml.lepton_eta()), "h_mu_rewgt_eta_sig", 1., netabins, etabins);
                hists.fill(isoml.lepton_pt(), "h_mu_rewgtfine_pt_sig", 1., 90, 0., 90.);
                hists.fill(fabs(isoml.lepton_eta()), "h_mu_rewgtfine_eta_sig", 1., netabins, etabins);
                hists.fill(isoml.lepton_pt(), fabs(isoml.lepton_eta()), "h_mu_sf_sig", 1., nptbins, ptbins, netabins, etabins);
                hists.fill(isoml.lepton_pt(), fabs(isoml.lepton_eta()), "h_mu_sf_sig_fine", 1., nptbinsfine, ptbinsfine, netabins, etabins);
            }
            else if (isoml.lepton_isFromW() == 0)
            {
                hists.fill(isoml.lepton_pt(), "h_mu_pt_bkg", 1., nptbins, ptbins);
                hists.fill(fabs(isoml.lepton_eta()), "h_mu_eta_bkg", 1., netabins, etabins);
                hists.fill(isoml.lepton_pt(), "h_mu_rewgt_pt_bkg", rewgt(isoml.lepton_pt(), isoml.lepton_eta()), 90, 0., 90.);
                hists.fill(fabs(isoml.lepton_eta()), "h_mu_rewgt_eta_bkg", rewgt(isoml.lepton_pt(), isoml.lepton_eta()), netabins, etabins);
                hists.fill(isoml.lepton_pt(), "h_mu_rewgtfine_pt_bkg", rewgtfine(isoml.lepton_pt(), isoml.lepton_eta()), 90, 0., 90.);
                hists.fill(fabs(isoml.lepton_eta()), "h_mu_rewgtfine_eta_bkg", rewgtfine(isoml.lepton_pt(), isoml.lepton_eta()), netabins, etabins);
                hists.fill(isoml.lepton_pt(), fabs(isoml.lepton_eta()), "h_mu_sf_bkg", 1., nptbins, ptbins, netabins, etabins);
                hists.fill(isoml.lepton_pt(), fabs(isoml.lepton_eta()), "h_mu_sf_bkg_fine", 1., nptbinsfine, ptbinsfine, netabins, etabins);
            }
        }
    }

    hists.save(output_name);
}

// eof
