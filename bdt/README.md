# bdt

This is to establish a strawman for the deep learning to beat.  
We will establish something that is reasonable that to first order what we would do if we were to improve upon the conventional relIso variable.  
Then, we compare with the deep learning.  
If deep learning performs marginally better (or worse), we will see what we can do. 

## To train the BDT.

    source setup_cmssw.sh # MUST BE CALLED BEFORE THE NEXT COMMAND
    source scripts/setup.sh
    compile.sh
    root -l -b -q train_bdt.C+

## Use GUI to take a look at BDT training result

After running above script there will be a ```BDT.root```.  
Copy that over to your local mac computer, and on your mac (assuming a reasonable ROOT is installed), fire up the command

    root -l $ROOTSYS/tmva/test/TMVAGui.C\(\"BDT.root\"\)

This should fire up a GUI with several buttons to start graphing few stuff.

## To apply the BDT to the original ROOT file used to train and create a new ttree with BDT branch added.

    source setup_cmssw.sh # MUST BE CALLED BEFORE THE NEXT COMMAND
    source scripts/setup.sh
    compile.sh
    run.sh -c apply_bdt.C output.root t -1 dummy /hadoop/cms/store/user/phchang/mlp/output_MLP_muon_25ktrain_25ktest_1000epoch_julianOriginal.root

