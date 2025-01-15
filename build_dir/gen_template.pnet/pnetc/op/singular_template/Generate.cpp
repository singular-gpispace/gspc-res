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
        ( const int& count_LIFT
        , const int& count_SubLIFT
        , const int& count_Reduce
        , long& sumTime
        , std::string& syz
        , int& count_all_transition
        )
      {
#line 1062 "/home/santosh/gspc-res/template/workflow/template.xpnet"

      
					
     
          
              // std::cout << "#LIFT: " << count_LIFT<< std::endl;
              // std::cout << "#SubLIFT: " << count_SubLIFT<< std::endl;
              //  std::cout << "#Reduce: " << count_Reduce << std::endl;
        count_all_transition=count_LIFT+count_SubLIFT+count_Reduce;
      std::cout << "Iterative_level_syz: "<<syz<< std::endl;
      std::cout << "Generate_Syz-sumTime:"<< sumTime<< std::endl;
      
      }
    }
  }
}