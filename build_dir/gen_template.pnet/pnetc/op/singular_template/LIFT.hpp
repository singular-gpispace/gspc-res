#ifndef _PNETC_OP_singular_template_LIFT
#define _PNETC_OP_singular_template_LIFT

#include <we/type/literal/control.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void LIFT
        ( const std::string& input_name
        , const std::string& lead
        , const std::string& all_lead
        , const std::string& needed_library
        , const std::string& base_filename
        , int& N
        , int& E
        , std::list<pnet::type::value::value_type>& LIFT
        , we::type::literal::control& D
        );
    }
  }
}
#endif
