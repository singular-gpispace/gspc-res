#ifndef _PNETC_OP_singular_template_SumTimes
#define _PNETC_OP_singular_template_SumTimes

#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void SumTimes
        ( const we::type::literal::control& c
        , const long& leadSyzTime
        , const long& schFrameTime
        , const long& liftTime
        , const long& subLiftTime
        , const long& reduceTime
        , long& sumTime
        );
    }
  }
}
#endif
