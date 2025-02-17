// Std_basis :: [InputGB :: string, ngen :: int]
#ifndef _PNETC_TYPE_Std_basis
#define _PNETC_TYPE_Std_basis

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
    namespace Std_basis
    {
      struct Std_basis
      {
        std::string InputGB;
        int ngen;
        Std_basis()
          : InputGB()
          , ngen()
        {}
        explicit Std_basis
          ( std::string const& _InputGB
          , int const& _ngen
          )
          : InputGB (_InputGB)
          , ngen (_ngen)
        {}
        bool operator== (Std_basis const& rhs) const
        {
          return true
            && pnetc_eq{} (this->InputGB, rhs.InputGB)
            && pnetc_eq{} (this->ngen, rhs.ngen)
            ;
        }
        bool operator< (Std_basis const& rhs) const
        {
          return pnetc_less (this->InputGB, rhs.InputGB) || (pnetc_eq{} (this->InputGB, rhs.InputGB) && (pnetc_less (this->ngen, rhs.ngen)));
        }
      };
    }
  }
}
#endif
