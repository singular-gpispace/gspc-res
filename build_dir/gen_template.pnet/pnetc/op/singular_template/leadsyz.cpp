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
        , int& nleads
        , int& counter
        , int& N
        , std::list<pnet::type::value::value_type>& Sublift
        , std::list<pnet::type::value::value_type>& OUT
        )
      {
#line 189 "/home/santosh/gspc-res/template/workflow/template.xpnet"

            
            
                  std::tuple<std::vector<std::string>, int, long> out = RESOLVE_INTERFACE_FUNCTION(singular_template_LEAD)
                        (input, library_name, base_filename);
                      std::vector<std::string> vec = std::get<0>(out);
                      int total_generator = std::get<1>(out);
                      auto runtime = std::get<2>(out);
                       N=total_generator;
                      counter=total_generator;
                      
                         for(int i (0); i<vec.size(); i++)
                                                                 {
                          //std::cout << "leadSyz_Success:"<<vec[i] << std::endl;
                          OUT.emplace_back(vec[i]);
                          Sublift.emplace_back(vec[i]);
                         //std::cout << "leadSyz_Success:"<<vec[i] << std::endl;
                          //std::cout << "SubliftleadSyz_Success:"<<vec[i] << std::endl;
                          
                          }
                      
                      //  std::cout << "leadSyz_place_counter:"<<N << std::endl;
                         nleads=total_generator; 
                          // std::cout << "#leadSyz="<<nleads<< std::endl;

            
                   
            
            
      }
    }
  }
}