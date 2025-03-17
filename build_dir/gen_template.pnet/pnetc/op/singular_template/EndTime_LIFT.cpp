#include <pnetc/op/singular_template/EndTime_LIFT.hpp>
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
      void EndTime_LIFT
        ( const long& start_time
        , const unsigned long& k2
        , const long& bigin_time
        )
      {
#line 368 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                  auto computation_time = std::chrono::high_resolution_clock::now();
                  auto duration = computation_time.time_since_epoch();
                  long current_time = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
                 auto  end_timer = current_time - start_time; // Calculate elapsed time
                 auto  end_timer1 = current_time - bigin_time; // Calculate elapsed time
                
                 std::cout << "Calculate  only LIFT: = " << end_timer << " nanoseconds" << std::endl;
                  std::cout << "Calculate LIFT: = " << end_timer1 << " nanoseconds" << std::endl;
              
      }
    }
  }
}