#include <pnetc/op/singular_template/Fill_IN_Add.hpp>
#include <we/type/literal/control.hpp>
#include <list>
#include <we/type/value.hpp>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <iostream>
#include <chrono>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      void Fill_IN_Add
        ( const int& N
        , const we::type::literal::control& k1
        , std::list<pnet::type::value::value_type>& outgoing
        )
      {
#line 956 "/home/santosh/gspc-res/template/workflow/template.xpnet"

       
         // Fill outgoing with the next batch
         for(int k =1; k <= N; k++) {
          outgoing.emplace_back(k);
          //  std::cout << "batch_start:"<<k << std::endl;
        }
        // Batch=N+1;
     
          // Fill outgoing with the next batch
          // for(unsigned long k = Batch; k <= N; k++) {
          //   outgoing.emplace_back(k);
          //   //  std::cout << "batch_start:"<<k << std::endl;
          // }
          // Batch=N+1;
          // N=N+M;
       
      // std::cout << "Last:"<<N << std::endl;
        
      }
    }
  }
}