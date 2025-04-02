#include <pnetc/op/singular_template/Generate.hpp>
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
      void Generate
        ( const std::string& res
        , const std::string& library_name
        , const std::string& base_filename
        , const std::string& input
        , const std::string& syz
        , const we::type::literal::control& drainage
        , int& len
        , std::string& Res
        , std::string& Input
        , we::type::literal::control& c
        , we::type::literal::control& addFill
        , unsigned long& M
        , unsigned long& N
        )
      {
#line 1009 "/home/santosh/gspc-res/template/workflow/template.xpnet"

       //std::cout << "Generate_Input:"<<input<< std::endl;
       //std::cout << "Generate_Syz:"<<syz<< std::endl;
       //std::cout << "Generate_control:"<<c<< std::endl;
        Res= RESOLVE_INTERFACE_FUNCTION(singular_template_Generate)(res,syz,library_name, base_filename
							); 
       Input=syz;
        M=300UL;
        N=300UL;
        c={};
      len=len-1;
      // std::cout << "Iterative_level_syz: "<<len<< std::endl;
      // std::cout << "Generate_Syz:"<< Res<< std::endl;
      
      }
    }
  }
}