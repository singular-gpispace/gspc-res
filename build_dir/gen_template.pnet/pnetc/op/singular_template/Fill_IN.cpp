#include <pnetc/op/singular_template/Fill_IN.hpp>
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
      void Fill_IN
        ( const unsigned long& k2
        , const unsigned long& M
        , const unsigned long& k1
        , unsigned long& N
        , std::list<pnet::type::value::value_type>& outgoing
        )
      {
#line 112 "/home/santosh/gspc-res/template/workflow/template.xpnet"

         
           // Fill outgoing with the next batch
           for(unsigned long k =N+1; k <= N+M; k++) {
            outgoing.emplace_back(k);
            //  std::cout << "batch_start:"<<k << std::endl;
          }
          // Batch=N+1;
          N=N+M;
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