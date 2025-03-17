#pragma once

#include <boost/filesystem/path.hpp>

namespace config
{
    static boost::filesystem::path const installation("/scratch/gnawali/Try/gspc-res/install_dir");
    static boost::filesystem::path const sing_path("/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/singular-4.4.0p2-f4kucmkjbh6zho27okeaxhwhwcickrfx");
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
