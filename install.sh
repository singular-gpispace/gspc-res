





#!/bin/bash

# Exit on any error
set -e
spack load gpi-space@24.12
# Activate the Spack environment
if ! spack env list | grep -q gpispace; then
echo "Creating Spack environment 'gpispace'..."
spack env create gpispace
fi
echo "Activating Spack environment 'gpispace'..."
spack env activate -p gpispace

# Set variables for directories
INSTALL_PREFIX=/scratch/gnawali/Try/gspc-res/install_dir
BUILD_DIR=/scratch/gnawali/Try/gspc-res/build_dir
SOURCE_DIR=/scratch/gnawali/Try/gspc-res/template

# Set the correct Singular installation directory

SINGULAR_HOME=/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/singular-4.4.0p2-f4kucmkjbh6zho27okeaxhwhwcickrfx

BOOST_DIR=/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/boost-1.63.0-suqj3khkv55qa6efb36qxknh7gadjn2x
GPISPACE_DIR=/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu
QT5_DIR=/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/qt-5.15.16-5batgesybps46hi7ayaffshharavefb7

# Clean previous build directory
if [ -d "$BUILD_DIR" ]; then
echo "Cleaning previous build directory..."
rm -rf "$BUILD_DIR"
fi

# Run CMake to configure the project
echo "Configuring the project with CMake…"
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