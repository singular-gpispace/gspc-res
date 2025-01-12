#ifndef _PNETC_OP_singular_template_Init
#define _PNETC_OP_singular_template_Init

#include <we/type/literal/control.hpp>
#include <string>

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
        );
    }
  }
}
#endif
