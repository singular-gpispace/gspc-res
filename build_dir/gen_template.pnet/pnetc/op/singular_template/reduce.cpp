#include <pnetc/op/singular_template/reduce.hpp>
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
        , const int& N
        , const unsigned long& k1
        , const unsigned long& k2
        , const unsigned long& k3
        , const unsigned long& k4
        , std::string& s
        )
      {
#line 505 "/home/santosh/gspc-res/template/workflow/template.xpnet"


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