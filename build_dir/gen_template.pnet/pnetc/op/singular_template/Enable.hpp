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
        ( const unsigned long& N
        , const we::type::literal::control& IN
        , const int& r
        , std::list<pnet::type::value::value_type>& out
        , we::type::literal::control& c
        );
    }
  }
}
#endif
