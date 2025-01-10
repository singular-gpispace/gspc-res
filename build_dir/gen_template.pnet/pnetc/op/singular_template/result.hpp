#ifndef _PNETC_OP_singular_template_result
#define _PNETC_OP_singular_template_result

#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void result
        ( const std::string& s
        , const int& counter
        , const int& N
        , const int& E
        , we::type::literal::control& ctrl
        , std::string& OUTPUT
        );
    }
  }
}
#endif
