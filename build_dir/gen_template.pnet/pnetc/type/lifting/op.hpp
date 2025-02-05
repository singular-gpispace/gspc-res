// defined in [/home/santosh/gspc-res/template/workflow/template.xpnet:7:7]
#ifndef _PNETC_TYPE_lifting_OP
#define _PNETC_TYPE_lifting_OP

#include <pnetc/type/lifting.hpp>
#include <string>
#include <we/type/value.hpp>
#include <we/type/value/from_value.hpp>
#include <we/type/value/to_value.hpp>
#include <iosfwd>

namespace pnetc
{
  namespace type
  {
    namespace lifting
    {
      lifting from_value (pnet::type::value::value_type const&);
      pnet::type::value::value_type to_value (lifting const&);
      std::ostream& operator<< (std::ostream&, lifting const&);
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
        inline value_type to_value<pnetc::type::lifting::lifting> (pnetc::type::lifting::lifting const& x)
      {
        return pnetc::type::lifting::to_value (x);
      }
      template<>
        inline pnetc::type::lifting::lifting from_value<pnetc::type::lifting::lifting> (value_type const& v)
      {
        return pnetc::type::lifting::from_value (v);
      }
    }
  }
}
#endif
