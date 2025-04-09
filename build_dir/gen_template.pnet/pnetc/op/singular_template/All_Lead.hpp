#ifndef _PNETC_OP_singular_template_All_Lead
#define _PNETC_OP_singular_template_All_Lead

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
      void All_Lead
        ( const std::string& init
        , const we::type::literal::control& C
        , const std::string& library_name
        , const std::string& base_filename
        , const int& degree
        , std::list<pnet::type::value::value_type>& OUTPUT
        , int& counter
        );
    }
  }
}
#endif
