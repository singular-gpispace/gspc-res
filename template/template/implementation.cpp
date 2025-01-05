#include <Singular/libsingular.h>

#include <interface/template_interface.hpp>
#include <interface/WorkflowResult.hpp>
#include <interface/ValuesOnPorts.hpp>
#include <interface/Workflow.hpp>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <chrono>
#include "config.hpp"
#include "singular_functions.hpp"

#include <filesystem>

NO_NAME_MANGLING


std::string  singular_template_compute_StdBasis(  std::string const& input_filename 
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
std::pair<std::vector<std::string>, int>  singular_template_leadSyzGPI(const std::string& input_filename,
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
    //  out = leadsyz_GpI(args.leftV());
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
   
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
    
    //std::cout<<"Check the output_LEADSYZ:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator};

}





NO_NAME_MANGLING
std::pair<std::vector<std::string>, int>  singular_template_all_leadsyz_GpI(const std::string& input_filename,
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
    std::string function_name = "all_leadsyz_GpI";
    out = call_user_proc(function_name, needed_library, args);

   
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
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
    
    //std::cout<<"Check the output_LEADSYZ:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator};

}







NO_NAME_MANGLING
std::pair<std::vector<std::string>, int> singular_template_LIFT(const std::string& input_filename,
                                                                const std::string& lead_syz,
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
    std::pair<int, lists> Lead_Syz;
    input = deserialize(input_filename, ids);
    Lead_Syz = deserialize(lead_syz, ids);
    // Debugging: Print input data
    // std::cout << "Input Data_LIFT:" << input.second << std::endl;
    // std::cout << "Lead_Syz:" << Lead_Syz.second << std::endl;

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,Lead_Syz.first, lCopy(Lead_Syz.second));
    // Call Singular procedure
    std::pair<int, lists> out;
   std::string function_name = "LIFT_GPI";
     //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
    out = call_user_proc(function_name, needed_library, args);
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    //std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    //std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
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
std::pair<std::vector<std::string>, int> singular_template_SUBLIFT(const std::string& input_filename,
                                                                const std::string& lift,
                                                  const std::string& needed_library,
                                                   const std::string& base_filename)
{ // Initialize Singular and load library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    
    // Debugging: Print worker ID
    std::string ids = worker();
   // std::cout << ids << "Inside_LIFT" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    std::pair<int, lists> lift_syz;
    input = deserialize(input_filename, ids);
    lift_syz= deserialize(lift, ids);
    // Debugging: Print input data
    // std::cout << "Input Data_SubLIFT:" << input.second << std::endl;
    // std::cout << "lift_syz:" << lift_syz.second << std::endl;

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,lift_syz.first, lCopy(lift_syz.second));
    // Call Singular procedure
    std::pair<int, lists> out;
    std::string function_name = "SubLIFT_GPI";
     //std::cout<<"function_name_SubLIFT:"<<function_name<< std:: endl;
    out = call_user_proc(function_name, needed_library, args);
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     int total_generator;

    // // Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
   // std::cout<<"checkMemory:"<<u->m[i].Data()<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"Check the output:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator};

}







