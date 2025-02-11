#include <pnetc/op/singular_template/Init.hpp>
#include <we/type/literal/control.hpp>
#include <string>
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
        , std::string& res
        , we::type::literal::control& c
        , std::string& init
        , std::string& output
        )
      {
#line 73 "/home/santosh/gspc-res/template/workflow/template.xpnet"

						 output = RESOLVE_INTERFACE_FUNCTION(singular_template_Init) (input);
              init=output;
              res=output;
            		
      }
    }
  }
}