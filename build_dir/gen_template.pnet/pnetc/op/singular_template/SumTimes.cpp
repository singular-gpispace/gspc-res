#include <pnetc/op/singular_template/SumTimes.hpp>
#include <we/type/literal/control.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void SumTimes
        ( const we::type::literal::control& c
        , const long& leadSyzTime
        , const long& liftTime
        , const long& subLiftTime
        , const long& reduceTime
        , long& sumTime
        )
      {
#line 922 "/home/santosh/gspc-res/template/workflow/template.xpnet"

              std::cout << "LeadSyz_Runtime: " << leadSyzTime << "nanoseconds" << std::endl;
              std::cout << "LIFT_Runtime: " << liftTime << "nanoseconds" << std::endl;
              std::cout << "SubLIFT_Runtime: " << subLiftTime << "nanoseconds" << std::endl;
               std::cout << "Reduce_Runtime: " << reduceTime << "nanoseconds" << std::endl;
                sumTime = leadSyzTime + liftTime + subLiftTime + reduceTime;
                std::cout << "Total Runtime: " << sumTime << "nanoseconds" << std::endl;


            
      }
    }
  }
}