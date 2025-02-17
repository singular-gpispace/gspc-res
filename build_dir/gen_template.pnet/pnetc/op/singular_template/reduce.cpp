#include <pnetc/op/singular_template/reduce.hpp>
#include <string>
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
      void reduce
        ( const std::string& library_name
        , const std::string& base_filename
        , const std::string& input
        , const std::string& l
        , const std::string& r
        , std::string& s
        , std::string& RHS_delete
        , std::string& LHS_delete
        )
      {
#line 524 "/home/santosh/gspc-res/template/workflow/template.xpnet"


                
                //std::cout << "Reduce_Input:" <<input << std::endl;
                //std::cout << "Reduce_l:" <<l << std::endl;
                //std::cout << "Reduce_r" <<r<< std::endl;
						 auto result  = RESOLVE_INTERFACE_FUNCTION(singular_template_reduce)(input,l,r,library_name, base_filename
							); 
                s = result.first;
        
            std::cout << "Reduce:" << s << std::endl;
     
               LHS_delete=l;
                          
                       RHS_delete=r;
//std::cout << "Reduce Duration: " << runtime << " milliseconds" << std::endl;

            		
      }
    }
  }
}