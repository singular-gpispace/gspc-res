#include <pnetc/op/singular_template/SumreduceRuntimes.hpp>
#include <iostream>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void SumreduceRuntimes
        ( const long& newreduceTime
        , long& currentSum
        )
      {
#line 801 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                currentSum = currentSum + newreduceTime;
               //std::cout << "TotalreduceRuntime Sum: " << currentSum << " milliseconds" << std::endl;
            
      }
    }
  }
}