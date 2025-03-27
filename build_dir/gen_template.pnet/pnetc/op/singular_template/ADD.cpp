#include <pnetc/op/singular_template/ADD.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <iostream>
#include <sstream>
#include <chrono>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void ADD
        ( const std::string& library_name
        , const std::string& base_filename
        , const std::string& input
        , const std::string& range
        , std::string& s
        )
      {
#line 954 "/scratch/gnawali/Try/gspc-res/template/workflow/template.xpnet"

             
                auto computation_time = std::chrono::high_resolution_clock::now();
              
              
        int start, end;
        char discard;
       

        // Parse single range "[n,n+k]"
        std::istringstream range_stream(range);
        range_stream >> discard >> start >> discard >> end >> discard;
        

        std::vector<std::pair<int, int>> range_pair = {{start, end}};


           auto result  = RESOLVE_INTERFACE_FUNCTION(singular_template_reduce)(input,{start, end},library_name, base_filename
            ); 
            auto end_computation = std::chrono::high_resolution_clock::now();
            auto computation_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - computation_time).count();
    
              s = result.first;
           
         
          // std::cout << "Reduce:" << s << std::endl;
             

std::cout << "Reduce Duration: " <<computation_duration << "nanosecond" << std::endl;

              
      }
    }
  }
}