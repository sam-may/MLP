cp checkpoints/output_MLP_$1.txt ../output_MLP.txt
cd ../babymaker
source scripts/setup.sh
run.sh -c zip_mlpoutputs_to_baby.C $1.root t 200000 dummy ../../IsoML_output.root
cd ../bdt
python makerocComparison.py