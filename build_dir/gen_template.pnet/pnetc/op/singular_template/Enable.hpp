#ifndef _PNETC_OP_singular_template_Enable
#define _PNETC_OP_singular_template_Enable

#include <list>
#include <we/type/value.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Enable
        ( const we::type::literal::control& IN
        , const int& r
        , unsigned long& N
        , std::list<pnet::type::value::value_type>& out
        );
    }
  }
}
#endif
