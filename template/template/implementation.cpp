
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
//#include "Singular/lists.h"
//for cached 
#include <vector>
#include <map>
#include <filesystem>

NO_NAME_MANGLING


std::string singular_template_compute_StdBasis(std::string const& input_filename 
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
 auto start_computation = std::chrono::high_resolution_clock::now();

	out = call_user_proc(function_name, needed_library, args);
  auto end_computation = std::chrono::high_resolution_clock::now();
               auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
               std::cout << "SchFrame_Runtime:_implementation " << computation_time << " milliseconds" << std::endl;
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
  auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
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
std::tuple<std::vector<std::string>, int, long> singular_template_LIFT(const std::string& input_filename,
                                                                       const std::string& all_lead,
                                                                       const std::string& lead,
                                                                       const std::string& needed_library,
                                                                       const std::string& base_filename)
{
    // Initialize Singular and load the necessary library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    // // Debugging: Optionally print out function parameters
    // std::cout << "Input Filename_LIFT: " << input_filename << std::endl;
    // std::cout << "All Lead in LIFT: " << all_lead << std::endl;
    // std::cout << "Lead in LIFT: " << lead << std::endl;
    // std::cout << "Needed Library_LIFT: " << needed_library << std::endl;
    // std::cout << "Base Filename_LIFT: " << base_filename << std::endl;

    // Debugging: Print worker ID
    std::string ids = worker();
    // std::cout << ids << "Inside_LIFT" << std::endl;

    // Deserialize input data
    std::pair<int, lists> input;
    std::pair<int, lists> Lead;
    std::pair<int, lists> All_Lead;

    input = deserialize(input_filename, ids);
    All_Lead = deserialize(all_lead, ids);
    Lead = deserialize(lead, ids);

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args, All_Lead.first, lCopy(All_Lead.second));
    ScopedLeftv arg1(arg, Lead.first, lCopy(Lead.second));
    //std::cout << arg1.leftV()->String() <<std::endl;
   
    // Call the Singular procedure directly (LIFT_GPI) without using name mangling
    std::pair<int, lists> out;
    
    // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
     // Direct call to LIFT_GPI 
  std::string function_name = "LIFT_GPI";
     //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
    out = call_user_proc(function_name, needed_library, args);
   
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
    
    // std::cout << "LIFT Runtime: " << computation_time << " milliseconds" << std::endl;
  //  lists Token = (lists)(args.leftV()->data);
   
  // int L_size = lSize(Token)+1;
  // std::cout << "Size of J in the transition: " << L_size << std::endl;
  // for (int i = 0; i < L_size; i++) {
  //         sleftv& listElement = Token->m[i];  // Access each element as `leftv`
  //     if(listElement.data==NULL) {
  //       std::cout << "Input: NULL"  << std::endl;
  //     }
  //  else
  //  std::cout << "INPUT: " << listElement.String()  << std::endl;
     
  // }

  //   lists Token1 = (lists)(arg1.leftV()->data);
   
  // int L_si = lSize(Token1)+1;
  // std::cout << "Size of J in the transition: " << L_si << std::endl;
  // for (int i = 0; i < L_si; i++) {
  //         sleftv& listElement = Token1->m[i];  // Access each element as `leftv`
  //     std::cout << "LeadSyz: " << listElement.String()  << std::endl;
  // }

  //   lists Token2 = (lists)(arg.leftV()->data);
   
  // int L = lSize(Token2)+1;
  // std::cout << "Size of J in the transition: " << L << std::endl;
  // for (int i = 0; i < L; i++) {
  //         sleftv& listElement = Token2->m[i];  // Access each element as `leftv`
  //     std::cout << "All_lead: " << listElement.String()  << std::endl;
  // }
// lists tmp = (lists)(Token->m[3].Data()); // Access `data` directly
// lists J = (lists)(tmp->m[0].Data());      // Similarly, access `data` here
// int L_size = lSize(J);
// std::cout << "Size of J in the transition: " << L_size << std::endl;



    // try {
    //     if (std::filesystem::exists(lead)) {
    //         std::filesystem::remove(lead); // Delete the file represented by Red
    //     }
       
    // } catch (const std::filesystem::filesystem_error& e) {
    //     std::cerr << "Error deleting file: " << e.what() << std::endl;
    // }


    // Extract the result list from the output
    lists u = (lists)out.second->m[3].Data();
    
    
    std::vector<std::string> vec;
    int total_generator;

    // Iterate through each element of the outer list and serialize it
    for(int i = 0; i < lSize(u); i++) {
    
        auto Outname = serialize((lists)u->m[i].Data(), base_filename);
        // std::cout << " Outname:LIFT2 " << Outname<< std::endl;
        vec.push_back(Outname);
    }

    total_generator = lSize(u);  // Assuming u contains the computed generators
    auto total_runtime = computation_time;
// std::cout << " total gen " << total_generator<< std::endl;
// std::cout << " total time " << total_runtime<< std::endl;
  //std::cout << arg1.leftV()->String() <<std::endl;

    return {vec, total_generator, total_runtime};
}












NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int, long> singular_template_SUBLIFT(const std::string& input_filename,
                                                                 const std::string& all_lead,
                                                                const std::string& lead,
                                                               
                                                  const std::string& needed_library,
                                                   const std::string& base_filename)
{ // Initialize Singular and load library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
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
    
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,All_Lead.first, lCopy(All_Lead.second));
    ScopedLeftv arg1(arg,Lead.first, lCopy(Lead.second));
    
    
    // Call Singular procedure
    std::pair<int, lists> out;
    
    //std::cout<<"function_name_SubLIFT:"<< std:: endl;
     // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
     
    std::string function_name = "SubLIFT_GPI";
     //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
    out = call_user_proc(function_name, needed_library, args);// Call  SubLIFT_GPI with the raw pointer
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
  //std::cout << "SubLIFT_Runtime: " << computation_time << " milliseconds" << std::endl;
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
//     lists Token = (lists)(args.leftV()->data);
   
//   int L_size = lSize(Token)+1;
//   std::cout << "Size of J in the transition: " << L_size << std::endl;
//   for (int i = 0; i < L_size; i++) {
//           sleftv& listElement = Token->m[i];  // Access each element as `leftv`
//       if(listElement.data==NULL) {
//         std::cout << "Input: NULL"  << std::endl;
//       }
//    else
//    std::cout << "INPUT: " << listElement.String()  << std::endl;
     
//   }

//     lists Token1 = (lists)(arg1.leftV()->data);
   
//   int L_si = lSize(Token1)+1;
//   std::cout << "Size of J in the transition: " << L_si << std::endl;
//   for (int i = 0; i < L_si; i++) {
//           sleftv& listElement = Token1->m[i];  // Access each element as `leftv`
//       std::cout << "LIFT: " << listElement.String()  << std::endl;
//   }



//     lists Token2 = (lists)(arg.leftV()->data);
   
//   int L = lSize(Token2)+1;
//   std::cout << "Size of J in the transition: " << L << std::endl;
//   for (int i = 0; i < L; i++) {
//           sleftv& listElement = Token2->m[i];  // Access each element as `leftv`
//       std::cout << "All_lead: " << listElement.String()  << std::endl;
//   }


     std::vector<std::string> vec;
     int total_generator;
    // Extract list from the output
    lists u=NULL;
    u = (lists)out.second->m[3].CopyD();
    // u= (lists)out.second->m[3].Data();
    //std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    //std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    if (lSize(u)==0)
     {
    //std::cerr << "Error: SubLIFT_GPI returned a null list." << std::endl;
   ///return std::make_tuple(std::vector<std::string>(), 0, 0);// Early exit with default values
   return{{},0,0};
    } 
    
       //std::cerr << "Size of SubLIFT Token:" << lSize(u)+1<<std::endl;
    // Iterate through each element of the list
      for (int i = 0; i < lSize(u); i++)
      {
        auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
      // std::cout<<"serialized:"<<Outname<< std::endl;
        vec.push_back(Outname);
     }
   
    
   //auto start_serialize = std::chrono::high_resolution_clock::now();

  

    
  // End timing for serialization
   //auto end_serialize = std::chrono::high_resolution_clock::now();
   //auto  serialization_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count(); // Convert to milliseconds
   //auto total_runtime=computation_time_deserialize+computation_time+serialization_time;

   auto total_runtime=computation_time;
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

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
   

     int r=0; int c=0;
     leftv L = nullptr; leftv LL = nullptr;
   
    if (tmp->m[0].Typ() == IDEAL_CMD) {
        // Handle case where `f` is an ideal
       r = IDELEMS((ideal)(tmp->m[0].Data()));
       lists tmpL = (lists)(Tok->m[3].Data()); // Tok.data
        L=&tmpL->m[4];
        r=r;
       c = IDELEMS((ideal)L->Data());
       c=c;
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        // Handle case where `f` is a vector
        ideal t=(ideal)(tmp->m[4].Data());
        r = IDELEMS(t);
        r=r;
        lists tmpLL = (lists)(Tok->m[3].Data()); // Tok.data
        LL=&tmpLL->m[4]; //Tok.data[5]
       c = IDELEMS((ideal)LL->Data());
       c=c;
    }

    lists tmpL1 = (lists)(Tok->m[3].Data()); // Tok.data
    int counter=(int)(long)tmpL1->m[5].Data();//Tok.data[6]
  
    // matrix A;
    // matrix B;
    //leftv L2=(ideal)tmpL1->m[1];
    lists tmpl=(lists)(tok->m[3].Data()); //tok.data
    //leftv l=(ideal)(tmpl->m[1]);
   ideal A0 = (ideal)tmpL1->m[1].Data(); // Tok.data[2]
   ideal B0 = (ideal)tmpl->m[1].Data(); // tok.data[2]
    //smatrix A0=A;
    // matrix A=id_Module2Matrix(id_Copy(A0,currRing),currRing);
    // //smatrix A0=A;
    // matrix B=id_Module2Matrix(id_Copy(B0,currRing),currRing);
    // Perform the matrix addition using Singular's API function
    // matrix C0 = mp_Add(A, B, currRing);
    ideal C = sm_Add(A0, B0, currRing);
    // mp_Delete(&A,currRing);mp_Delete(&B,currRing);
    // ideal C=id_Matrix2Module(mp_Copy(C0,currRing),currRing);
    // matrix C=id_Module2Matrix(C0,currRing);
