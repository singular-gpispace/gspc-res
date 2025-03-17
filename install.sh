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
from spack import *
from spack.package import CMakePackage
from spack.package import *






# Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)

from spack import *
from spack.package import CMakePackage
from spack.package import *


class Flint(Package):
    """FLINT (Fast Library for Number Theory)."""

    homepage = "https://www.flintlib.org"
    url      = "https://mirrors.mit.edu/sage/spkg/upstream/flint/flint-2.6.3.tar.gz"
    git      = "https://github.com/wbhart/flint2.git"

    version('develop', branch='trunk')
    version('2.6.3', sha256='ce1a750a01fa53715cad934856d4b2ed76f1d1520bac0527ace7d5b53e342ee3')
    version('2.5.2', sha256='cbf1fe0034533c53c5c41761017065f85207a1b770483e98b2392315f6575e87')
    version('2.4.5', sha256='e489354df00f0d84976ccdd0477028693977c87ccd14f3924a89f848bb0e01e3')

    # Overlap in functionality between gmp and mpir
    # All other dependencies must also be built with
    # one or the other
    # variant('mpir', default=False,
    #         description='Compile with the MPIR library')

    # Build dependencies
    depends_on('autoconf', type='build')

    # Other dependencies
    depends_on('gmp')   # mpir is a drop-in replacement for this
    depends_on('mpfr')  # Could also be built against mpir

    phases = ['configure', 'build', 'install']

    def configure_args(self):
        prefix = self.prefix
        spec = self.spec
        args = ["--prefix=%s" % prefix,
                   "--with-gmp=%s" % spec['gmp'].prefix,
                   "--with-mpfr=%s" % spec['mpfr'].prefix]

        # if '+mpir' in spec:
        #     args.extend([
        #         "--with-mpir=%s" % spec['mpir'].prefix
        #     ])

        return args

    def configure(self, spec, prefix):
        configure_script = Executable("./configure")
        configure(*self.configure_args())

    def build(self, spec, prefix):
        make()
        if self.run_tests:
            make("check")

    def install(self, spec, prefix):
        #options = ["--prefix=%s" % prefix,
        #           "--with-gmp=%s" % spec['gmp'].prefix,
        #           "--with-mpfr=%s" % spec['mpfr'].prefix]
        #configure(*options)
        #make()
        #if self.run_tests:
        #    make("check")
        make("install")