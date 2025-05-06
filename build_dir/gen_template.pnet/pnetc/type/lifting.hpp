// lifting :: [lead_Syz :: string, lift :: string, leadsyz_count :: int]
#ifndef _PNETC_TYPE_lifting
#define _PNETC_TYPE_lifting

#include <string>
// Copyright (C) 2025 Fraunhofer ITWM
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GUARD_PNETC_COMPARE
#define GUARD_PNETC_COMPARE
#include <cmath>
#include <type_traits>
namespace pnetc
{
  namespace type
  {
    template<typename T>
      bool pnetc_less (T const& lhs, T const& rhs)
      {
        return lhs < rhs;
      }
    struct pnetc_eq
    {
      template< typename F
              , typename
              = typename std::enable_if<std::is_floating_point<F>::value>::type
              >
        bool operator() (F const& lhs, F const& rhs)
      {
        return !std::isnan (lhs)
            && !std::isnan (rhs)
            && !pnetc_less (lhs, rhs)
            && !pnetc_less (rhs, lhs)
            ;
      }
      template< typename T
              , typename
              = typename std::enable_if<!std::is_floating_point<T>::value>::type
              >
        bool operator() (T const& lhs, T const& rhs) const
        {
          return lhs == rhs;
        }
    };
  }
}
#endif

namespace pnetc
{
  namespace type
  {
    namespace lifting
    {
      struct lifting
      {
        std::string lead_Syz;
        std::string lift;
        int leadsyz_count;
        lifting()
          : lead_Syz()
          , lift()
          , leadsyz_count()
        {}
        explicit lifting
          ( std::string const& _lead_Syz
          , std::string const& _lift
          , int const& _leadsyz_count
          )
          : lead_Syz (_lead_Syz)
          , lift (_lift)
          , leadsyz_count (_leadsyz_count)
        {}
        bool operator== (lifting const& rhs) const
        {
          return true
            && pnetc_eq{} (this->lead_Syz, rhs.lead_Syz)
            && pnetc_eq{} (this->lift, rhs.lift)
            && pnetc_eq{} (this->leadsyz_count, rhs.leadsyz_count)
            ;
        }
        bool operator< (lifting const& rhs) const
        {
          return pnetc_less (this->lead_Syz, rhs.lead_Syz) || (pnetc_eq{} (this->lead_Syz, rhs.lead_Syz) && (pnetc_less (this->lift, rhs.lift) || (pnetc_eq{} (this->lift, rhs.lift) && (pnetc_less (this->leadsyz_count, rhs.leadsyz_count)))));
        }
      };
    }
  }
}
#endif
