# Assumes user is running in sinuglarity container on uaf 1
# User should supply 1 arugment: a root file to put the results of the MLP output in (e.g. "myOutput.root")

python3 learn.py
python3 eval.py
cd babymaker
source scripts/setup.sh
run.sh -c zip_mlpoutputs_to_baby.C $1 t 100000 dummy ../../IsoML_output.root

