# Image-based approach to particle flow candidates

The file learn_grid.py loops through events in a `.json` file and creates a 1-d array containing the 11 lepton vector variables ("lepton vector") and a 3-d array containing the particle flow candidate summary variables ("pf cand image"). It appends each lepton vector and each pf cand image to arrays which are then 2-d and 4-d, respectively.

After parsing the data, the remainder of the file is left identical to Julian's original `learn.py`.

# Pf Cand Image
The "pf cand image" is a `nR x nAlpha x 6` array which contains the sum of pf candidate energies, sorted by the type of pf candidate and location relative to the lepton. For example, the `[i][j][k]`th element of the array tells us how much energy of particle type `k` is contained in the `j`th angular portion of the `i`th annulus.

![alt tag](http://uaf-10.t2.ucsd.edu/~sjmay/MLP/pfCandImage.png)

### Modifying pf cand image
1. The number of annuli around the lepton can be modified by changing the value of `nR`. `nR = 5` is shown in the image above.
2. The number of angular sections can be modified by changing the value of `nAlpha`. For example, `nAlpha = 4` splits the cone around the lepton into 4 90-degree sections. `nAlpha = 1` corresponds to a 1-d grid. `nAlpha = 8` is shown in the image above. 
