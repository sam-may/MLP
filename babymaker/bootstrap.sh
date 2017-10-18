#!/bin/bash

#  .
# ..: P. Chang, philip@physics.ucsd.edu

git clone git@github.com:sgnoohc/scripts.git

# Setup root
source scripts/setup.sh

# Download RooUtil packages
git clone git@github.com:sgnoohc/rooutil.git && cd rooutil/ && git checkout 0.0.1 && make -j 20 && cd ../ && git clone git@github.com:cmstas/CORE.git && cd CORE && git checkout CMS3_archive && make -j 20 && cd ../ && git clone git@github.com:sgnoohc/COREHelper.git && cd COREHelper/ && git checkout 0.0.1 && make -j 20 && cd ../

#eof
