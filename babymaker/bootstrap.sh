#!/bin/bash

#  .
# ..: P. Chang, philip@physics.ucsd.edu

# Setup root
source root.sh

# Download RooUtil packages
git clone git@github.com:sgnoohc/rooutil.git
cd rooutil/
git checkout 0.0.1
make -j 20
cd ../

# Download CORE
git clone git@github.com:cmstas/CORE.git
cd CORE
git checkout CMS3_archive
make -j 20
cd ../

# Download CORE Helper package
git clone git@github.com:sgnoohc/COREHelper.git
cd COREHelper/
git checkout 0.0.1
make -j 20
cd ../

#eof
