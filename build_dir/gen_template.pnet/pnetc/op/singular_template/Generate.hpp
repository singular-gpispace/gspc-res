#ifndef _PNETC_OP_singular_template_Generate
#define _PNETC_OP_singular_template_Generate

#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Generate
        ( const std::string& res
        , const std::string& library_name
        , const std::string& base_filename
        , const std::string& input
        , const std::string& syz
        , const we::type::literal::control& drainage
        , int& len
        , std::string& Res
        , std::string& Input
        , we::type::literal::control& c
        , we::type::literal::control& addFill
        , unsigned long& M
        , unsigned long& N
        );
    }
  }
}
#endif
