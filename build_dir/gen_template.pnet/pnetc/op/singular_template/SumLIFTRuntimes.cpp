#include <pnetc/op/singular_template/SumLIFTRuntimes.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void SumLIFTRuntimes
        ( const long& newLiftTime
        , long& currentSum
        )
      {
#line 306 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                currentSum = currentSum + newLiftTime;
                //  std::cout << "newLiftTime: " << newLiftTime << " milliseconds" << std::endl;
                std::cout << "TotalLIFT Runtime Sum: " << currentSum << " nanosecond" << std::endl;
            
      }
    }
  }
}