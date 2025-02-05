#include <pnetc/op/singular_template/Init.hpp>
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
      void Init
        ( const std::string& input
        , std::string& output
        )
      {
#line 60 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                     std::cout <<"Initiate:"<< std::endl;
						 output = RESOLVE_INTERFACE_FUNCTION(singular_template_Init) (input);
          
              
            		
      }
    }
  }
}