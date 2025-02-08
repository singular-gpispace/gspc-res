#include <pnetc/op/singular_template/EndTime.hpp>
#include <we/type/literal/control.hpp>
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
        ( const we::type::literal::control& computed_output
        , const long& start_timer
        , long& end_timer
        )
      {
#line 429 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                auto computation_time = std::chrono::high_resolution_clock::now();
                auto duration = computation_time.time_since_epoch();
                long current_time = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
                end_timer = current_time - start_timer; // Calculate elapsed time LIFT
              
                
                  
                // std::cout << "elapsed time_LIFT:= " << end_timer << " nanoseconds" << std::endl;
            
      }
    }
  }
}