#include <pnetc/op/singular_template/EndTime.hpp>
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
      void EndTime
        ( const long& start_time_LIFT
        , const long& start_timer
        , std::string& computed_output
        , long& end_timer
        )
      {
#line 433 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                auto computation_time = std::chrono::high_resolution_clock::now();
                auto duration = computation_time.time_since_epoch();
                long current_time = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
               auto end_time = current_time - start_timer; // Calculate elapsed time LIFT
               end_timer =end_time - start_time_LIFT; 
                 std::cout << "Calculate elapsed time:SchFrame = " << start_time_LIFT << " nanoseconds" << std::endl;
                   std::cout << " elapsed time_LIFT: = " << end_time << " nanoseconds" << std::endl;
                std::cout << "only elapsed time_LIFT:= " << end_timer << " nanoseconds" << std::endl;
            
      }
    }
  }
}