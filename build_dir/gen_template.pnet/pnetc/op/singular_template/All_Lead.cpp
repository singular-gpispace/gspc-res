#include <pnetc/op/singular_template/All_Lead.hpp>
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
#include <numeric>
#include <chrono>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void All_Lead
        ( const std::string& init
        , const std::string& library_name
        , const std::string& base_filename
        , we::type::literal::control& C
        , std::list<pnet::type::value::value_type>& OUTPUT
        , int& counter
        )
      {
#line 239 "/home/santosh/gspc-res/template/workflow/template.xpnet"

          //std::cout <<"All_LeadSyz_Input:"<<init<< std::endl;
           auto start_computation = std::chrono::high_resolution_clock::now();
				std::tuple<std::vector<std::string>, int, long> out = RESOLVE_INTERFACE_FUNCTION( singular_template_ALL_LEAD) (init,library_name, base_filename);
              

                std::vector<std::string> vec = std::get<0>(out);
                     counter = std::get<1>(out);
                    //  auto runtime = std::get<2>(out);
             
                    
                 for(int i (0); i<vec.size(); i++)
                                                                 {
                        // std::cout << "SchFrame_Success:"<<vec[i] << std::endl;
                       
                          OUTPUT.emplace_back(vec[i]);

                    }
            std::cout << "place_SchFrame_counter: " << counter << std::endl;
            // std::cout << "total_runtime_SchFrame = " << runtime << " nanoseconds" << std::endl;
          
            		
      }
    }
  }
}