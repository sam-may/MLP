# Baby maker for MLP input

The setup assumes you do NOT set up any ROOT or CMSSW on your own.
If something does not work, try using a clean ```.bashrc``` that does not have any ROOT or CMSSW setup.

The setup will use ROOT version defined in the ```root.sh```.

# First time installing

Few additional packages are needed.

    $ source bootstrap.sh

# Second time

Once packages are all compiled, the next time you login, just type the following.

    $ source root.sh

# Running a test case

To run a test example of just 1000 events on one of the ttbar sample.

    $ sh run.sh -c IsoML_ScanChain.C output.root Events 1000 dummy /hadoop/cms/store/group/snt/run2_moriond17/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V08-00-16/merged_ntuple_1.root

To run over everything change ```1000``` to ```-1```.
