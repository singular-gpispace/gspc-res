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
        int N;
        int E;
        std::list<pnet::type::value::value_type> LIFT;
        we::type::literal::control D;
        ::pnetc::op::singular_template::LIFT (input_name, lead, all_lead, needed_library, base_filename, N, E, LIFT, D);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "E"), E);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "D"), D);
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
        int N;
        int counter;
        std::list<pnet::type::value::value_type> OUT;
        ::pnetc::op::singular_template::leadsyz (input, s, library_name, base_filename, N, counter, OUT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "counter"), counter);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "OUT"), OUT);
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
#include <pnetc/op/singular_template/Result_LIFT.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Result_LIFT
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & counter (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "counter"))));
        const int & lift_count (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "lift_count"))));
        we::type::literal::control output;
        ::pnetc::op::singular_template::Result_LIFT (counter, lift_count, output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "output"), output);
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
#include <pnetc/op/singular_template/EndTime.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void EndTime
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const we::type::literal::control & computed_output (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "computed_output"))));
        const long & start_timer (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "start_timer"))));
        long end_timer;
        ::pnetc::op::singular_template::EndTime (computed_output, start_timer, end_timer);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "end_timer"), end_timer);
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
        int N;
        int E;
        std::list<pnet::type::value::value_type> LIFT;
        std::string SUBLIFT;
        ::pnetc::op::singular_template::SUBLIFT (input_name, place_Tau, all_lead, needed_library, base_filename, N, E, LIFT, SUBLIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "E"), E);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "SUBLIFT"), SUBLIFT);
      }
    }
  }
}
#include <pnetc/op/singular_template/rResult_SubLIFTesult.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void rResult_SubLIFTesult
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & N (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        const int & E (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "E"))));
        std::string s (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "s"))));
        ::pnetc::op::singular_template::rResult_SubLIFTesult (N, E, s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
#include <pnetc/op/singular_template/EndTime_SubLIFT.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void EndTime_SubLIFT
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const long & start_time_LIFT (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "start_time_LIFT"))));
        const long & start_timer (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "start_timer"))));
        std::string computed_output (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "computed_output"))));
        long end_timer;
        ::pnetc::op::singular_template::EndTime_SubLIFT (start_time_LIFT, start_timer, computed_output, end_timer);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "computed_output"), computed_output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "end_timer"), end_timer);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::StartTime,"StartTime");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::LIFT,"LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::leadsyz,"leadsyz");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::count_LIFT,"count_LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Result_LIFT,"Result_LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::down,"down");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Up,"Up");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::All_Lead,"All_Lead");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::EndTime,"EndTime");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::SUBLIFT,"SUBLIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::rResult_SubLIFTesult,"rResult_SubLIFTesult");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::EndTime_SubLIFT,"EndTime_SubLIFT");
}
WE_MOD_INITIALIZE_END()
