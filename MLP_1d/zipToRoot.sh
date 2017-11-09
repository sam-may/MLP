cd ../babymaker
source scripts/setup.sh
run.sh -c zip_mlpoutputs_to_baby.C $1.root t 100000 dummy ../../IsoML_output.root
