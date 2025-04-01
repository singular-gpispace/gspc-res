#ifndef _PNETC_OP_singular_template_SUBLIFT
#define _PNETC_OP_singular_template_SUBLIFT

#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void SUBLIFT
        ( const std::string& input_name
        , const std::string& place_Tau
        , const std::string& needed_library
        , const std::string& base_filename
        , const unsigned long& N
        , std::list<pnet::type::value::value_type>& LIFT
        );
    }
  }
}
#endif
