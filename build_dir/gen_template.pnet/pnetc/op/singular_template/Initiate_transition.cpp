#include <pnetc/op/singular_template/Initiate_transition.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <iostream>
#include <chrono>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Initiate_transition
        ( const std::string& input
        , const std::string& library_name
        , const std::string& base_filename
        , std::string& output
        , int& degree
        )
      {
#line 76 "/home/santosh/gspc-res/template/workflow/template.xpnet"

    // std::cout<<"Starting.."<<std::endl;
      output = RESOLVE_INTERFACE_FUNCTION(singular_template_compute_StdBasis)
      (input, library_name, base_filename);
      // std::cout<<"Starting1.."<<std::endl;
      degree=RESOLVE_INTERFACE_FUNCTION(singular_template_compute_getHomological_degree)(input, library_name);
      // degree=1;
      std::cout<<"homological degree:="<<degree<<std::endl;

      }
    }
  }
}