#include <pnetc/op/singular_template/EndTime_SchFrame.hpp>
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
      void EndTime_SchFrame
        ( const long& start_time
        , std::string& computed_output
        , long& end_timer
        )
      {
#line 690 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                auto computation_time = std::chrono::high_resolution_clock::now();
                auto duration = computation_time.time_since_epoch();
                long current_time = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
                end_timer = current_time - start_time; // Calculate elapsed time
              
                std::cout << "Calculate Syz: = " << end_timer << " nanoseconds" << std::endl;
            
      }
    }
  }
}