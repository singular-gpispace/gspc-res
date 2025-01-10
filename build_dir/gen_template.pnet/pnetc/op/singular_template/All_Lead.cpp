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

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void All_Lead
        ( const std::string& init
        , const we::type::literal::control& C
        , const std::string& library_name
        , const std::string& base_filename
        , std::list<pnet::type::value::value_type>& OUTPUT
        , int& counter
        )
      {
#line 105 "/home/santosh/gspc-res/template/workflow/template.xpnet"

          //std::cout <<"All_LeadSyz_Input:"<<init<< std::endl;
					std::pair<std::vector<std::string>, int> out = RESOLVE_INTERFACE_FUNCTION( singular_template_all_leadsyz_GpI) (init,library_name, base_filename);
               counter=out.second;

                    
                         for(int i (0); i<out.first.size(); i++)
                                                                 {
                         std::cout << "All_Success:"<<out.first[i] << std::endl;
                         OUTPUT.emplace_back(out.first[i]);
                       
                         
                         
      
                    }
                    std::cout << "SchFrame"<<counter<< std::endl;
          
            		
      }
    }
  }
}