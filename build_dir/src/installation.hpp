#pragma once

#include <drts/drts.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>

#define SP_INSTALL_PATH "/home/santosh/gspc-res/install_dir"

namespace singular_template
{
  //! \note collects information relative to the path of the executable
  class installation
  {
  public:
    installation();
    installation (boost::filesystem::path const&);
    installation (boost::filesystem::path const&,
     boost::filesystem::path const&);

    boost::filesystem::path workflow_all() const;
    boost::filesystem::path workflow_dir() const;
    gspc::installation gspc_installation
      (boost::program_options::variables_map&) const;

  private:
    boost::filesystem::path const _path;
    boost::filesystem::path const _gspc_path;
  };
}
