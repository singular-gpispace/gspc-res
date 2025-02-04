#ifndef _PNETC_OP_singular_template_All_leadsyz
#define _PNETC_OP_singular_template_All_leadsyz

#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void All_leadsyz
        ( const std::string& input
        , const std::string& library_name
        , const std::string& base_filename
        , int& counter
        , long& runtime
        , std::list<pnet::type::value::value_type>& OUT
        );
    }
  }
}
#endif
