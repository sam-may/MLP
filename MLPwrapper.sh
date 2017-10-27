# Assumes user is running in sinuglarity container on uaf 1
# User should supply 3 arugments: 
# arg1: a root file to put the results of the MLP output in (e.g. "myOutput.root")
# arg2: options for learning (0 = Julian's original MLP, 5 = original + info for top 5 pf cands)
# arg3: number of training epochs

python3 learn.py $2 $3
python3 eval.py $2
cd babymaker
source scripts/setup.sh
run.sh -c zip_mlpoutputs_to_baby.C $1 t 100000 dummy ../../IsoML_output.root

