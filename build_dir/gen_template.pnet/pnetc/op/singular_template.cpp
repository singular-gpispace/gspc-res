#include <we/loader/macros.hpp>

#include <pnetc/op/singular_template/Drainage.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Drainage
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&
        , std::map<std::string, void*> const&
        )
      {
        const we::type::literal::control & N (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        const unsigned long & IN (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "IN"))));
        ::pnetc::op::singular_template::Drainage (N, IN);
      }
    }
  }
}
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
        we::type::literal::control c;
        std::string output;
        long start_time;
        ::pnetc::op::singular_template::StartTime (input, c, output, start_time);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "c"), c);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "output"), output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "start_time"), start_time);
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
#include <pnetc/op/singular_template/ADD.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void ADD
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&
        , std::map<std::string, void*> const&
        )
      {
        const std::string & library_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "library_name"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        const std::string & input (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input"))));
        const std::string & range (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "range"))));
        const int & N (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        ::pnetc::op::singular_template::ADD (library_name, base_filename, input, range, N);
      }
    }
  }
}
#include <pnetc/op/singular_template/Enable.hpp>
#include <list>
#include <we/type/value.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Enable
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const unsigned long & N (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        const we::type::literal::control & IN (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "IN"))));
        const int & r (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "r"))));
        std::list<pnet::type::value::value_type> out;
        we::type::literal::control c;
        ::pnetc::op::singular_template::Enable (N, IN, r, out, c);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "out"), out);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "c"), c);
      }
    }
  }
}
#include <pnetc/op/singular_template/INIT_Fill.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void INIT_Fill
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        unsigned long N (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        std::list<pnet::type::value::value_type> outgoing;
        ::pnetc::op::singular_template::INIT_Fill (N, outgoing);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "outgoing"), outgoing);
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
#include <pnetc/op/singular_template/Empty.hpp>
#include <we/type/literal/control.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Empty
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const unsigned long & k2 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k2"))));
        const unsigned long & k3 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k3"))));
        const unsigned long & k4 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k4"))));
        const we::type::literal::control & IN (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "IN"))));
        we::type::literal::control N;
        we::type::literal::control drain;
        ::pnetc::op::singular_template::Empty (k2, k3, k4, IN, N, drain);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "drain"), drain);
      }
    }
  }
}
#include <pnetc/op/singular_template/Fill_IN_Add.hpp>
#include <we/type/literal/control.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Fill_IN_Add
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const int & N (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        const we::type::literal::control & k1 (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "k1"))));
        std::list<pnet::type::value::value_type> outgoing;
        ::pnetc::op::singular_template::Fill_IN_Add (N, k1, outgoing);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "outgoing"), outgoing);
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
        const std::string & needed_library (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "needed_library"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        const unsigned long & N (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        std::list<pnet::type::value::value_type> LIFT;
        ::pnetc::op::singular_template::SUBLIFT (input_name, place_Tau, needed_library, base_filename, N, LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
      }
    }
  }
}
#include <pnetc/op/singular_template/ADD_Seq.hpp>
#include <we/type/literal/control.hpp>
#include <string>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void ADD_Seq
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const std::string & library_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "library_name"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        const std::string & input (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "input"))));
        const int & N (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        const unsigned long & k2 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k2"))));
        const we::type::literal::control & c (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "c"))));
        std::string s;
        ::pnetc::op::singular_template::ADD_Seq (library_name, base_filename, input, N, k2, c, s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
      }
    }
  }
}
#include <pnetc/op/singular_template/Fill_IN.hpp>
#include <list>
#include <we/type/value.hpp>

namespace pnetc
{
  namespace op
  {
    namespace singular_template
    {
      static void Fill_IN
        ( drts::worker::context *
        , expr::eval::context const&_pnetc_input
        , expr::eval::context&_pnetc_output
        , std::map<std::string, void*> const&
        )
      {
        const unsigned long & k2 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k2"))));
        const unsigned long & M (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "M"))));
        const unsigned long & k1 (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "k1"))));
        unsigned long N (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        std::list<pnet::type::value::value_type> outgoing;
        ::pnetc::op::singular_template::Fill_IN (k2, M, k1, N, outgoing);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "outgoing"), outgoing);
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
        const std::string & needed_library (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "needed_library"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        const unsigned long & N (::boost::get< unsigned long > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        std::list<pnet::type::value::value_type> LIFT;
        ::pnetc::op::singular_template::LIFT (input_name, lead, needed_library, base_filename, N, LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
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
        const std::string & library_name (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "library_name"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        we::type::literal::control s (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "s"))));
        std::list<pnet::type::value::value_type> OUT;
        ::pnetc::op::singular_template::leadsyz (input, library_name, base_filename, s, OUT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "OUT"), OUT);
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
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Drainage,"Drainage");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::StartTime,"StartTime");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::EndTime_Syz,"EndTime_Syz");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::EndTime_SubLIFT,"EndTime_SubLIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::ADD,"ADD");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Enable,"Enable");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::INIT_Fill,"INIT_Fill");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::StartTime_LIFT,"StartTime_LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Empty,"Empty");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Fill_IN_Add,"Fill_IN_Add");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::SUBLIFT,"SUBLIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::ADD_Seq,"ADD_Seq");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Fill_IN,"Fill_IN");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::EndTime_LIFT,"EndTime_LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::LIFT,"LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::leadsyz,"leadsyz");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::StartTime_SubLIFT,"StartTime_SubLIFT");
}
WE_MOD_INITIALIZE_END()
