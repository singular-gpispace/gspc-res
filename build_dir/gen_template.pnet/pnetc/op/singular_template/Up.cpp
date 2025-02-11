#include <pnetc/op/singular_template/Up.hpp>
#include <we/type/literal/control.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Up
        ( const int& n
        , int& a
        , we::type::literal::control& c
        )
      {
#line 522 "/home/santosh/gspc-res/template/workflow/template.xpnet"

        //std::cout << "T1 _n: " << n << std::endl;
        a = a + n;
       //std::cout << "T1_sum: " << a<< std::endl;
        
      }
    }
  }
}