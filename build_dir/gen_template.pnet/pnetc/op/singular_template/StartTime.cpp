#include <pnetc/op/singular_template/StartTime.hpp>
#include <we/type/literal/control.hpp>
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
      void StartTime
        ( const std::string& input
        , we::type::literal::control& c
        , std::string& output
        )
      {
#line 52 "/home/santosh/gspc-res/template/workflow/template.xpnet"

 
     output=input;
    
      }
    }
  }
}