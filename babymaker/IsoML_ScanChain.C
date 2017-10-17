//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "IsoML_ScanChain.h"

//_________________________________________________________________________________________________
void ScanChain( TChain* chain, TString output_name, TString base_optstr, int nevents )
{

    // -~-~-~-~-~-~
    // Event Looper
    // -~-~-~-~-~-~
    Looper<CMS3> looper( chain, &cms3, nevents );
    chain->GetEntry( 0 );
    cms3.Init( chain->GetTree() );

    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    // CORE Helper for 2016 Analysis
    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    // This sets up JEC/Elec MVA/Good Runs List and all the goodies.
    CORE2016 core;

    TString option = base_optstr;
    option += "applyJEC";

    core.initializeCORE( option );

    // -~-~-~-~-~-
    // Output baby
    // -~-~-~-~-~-

    TFile* ofile = new TFile( output_name, "recreate" );
    ofile->cd();
    TTree* thettree = new TTree( "t", "IsoML baby" );
    TTreeX* ttree = new TTreeX( "t", "IsoML baby" );
    ttree->setTree(thettree);
    ttree->createBranch<Int_t  >( "run" );
    ttree->createBranch<Int_t  >( "lumiBlock" );
    ttree->createBranch<Int_t  >( "event" );
    ttree->createBranch<Int_t  >( "nvtx" );
    ttree->createBranch<Float_t>( "lepton_eta" );
    ttree->createBranch<Float_t>( "lepton_phi" );
    ttree->createBranch<Float_t>( "lepton_pt" );
    ttree->createBranch<Int_t  >( "lepton_flavor" );
    ttree->createBranch<Int_t  >( "lepton_isFromW" );
    ttree->createBranch<Int_t  >( "lepton_isFromB" );
    ttree->createBranch<Int_t  >( "lepton_isFromC" );
    ttree->createBranch<Int_t  >( "lepton_isFromL" );
    ttree->createBranch<Int_t  >( "lepton_isFromLF" );
    ttree->createBranch<Float_t>( "lepton_relIso03EA" );
    ttree->createBranch<Float_t>( "lepton_chiso" );
    ttree->createBranch<Float_t>( "lepton_nhiso" );
    ttree->createBranch<Float_t>( "lepton_emiso" );
    ttree->createBranch<Float_t>( "lepton_ncorriso" );
    ttree->createBranch<Float_t>( "lepton_dxy" );
    ttree->createBranch<Float_t>( "lepton_dz" );
    ttree->createBranch<Float_t>( "lepton_ip3d" );
    ttree->createBranch<std::vector<Float_t>>( "pf_eta" );
    ttree->createBranch<std::vector<Float_t>>( "pf_phi" );
    ttree->createBranch<std::vector<Float_t>>( "pf_pt" );
    ttree->createBranch<std::vector<Int_t  >>( "pf_charge" );
    ttree->createBranch<std::vector<Int_t  >>( "pf_el" );
    ttree->createBranch<std::vector<Int_t  >>( "pf_mu" );
    ttree->createBranch<std::vector<Int_t  >>( "pf_chHad" );
    ttree->createBranch<std::vector<Int_t  >>( "pf_nEM" );
    ttree->createBranch<std::vector<Int_t  >>( "pf_nHad" );
    ttree->createBranch<std::vector<Int_t  >>( "pf_HFHad" );
    ttree->createBranch<std::vector<Int_t  >>( "pf_HFEM" );

    // -~-~-~-~-~-~-~-
    // Main event loop
    // -~-~-~-~-~-~-~-

    while ( looper.nextEvent() )
    {

        // Clean the ttree entry.
        ttree->clear();

        // Set jet corrector.
        core.setJetCorrector();

        // If first good vertex is not 0th one, skip the event.
        // This is particularly important to have dxy/dz/ip3d variables to agree with each other.
        // (n.b. dxy/dz and ip3d are calculated off of different vertex reference frame.
        //       dxy/dz is calculated off of first good vertex, while ip3d is off of 0th vertex.)
        if (!( firstGoodVertex() == 0 )) continue;

        // Declare vectors holding indices of good leptons in CMS3 event.
        std::vector<unsigned int> good_elec_idx = goodElecIdx();
        std::vector<unsigned int> good_muon_idx = goodMuonIdx();

        // Select e, mu events. (i.e. need to have at least one of each leptons)
        if (!( good_elec_idx.size() > 0 )) continue;
        if (!( good_muon_idx.size() > 0 )) continue;

        // Fill the TTree
        fill( ttree, good_elec_idx, 11 );
        fill( ttree, good_muon_idx, 13 );

    }

    // -~-~-~
    // Output
    // -~-~-~
    ofile->cd();
    ttree->getTree()->Write();
    ofile->Close();

}

