#include <we/loader/macros.hpp>

#include <pnetc/op/singular_template/count.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void count
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        std::string a (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "a"))));
        ::pnetc::op::singular_template::count (a);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "a"), a);
      }
    }
  }
}
#include <pnetc/op/singular_template/All_Lead.hpp>
#include <we/type/literal/control.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void All_Lead
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & init (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "init"))));
        const we::type::literal::control & C (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "C"))));
        const std::string & library_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "library_name"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        std::list<pnet::type::value::value_type> OUTPUT;
        int counter;
        ::pnetc::op::singular_template::All_Lead (init, C, library_name, base_filename, OUTPUT, counter);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "OUTPUT"), OUTPUT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "counter"), counter);
      }
    }
  }
}
#include <pnetc/op/singular_template/reduce.hpp>
#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void reduce
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & library_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "library_name"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        const std::string & input (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input"))));
        const std::string & l (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "l"))));
        const std::string & r (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "r"))));
        long runtime;
        std::string LHS_delete;
        std::string RHS_delete;
        we::type::literal::control nsummand;
        std::string s;
        ::pnetc::op::singular_template::reduce (library_name, base_filename, input, l, r, runtime, LHS_delete, RHS_delete, nsummand, s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "runtime"), runtime);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LHS_delete"), LHS_delete);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "RHS_delete"), RHS_delete);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "nsummand"), nsummand);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