NO_NAME_MANGLING
std::pair<int, lists> reduce_GPI(leftv arg1) {

    //std::cout << "Type of 1.arg:" << arg1->Typ() <<std::endl;
    lists input = (lists)(arg1->Data()); //extract input
    //std::cout << "Type of 2.arg:" << arg1->next->Typ() <<std::endl;
    lists Tok = (lists)(arg1->next->Data()); // extract Tok
    //std::cout << "Type of 3.arg:" << arg1->next->next->Typ() <<std::endl;
    lists tok = (lists)(arg1->next->next->Data()); //extract tok

    lists tmp = (lists)(input->m[3].Data()); // input.data
   

     int r=0; 
     //int c=0;
   
  
        // Handle case where `f` is an ideal
       r = IDELEMS((ideal)(tmp->m[0].Data()));
       r=r;
       lists tmpL = (lists)(Tok->m[3].Data()); // Tok.data
      
  
    matrix A;
    matrix B;
    //leftv L2=(ideal)tmpL1->m[1];
    lists tmpl=(lists)(tok->m[3].Data()); //tok.data
    //leftv l=(ideal)(tmpl->m[1]);
    A = (matrix)tmpL->m[1].Data(); // Tok.data[2]

    // if (tmpL->m[0].Typ() == SMATRIX_CMD){
      
    //   std::cout << "A is smatrix"<<std::endl;
    // }
   
      // std::cout << "Colum of A:="<< MATCOLS(A) << std::endl;
      // std::cout << "Row of A:="<< MATROWS(A) << std::endl;
    //std::cout << "column of A:" << c<<std::endl;
     B = (matrix)tmpl->m[1].Data(); // tok.data[2]
  //  std::cout << "Colum of B:="<< MATCOLS(B) << std::endl;
  //  std::cout << "Row of B:="<< MATROWS(B) << std::endl;
    // Perform the matrix addition using Singular's API function
    matrix C = mp_Add(A, B, currRing);
  //   std::cout << "Row of C:="<< MATROWS(C) << std::endl;
  //  std::cout << "Colum of C:="<< MATCOLS(C) << std::endl;

  // Prepare the output token
    lists output=(lists)omAlloc0Bin(slists_bin);
    output->Init(4);// type token
    // fieldnames
    lists t=(lists)omAlloc0Bin(slists_bin);
    t->Init(2);
    t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
    t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("FirstSyz_matrix");
    output->m[1].rtyp=LIST_CMD; output->m[1].data=t;
     output->m[0].rtyp=RING_CMD; output->m[0].data=currRing;
      output->m[2].rtyp=RING_CMD; output->m[2].data=currRing;
    //data
    t=(lists)omAlloc0Bin(slists_bin);
    t->Init(2);
    t->m[0].rtyp = tmpL->m[0].rtyp;t->m[0].data=tmpL->m[0].CopyD(); // copy Tok.data[1]
    t->m[1].rtyp=SMATRIX_CMD; t->m[1].data=C;
  
    output->m[3].rtyp=LIST_CMD; output->m[3].data=t;//data is in postion 4
    
     int cmd_nr;
     blackboxIsCmd("token",cmd_nr); // get the type id of "token"
     
     return {cmd_nr,output};
}

NO_NAME_MANGLING
std::string  singular_template_reduce(const std::string& Red, 
                                                      const std::string& input_filename,
                                                      const std::string& lift,
                                                      const std::string& needed_library,
                                                      const std::string& base_filename) 
{
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);

    std::pair<int, lists> out;
    std::string ids;
    std::string out_filename;
    ids = worker();
    
    std::pair<int, lists> Gb = deserialize(Red, ids);
    std::pair<int, lists> input = deserialize(input_filename, ids);
    std::pair<int, lists> lift_syz = deserialize(lift, ids);

    // Prepare arguments
    ScopedLeftv arg1(Gb.first, lCopy(Gb.second));
    ScopedLeftv args(arg1, input.first, lCopy(input.second));
    ScopedLeftv arg(args, lift_syz.first, lCopy(lift_syz.second));
    
    // Measure Computation Time
    // auto start_computation = std::chrono::high_resolution_clock::now();

    out = reduce_GPI(arg1.leftV());  // Call reduce_GPI with the raw pointer

   
    
  
    // auto end_computation = std::chrono::high_resolution_clock::now();
    // auto computation_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
    out_filename = serialize(out.second, base_filename);
    // auto total_runtime = computation_time;
    //  std::cout << "Total_time_Reduce:" << total_runtime <<std::endl;
    return {out_filename};
}



NO_NAME_MANGLING
// Define singular_template_removeFiles instead of delete
std::string singular_template_removeFiles(const std::string& Red, const std::string& lift) {
    try {
        if (std::filesystem::exists(Red)) {
            std::filesystem::remove(Red); // Delete the file represented by Red
        }
        if (std::filesystem::exists(lift)) {
            std::filesystem::remove(lift); // Delete the file represented by lift
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error deleting file: " << e.what() << std::endl;
    }
    return {};
}