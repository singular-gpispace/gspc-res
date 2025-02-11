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
#line 353 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                currentSum = currentSum + newLiftTime;
                // std::cout << "TotalLIFT Runtime Sum: " << currentSum << " milliseconds" << std::endl;
            
      }
    }
  }
}