#include <pnetc/op/singular_template/Result_SubLIFTesult.hpp>
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
      void Result_SubLIFTesult
        ( const int& N
        , const int& E
        , std::string& s
        )
      {
#line 544 "/home/santosh/gspc-res/template/workflow/template.xpnet"

        
         
      std::cout << "Result_Success: " << s<< std::endl;
  
        std::cout << "place_E: " <<E<< std::endl;
        std::cout << "place_N: " <<N<< std::endl;
         
         
     
      
      }
    }
  }
}