#ifndef _PNETC_OP_singular_template_Initiate_transition
#define _PNETC_OP_singular_template_Initiate_transition

#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Initiate_transition
        ( const std::string& input
        , const std::string& library_name
        , const std::string& base_filename
        , std::string& output
        , int& degree
        );
    }
  }
}
#endif
