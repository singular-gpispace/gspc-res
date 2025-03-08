#ifndef _PNETC_OP_singular_template_Empty
#define _PNETC_OP_singular_template_Empty

#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Empty
        ( const unsigned long& k2
        , const unsigned long& k3
        , const unsigned long& k4
        , const we::type::literal::control& IN
        , we::type::literal::control& N
        , we::type::literal::control& drain
        );
    }
  }
}
#endif
