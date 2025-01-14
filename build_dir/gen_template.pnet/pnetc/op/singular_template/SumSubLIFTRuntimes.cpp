#include <pnetc/op/singular_template/SumSubLIFTRuntimes.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void SumSubLIFTRuntimes
        ( const long& newsubLiftTime
        , long& currentSum
        )
      {
#line 498 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                currentSum = currentSum + newsubLiftTime;
                //std::cout << "TotalsubLIFT Runtime Sum: " << currentSum << " milliseconds" << std::endl;
            
      }
    }
  }
}