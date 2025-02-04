#include <we/loader/macros.hpp>

#include <pnetc/op/singular_template/Init.hpp>
#include <string>

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
        std::string output;
        ::pnetc::op::singular_template::Init (input, output);
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
WE_MOD_INITIALIZE_START()
{
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::Init,"Init");
  WE_REGISTER_FUN_AS (::pnetc::op::singular_template::All_leadsyz,"All_leadsyz");
}
WE_MOD_INITIALIZE_END()
