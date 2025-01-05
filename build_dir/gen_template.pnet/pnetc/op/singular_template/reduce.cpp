#include <pnetc/op/singular_template/reduce.hpp>
#include <we/type/literal/control.hpp>
#include <string>
#include <interface/template_interface.hpp>
#include <util-generic/dynamic_linking.hpp>
#include <iostream>

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
        , std::string& LHS_delete
        , we::type::literal::control& nsummand
        , std::string& RHS_delete
        , std::string& s
        )
      {
#line 457 "/home/santosh/gspc-res/template/workflow/template.xpnet"

						s = RESOLVE_INTERFACE_FUNCTION(singular_template_reduce)(input,l,r,library_name, base_filename
							); 
                  
                         
                        LHS_delete=l;
                          
                       RHS_delete=r;
                          
                          
                       
                   
            std::cout << "Reduce:" << s << std::endl;
            		
      }
    }
  }
}