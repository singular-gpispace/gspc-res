#ifndef _PNETC_OP_singular_template_StartTime_LIFT
#define _PNETC_OP_singular_template_StartTime_LIFT

#include <string>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void StartTime_LIFT
        ( const we::type::literal::control& c
        , const std::string& in
        , long& start_time
        );
    }
  }
}
#endif
