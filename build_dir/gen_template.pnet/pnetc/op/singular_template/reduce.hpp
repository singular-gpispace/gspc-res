#ifndef _PNETC_OP_singular_template_reduce
#define _PNETC_OP_singular_template_reduce

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
        , std::string& s
        );
    }
  }
}
#endif
