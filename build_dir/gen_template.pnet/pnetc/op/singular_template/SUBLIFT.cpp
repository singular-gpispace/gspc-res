#include <pnetc/op/singular_template/SUBLIFT.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <chrono>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void SUBLIFT
        ( const std::string& input_name
        , const std::string& place_Tau
        , const std::string& all_lead
        , const std::string& needed_library
        , const std::string& base_filename
        , std::list<pnet::type::value::value_type>& LIFT
        , std::string& SUBLIFT
        )
      {
#line 348 "/home/santosh/gspc-res/template/workflow/template.xpnet"


             
                   //std::cout << "in singular _PetriNet_.._SubLIFT" << std::endl;
                           
                 std::tuple<std::vector<std::string>, int, long> out = RESOLVE_INTERFACE_FUNCTION(singular_template_SUBLIFT)
                 (input_name,all_lead,place_Tau, needed_library, base_filename);
                       
                     std::vector<std::string> vec = std::get<0>(out);
                      int total_generator = std::get<1>(out);
                     

                        
                         for(int i (0); i<vec.size(); i++)
                                                                 {
                         //std::cout << "SUBLIFT_Success:"<<out.first[i] << std::endl;
                          LIFT.emplace_back(vec[i]);
                          
                         
                          
                          
                       
                    
                         
      
                    }
              
           
              
                           
                
              // std::cout << "SubLIFT_COUNT_Parent:"<< N << std::endl;
              // std::cout << "SubLIFT_COUNT_Child:"<< E << std::endl;
              
                 


                    SUBLIFT = place_Tau;
                   //std::cout << "SubLIFT_IN_Place:" << SUBLIFT << std::endl;
                  //std::cout << "SubLIFT_COUNT:"<< tau_count << std::endl; 





       
               
            
      }
    }
  }
}