#include <pnetc/op/singular_template/SUBLIFT.hpp>
#include <we/type/literal/control.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <list>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void SUBLIFT
        ( const std::string& input_name
        , const std::string& place_Tau
        , const std::string& needed_library
        , const std::string& base_filename
        , std::list<pnet::type::value::value_type>& LIFT
        , std::string& SUBLIFT
        , we::type::literal::control& lift_count
        , int& N
        , int& E
        )
      {
#line 344 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                  // std::cout << "in singular _PetriNet_.._SubLIFT" << std::endl;
                           
                   std::pair<std::vector<std::string>, int> out = RESOLVE_INTERFACE_FUNCTION(singular_template_SUBLIFT)
                 (input_name,place_Tau, needed_library, base_filename);
                       
                     
                        
                         for(int i (0); i<out.first.size(); i++)
                                                                 {
                         //std::cout << "SUBLIFT_Success:"<<out.first[i] << std::endl;
                          LIFT.emplace_back(out.first[i]);
                          
                         
                          
                          
                       
               
      
                    } int total_generator=out.second;
                      if (total_generator>=1)
                {
                   N=total_generator-1;
                   E=0;
                } else{
                    if(total_generator==0){
                      E=1;
                      N=0;
                  } 
                }
                 


                    SUBLIFT = place_Tau;
                  
                   //std::cout << "SubLIFT_IN_Place:" << SUBLIFT << std::endl;
                  //std::cout << "SubLIFT_COUNT:"<< tau_count << std::endl;        
               
            
      }
    }
  }
}