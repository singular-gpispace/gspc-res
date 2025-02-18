#ifndef _PNETC_OP_singular_template_result
#define _PNETC_OP_singular_template_result

#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void result
        ( const unsigned long& k3
        , const unsigned long& k4
        , const we::type::literal::control& c
        , std::string& s
        );
    }
  }
}
#endif
