#ifndef _PNETC_OP_singular_template_ADD_Seq
#define _PNETC_OP_singular_template_ADD_Seq

#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void ADD_Seq
        ( const std::string& library_name
        , const std::string& base_filename
        , const std::string& input
        , const int& N
        , const unsigned long& k2
        , const we::type::literal::control& c
        , const unsigned long& Nplus
        , std::string& s
        );
    }
  }
}
#endif
