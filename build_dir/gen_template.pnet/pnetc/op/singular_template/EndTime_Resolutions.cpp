#include <pnetc/op/singular_template/EndTime_Resolutions.hpp>
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
      void EndTime_Resolutions
        ( const long& start_time
        , std::string& computed_output
        )
      {
#line 807 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                auto computation_time = std::chrono::high_resolution_clock::now();
                auto duration = computation_time.time_since_epoch();
                long current_time = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
               auto  end_timer = current_time - start_time; // Calculate elapsed time
              
                std::cout << "Calculate Res: = " << end_timer << " nanoseconds" << std::endl;
            
      }
    }
  }
}