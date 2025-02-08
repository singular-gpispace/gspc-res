#ifndef _PNETC_OP_singular_template_EndTime
#define _PNETC_OP_singular_template_EndTime

#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void EndTime
        ( const we::type::literal::control& computed_output
        , const long& start_timer
        , long& end_timer
        );
    }
  }
}
#endif
