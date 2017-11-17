# This script assumes you have trained an MLP_1D and have its results in a text file MLP_1d/checkpoints/$1.txt
# The training is preferably done in a singularity container on uaf-1

#################
### Arguments ###
#################
# Arg 1: filename
# Arg 2: train bdt? 0 for train, anything else for skip

### Zip MLP results to root file
cp checkpoints/output_MLP_$1.txt ../output_MLP.txt
cd ../babymaker
source scripts/setup.sh
run.sh -c zip_mlpoutputs_to_baby.C MLP_BDT_outputs/$1.root t 200000 dummy /home/users/sjmay/ML/IsoML_output.root

### Train BDT
cd ../bdt
source setup_cmssw.sh
source scripts/setup.sh
compile.sh
if [[ ( $2 == 0 ) ]]; then
  root -l -b -q train_bdt.C+
fi

### Zip BDT results to same root file
source setup_cmssw.sh
source scripts/setup.sh
compile.sh
run.sh -c apply_bdt_v2.C /home/users/sjmay/ML/MLP/babymaker/MLP_BDT_outputs/$1.root t 200000 dummy /home/users/sjmay/ML/MLP/babymaker/MLP_BDT_outputs/$1.root

### Make ROC curve and tables
source ~/philSetup.sh
python makerocMLP1DvsBDT.py $1
