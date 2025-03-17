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
        ::pnetc::op::singular_template::LIFT (input_name, lead, all_lead, needed_library, base_filename, LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
      }
    }
  }
}
#include <pnetc/op/singular_template/EndTime_LIFT.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void EndTime_LIFT
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&
        , std::map<std::string, void*> const&
        )
      {
        const long & start_time (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "start_time"))));
        const unsigned long & k2 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k2"))));
        ::pnetc::op::singular_template::EndTime_LIFT (start_time, k2);
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
#include <pnetc/op/singular_template/StartTime_SubLIFT.hpp>
#include <string>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void StartTime_SubLIFT
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const we::type::literal::control & c (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "c"))));
        const std::string & in (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "in"))));
        long start_time;
        ::pnetc::op::singular_template::StartTime_SubLIFT (c, in, start_time);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "start_time"), start_time);
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
#include <pnetc/op/singular_template/StartTime_LIFT.hpp>
#include <string>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void StartTime_LIFT
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const we::type::literal::control & c (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "c"))));
        const std::string & in (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "in"))));
        long start_time;
        ::pnetc::op::singular_template::StartTime_LIFT (c, in, start_time);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "start_time"), start_time);
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
#include <pnetc/op/singular_template/EndTime_SubLIFT.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void EndTime_SubLIFT
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&
        , std::map<std::string, void*> const&
        )
      {
        const long & start_time (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "start_time"))));
        const unsigned long & k2 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k2"))));
        ::pnetc::op::singular_template::EndTime_SubLIFT (start_time, k2);
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
        const unsigned long & k2 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k2"))));
        std::string computed_output (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "computed_output"))));
        ::pnetc::op::singular_template::EndTime_Syz (start_time, k2, computed_output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "computed_output"), computed_output);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::StartTime,"StartTime");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::LIFT,"LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::EndTime_LIFT,"EndTime_LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::leadsyz,"leadsyz");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::StartTime_SubLIFT,"StartTime_SubLIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::All_Lead,"All_Lead");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::StartTime_LIFT,"StartTime_LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::SUBLIFT,"SUBLIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::EndTime_SubLIFT,"EndTime_SubLIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::EndTime_Syz,"EndTime_Syz");
}
WE_MOD_INITIALIZE_END()
