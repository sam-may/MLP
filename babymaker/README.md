# Baby maker for MLP input

The setup assumes you do NOT set up any ROOT or CMSSW on your own.
If something does not work, try using a clean ```.bashrc``` that does not have any ROOT or CMSSW setup.

The setup will use ROOT version defined in the ```root.sh```.

# First time installing

I assume you are sitting in ```babymaker/``` directory.
Few additional packages are needed.
If you have not cloned recursively when cloning this repo, issue the following command to checkout all the submodules.

    $ cd ../ # to go to the MLP/ parent directory
    $ git submodule update --init --recursive
    $ cd babymaker/

Afterwards,

    $ source scripts/setup.sh
    $ compile.sh

# Second time

Once packages are all compiled, the next time you login, just type the following.

    $ source scripts/setup.sh

# Running a test case

To run a test example of just 1000 events on one of the ttbar sample.

    $ run.sh -c IsoML_ScanChain.C output.root Events 1000 dummy /hadoop/cms/store/group/snt/run2_moriond17/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V08-00-16/merged_ntuple_1.root

To run over everything change ```1000``` to ```-1```.

To generate the json file from a ```output.root```, do

    $ run.sh -c convert_to_json.C output.json t -1 dummy output.root

# Computing reweighting factor

The script to compute the reweighting factor is here.

    $ run.sh -c compute_pt_rewgt.C rewgt.root t -1 dummy ~/public_html/tasutil/IsoML_output.root

The output ```rewgt.root``` contains histograms.
The plotter will then print out reweighting factors.

    $ python plot_rewgt.py

The rewgt factors are copied and saved to ```rewgt.C```.

# Zipping the MLP output from TensorFlow with the original lepton baby

The following example zips the output from tensorflow which is saved in a text file ```../output_MLP.txt``` wrt to this directory with the IsoML_output.root

    run.sh -c zip_mlpoutputs_to_baby.C zipoutput.root t 100000 dummy ~/public_html/tasutil/IsoML_output.root

An example of ```../output_MLP.txt``` will look like this

    49965 0.9934547543525696
    49967 0.9961197376251221
    49969 0.9731799960136414
    49971 0.9921365976333618
    49973 0.9660279750823975
    49975 0.9927423000335693
    49977 0.8610332608222961
    49979 0.9907755255699158
    49981 0.9922897219657898
    49983 0.9968879818916321

The first column is the event index number in the TTree where we got the data for the TensorFlow, and the second column is the MLP output for that event.
