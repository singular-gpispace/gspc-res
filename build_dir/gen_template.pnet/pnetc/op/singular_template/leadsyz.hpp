#ifndef _PNETC_OP_singular_template_leadsyz
#define _PNETC_OP_singular_template_leadsyz

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
      void leadsyz
        ( const std::string& input
        , const std::string& library_name
        , const std::string& base_filename
        , const int& len
        , we::type::literal::control& s
        , std::list<pnet::type::value::value_type>& OUT
        );
    }
  }
}
#endif
