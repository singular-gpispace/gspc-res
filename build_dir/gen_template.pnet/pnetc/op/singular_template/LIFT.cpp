#include <pnetc/op/singular_template/LIFT.hpp>
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

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void LIFT
        ( const std::string& input_name
        , const std::string& lead
        , const std::string& needed_library
        , const std::string& base_filename
        , we::type::literal::control& D
        , std::list<pnet::type::value::value_type>& LIFT
        , int& N
        , int& E
        )
      {
#line 246 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                   //std::cout << "in singular _PetriNet_.._LIFT" << std::endl;

                   std::pair<std::vector<std::string>, int> out = RESOLVE_INTERFACE_FUNCTION(singular_template_LIFT)
                 (input_name, lead, needed_library, base_filename);
                       
                       //std::cout << "LIFT_COUNT:"<< lift_count << std::endl;
                        
                         for(int i (0); i<out.first.size(); i++)
                                                                 {
                         //std::cout << "LIFT_Success:"<<out.first[i] << std::endl;
                          LIFT.emplace_back(out.first[i]);
                       
                         
                         
      
                    }
                    int total_generator=out.second;
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
               //std::cout << "LIFT_Success:"<<lift_count << std::endl;
                              

            
      }
    }
  }
}