//_________________________________________________________________________________________________
// Returns a vector of indices for good loose muons in CMS3.
std::vector<unsigned int> goodMuonIdx()
{
    // Loop over the muons and select good baseline muons.
    std::vector<unsigned int> good_muon_idx;
    for ( unsigned int imu = 0; imu < cms3.mus_p4().size(); ++imu )
    {
        if (!( isLooseMuonPOG( imu )                      )) continue;
        if (!( fabs(cms3.mus_p4()[imu].pt())     >  10    )) continue;
        if (!( fabs(cms3.mus_p4()[imu].eta())    <=  2.4  )) continue;
        if (!( fabs(cms3.mus_dxyPV()[imu])       <=  0.05 )) continue;
        if (!( fabs(cms3.mus_dzPV()[imu])        <=  0.1  )) continue;
        if (!( muRelIso03EA( imu, 1 )            <   0.5  )) continue;
        good_muon_idx.push_back( imu );
    }
    return good_muon_idx;
}

//_________________________________________________________________________________________________
// Returns a vector of indices for good loose muons in CMS3.
std::vector<unsigned int> goodElecIdx()
{
    // Loop over the electrons and select good baseline electrons.
    std::vector<unsigned int> good_elec_idx;
    for ( unsigned int iel = 0; iel < cms3.els_p4().size(); ++iel )
    {
//      if (!( isTriggerSafenoIso_v1( iel )               )) continue; // If at some point we ever need it we can turn it on later.
        if (!( isVetoElectronPOGspring16noIso_v1( iel )   )) continue;
        if (!( fabs(cms3.els_p4()[iel].pt())     >  10    )) continue;
        if (!( fabs(cms3.els_p4()[iel].eta())    <=  2.5  )) continue;
        if (!( fabs(cms3.els_dxyPV()[iel])       <=  0.05 )) continue;
        if (!( fabs(cms3.els_dzPV()[iel])        <=  0.1  )) continue;
        if (!( eleRelIso03EA( iel, 2 )           <   0.5  )) continue;
        good_elec_idx.push_back( iel );
    }
    return good_elec_idx;
}

