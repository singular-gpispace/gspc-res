#include <pnetc/op/singular_template/removeFiles.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void removeFiles
        ( const int& LIFT
        , const int& nleads
        , const std::string& l
        , const std::string& r
        )
      {
#line 552 "/home/santosh/gspc-res/template/workflow/template.xpnet"

        // std::cout <<"LIFT: " << LIFT << std::endl;
        // std::cout <<"LeadSyz: " << nleads << std::endl;
       auto s = RESOLVE_INTERFACE_FUNCTION(singular_template_removeFiles)(l, r);
        // std::cout << "Delete:" << s << std::endl;
      
      }
    }
  }
}