//     std::cout << "Final in ADD transition _Reduce=" << std::endl;
// for(int k = 1; k <= MATROWS(C); k++) {
//     for(int l = 1; l <= MATCOLS(C); l++) {
//         poly entr = MATELEM(C, k, l);
//         std::cout << "Entry at position (" << k << ", " << l << ") = " << pString(entr) << std::endl;
//     }
// }


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
    t->Init(7);
    t->m[0].rtyp = tmpL1->m[0].rtyp;t->m[0].data=tmpL1->m[0].CopyD(); // copy Tok.data[1]
    t->m[1].rtyp=SMATRIX_CMD; t->m[1].data=C;
    t->m[2].rtyp=INT_CMD;  t->m[2].data = (void*)(long)r;
    t->m[3].rtyp=INT_CMD; t->m[3].data = (void*)(long)c;
    
   

   if (tmp->m[0].Typ() == IDEAL_CMD) {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= L->CopyD();//Tok.data[5]

     } 
     else if (tmp->m[0].Typ() == VECTOR_CMD) {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= LL->CopyD();//Tok.data[5]
     }

    t->m[5].rtyp=INT_CMD;  t->m[5].data = (void*)(long)counter;
   t->m[6].rtyp=INT_CMD; t->m[6].data = (void*)(long)(counter + 1);
   output->m[3].rtyp=LIST_CMD; output->m[3].data=t;//data is in postion 4
    
     int cmd_nr;
     blackboxIsCmd("token",cmd_nr); // get the type id of "token"
     
     return {cmd_nr,output};
}




NO_NAME_MANGLING
std::pair<std::string, long> singular_template_reduce(const std::string& Red, 
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
    auto start_computation = std::chrono::high_resolution_clock::now();

    out = reduce_GPI(arg1.leftV());  // Call reduce_GPI with the raw pointer

   
      // Delete the files represented by Red and lift after using them
    // try {
    //     if (std::filesystem::exists(input_filename)) {
    //         std::filesystem::remove(input_filename); // Delete the file represented by Red
    //     }
    //     if (std::filesystem::exists(lift)) {
    //         std::filesystem::remove(lift); // Delete the file represented by lift
    //     }
    // } catch (const std::filesystem::filesystem_error& e) {
    //     std::cerr << "Error deleting file: " << e.what() << std::endl;
    // }
    
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
    out_filename = serialize(out.second, base_filename);
    auto total_runtime = computation_time;
    
   
    return {out_filename, total_runtime};
}



NO_NAME_MANGLING


std::string singular_template_Generate(const std::string& res,
                                       const std::string& syz,
										                   const std::string & needed_library,
            							             const std::string& base_filename)
{
	init_singular (config::singularLibrary().string());
	load_singular_library(needed_library);
	std::pair<int,lists> Res;
  std::pair<int,lists> Syz;
	std::pair<int, lists> out;
	std::string ids;
	std::string out_filename;
	ids = worker();
	//std::cout << ids << " in singular_..._compute" << std::endl;
	Res = deserialize(res,ids);
  Syz = deserialize(syz,ids);
  
	ScopedLeftv args( Res.first, lCopy(Res.second));
  ScopedLeftv arg(args,Syz.first, lCopy(Syz.second));
  std::string function_name = "Generate_GPI";
	out = call_user_proc(function_name, needed_library, args);
  out_filename = serialize(out.second, base_filename);
    
	return out_filename;
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