//_________________________________________________________________________________________________
// Loop over good lepton indices and fill the output tree with the candidate leptons
void fill( RooUtil::TTreeX* ttree, std::vector<unsigned int >& idx, int pdgid )
{
    // Loop over the indices.
    for ( auto& i : idx )
    {
        //==========================================
        // Clear all the variables for the branches.
        //==========================================
        ttree->clear();

        //==========================
        // Set event wide variables.
        //==========================
        ttree->setBranch<Int_t>( "run", cms3.evt_run() );
        ttree->setBranch<Int_t>( "lumiBlock", cms3.evt_lumiBlock() );
        ttree->setBranch<Int_t>( "event", cms3.evt_event() );
        unsigned int nvtx = 0;
        for ( unsigned int ivtx = 0; ivtx < cms3.evt_nvtxs(); ivtx++ )
            if ( isGoodVertex( ivtx ) ) nvtx++;
        ttree->setBranch<Int_t>( "nvtx", nvtx );

        //==========================
        // Set the lepton variables.
        //==========================
        ttree->setBranch<Float_t>( "lepton_eta"       , abs( pdgid ) == 11 ? cms3.els_p4()[i].eta() : cms3.mus_p4()[i].eta() ); 
        ttree->setBranch<Float_t>( "lepton_phi"       , abs( pdgid ) == 11 ? cms3.els_p4()[i].phi() : cms3.mus_p4()[i].phi() ); 
        ttree->setBranch<Float_t>( "lepton_pt"        , abs( pdgid ) == 11 ? cms3.els_p4()[i].pt()  : cms3.mus_p4()[i].pt()  ); 
        ttree->setBranch<Int_t  >( "lepton_flavor"    , abs( pdgid ) == 11 ? 0 : 1         ); 
        ttree->setBranch<Int_t  >( "lepton_isFromW"   , isFromW( abs( pdgid ), i )         ); 
        ttree->setBranch<Int_t  >( "lepton_isFromB"   , isFromB( abs( pdgid ), i )         ); 
        ttree->setBranch<Int_t  >( "lepton_isFromC"   , isFromC( abs( pdgid ), i )         ); 
        ttree->setBranch<Int_t  >( "lepton_isFromL"   , isFromLight( abs( pdgid ), i )     ); 
        ttree->setBranch<Int_t  >( "lepton_isFromLF"  , isFromLightFake( abs( pdgid ), i ) ); 
        ttree->setBranch<Float_t>( "lepton_relIso03EA", abs( pdgid ) == 11 ? eleRelIso03EA( i, 2 ) : muRelIso03EA( i, 1 ) );
        float chiso = abs( pdgid ) == 11 ? cms3.els_pfChargedHadronIso()[i] : cms3.mus_isoR03_pf_ChargedHadronPt()[i];
        float nhiso = abs( pdgid ) == 11 ? cms3.els_pfNeutralHadronIso()[i] : cms3.mus_isoR03_pf_NeutralHadronEt()[i];
        float emiso = abs( pdgid ) == 11 ? cms3.els_pfPhotonIso()[i]        : cms3.mus_isoR03_pf_PhotonEt()[i];
        float ea    = abs( pdgid ) == 11 ? elEA03( i, 2 )              : muEA03( i, 1 );
        float ncorriso = nhiso + emiso - evt_fixgridfastjet_all_rho() * ea;
        ttree->setBranch<Float_t>( "lepton_chiso"     , chiso );
        ttree->setBranch<Float_t>( "lepton_nhiso"     , nhiso );
        ttree->setBranch<Float_t>( "lepton_emiso"     , emiso );
        ttree->setBranch<Float_t>( "lepton_ncorriso"  , ncorriso );
        ttree->setBranch<Float_t>( "lepton_dxy"       , abs( pdgid ) == 11 ? cms3.els_dxyPV()[i] : cms3.mus_dxyPV()[i] );
        ttree->setBranch<Float_t>( "lepton_dz"        , abs( pdgid ) == 11 ? cms3.els_dzPV()[i]  : cms3.mus_dzPV()[i]  );
        ttree->setBranch<Float_t>( "lepton_ip3d"      , abs( pdgid ) == 11 ? cms3.els_ip3d()[i]  : cms3.mus_ip3d()[i]  );

        //============================
        // Set the Particle Flow list.
        //============================
        for ( unsigned int ipf = 0; ipf < cms3.pfcands_particleId().size(); ++ipf )
        {
            float thisDR = fabs( ROOT::Math::VectorUtil::DeltaR( cms3.pfcands_p4()[ipf], abs( pdgid ) == 11 ? cms3.els_p4()[i] : cms3.mus_p4()[i] ) );
            if (!( thisDR < 1.0 )) continue;
            ttree->pushbackToBranch<Float_t>( "pf_eta"   , cms3.pfcands_p4()[ipf].eta() );
            ttree->pushbackToBranch<Float_t>( "pf_phi"   , cms3.pfcands_p4()[ipf].phi() );
            ttree->pushbackToBranch<Float_t>( "pf_pt"    , cms3.pfcands_p4()[ipf].pt() );
            ttree->pushbackToBranch<Int_t  >( "pf_charge", cms3.pfcands_charge()[ipf] );
            ttree->pushbackToBranch<Int_t  >( "pf_el"    , abs( cms3.pfcands_particleId()[ipf] ) ==  11 ? 1 : 0 );
            ttree->pushbackToBranch<Int_t  >( "pf_mu"    , abs( cms3.pfcands_particleId()[ipf] ) ==  13 ? 1 : 0 );
            ttree->pushbackToBranch<Int_t  >( "pf_nEM"   , abs( cms3.pfcands_particleId()[ipf] ) ==  22 ? 1 : 0 );
            ttree->pushbackToBranch<Int_t  >( "pf_chHad" , abs( cms3.pfcands_particleId()[ipf] ) == 211 ? 1 : 0 );
            ttree->pushbackToBranch<Int_t  >( "pf_nHad"  , abs( cms3.pfcands_particleId()[ipf] ) == 130 ? 1 : 0 );
            ttree->pushbackToBranch<Int_t  >( "pf_HFEM"  , abs( cms3.pfcands_particleId()[ipf] ) ==   1 ? 1 : 0 );
            ttree->pushbackToBranch<Int_t  >( "pf_HFHad" , abs( cms3.pfcands_particleId()[ipf] ) ==   2 ? 1 : 0 );
        }

        //================
        // Fill the TTree.
        //================
        ttree->getTree()->Fill();
    }
}

// eof
