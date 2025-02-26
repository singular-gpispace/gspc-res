#include <pnetc/op/singular_template/result.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void result
        ( const unsigned long& k1
        , const unsigned long& k3
        , const unsigned long& k4
        , std::string& s
        )
      {
#line 590 "/home/santosh/gspc-res/template/workflow/template.xpnet"

        
         
      std::cout << "Result_Success: " << s<< std::endl;
      //   std::cout << "counter: " <<counter<< std::endl;
      //   std::cout << "place_E: " <<E<< std::endl;
      //   std::cout << "place_N: " <<N<< std::endl;
       
      
      
      }
    }
  }
}