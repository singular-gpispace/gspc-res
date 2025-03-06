#include <pnetc/op/singular_template/LIFT.hpp>
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
      void LIFT
        ( const std::string& input_name
        , const std::string& lead
        , const std::string& all_lead
        , const std::string& needed_library
        , const std::string& base_filename
        , const int& N
        , std::list<pnet::type::value::value_type>& LIFT
        , std::string& SUBLIFT
        )
      {
#line 268 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                  //  std::cout << "in singular _PetriNet_.._LIFT" << std::endl;
                   
                   std::tuple<std::vector<std::string>, int, long> out = RESOLVE_INTERFACE_FUNCTION(singular_template_LIFT)
                  (input_name,all_lead, lead, needed_library, base_filename,N);
                       

                       std::vector<std::string> vec = std::get<0>(out);
                       int total_generator = std::get<1>(out);
                      
                      //  std::cout << "#LIFT_Terms:"<< counter << std::endl;
                        
                         for(int i (0); i<vec.size(); i++)
                                                                 {
                         //std::cout << "LIFT_Success:"<<out.first[i] << std::endl;
                          LIFT.emplace_back(vec[i]);
                       
                         
                         
      
                    }
                    SUBLIFT = lead;
               
              // std::cout << "LIFT::"<<  SUBLIFT << std::endl;
              // std::cout << "LIFT_COUNT_Child:"<< E << std::endl;
              


// std::cout << "LIFT Duration: " << runtime << " milliseconds" << std::endl;
              

            
      }
    }
  }
}