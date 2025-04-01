#include <pnetc/op/singular_template/StartTime.hpp>
#include <we/type/literal/control.hpp>
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
      void StartTime
        ( const std::string& input
        , we::type::literal::control& c
        , std::string& output
        , long& start_time
        )
      {
#line 72 "/scratch/gnawali/Try/gspc-res/template/workflow/template.xpnet"

          auto computation_time = std::chrono::high_resolution_clock::now();
          auto duration = computation_time.time_since_epoch();
      
          start_time= std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        
     output=input;
    
      }
    }
  }
}