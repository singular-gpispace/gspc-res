#include <we/loader/macros.hpp>

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
        int counter;
        int N;
        std::list<pnet::type::value::value_type> Sublift;
        std::list<pnet::type::value::value_type> OUT;
        int nleads;
        ::pnetc::op::singular_template::leadsyz (input, s, library_name, base_filename, counter, N, Sublift, OUT, nleads);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "counter"), counter);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "Sublift"), Sublift);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "OUT"), OUT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "nleads"), nleads);
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
        const int & counter (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "counter"))));
        const int & N (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "N"))));
        const int & E (::boost::get< int > (_pnetc_input.value (std::list<std::string> (1, "E"))));
        std::string s (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "s"))));
        we::type::literal::control ctrl (::boost::get< we::type::literal::control > (_pnetc_input.value (std::list<std::string> (1, "ctrl"))));
        ::pnetc::op::singular_template::result (counter, N, E, s, ctrl);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "ctrl"), ctrl);
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
        std::list<pnet::type::value::value_type> output;
        int counter;
        ::pnetc::op::singular_template::All_Lead (init, C, library_name, base_filename, output, counter);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "output"), output);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "counter"), counter);
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
        const std::string & needed_library (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "needed_library"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        std::list<pnet::type::value::value_type> LIFT;
        std::string SUBLIFT;
        we::type::literal::control lift_count;
        int N;
        int E;
        ::pnetc::op::singular_template::SUBLIFT (input_name, place_Tau, needed_library, base_filename, LIFT, SUBLIFT, lift_count, N, E);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "SUBLIFT"), SUBLIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "lift_count"), lift_count);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "E"), E);
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
        const std::string & needed_library (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "needed_library"))));
        const std::string & base_filename (::boost::get< std::string > (_pnetc_input.value (std::list<std::string> (1, "base_filename"))));
        we::type::literal::control D;
        std::list<pnet::type::value::value_type> LIFT;
        int N;
        int E;
        ::pnetc::op::singular_template::LIFT (input_name, lead, needed_library, base_filename, D, LIFT, N, E);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "D"), D);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LIFT"), LIFT);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "N"), N);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "E"), E);
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
        std::string LHS_delete;
        we::type::literal::control nsummand;
        std::string RHS_delete;
        std::string s;
        ::pnetc::op::singular_template::reduce (library_name, base_filename, input, l, r, LHS_delete, nsummand, RHS_delete, s);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "LHS_delete"), LHS_delete);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "nsummand"), nsummand);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "RHS_delete"), RHS_delete);
        _pnetc_output.bind_and_discard_ref (std::list<std::string> (1, "s"), s);
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
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::removeFiles,"removeFiles");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::leadsyz,"leadsyz");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::result,"result");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::plus,"plus");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Decrement,"Decrement");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::All_Lead,"All_Lead");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::SUBLIFT,"SUBLIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::down,"down");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Increment,"Increment");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::LIFT,"LIFT");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::reduce,"reduce");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Up,"Up");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Init,"Init");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::count,"count");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::count_LIFT,"count_LIFT");
}
WE_MOD_INITIALIZE_END()
