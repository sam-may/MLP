# Image-based approach to particle flow candidates

The file `learn_grid.py` loops through events in a `.json` file and creates a 1-d array containing the 11 lepton vector variables ("lepton vector") and a 3-d array containing the particle flow candidate summary variables ("pf cand image"). It appends each lepton vector and each pf cand image to arrays which are then 2-d and 4-d, respectively.

Main difference between `learn_grid.py` and Julian's original `learn.py`:
1. In `learn.py` the variable `XX` was a 3-d array: we stored an 11-variable pf candidate vector (dimension 3) for each pf candidate (dimension 2) in each event (dimension 1). In `learn_grid.py`, the variables `XX` is a 4-d array: we store the energy sum for each type of pf candidate (dimension 4) in each angular section around the lepton (dimension 3) in each radial annulus around the lepton (dimension 2) for each event (dimension 1).  

After parsing the data, the remainder of the file is left identical to Julian's original `learn.py`.

# Pf Cand Image
For each event, we summarize the information contained in the pf candidates with an "image". Rather than providing information about the pf candidates in a 2-d array which contains `N` 11-variable vectors, one for each of the `N` pf candidates in the event, information about the pf candidates is now provided in a 3-d array: the "pf cand image". The pf cand image is a 3-d `nR x nAlpha x 6` array which contains the sum of pf candidate energies, sorted by the type of pf candidate and location relative to the lepton. For example, the `[i][j][k]`th element of the array tells us how much energy of particle type `k` is contained in the `j`th angular portion of the `i`th annulus about the lepton.

A traditional color image has 3 color (RGB) values for each pixel, a pf cand image has 6 "color" values for each pixel.

![alt tag](http://uaf-10.t2.ucsd.edu/~sjmay/MLP/pfCandImage.png)

### Modifying pf cand image
1. The number of annuli around the lepton can be modified by changing the value of `nR`. `nR = 5` is shown in the image above.
2. The number of angular sections can be modified by changing the value of `nAlpha`. For example, `nAlpha = 4` splits the cone around the lepton into 4 90-degree sections. `nAlpha = 1` corresponds to a 1-d grid. `nAlpha = 8` is shown in the image above. 
