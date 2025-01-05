#pragma once

#include <boost/filesystem/path.hpp>

namespace config
{
    static boost::filesystem::path const installation("/home/santosh/gspc-res/install_dir");
    static boost::filesystem::path const sing_path("/home/santosh/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/singular-4.4.0p2-syrkttc4im2j3tzob5jykruuxnushksj");
    inline boost::filesystem::path const& singularLibrary()
    {
        static boost::filesystem::path const library
         ( sing_path / "lib" / "libSingular.so"
        );

        return library;
    }
    inline boost::filesystem::path const& implementation()
    {
        static boost::filesystem::path const library(
         installation / "libexec" / "workflow" / "libtemplate_implementation.so"
         );
        return library;
  }
}
