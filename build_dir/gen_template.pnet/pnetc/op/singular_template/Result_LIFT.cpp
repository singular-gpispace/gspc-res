#include <pnetc/op/singular_template/Result_LIFT.hpp>
#include <we/type/literal/control.hpp>
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
        ( const int& counter
        , const int& lift_count
        , we::type::literal::control& output
        )
      {
#line 381 "/home/santosh/gspc-res/template/workflow/template.xpnet"

          
               
                 std::cout <<"LIFT: " << lift_count << std::endl;
                 std::cout <<"LeadSyz: " << counter << std::endl;
           
            
      }
    }
  }
}