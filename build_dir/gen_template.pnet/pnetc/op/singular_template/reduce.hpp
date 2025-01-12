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
        , const std::string& l
        , const std::string& r
        , std::string& LHS_delete
        , std::string& RHS_delete
        , we::type::literal::control& nsummand
        , std::string& s
        );
    }
  }
}
#endif
