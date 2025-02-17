#!/bin/bash

# Ensure that LD_LIBRARY_PATH is set correctly for library resolution
export LD_LIBRARY_PATH=/home/santosh/gspc-res/build_dir/src:$LD_LIBRARY_PATH

# Set the SINGULARPATH environment variable
export SINGULARPATH=/home/santosh/gspc-res/install_dir

# Get the path for the correct Singular version installed by Spack
SINGULAR_VERSION="4.4.0p2"
SINGULAR_BIN=$(spack location -i singular@$SINGULAR_VERSION)/bin
SINGULAR_LIB=$(spack location -i singular@$SINGULAR_VERSION)/lib

# Ensure that the Spack-managed Singular is in the PATH
export PATH=$SINGULAR_BIN:$PATH

# Ensure that the library path for Singular is correctly set
export LD_LIBRARY_PATH=$SINGULAR_LIB:$LD_LIBRARY_PATH

# Copy the example library to the example directory
cp /home/santosh/gspc-res/install_dir/share/examples/templategp.lib /home/santosh/gspc-res/example_dir

# Set GSPC_HOME
export GSPC_HOME="/home/santosh/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/gpi-space-24.12-ugaltunbzethad4p3szsepjqdbgthcch/"

# Check if the package is available in Spack
# spack find gpi-space@24.12 || { echo "Package gpi-space@24.12 not found. Installing..."; spack install gpi-space@24.12; }

# Activate the Spack environment
# spack env activate -p gpispace
spack load gpi-space@24.12 || { echo "Failed to load GPI-Space"; exit 1; }

# Run Singular with the example script
Singular example.sing ./run.sh
