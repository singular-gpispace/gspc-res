#include <pnetc/op/singular_template/Rename_file.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <chrono>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Rename_file
        ( const std::string& Sublift
        , int& N
        )
      {
#line 458 "/home/santosh/gspc-res/template/workflow/template.xpnet"

        N=N+1;
        // std::cout << "Nin Rename:" << N << std::endl;
        auto s  = RESOLVE_INTERFACE_FUNCTION(singular_template_Rename_File) (Sublift,N);
  
      }
    }
  }
}