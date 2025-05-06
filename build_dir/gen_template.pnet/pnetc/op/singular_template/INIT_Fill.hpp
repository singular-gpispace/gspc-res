#ifndef _PNETC_OP_singular_template_INIT_Fill
#define _PNETC_OP_singular_template_INIT_Fill

#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void INIT_Fill
        ( unsigned long& N
        , std::list<pnet::type::value::value_type>& outgoing
        );
    }
  }
}
#endif
