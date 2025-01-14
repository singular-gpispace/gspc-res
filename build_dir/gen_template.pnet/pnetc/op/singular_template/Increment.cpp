#include <pnetc/op/singular_template/Increment.hpp>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Increment
        ( int& s
        )
      {
#line 580 "/home/santosh/gspc-res/template/workflow/template.xpnet"

       
        //std::cout << "Increment Transition - Before: " << s << std::endl;
        s = s + 1;
      //std::cout << "Increment Transition - After: " << s << std::endl;
       
      
      }
    }
  }
}