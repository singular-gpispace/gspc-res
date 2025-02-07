#ifndef _PNETC_OP_singular_template_EndTime
#define _PNETC_OP_singular_template_EndTime

#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void EndTime
        ( const long& start_time_LIFT
        , const long& start_timer
        , std::string& computed_output
        , long& end_timer
        );
    }
  }
}
#endif
