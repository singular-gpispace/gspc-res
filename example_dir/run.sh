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

# Load GPI-Space
spack load gpi-space@24.12 || { echo "Failed to load GPI-Space"; exit 1; }


Singular example.sing 
# # Run Singular with GDB for debugging and show output in both terminal and file
# echo "Running Singular inside GDB for debugging..."
# gdb -ex run --args Singular example.sing 2>&1 | tee gdb_log.txt

# # Display the GDB output location
# echo "GDB session output saved to gdb_log.txt"

# # Run Singular with Valgrind for memory leak detection
# echo "Running Singular inside Valgrind for memory leak detection..."
# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_log.txt Singular example.sing 2>&1 | tee -a valgrind_log.txt

# # Display the Valgrind output location
# echo "Valgrind output saved to valgrind_log.txt"
