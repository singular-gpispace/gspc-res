#include <pnetc/op/singular_template/INIT_Fill.hpp>
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
      void INIT_Fill
        ( unsigned long& N
        , std::list<pnet::type::value::value_type>& outgoing
        )
      {
#line 113 "/scratch/gnawali/Try/gspc-res/template/workflow/template.xpnet"

         
           // Fill outgoing with the next batch
           for(unsigned long k =1; k <= N; k++) {
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
      // std::cout << "INIT:"<<N<< std::endl;
        // std::cout << "Last:"<<N << std::endl;
          
      }
    }
  }
}