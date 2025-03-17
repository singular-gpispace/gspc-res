cmake \
  -D GPISpace_ROOT=~/spack/ \
 -D GSPC_WITH_MONITOR_APP=OFF \
  -D GPISpace_ROOT=/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu

  -D Boost_ROOT=/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/boost-1.63.0-suqj3khkv55qa6efb36qxknh7gadjn2x

  -D CMAKE_INSTALL_PREFIX=/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/bin \
  -D Boost_NO_BOOST_CMAKE=TRUE \
  -B "/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/build" \
  -S "/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/"










# Set variables for directories
INSTALL_PREFIX=/scratch/gnawali/gspc-res/install_dir
BUILD_DIR=/scratch/gnawali/gspc-res/build_dir
SOURCE_DIR=/scratch/gnawali/gspc-res/template

# Set the correct Singular installation directory

SINGULAR_HOME=/home/santosh/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/singular-4.4.0p2-syrkttc4im2j3tzob5jykruuxnushksj

BOOST_DIR=/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/boost-1.63.0-suqj3khkv55qa6efb36qxknh7gadjn2x
GPISPACE_DIR="/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu"

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
