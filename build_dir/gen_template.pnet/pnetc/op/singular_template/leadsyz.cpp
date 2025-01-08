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
        , int& counter
        , int& N
        , std::list<pnet::type::value::value_type>& Sublift
        , std::list<pnet::type::value::value_type>& OUT
        , int& nleads
        )
      {
#line 169 "/home/santosh/gspc-res/template/workflow/template.xpnet"

                   
                   std::pair<std::vector<std::string>, int> out = RESOLVE_INTERFACE_FUNCTION(singular_template_leadSyzGPI)
                        (input, library_name, base_filename);
                     
                       // std::cout << "out.first:"<<out.first.size() << std::endl;
                         for(int i (0); i<out.first.size(); i++)
                                                                 {
                          //std::cout << "leadSyz_Success:"<<out.first[i] << std::endl;
                          OUT.emplace_back(out.first[i]);
                          Sublift.emplace_back(out.first[i]);
                         // std::cout << "leadSyz_Success:"<<out.first[i] << std::endl;
                          //std::cout << "SubliftleadSyz_Success:"<<out.first[i] << std::endl;
                          
                          }
                      
               
                        nleads=out.second;   

                         counter=out.second; 
                         N=out.second; 
                              

            
      }
    }
  }
}