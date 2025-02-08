#ifndef _PNETC_OP_singular_template_EndTime_SubLIFT
#define _PNETC_OP_singular_template_EndTime_SubLIFT

#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void EndTime_SubLIFT
        ( const long& start_time_LIFT
        , const long& start_timer
        , std::string& computed_output
        , long& end_timer
        );
    }
  }
}
#endif
