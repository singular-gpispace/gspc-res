#include <pnetc/op/singular_template/All_leadsyz.hpp>
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
      void All_leadsyz
        ( const std::string& input
        , const std::string& library_name
        , const std::string& base_filename
        , int& counter
        , long& runtime
        , std::list<pnet::type::value::value_type>& OUT
        )
      {
#line 81 "/home/santosh/gspc-res/template/workflow/template.xpnet"

            std::cout << "SchFrame_control:" << std::endl;
            std::tuple<std::vector<std::string>, int, long> out = RESOLVE_INTERFACE_FUNCTION(singular_template_ALL_LEAD)(input, library_name, base_filename);
            std::vector<std::string> vec = std::get<0>(out);
            int total_generator = std::get<1>(out);
            runtime = std::get<2>(out);
            counter = total_generator;
            for (int i = 0; i < vec.size(); i++) {
              OUT.emplace_back(vec[i]);
            }
            std::cout << "place_SchFrame_counter: " << counter << std::endl;
            std::cout << "total_runtime_SchFrame = " << runtime << " nanoseconds" << std::endl;
          
      }
    }
  }
}