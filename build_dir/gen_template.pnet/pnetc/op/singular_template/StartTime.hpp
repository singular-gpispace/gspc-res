#ifndef _PNETC_OP_singular_template_StartTime
#define _PNETC_OP_singular_template_StartTime

#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void StartTime
        ( const std::string& input
        , we::type::literal::control& c
        , std::string& output
        , long& start_time
        );
    }
  }
}
#endif
