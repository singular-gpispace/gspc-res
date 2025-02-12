#include <pnetc/op/singular_template/Init.hpp>
#include <string>
#include <we/type/literal/control.hpp>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Init
        ( const std::string& input
        , we::type::literal::control& c
        , std::string& output
        , we::type::literal::control& d
        )
      {
#line 73 "/home/santosh/gspc-res/template/workflow/template.xpnet"

						 output = RESOLVE_INTERFACE_FUNCTION(singular_template_Init) (input);
          
              
            		
      }
    }
  }
}