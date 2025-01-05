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
#include <chrono>
#include <tuple>

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
std::string singular_template_Init( std::string const& input) {

    return input;
}






NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long> singular_template_leadSyzGPI(const std::string& input_filename,
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
    //auto start_deserialize = std::chrono::high_resolution_clock::now();
    input = deserialize(input_filename, ids);
   
  //auto end_deserialize = std::chrono::high_resolution_clock::now();
  //auto computation_time_deserialize = std::chrono::duration_cast<std::chrono::milliseconds>( end_deserialize - start_deserialize).count();

    // Debugging: Print input data
    //std::cout << "Input Data:" << input.second << std::endl;

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));

    // Call Singular procedure
    std::pair<int, lists> out;
    std::string function_name = "leadsyz_GpI";
    auto start_computation = std::chrono::high_resolution_clock::now();
    out = call_user_proc(function_name, needed_library, args);
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
  auto end_computation = std::chrono::high_resolution_clock::now();
  auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
  //std::cout << "LEADSYZ_Runtime: " << computation_time << " milliseconds" << std::endl;
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     int total_generator;

    // // Iterate through each element of the outer list
     //auto start_serialize = std::chrono::high_resolution_clock::now();
    for(int i (0); i<lSize(u); i++)

  {
    //std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output_LEADSYZ:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
  //auto end_serialize = std::chrono::high_resolution_clock::now();
   //auto  serialization_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count(); // Convert to milliseconds
 //auto total_runtime= computation_time_deserialize+computation_time+ serialization_time;
auto total_runtime=computation_time;

   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}





NO_NAME_MANGLING


std::pair<std::vector<std::string>, int> singular_template_all_leadsyz_GpI( std::string const& input_filename 
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
  std::string function_name = "all_leadsyz_GpI";
	out = call_user_proc(function_name, needed_library, args);
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     int total_generator;

    // // Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    //std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator};
}


NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long> singular_template_LIFT(const std::string& input_filename,
                                                                 const std::string& all_lead,
                                                                const std::string& lead,
                                                               
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
    //std::cout << ids << "Inside_LIFT" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    std::pair<int, lists> Lead;
    std::pair<int, lists> All_Lead;
    
    //auto start_deserialize = std::chrono::high_resolution_clock::now();
    input = deserialize(input_filename, ids);
    All_Lead = deserialize(all_lead, ids);
    Lead = deserialize(lead, ids);
   
  //auto end_deserialize = std::chrono::high_resolution_clock::now();
  //auto computation_time_deserialize = std::chrono::duration_cast<std::chrono::milliseconds>( end_deserialize - start_deserialize).count();

    // Debugging: Print input data
    // std::cout << "Input Data_LIFT:" << input.second << std::endl;
    // std::cout << "Lead_Syz:" << Lead_Syz.second << std::endl;
    // std::cout << "L0:" << L0.second << std::endl;
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,All_Lead.first, lCopy(All_Lead.second));
    ScopedLeftv arg1(arg,Lead.first, lCopy(Lead.second));
    
    
    // Call Singular procedure
    std::pair<int, lists> out;
    std::string function_name = "LIFT_GPI";
    //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
    // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
    out = call_user_proc(function_name, needed_library, args);
  auto end_computation = std::chrono::high_resolution_clock::now();
  auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
  //std::cout << "LIFT_Runtime: " << computation_time << " milliseconds" << std::endl;
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     int total_generator;
    //auto start_serialize = std::chrono::high_resolution_clock::now();
    // // Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    //std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
  // End timing for serialization
   //auto end_serialize = std::chrono::high_resolution_clock::now();
   //auto  serialization_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count(); // Convert to milliseconds

  //auto total_runtime= computation_time_deserialize+computation_time+ serialization_time;
   auto total_runtime= computation_time;
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}


NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long> singular_template_SUBLIFT(const std::string& input_filename,
                                                                 const std::string& all_lead,
                                                                const std::string& lead,
                                                               
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
    //std::cout << ids << "Inside_LIFT" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    std::pair<int, lists> Lead;
    std::pair<int, lists> All_Lead;
    
    //auto start_deserialize = std::chrono::high_resolution_clock::now();
    input = deserialize(input_filename, ids);
    All_Lead = deserialize(all_lead, ids);
    Lead = deserialize(lead, ids);
  //auto end_deserialize = std::chrono::high_resolution_clock::now();
  //auto computation_time_deserialize = std::chrono::duration_cast<std::chrono::milliseconds>( end_deserialize - start_deserialize).count();

    
    // Debugging: Print input data
    // std::cout << "Input Data_LIFT:" << input.second << std::endl;
    // std::cout << "Lead_Syz:" << Lead_Syz.second << std::endl;
    // std::cout << "L0:" << L0.second << std::endl;
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,All_Lead.first, lCopy(All_Lead.second));
    ScopedLeftv arg1(arg,Lead.first, lCopy(Lead.second));
    
    
    // Call Singular procedure
    std::pair<int, lists> out;
    std::string function_name = "SubLIFT_GPI";
    //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
     // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
    out = call_user_proc(function_name, needed_library, args);
    auto end_computation = std::chrono::high_resolution_clock::now();
   auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
  //std::cout << "SubLIFT_Runtime: " << computation_time << " milliseconds" << std::endl;
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     int total_generator;
   //auto start_serialize = std::chrono::high_resolution_clock::now();
    //Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    //std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
  // End timing for serialization
   //auto end_serialize = std::chrono::high_resolution_clock::now();
   //auto  serialization_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count(); // Convert to milliseconds
   //auto total_runtime=computation_time_deserialize+computation_time+serialization_time;

   auto total_runtime=computation_time;
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}







std::pair<std::string, long>  singular_template_reduce( const std::string& Red, 
                                                  const std::string& input_filename,
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
    std::pair<int, lists> Gb;
    std::pair<int, lists> input;
    std::pair<int, lists> lift_syz;

      //auto start_deserialize = std::chrono::high_resolution_clock::now();
     // Deserialize input data
     Gb= deserialize(Red, ids);
    input = deserialize(input_filename, ids);
    lift_syz= deserialize(lift, ids);
    //auto end_deserialize = std::chrono::high_resolution_clock::now();
    //auto computation_time_deserialize = std::chrono::duration_cast<std::chrono::milliseconds>( end_deserialize - start_deserialize).count();


 // Prepare arguments
    ScopedLeftv arg1(Gb.first, lCopy(Gb.second));
    ScopedLeftv args(arg1,input.first, lCopy(input.second));
    ScopedLeftv arg(args,lift_syz.first, lCopy(lift_syz.second));
    std::string function_name = "reduce_GPI";
   // std::cout<<"function_name_reduce:"<<function_name<< std:: endl;
    
    // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
	out = call_user_proc(function_name, needed_library, arg1);
  auto end_computation = std::chrono::high_resolution_clock::now();
  auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();


   //auto start_serialize = std::chrono::high_resolution_clock::now();
  out_filename = serialize(out.second, base_filename);
  //auto end_serialize = std::chrono::high_resolution_clock::now();
  //auto computation_time_serialize = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count();
  //auto total_runtime=computation_time_deserialize+computation_time+computation_time_serialize;
  auto total_runtime=computation_time;
	return {out_filename, total_runtime};
}