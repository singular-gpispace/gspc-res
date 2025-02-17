#include <pnetc/op/singular_template/Decrement.hpp>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Decrement
        ( int& s
        )
      {
#line 682 "/home/santosh/gspc-res/template/workflow/template.xpnet"

       // std::cout << "Decrement Transition - Before: " << s << std::endl;
        s = s - 1;
      //std::cout << "Decrement Transition - After: " << s << std::endl;
      
      }
    }
  }
}