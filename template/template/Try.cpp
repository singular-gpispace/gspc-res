#include <Singular/libsingular.h>

#include <interface/template_interface.hpp>
#include <interface/WorkflowResult.hpp>
#include <interface/ValuesOnPorts.hpp>
#include <interface/Workflow.hpp>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include "config.hpp"
#include "singular_functions.hpp"



NO_NAME_MANGLING


std::string  singular_template_compute_StdBasis(   std::string const& input_filename 
										, std::string const& needed_library
            							, std::string const& base_filename)
{
	init_singular (config::singularLibrary().string());
	load_singular_library(needed_library);
	std::pair<int,lists> input;
	std::pair<int, lists> out;
	std::string ids;
	std::string out_filename;
	ids = worker();
	//std::cout << ids << " in singular_..._compute" << std::endl;
	input = deserialize(input_filename,ids);
  
	ScopedLeftv args( input.first, lCopy(input.second));
  std::string function_name2 = "stdBasis";
	out = call_user_proc(function_name2, needed_library, args);
  out_filename = serialize(out.second, base_filename);
    
	return out_filename;
}


NO_NAME_MANGLING
std::pair<std::vector<std::string>, unsigned long>  singular_template_leadSyzGPI(const std::string& input_filename,
                                                     const std::string& needed_library,
                                                     const std::string& base_filename)
{
    // Initialize Singular and load library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    // Debugging: Print out function parameters
    // std::cout << "Function Parameters:" << std::endl;
    // std::cout << "Input Filename: " << input_filename << std::endl;
    // std::cout << "Needed Library: " << needed_library << std::endl;
    // std::cout << "Base Filename: " << base_filename << std::endl;

    // Debugging: Print worker ID
    std::string ids = worker();
    //std::cout << ids << " in singular_template_Leadvector" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    input = deserialize(input_filename, ids);

    // Debugging: Print input data
    //std::cout << "Input Data:" << input.second << std::endl;

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));

    // Call Singular procedure
    std::pair<int, lists> out;
    std::string function_name = "leadsyz_GpI";
    out = call_user_proc(function_name, needed_library, args);
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     unsigned long total_generator;

    // // Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    //std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output_LEADSYZ:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator};

}

NO_NAME_MANGLING
std::pair<std::vector<std::string>, unsigned long> singular_template_LIFT(const std::string& input_filename,
                                                                const std::string& lead_syz,
                                                  const std::string& needed_library,
                                                   const std::string& base_filename)
{ // Initialize Singular and load library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    // Debugging: Print out function parameters
    std::cout << "Function Parameters:" << std::endl;
    std::cout << "Input Filename_LIFT: " << input_filename << std::endl;
    std::cout << "Input Filename_LIFT: " << lead_syz << std::endl;
    std::cout << "Needed Library_LIFT: " << needed_library << std::endl;
    std::cout << "Base Filename_LIFT: " << base_filename << std::endl;

    // Debugging: Print worker ID
    std::string ids = worker();
    std::cout << ids << "Inside_LIFT" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    std::pair<int, lists> Lead_Syz;
    input = deserialize(input_filename, ids);
    Lead_Syz = deserialize(lead_syz, ids);
    // Debugging: Print input data
    std::cout << "Input Data_LIFT:" << input.second << std::endl;
    std::cout << "Lead_Syz:" << Lead_Syz.second << std::endl;

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,Lead_Syz.first, lCopy(Lead_Syz.second));
    // Call Singular procedure
    std::pair<int, lists> out;
    std::string function_name = "LIFT_GPI";
     std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
    out = call_user_proc(function_name, needed_library, args);
    std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
    unsigned long  total_generator;

    // // Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    std::cout<<"Check the output:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator};

}



NO_NAME_MANGLING
std::pair<std::vector<std::string>, unsigned long> singular_template_SUBLIFT(const std::string& input_filename,
                                                                const std::string& lift,
                                                  const std::string& needed_library,
                                                   const std::string& base_filename)
{ // Initialize Singular and load library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    // Debugging: Print out function parameters
    // std::cout << "Function Parameters:" << std::endl;
    // std::cout << "Input Filename_LIFT: " << input_filename << std::endl;
    // std::cout << "Input Filename_LIFT: " << lead_syz << std::endl;
    // std::cout << "Needed Library_LIFT: " << needed_library << std::endl;
    // std::cout << "Base Filename_LIFT: " << base_filename << std::endl;

    // Debugging: Print worker ID
    std::string ids = worker();
    std::cout << ids << "Inside_LIFT" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    std::pair<int, lists> lift_syz;
    input = deserialize(input_filename, ids);
    lift_syz= deserialize(lift, ids);
    // Debugging: Print input data
    std::cout << "Input Data_SubLIFT:" << input.second << std::endl;
    std::cout << "lift_syz:" << lift_syz.second << std::endl;

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,lift_syz.first, lCopy(lift_syz.second));
    // Call Singular procedure
    std::pair<int, lists> out;
    std::string function_name = "SubLIFT_GPI";
     std::cout<<"function_name_SubLIFT:"<<function_name<< std:: endl;
    out = call_user_proc(function_name, needed_library, args);
    std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     unsigned long total_generator;

    // // Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    std::cout<<"Check the output:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator};

}
NO_NAME_MANGLING

std::string  singular_template_reduce( const std::string& input_filename,
                                                  const std::string& lift,
                                                  const std::string& needed_library,
                                                  const std::string& base_filename)
{
	init_singular (config::singularLibrary().string());
	load_singular_library(needed_library);
	std::pair<int, lists> out;
	std::string ids;
	std::string out_filename;
	ids = worker();
    std::pair<int, lists> input;
    std::pair<int, lists> lift_syz;
     // Deserialize input data
    input = deserialize(input_filename, ids);
    lift_syz= deserialize(lift, ids);


 // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,lift_syz.first, lCopy(lift_syz.second));
    std::string function_name = "reduce_GPI";
	out = call_user_proc(function_name, needed_library, args);
  out_filename = serialize(out.second, base_filename);
    
	return out_filename;
}
