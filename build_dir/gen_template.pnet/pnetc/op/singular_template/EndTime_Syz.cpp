#include <pnetc/op/singular_template/EndTime_Syz.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <iostream>
#include <chrono>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void EndTime_Syz
        ( const long& start_time
        , std::string& computed_output
        )
      {
#line 1083 "/scratch/gnawali/Try/gspc-res/template/workflow/template.xpnet"

                  auto computation_time = std::chrono::high_resolution_clock::now();
                  auto duration = computation_time.time_since_epoch();
                  long current_time = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
                 auto  end_timer = current_time - start_time; // Calculate elapsed time
                
                  std::cout << "Calculate Syz: = " << end_timer << " nanoseconds" << std::endl;
              
      }
    }
  }
}