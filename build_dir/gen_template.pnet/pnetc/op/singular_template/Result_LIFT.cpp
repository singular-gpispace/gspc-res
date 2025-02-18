#include <pnetc/op/singular_template/Result_LIFT.hpp>
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
      void Result_LIFT
        ( const std::string& computed_output
        , const int& counter
        , const int& lift_count
        , std::string& output
        )
      {
#line 390 "/home/santosh/gspc-res/template/workflow/template.xpnet"

          
                output=computed_output;
                 std::cout <<"LIFT: " << lift_count << std::endl;
        std::cout <<"LeadSyz: " << computed_output << std::endl;
           
            
      }
    }
  }
}