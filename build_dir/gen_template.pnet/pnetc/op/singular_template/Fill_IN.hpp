#ifndef _PNETC_OP_singular_template_Fill_IN
#define _PNETC_OP_singular_template_Fill_IN

#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Fill_IN
        ( const unsigned long& k2
        , const unsigned long& M
        , const unsigned long& k1
        , unsigned long& N
        , std::list<pnet::type::value::value_type>& outgoing
        );
    }
  }
}
#endif
