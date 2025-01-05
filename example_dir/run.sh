# #!/bin/bash

# # Ensure that LD_LIBRARY_PATH is set correctly for library resolution
# export LD_LIBRARY_PATH=/home/santosh/gspc-res/build_dir/src:$LD_LIBRARY_PATH


# # Set the SINGULARPATH environment variable
# export SINGULARPATH=/home/santosh/gspc-res/install_dir

# # Ensure that the Spack-managed Singular is in the PATH
# export PATH=/home/santosh/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/singular-snapshot_22_03-wjiftrh4ygptyx62jjwf2vnoj5qms67n/bin:$PATH

# # Copy the example library to the example directory
# cp /home/santosh/gspc-res/install_dir/share/examples/templategp.lib /home/santosh/gspc-res/example_dir

# # Activate the Spack environment
# spack env activate -p gpispace

# # Run Singular with the example script
# Singular example.sing


# #gdb Singular example.sing
# #gdb Singular example.sing


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

# Activate the Spack environment
spack env activate -p gpispace

# Run Singular with the example script
Singular example.sing
