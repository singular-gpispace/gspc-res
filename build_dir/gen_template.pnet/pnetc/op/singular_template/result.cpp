#include <pnetc/op/singular_template/result.hpp>
#include <we/type/literal/control.hpp>
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
        ( const std::string& s
        , const int& counter
        , const int& N
        , const int& E
        , we::type::literal::control& ctrl
        , std::string& OUTPUT
        )
      {
#line 702 "/home/santosh/gspc-res/template/workflow/template.xpnet"

        
          OUTPUT=s;
      std::cout << "Result_Success: " << OUTPUT<< std::endl;
        std::cout << "counter: " <<counter<< std::endl;
        std::cout << "place_E: " <<E<< std::endl;
        std::cout << "place_N: " <<N<< std::endl;
         
        
        //std::cout << "E_" <<e<< std::endl;
        //std::cout << "N_" <<n<< std::endl;
       
      
      }
    }
  }
}