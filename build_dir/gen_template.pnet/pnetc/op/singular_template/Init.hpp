#ifndef _PNETC_OP_singular_template_Init
#define _PNETC_OP_singular_template_Init

#include <string>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Init
        ( const std::string& input
        , we::type::literal::control& c
        , we::type::literal::control& C
        , std::string& init
        , std::string& output
        );
    }
  }
}
#endif
