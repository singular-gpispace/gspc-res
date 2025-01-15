#ifndef _PNETC_OP_singular_template_Generate
#define _PNETC_OP_singular_template_Generate

#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Generate
        ( const int& count_LIFT
        , const int& count_SubLIFT
        , const int& count_Reduce
        , long& sumTime
        , std::string& syz
        , int& count_all_transition
        );
    }
  }
}
#endif
