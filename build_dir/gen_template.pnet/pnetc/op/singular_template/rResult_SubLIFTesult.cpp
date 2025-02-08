#include <pnetc/op/singular_template/rResult_SubLIFTesult.hpp>
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
      void rResult_SubLIFTesult
        ( const int& N
        , const int& E
        , std::string& s
        )
      {
#line 651 "/home/santosh/gspc-res/template/workflow/template.xpnet"

        
         
      std::cout << "Result_Success: " << s<< std::endl;
        // std::cout << "counter: " <<counter<< std::endl;
        // std::cout << "place_E: " <<E<< std::endl;
        // std::cout << "place_N: " <<N<< std::endl;
         
         
        //std::cout << "E_" <<e<< std::endl;
        //std::cout << "N_" <<n<< std::endl;
      
      }
    }
  }
}