### This script trains and evaluates a BDT with the 11-variable lepton vector and a specified number of pf cand summary variables and plots the results in a ROC curve against Rel-Iso and MLP ###
### The MLP results are assumed to be in a file ../output_MLP.txt
### Arg1: number of annuli
### Arg2: number of angular sections
### Arg3: number of pf candidate types (use either 1 or 7) 1 = ignore type, 7 = consider all types
### Arg4: number of sig training events (hopefully the same as what you trained MLP with!)
### Arg5: number of bkg training events (hopefully the same as what you trained MLP with!)

version=$1"annuli_"$2"alpha_"$3"cands"

# Setup environment
source prelimSetup.sh
source setup_cmssw.sh
source scripts/setup.sh
compile.sh

# Create root file with TTree branches for the summary variables
ttreeName="IsoML_output_""$version"
ttreeDir="../babymaker/MLP_BDT_outputs"
ttree="$ttreeDir""/""$ttreeName"".root"
echo $ttree
if [ ! -d $ttreeDir ]; then
  mkdir $ttreeDir
fi

cp add_summary_variables.C "add_summary_variables_"$version".C"
sed -i "s@REPLACENR@$1@g" "add_summary_variables_"$version".C"
sed -i "s@REPLACENALPHA@$2@g" "add_summary_variables_"$version".C"
sed -i "s@REPLACENSUMMARYVARIABLES@$3@g" "add_summary_variables_"$version".C"

nEventsTot=`expr  $4 + $4 + $5 + $5 + 10000` # add an extra 10,000 to be sure that there are at least 2*$4 sig and 2*$5 bkg
run.sh -c "add_summary_variables_"$version".C" $ttree t 1000000 dummy /home/users/sjmay/ML/IsoML_output.root

# Make class files for root file
#../babymaker/scripts/makeclass.sh -f $ttree t IsoMLTree"$version" tas isoml

# Train BDT
root -l -b -q "train_bdt_v3.C+($1,$2,$3,$4,$5)"

# Evaluate BDT and zip BDT, MLP results into root file
source ../babymaker/scripts/setup.sh
sed "s@REPLACEME@IsoMLTree$version@g" "apply_bdt_mlp.C" > "apply_bdt_mlp_"$version".C"
map={
for (( i=0; i<$1; i++)) {
  for (( j=0; j<$2; j++)) {
    for (( k=0; k<$3; k++)) {
      map=$map'{ ''"'"isoml.summaryVar_R$i""_Alpha$j""_Cand$k"'"'', '"isoml.summarVar_R$i""_Alpha$j""_Cand$k"'}'
      val1=`expr $1 - 1`
      val2=`expr $2 - 1`
      val3=`expr $3 - 1`
      if [[ ( $i == $val1 ) && ( $j == $val2  ) && ( $k == $val3 ) ]]; then
        map=$map'}'
      else
        map=$map','
      fi
    }
  }
}
sed -i "s@REPLACEMAP@$map@g" "apply_bdt_mlp_"$version".C" 
sed -i "s@REPLACENR@$1@g" "apply_bdt_mlp_"$version".C"
sed -i "s@REPLACENALPHA@$2@g" "apply_bdt_mlp_"$version".C"
sed -i "s@REPLACENSUMMARYVARIABLES@$3@g" "apply_bdt_mlp_"$version".C"

#run.sh -c "apply_bdt_mlp_"$version".C" $ttree t 200000 dummy $ttree 

# Make ROC curve
source prelimSetup.sh
#python makerocMLP1DvsBDT.py $version
