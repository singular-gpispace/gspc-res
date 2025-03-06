#include <pnetc/op/singular_template/reduce.hpp>
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
      void reduce
        ( const std::string& library_name
        , const std::string& base_filename
        , const std::string& input
        , const we::type::literal::control& IN
        , std::string& s
        )
      {
#line 525 "/home/santosh/gspc-res/template/workflow/template.xpnet"

               int N=7047;
                auto computation_time = std::chrono::high_resolution_clock::now();
              
              //std::cout << "Reduce_Input:" <<input << std::endl;
              //std::cout << "Reduce_l:" <<l << std::endl;
              //std::cout << "Reduce_r" <<r<< std::endl;
           auto result  = RESOLVE_INTERFACE_FUNCTION(singular_template_reduce)(input,N,library_name, base_filename
            ); 
            auto end_computation = std::chrono::high_resolution_clock::now();
            auto computation_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - computation_time).count();
    
              s = result.first;
           
         
          std::cout << "Reduce:" << s << std::endl;
             

std::cout << "Reduce Duration: " <<computation_duration << "nanosecond" << std::endl;

              
      }
    }
  }
}