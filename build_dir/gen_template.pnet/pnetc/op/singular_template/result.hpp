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
        ( const int& nleads
        , const int& d_lift
        , const int& counter
        , const int& N
        , const int& E
        , std::string& s
        , we::type::literal::control& ctrl
        , int& n
        , int& e
        , we::type::literal::control& c_time
        , int& d_back
        );
    }
  }
}
#endif
