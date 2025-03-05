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
        , const int& N
        , const unsigned long& k1
        , const unsigned long& k2
        , const unsigned long& k3
        , const unsigned long& k4
        , std::string& s
        );
    }
  }
}
#endif
