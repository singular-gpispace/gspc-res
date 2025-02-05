#include <we/loader/macros.hpp>

#include <pnetc/op/singular_template/StartTime.hpp>
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
        std::string output;
        ::pnetc::op::singular_template::StartTime (input, start_time, output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "start_time"), start_time);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "output"), output);
      }
    }
  }
}
#include <pnetc/op/singular_template/All_leadsyz.hpp>
#include <list>
#include <we/type/value.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void All_leadsyz
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & input (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input"))));
        const std::string & library_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "library_name"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        int counter;
        long runtime;
        std::list<pnet::type::value::value_type> OUT;
        ::pnetc::op::singular_template::All_leadsyz (input, library_name, base_filename, counter, runtime, OUT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "counter"), counter);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "runtime"), runtime);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "OUT"), OUT);
      }
    }
  }
}
#include <pnetc/op/singular_template/EndTime.hpp>
#include <string>

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
        const long & start_time (::boost::get< long > (_pnetc_input.value (std::list<std::string> (1, "start_time"))));
        const std::string & computed_output (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "computed_output"))));
        std::string output;
        long end_timer;
        ::pnetc::op::singular_template::EndTime (start_time, computed_output, output, end_timer);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "output"), output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "end_timer"), end_timer);
      }
    }
  }
}
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::StartTime,"StartTime");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::All_leadsyz,"All_leadsyz");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::EndTime,"EndTime");
}
WE_MOD_INITIALIZE_END()
