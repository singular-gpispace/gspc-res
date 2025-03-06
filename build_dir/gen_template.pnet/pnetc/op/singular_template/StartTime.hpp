#ifndef _PNETC_OP_singular_template_StartTime
#define _PNETC_OP_singular_template_StartTime

#include <list>
#include <we/type/value.hpp>
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
        , const int& N
        , we::type::literal::control& c
        , std::string& output
        , std::list<pnet::type::value::value_type>& outgoing
        , long& start_time
        );
    }
  }
}
#endif
