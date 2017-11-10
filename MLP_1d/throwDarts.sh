if [ -f dartsList.txt ] ; then
  rm dartsList.txt
fi

for LAYER in 1 2 3 5 10; do
    for NODES in 5 10 12 20 50; do
        for EPOCHS in 1000 2000 10000; do
            name="MLP1D_11var_"${LAYER}"layers_"${NODES}"nodes_"${EPOCHS}"epochs"
            echo $name >> dartsList.txt
            python3 learn_1d.py 11 ${EPOCHS} ${LAYER} $name ${NODES}
        done
    done
done
