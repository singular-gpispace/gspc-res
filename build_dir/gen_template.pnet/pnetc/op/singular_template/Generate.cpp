#include <pnetc/op/singular_template/Generate.hpp>
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
      void Generate
        ( long& sumTime
        , std::string& syz
        )
      {
#line 973 "/home/santosh/gspc-res/template/workflow/template.xpnet"

      
					
     
       
       
      std::cout << "Iterative_level_syz: "<<syz<< std::endl;
      std::cout << "Generate_Syz-sumTime:"<< sumTime<< std::endl;
      
      }
    }
  }
}