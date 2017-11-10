# Assumes user is running in sinuglarity container on uaf 1
# User should supply 3 arugments: 
# arg1: a name for the results of the MLP output in (e.g. "myOutput")
# arg2: options for learning 
# arg3: number of training epochs
# arg4: number of layers
# arg6: number of nodes

python3 learn_1d.py $2 $3 $4 $1 $5
