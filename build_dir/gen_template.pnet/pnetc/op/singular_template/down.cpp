#include <pnetc/op/singular_template/down.hpp>
#include <we/type/literal/control.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void down
        ( const int& e
        , int& a
        , we::type::literal::control& c
        )
      {
#line 437 "/home/santosh/gspc-res/template/workflow/template.xpnet"

       //std::cout << "T2_e:" << e << std::endl;
        a = a + e;
        //std::cout << "T2_sum:" << a << std::endl;
        
      }
    }
  }
}