#include <pnetc/op/singular_template/SUBLIFT.hpp>
#include <we/type/literal/control.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void SUBLIFT
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & input_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input_name"))));
        const std::string & place_Tau (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "place_Tau"))));
        const std::string & all_lead (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "all_lead"))));
        const std::string & needed_library (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "needed_library"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        int N;
        int E;
        std::list<pnet::type::value::value_type> LIFT;
        long runtime;
        we::type::literal::control lift_count;
        std::string SUBLIFT;
        ::pnetc::op::singular_template::SUBLIFT (input_name, place_Tau, all_lead, needed_library, base_filename, N, E, LIFT, runtime, lift_count, SUBLIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "E"), E);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "runtime"), runtime);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "lift_count"), lift_count);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "SUBLIFT"), SUBLIFT);
      }
    }
  }
}
#include <pnetc/op/singular_template/LIFT.hpp>
#include <list>
#include <we/type/value.hpp>
#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void LIFT
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & input_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input_name"))));
        const std::string & lead (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "lead"))));
        const std::string & all_lead (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "all_lead"))));
        const std::string & needed_library (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "needed_library"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        int N;
        int E;
        we::type::literal::control D;
        long runtime;
        std::list<pnet::type::value::value_type> LIFT;
        ::pnetc::op::singular_template::LIFT (input_name, lead, all_lead, needed_library, base_filename, N, E, D, runtime, LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "E"), E);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "D"), D);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "runtime"), runtime);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
      }
    }
  }
}
#include <pnetc/op/singular_template/SumSubLIFTRuntimes.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void SumSubLIFTRuntimes
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const long & newsubLiftTime (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "newsubLiftTime"))));
        long currentSum (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "currentSum"))));
        ::pnetc::op::singular_template::SumSubLIFTRuntimes (newsubLiftTime, currentSum);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "currentSum"), currentSum);
      }
    }
  }
}
#include <pnetc/op/singular_template/Generate.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Generate
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        long sumTime (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "sumTime"))));
        std::string syz (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "syz"))));
        ::pnetc::op::singular_template::Generate (sumTime, syz);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "sumTime"), sumTime);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "syz"), syz);
      }
    }
  }
}
#include <pnetc/op/singular_template/count_LIFT.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void count_LIFT
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const we::type::literal::control & s (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "s"))));
        int a (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "a"))));
        ::pnetc::op::singular_template::count_LIFT (s, a);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "a"), a);
      }
    }
  }
}
#include <pnetc/op/singular_template/SumTimes.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void SumTimes
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const we::type::literal::control & c (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "c"))));
        const long & leadSyzTime (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "leadSyzTime"))));
        const long & liftTime (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "liftTime"))));
        const long & subLiftTime (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "subLiftTime"))));
        const long & reduceTime (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "reduceTime"))));
        long sumTime;
        ::pnetc::op::singular_template::SumTimes (c, leadSyzTime, liftTime, subLiftTime, reduceTime, sumTime);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "sumTime"), sumTime);
      }
    }
  }
}
#include <pnetc/op/singular_template/Decrement.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Decrement
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        int s (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "s"))));
        ::pnetc::op::singular_template::Decrement (s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
#include <pnetc/op/singular_template/SumreduceRuntimes.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void SumreduceRuntimes
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const long & newreduceTime (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "newreduceTime"))));
        long currentSum (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "currentSum"))));
        ::pnetc::op::singular_template::SumreduceRuntimes (newreduceTime, currentSum);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "currentSum"), currentSum);
      }
    }
  }
}
#include <pnetc/op/singular_template/SumLIFTRuntimes.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void SumLIFTRuntimes
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const long & newLiftTime (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "newLiftTime"))));
        long currentSum (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "currentSum"))));
        ::pnetc::op::singular_template::SumLIFTRuntimes (newLiftTime, currentSum);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "currentSum"), currentSum);
      }
    }
  }
}
#include <pnetc/op/singular_template/removeFiles.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void removeFiles
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & LIFT (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "LIFT"))));
        const int & nleads (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "nleads"))));
        const std::string & l (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "l"))));
        const std::string & r (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "r"))));
        std::string s;
        ::pnetc::op::singular_template::removeFiles (LIFT, nleads, l, r, s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
#include <pnetc/op/singular_template/leadsyz.hpp>
#include <list>
#include <we/type/value.hpp>
#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void leadsyz
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & input (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input"))));
        const we::type::literal::control & s (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "s"))));
        const std::string & library_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "library_name"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        int nleads;
        int counter;
        int N;
        std::list<pnet::type::value::value_type> Sublift;
        long runtime;
        std::list<pnet::type::value::value_type> OUT;
        ::pnetc::op::singular_template::leadsyz (input, s, library_name, base_filename, nleads, counter, N, Sublift, runtime, OUT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "nleads"), nleads);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "counter"), counter);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "Sublift"), Sublift);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "runtime"), runtime);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "OUT"), OUT);
      }
    }
  }
}
#include <pnetc/op/singular_template/down.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void down
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & e (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "e"))));
        int a (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "a"))));
        we::type::literal::control c (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "c"))));
        ::pnetc::op::singular_template::down (e, a, c);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "a"), a);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "c"), c);
      }
    }
  }
}
#include <pnetc/op/singular_template/Increment.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Increment
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        int s (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "s"))));
        ::pnetc::op::singular_template::Increment (s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
#include <pnetc/op/singular_template/plus.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void plus
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        std::string a (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "a"))));
        ::pnetc::op::singular_template::plus (a);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "a"), a);
      }
    }
  }
}
#include <pnetc/op/singular_template/result.hpp>
#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void result
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & nleads (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "nleads"))));
        const int & d_lift (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "d_lift"))));
        const int & counter (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "counter"))));
        const int & N (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        const int & E (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "E"))));
        std::string s (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "s"))));
        we::type::literal::control c_time;
        ::pnetc::op::singular_template::result (nleads, d_lift, counter, N, E, s, c_time);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "c_time"), c_time);
      }
    }
  }
}
#include <pnetc/op/singular_template/Init.hpp>
#include <string>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Init
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & input (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input"))));
        we::type::literal::control c;
        std::string init;
        std::string output;
        ::pnetc::op::singular_template::Init (input, c, init, output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "c"), c);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "init"), init);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "output"), output);
      }
    }
  }
}
#include <pnetc/op/singular_template/Up.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Up
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & n (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "n"))));
        int a (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "a"))));
        we::type::literal::control c (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "c"))));
        ::pnetc::op::singular_template::Up (n, a, c);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "a"), a);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "c"), c);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::count,"count");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::All_Lead,"All_Lead");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::reduce,"reduce");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::SUBLIFT,"SUBLIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::LIFT,"LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::SumSubLIFTRuntimes,"SumSubLIFTRuntimes");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Generate,"Generate");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::count_LIFT,"count_LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::SumTimes,"SumTimes");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Decrement,"Decrement");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::SumreduceRuntimes,"SumreduceRuntimes");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::SumLIFTRuntimes,"SumLIFTRuntimes");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::removeFiles,"removeFiles");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::leadsyz,"leadsyz");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::down,"down");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Increment,"Increment");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::plus,"plus");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::result,"result");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Init,"Init");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Up,"Up");
}
WE_MOD_INITIALIZE_END()
