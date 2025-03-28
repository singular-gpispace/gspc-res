#ifndef _PNETC_OP_singular_template_Fill_IN_Add
#define _PNETC_OP_singular_template_Fill_IN_Add

#include <we/type/literal/control.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Fill_IN_Add
        ( const int& N
        , const we::type::literal::control& k1
        , std::list<pnet::type::value::value_type>& outgoing
        );
    }
  }
}
#endif
