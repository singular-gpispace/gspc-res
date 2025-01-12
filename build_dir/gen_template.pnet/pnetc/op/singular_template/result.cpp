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
        ( const int& nleads
        , const int& d_lift
        , const int& counter
        , const int& N
        , const int& E
        , std::string& s
        , we::type::literal::control& ctrl
        , int& n
        , int& e
        , int& d_back
        )
      {
#line 865 "/home/santosh/gspc-res/template/workflow/template.xpnet"

        
         
      std::cout << "Result_Success: " << s<< std::endl;
        // std::cout << "counter: " <<counter<< std::endl;
        // std::cout << "place_E: " <<E<< std::endl;
        // std::cout << "place_N: " <<N<< std::endl;
         
          e=0;
          n=0;
          d_back=0;
        //std::cout << "E_" <<e<< std::endl;
        //std::cout << "N_" <<n<< std::endl;
      
      }
    }
  }
}