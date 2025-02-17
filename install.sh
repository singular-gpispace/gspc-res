# #!/bin/bash

# # Exit on any error
# set -e

# # Activate the Spack environment
# if ! spack env list | grep -q gpispace; then
#     echo "Creating Spack environment 'gpispace'..."
#     spack env create gpispace
# fi
# echo "Activating Spack environment 'gpispace'..."
# spack env activate -p gpispace

# # Set variables for directories
# INSTALL_PREFIX=/home/santosh/gspc-res/install_dir
# BUILD_DIR=/home/santosh/gspc-res/build_dir
# SOURCE_DIR=/home/santosh/gspc-res/template
# SINGULAR_HOME=/home/santosh/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/singular-snapshot_22_03-wjiftrh4ygptyx62jjwf2vnoj5qms67n
# BOOST_DIR=/home/santosh/singular-gpispace/spack/var/spack/environments/gpispace/.spack-env/view
# GPISPACE_DIR=/home/santosh/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/gpi-space-23.06-yy4zzk24obghifdb5wvrqg4ojestij6d/share/GPISpace/cmake
# QT5_DIR=/home/santosh/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/qt-5.15.11-v2xrk5hnmdfygvc7v2wcy2hz27cu5ofi/lib/cmake/Qt5

# # Clean previous build directory
# if [ -d "$BUILD_DIR" ]; then
#     echo "Cleaning previous build directory..."
#     rm -rf "$BUILD_DIR"
# fi

# # Run CMake to configure the project
# echo "Configuring the project with CMake..."
# cmake -D CMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
#       -D CMAKE_BUILD_TYPE=Release \
#       -D SINGULAR_HOME=$SINGULAR_HOME \
#       -D Boost_NO_BOOST_CMAKE=on \
#       -D GPISpace_DIR=$GPISPACE_DIR \
#       -D Qt5_DIR="$QT5_DIR" \
#       -B $BUILD_DIR \
#       -S $SOURCE_DIR

# # Build and install the project
# echo "Building and installing the project..."
# cmake --build $BUILD_DIR --target install -j $(nproc)

# echo "Build and installation complete."







#!/bin/bash

# Exit on any error
set -e

# Activate the Spack environment
if ! spack env list | grep -q gpispace; then
    echo "Creating Spack environment 'gpispace'..."
    spack env create gpispace
fi
echo "Activating Spack environment 'gpispace'..."
spack env activate -p gpispace

# Set variables for directories
INSTALL_PREFIX=/home/santosh/gspc-res/install_dir
BUILD_DIR=/home/santosh/gspc-res/build_dir
SOURCE_DIR=/home/santosh/gspc-res/template

# Set the correct Singular installation directory

SINGULAR_HOME=/home/santosh/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/singular-4.4.0p2-syrkttc4im2j3tzob5jykruuxnushksj

BOOST_DIR=/home/santosh/singular-gpispace/spack/var/spack/environments/gpispace/.spack-env/view
GPISPACE_DIR="/home/santosh/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/gpi-space-24.12-ugaltunbzethad4p3szsepjqdbgthcch/share/GPISpace/cmake"

QT5_DIR=/home/santosh/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/qt-5.15.11-v2xrk5hnmdfygvc7v2wcy2hz27cu5ofi/lib/cmake/Qt5

# Clean previous build directory
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning previous build directory..."
    rm -rf "$BUILD_DIR"
fi

# Run CMake to configure the project
echo "Configuring the project with CMake..."
cmake -D CMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
      -D CMAKE_BUILD_TYPE=Release \
      -D SINGULAR_HOME=$SINGULAR_HOME \
      -D Boost_NO_BOOST_CMAKE=on \
      -D GPISpace_DIR=$GPISPACE_DIR \
      -D Qt5_DIR="$QT5_DIR" \
      -B $BUILD_DIR \
      -S $SOURCE_DIR

# Build and install the project
echo "Building and installing the project..."
cmake --build $BUILD_DIR --target install -j $(nproc)

echo "Build and installation complete."