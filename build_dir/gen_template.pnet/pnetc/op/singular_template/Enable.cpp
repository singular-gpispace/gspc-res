#include <pnetc/op/singular_template/Enable.hpp>
#include <list>
#include <we/type/value.hpp>
#include <we/type/literal/control.hpp>
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
      void Enable
        ( const we::type::literal::control& IN
        , const int& r
        , unsigned long& N
        , std::list<pnet::type::value::value_type>& out
        )
      {
#line 741 "/home/santosh/gspc-res/template/workflow/template.xpnet"

  
      int n = N / r; 
      int remainder = N % r; 
      // std::vector<std::pair<int, int>> ranges;
      
      int start = 1;
      for (int i = 0; i < r; ++i) {
        int end = start + n - 1 + (i < remainder ? 1 : 0);
        std::string range_str = "[" + std::to_string(start) + "," + std::to_string(end) + "]";
        // std::cout << "Generated range: " << range_str << std::endl;
        out.push_back(range_str);
        start = end + 1;
    }

      }
    }
  }
}