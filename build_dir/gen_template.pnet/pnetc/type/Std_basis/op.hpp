// defined in [/home/santosh/gspc-res/template/workflow/template.xpnet:4:5]
#ifndef _PNETC_TYPE_Std_basis_OP
#define _PNETC_TYPE_Std_basis_OP

#include <pnetc/type/Std_basis.hpp>
#include <string>
#include <we/type/value.hpp>
#include <we/type/value/from_value.hpp>
#include <we/type/value/to_value.hpp>
#include <iosfwd>

namespace pnetc
{
  namespace type
  {
    namespace Std_basis
    {
      Std_basis from_value (pnet::type::value::value_type const&);
      pnet::type::value::value_type to_value (Std_basis const&);
      std::ostream& operator<< (std::ostream&, Std_basis const&);
    }
  }
}
namespace pnet
{
  namespace type
  {
    namespace value
    {
      template<>
        inline value_type to_value<pnetc::type::Std_basis::Std_basis> (pnetc::type::Std_basis::Std_basis const& x)
      {
        return pnetc::type::Std_basis::to_value (x);
      }
      template<>
        inline pnetc::type::Std_basis::Std_basis from_value<pnetc::type::Std_basis::Std_basis> (value_type const& v)
      {
        return pnetc::type::Std_basis::from_value (v);
      }
    }
  }
}
#endif
