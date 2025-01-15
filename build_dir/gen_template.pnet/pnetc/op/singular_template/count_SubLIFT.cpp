#include <pnetc/op/singular_template/count_SubLIFT.hpp>
#include <we/type/literal/control.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void count_SubLIFT
        ( const we::type::literal::control& s
        , int& a
        )
      {
#line 516 "/home/santosh/gspc-res/template/workflow/template.xpnet"

          a=a+1;
      //  std::cout <<"Count_SubLIFT: " << a << std::endl;
        
      }
    }
  }
}