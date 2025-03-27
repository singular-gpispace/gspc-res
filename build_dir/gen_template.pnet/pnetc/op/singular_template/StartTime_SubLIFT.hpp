#ifndef _PNETC_OP_singular_template_StartTime_SubLIFT
#define _PNETC_OP_singular_template_StartTime_SubLIFT

#include <string>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void StartTime_SubLIFT
        ( const we::type::literal::control& c
        , const std::string& in
        , long& start_time
        );
    }
  }
}
#endif
