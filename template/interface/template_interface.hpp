#pragma once

#define NO_NAME_MANGLING extern "C"
#include <vector>
#include <string>
#include <config.hpp>

#define RESOLVE_INTERFACE_FUNCTION(function) \
      (fhg::util::scoped_dlhandle \
      (config::implementation(), \
      RTLD_GLOBAL | RTLD_NOW | RTLD_DEEPBIND) \
      .sym<decltype(function)> \
      (BOOST_PP_STRINGIZE(function)))





NO_NAME_MANGLING
std::string singular_template_compute_StdBasis ( std::string const&
                                      , std::string const&
									  , std::string const&
                                      );





NO_NAME_MANGLING
std::string singular_template_Init( std::string const& );







NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long>  singular_template_ALL_LEAD( std::string const&
                                                    
                                                         , std::string const&
									  , std::string const&
                                      );


NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long> singular_template_LEAD( std::string const&
                                                    
                                                         , std::string const&
									  , std::string const&
                                      );

NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long> singular_template_LIFT( std::string const&
                                      , std::string const&
                                     , std::string const&
                                   
                                      , std::string const&
						 , unsigned long N
                                      );

          
NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long> singular_template_SUBLIFT ( std::string const&
                                      , std::string const&
                                  
                                     ,std::string const&
                                      , std::string const&
						 , unsigned long N
                                      );

                                      NO_NAME_MANGLING
                                      std::pair<std::string, long> singular_template_reduce(
                                          std::string const& Red,
                                          const std::pair<int, int>& ranges,
                                          int N,
                                          std::string const& needed_library,
                                          std::string const& base_filename
                                      );
                                      


                                      NO_NAME_MANGLING
                                      std::pair<std::string, long> singular_template_ADD_Seq( std::string const&
                                                                            ,int N
                                                                           , std::string const&
                                                                         
                                                                , std::string const&
                                                                            );   


                                      

                                      
                                      NO_NAME_MANGLING
std::string  singular_template_removeFiles(const std::string&,
                                      const std::string&);   

                                      NO_NAME_MANGLING
std::string singular_template_Rename_File(const std::string&,int N,const std::string&,const std::string&);                   