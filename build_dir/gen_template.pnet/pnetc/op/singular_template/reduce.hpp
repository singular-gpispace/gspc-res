#ifndef _PNETC_OP_singular_template_reduce
#define _PNETC_OP_singular_template_reduce

#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void reduce
        ( const std::string& library_name
        , const std::string& base_filename
        , const std::string& input
        , const we::type::literal::control& IN
        , std::string& s
        );
    }
  }
}
#endif
