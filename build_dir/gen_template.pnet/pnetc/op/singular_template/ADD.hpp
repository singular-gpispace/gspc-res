#ifndef _PNETC_OP_singular_template_ADD
#define _PNETC_OP_singular_template_ADD

#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void ADD
        ( const std::string& library_name
        , const std::string& base_filename
        , const std::string& input
        , const unsigned long& N
        , std::string& s
        );
    }
  }
}
#endif
