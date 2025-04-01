// defined in [/scratch/gnawali/Try/gspc-res/template/workflow/template.xpnet:8:7]

#include <pnetc/type/lifting/op.hpp>
#include <we/field.hpp>
#include <we/signature_of.hpp>
#include <we/type/value/poke.hpp>
#include <we/type/value/show.hpp>
#include <iostream>

namespace pnetc
{
  namespace type
  {
    namespace lifting
    {
      lifting from_value (pnet::type::value::value_type const& v)
      {
        return lifting
          ( pnet::field_as< std::string > ("lead_Syz", v, std::string("string"))
          , pnet::field_as< std::string > ("lift", v, std::string("string"))
          , pnet::field_as< int > ("leadsyz_count", v, std::string("int"))
          );
      }
      pnet::type::value::value_type to_value (lifting const& x)
      {
        pnet::type::value::value_type v;
        pnet::type::value::poke ("lead_Syz", v, x.lead_Syz);
        pnet::type::value::poke ("lift", v, x.lift);
        pnet::type::value::poke ("leadsyz_count", v, x.leadsyz_count);
        return v;
      }
      std::ostream& operator<< (std::ostream& os, lifting const& x)
      {
        return os << pnet::type::value::show (to_value (x));
      }
    }
  }
}
