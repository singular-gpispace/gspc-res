#ifndef _PNETC_OP_singular_template_Result_LIFT
#define _PNETC_OP_singular_template_Result_LIFT

#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Result_LIFT
        ( const std::string& computed_output
        , const int& counter
        , const int& lift_count
        , std::string& output
        );
    }
  }
}
#endif
