#include <we/loader/macros.hpp>

#include <pnetc/op/singular_template/StartTime.hpp>
#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void StartTime
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & input (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input"))));
        long start_time;
        we::type::literal::control c;
        std::string output;
        ::pnetc::op::singular_template::StartTime (input, start_time, c, output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "start_time"), start_time);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "c"), c);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "output"), output);
      }
    }
  }
}
#include <pnetc/op/singular_template/LIFT.hpp>
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
        std::list<pnet::type::value::value_type> LIFT;
        we::type::literal::control D;
        ::pnetc::op::singular_template::LIFT (input_name, lead, all_lead, needed_library, base_filename, LIFT, D);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "D"), D);
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
#include <pnetc/op/singular_template/SUBLIFT.hpp>
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
        std::list<pnet::type::value::value_type> LIFT;
        std::string SUBLIFT;
        ::pnetc::op::singular_template::SUBLIFT (input_name, place_Tau, all_lead, needed_library, base_filename, LIFT, SUBLIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "SUBLIFT"), SUBLIFT);
      }
    }
  }
}
#include <pnetc/op/singular_template/reduce.hpp>
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
        std::string s;
        std::string RHS_delete;
        std::string LHS_delete;
        ::pnetc::op::singular_template::reduce (library_name, base_filename, input, l, r, s, RHS_delete, LHS_delete);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "RHS_delete"), RHS_delete);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LHS_delete"), LHS_delete);
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
        int counter;
        std::list<pnet::type::value::value_type> OUT;
        std::list<pnet::type::value::value_type> Sublift;
        ::pnetc::op::singular_template::leadsyz (input, s, library_name, base_filename, counter, OUT, Sublift);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "counter"), counter);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "OUT"), OUT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "Sublift"), Sublift);
      }
    }
  }
}
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
        , expr::eval::context&
        , std::map<std::string, void*> const&
        )
      {
        const int & LIFT (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "LIFT"))));
        const int & nleads (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "nleads"))));
        const std::string & l (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "l"))));
        const std::string & r (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "r"))));
        ::pnetc::op::singular_template::removeFiles (LIFT, nleads, l, r);
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
        const unsigned long & k1 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k1"))));
        const unsigned long & k2 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k2"))));
        const unsigned long & k3 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k3"))));
        const unsigned long & k4 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k4"))));
        std::string s (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "s"))));
        ::pnetc::op::singular_template::result (k1, k2, k3, k4, s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
#include <pnetc/op/singular_template/EndTime_Syz.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void EndTime_Syz
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const long & start_time (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "start_time"))));
        std::string computed_output (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "computed_output"))));
        ::pnetc::op::singular_template::EndTime_Syz (start_time, computed_output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "computed_output"), computed_output);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::StartTime,"StartTime");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::LIFT,"LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::count_LIFT,"count_LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::All_Lead,"All_Lead");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::SUBLIFT,"SUBLIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::reduce,"reduce");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::leadsyz,"leadsyz");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::count,"count");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::removeFiles,"removeFiles");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::plus,"plus");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::result,"result");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::EndTime_Syz,"EndTime_Syz");
}
WE_MOD_INITIALIZE_END()
