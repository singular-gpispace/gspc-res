#include <pnetc/op/singular_template/StartTime_SubLIFT.hpp>
#include <string>
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
      void StartTime_SubLIFT
        ( const we::type::literal::control& c
        , const std::string& in
        , long& start_time
        )
      {
#line 563 "/scratch/gnawali/Try/gspc-res/template/workflow/template.xpnet"

          auto computation_time = std::chrono::high_resolution_clock::now();
          auto duration = computation_time.time_since_epoch();
      
          start_time= std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        
   
    
      }
    }
  }
}