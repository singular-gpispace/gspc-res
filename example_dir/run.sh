#!/bin/bash

# Ensure that LD_LIBRARY_PATH is set correctly for library resolution
export LD_LIBRARY_PATH=/scratch/gnawali/Try/gspc-res/build_dir/src:$LD_LIBRARY_PATH

# Set the SINGULARPATH environment variable
export SINGULARPATH=/scratch/gnawali/Try/gspc-res/install_dir

# Specify the version of Singular to use
SINGULAR_VERSION="4.4.0p2"

# Set the paths for Singular binaries and libraries
SINGULAR_BIN=/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/singular-4.4.0p2-f4kucmkjbh6zho27okeaxhwhwcickrfx/bin
SINGULAR_LIB=/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/singular-4.4.0p2-f4kucmkjbh6zho27okeaxhwhwcickrfx/lib

# Ensure that the Singular binaries are in your PATH
export PATH=$SINGULAR_BIN:$PATH

# Ensure that the library path for Singular is correctly set
export LD_LIBRARY_PATH=$SINGULAR_LIB:$LD_LIBRARY_PATH

# Copy the example library to the example directory
cp /scratch/gnawali/Try/gspc-res/install_dir/share/examples/templategp.lib /scratch/gnawali/Try/gspc-res/example_dir

# cp /home/santosh/gspc-res/install_dir/share/examples/templategp.lib /home/santosh/gspc-res/example_dir

# Set GSPC_HOME
export GSPC_HOME="/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu"

# Load the gpi-space package via Spack (if needed)
spack load gpi-space@24.12 || { echo "Failed to load GPI-Space"; exit 1; }

# Run Singular with the example script
Singular example.sing
# "$SINGULAR_BIN" example.sing