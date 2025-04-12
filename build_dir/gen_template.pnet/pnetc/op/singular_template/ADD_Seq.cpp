#include <pnetc/op/singular_template/ADD_Seq.hpp>
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
      void ADD_Seq
        ( const std::string& library_name
        , const std::string& base_filename
        , const std::string& input
        , const int& N
        , const unsigned long& k2
        , const we::type::literal::control& c
        , const unsigned long& Nplus
        , std::string& s
        )
      {
#line 944 "/home/santosh/gspc-res/template/workflow/template.xpnet"

             
                auto computation_time = std::chrono::high_resolution_clock::now();
              
              
           auto result  = RESOLVE_INTERFACE_FUNCTION(singular_template_ADD_Seq)(input,N,Nplus,library_name, base_filename
            ); 
            auto end_computation = std::chrono::high_resolution_clock::now();
            auto computation_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - computation_time).count();
    
              s = result.first;
           
         
          // std::cout << "Add_seq:" << s << std::endl;
             

// std::cout << "Reduce Duration: " <<computation_duration << "nanosecond" << std::endl;

              
      }
    }
  }
}