# Read through each of the MLP_1D variations ("darts") and make ROC curve and print TPR/FPR values for same TPR/FPR as Rel-Iso 0.06
while read f; do
  cp checkpoints/output_MLP_$f.txt ../output_MLP.txt
  cd ../babymaker
  source scripts/setup.sh
  run.sh -c zip_mlpoutputs_to_baby.C $f.root t 200000 dummy ../../IsoML_output.root
  cd ../bdt
  python makerocComparison.py $f
  cd ../MLP_1d
done < 'dartsList.txt'
