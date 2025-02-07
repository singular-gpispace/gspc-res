#ifndef _PNETC_OP_singular_template_EndTime_SchFrame
#define _PNETC_OP_singular_template_EndTime_SchFrame

#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void EndTime_SchFrame
        ( const long& start_time
        , const std::string& computed_output
        , const int& counter
        , const we::type::literal::control& s
        , long& end_timer
        );
    }
  }
}
#endif
