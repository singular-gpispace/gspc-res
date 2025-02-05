// defined in [/home/santosh/gspc-res/template/workflow/template.xpnet:3:5]

#include <pnetc/type/Std_basis/op.hpp>
#include <we/field.hpp>
#include <we/signature_of.hpp>
#include <we/type/value/poke.hpp>
#include <we/type/value/show.hpp>
#include <iostream>

namespace pnetc
{
  namespace type
  {
    namespace Std_basis
    {
      Std_basis from_value (pnet::type::value::value_type const& v)
      {
        return Std_basis
          ( pnet::field_as< std::string > ("InputGB", v, std::string("string"))
          , pnet::field_as< int > ("ngen", v, std::string("int"))
          );
      }
      pnet::type::value::value_type to_value (Std_basis const& x)
      {
        pnet::type::value::value_type v;
        pnet::type::value::poke ("InputGB", v, x.InputGB);
        pnet::type::value::poke ("ngen", v, x.ngen);
        return v;
      }
      std::ostream& operator<< (std::ostream& os, Std_basis const& x)
      {
        return os << pnet::type::value::show (to_value (x));
      }
    }
  }
}
