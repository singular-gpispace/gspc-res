#include <pnetc/op/singular_template/leadsyz.hpp>
#include <list>
#include <we/type/value.hpp>
#include <we/type/literal/control.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <numeric>
#include <chrono>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void leadsyz
        ( const std::string& input
        , const we::type::literal::control& s
        , const std::string& library_name
        , const std::string& base_filename
        , std::list<pnet::type::value::value_type>& OUT
        )
      {
#line 308 "/home/santosh/gspc-res/template/workflow/template.xpnet"

            
              auto computation_time = std::chrono::high_resolution_clock::now();
                  std::tuple<std::vector<std::string>, int, long> out = RESOLVE_INTERFACE_FUNCTION(singular_template_LEAD)
                        (input, library_name, base_filename);
                      std::vector<std::string> vec = std::get<0>(out);
                      int total_generator = std::get<1>(out);
                    
                    
              auto end_computation = std::chrono::high_resolution_clock::now();
              auto computation_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - computation_time).count();
      
                    
                        for(int i (0); i<total_generator; i++)
                                                                 {
                          //std::cout << "leadSyz_Success:"<<vec[i] << std::endl;
                          OUT.emplace_back(vec[i]);
                         
                         //std::cout << "leadSyz_Success:"<<vec[i] << std::endl;
                          //std::cout << "SubliftleadSyz_Success:"<<vec[i] << std::endl;
                          
                          }
                      
                        // std::cout << "LeadSyzTiming:= " << computation_duration<< " nanoseconds" << std::endl;
                         
                      
              
              

            
                   
            
            
      }
    }
  }
}