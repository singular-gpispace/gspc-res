
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









NO_NAME_MANGLING


matrix lcm_mod(ideal G) { //ideal G is Singular module

    int a=0;
    int b=0;
    int i=0;
    int j=0;
    int r = IDELEMS(G);
    matrix l=mpNew(r,r);
    poly s10=NULL;
    poly t10=NULL;
    for (a = 0; a < r; a++) {
        for (b = 0; b < r; b++) {
             //std::cout << "G->m[a]>: " << pString(G->m[a]) << std::endl;
             //std::cout << "G->m[b]>: " << pString(G->m[b]) << std::endl;
           
            //i = leadexp(G[a])[nvars(basering) + 1];
            i= p_GetComp(G->m[a],currRing);
            //j = leadexp(G[b])[nvars(basering) + 1];
            j= p_GetComp(G->m[b],currRing);
            
   
            s10 = pHead(G->m[a]); //Should be leadmonomial
            pSetComp(s10,0);
            pSetmComp(s10);
            t10 = pHead(G->m[b]);//Should be leadmonomial
            pSetComp(t10,0);
            pSetmComp(t10);
            poly lcm_poly =  p_Lcm(s10, t10, currRing);
            pSetCoeff0(lcm_poly,nInit(1));

            if (i == j) {
               // l[a, b] = lcm(leadmonomial(G[a]), leadmonomial(G[b])) / lead(t10);
                
                MATELEM(l, a, b) = pp_Divide(lcm_poly, t10, currRing);
                 //std::cout << "m[a.b] in lcm_mod: " << pString(MATELEM(l, a, b)) << std::endl;

            } else {
                // If i is not equal to j, set l[a, b] to 0
                MATELEM(l,a,b)= NULL;
            }
        }
    }
    

    return l;
}





//First Level Leadsyz
ideal leadSyz(ideal f) {
    int a = 0, b = 0, j = 0, k = 0;
    poly s = NULL; // vector s
    poly t = NULL; // vector t
    int r = IDELEMS(f);  // Get number of elements in ideal f
    ideal L = idInit(0, 1);   // Initialize module L with initial size 0
    matrix m = mpNew(r, r);

    //std::cout << "ideal in leadSyz: " << IDELEMS(f) << std::endl;

    // Fill matrix m with LCM-based computations
    for (a = 0; a < r; a++) {
        for (b = 0; b < r; b++) {
            poly lcm = p_Lcm(pHead(f->m[b]), pHead(f->m[a]), currRing);
            pSetCoeff0(lcm, nInit(1));  // Set leading coefficient to 1
            MATELEM(m, a, b) = pp_Divide(lcm, pHead(f->m[b]), currRing);  // Fill matrix with quotient
        }
    }
         
    int cc = 0; // Counter for elements in L

   
    for (int i = 1; i < r; i++) {
        for (j = 0; j < i; j++) {
            // Initialize t from MATELEM(m, j, i)
            poly t0 = pCopy(MATELEM(m, j, i));
            p_SetComp(t0, i + 1, currRing);  // Set component for t0
            p_SetmComp(t0, currRing);  // Set component for t0
            t = pCopy(t0);  // Copy t0 to t

            
            for (k = 0; k < IDELEMS(L); k++) {
                s = (poly)L->m[k]; // Get element s = L[k]
                //  std::cout << "s=L[k] " << k << ": " << pString(s) << std::endl;
                bool c = p_DivisibleBy(pHead(s), t, currRing);  // Check if s divides t
                if (c == TRUE) {
                    t = NULL;  // If s divides t, set t to NULL
                    break;  // Exit inner loop since t is "zero"
                } else {
                    // Check if t divides s
                    bool d = p_DivisibleBy(pHead(t), s, currRing);
                    if (d == TRUE) {
                        // std::cout << "s =: " << pString(s) << ": t=" << pString(t) << std::endl;
                        
                        // If t divides s, remove s from L
                        {
                            ideal tmp = id_Delete_Pos(L, k, currRing); // Delete element at position k
                            L = NULL;  // Set L to NULL before deletion
                            idDelete(&L); // Delete old L to free memory
                            L = tmp;   // Assign the new ideal after deletion

                            // Adjust counter and reindex
                            k--;  // Reindex after deletion to avoid skipping the next element
                            cc--; // Decrement counter since an element was removed
                        }
                    }
                }
            }

            // If t is still not NULL, add it to L
            if (t != NULL) {
                if (cc >= IDELEMS(L)) {
                    // If L is full, resize it
                    ideal tmpL = idInit(cc + 1, 1);  // Resize with extra space
                    for (int i = 0; i < cc; i++) {
                        tmpL->m[i] = pCopy(L->m[i]);  // Copy elements from L to tmpL
                        L->m[i] = NULL;  // Clear L after copying
                    }
                    idDelete(&L);  // Delete old L to free memory
                    L = tmpL;  // Point L to resized tmpL
                }
                // Add t to L
                L->m[cc] = pCopy(t); // Copy t into L at position cc
                cc++;  // Increment counter
            }
        }
    }
        // Debug output
    // std::cout << "Final first leasyz size: " << IDELEMS(L) << std::endl;
    // for (int k = 0; k < cc; k++) {
    //     std::cout << "Generator " << k << ": " << pString((poly)L->m[k]) << std::endl;
    // }
  //  std::cout << "size of ideal in leadSyz: " << IDELEMS(L) << std::endl;

    return L;  // Return the ideal L
}





//iterative leadsyz

ideal Sec_leadSyz(ideal f0) {
    int r = IDELEMS(f0);  // Get the number of elements in the ideal f0
    poly s = NULL;  // Polynomial s is a singular vector
    poly t = NULL;  // Polynomial t is a singular vector
    int cc = 0; // Counter for elements in L
    
    // Initialize ideal L with initial size 0
    ideal L = idInit(0, 1);

    // Create a matrix M using lcm_mod for the input ideal f0
    matrix M = lcm_mod(f0);  // Ensure lcm_mod returns a valid matrix

    // Loop through pairs (i, j) in the matrix
    for (int i = 1; i < r; i++) {
        for (int j = 0; j < i; j++) {
            // Fetch the matrix element at (j, i)
            poly t0 = MATELEM(M, j, i);

            if (t0 != NULL) {
                // Set the component and multigrade component for t0
                p_SetComp(t0, i + 1, currRing);
                p_SetmComp(t0, currRing);

                // Copy t0 into t
                t = pCopy(t0);  
            }

            // Ensure L is not NULL before accessing it
            for (int k = 0; k < IDELEMS(L); k++) { 
                // Fetch the k-th element of L (s = L[k])
                s = (poly)L->m[k];  

                // Ensure both s and t are not NULL before checking divisibility
                if (s != NULL && t != NULL) {
                    // Check if s divides t
                    if (p_DivisibleBy(pHead(s), t, currRing)) {
                        // If s divides t, set t to NULL and break out of the loop
                        t = NULL; 
                        break;
                    } 
                    // Check if t divides s
                    else if (p_DivisibleBy(t, pHead(s), currRing)) {
                        // Log the removal for debugging purposes
                        // std::cout << "Removing s =: " << pString(s) << ": t=" << pString(t) << std::endl;

                        // Remove s from L using id_Delete_Pos
                        ideal tmp = id_Delete_Pos(L, k, currRing);
                        
                        // Set L to NULL, delete the old L, and assign the new ideal
                        idDelete(&L);  // Delete old L
                        L = tmp;  // Assign new ideal

                        // Adjust indexing and counter after deletion
                        k--;  // Reindex to avoid skipping elements
                        cc--; // Decrement the counter after the deletion
                    }
                }
            }

            // If t is not NULL, add it to L
            if (t != NULL) {
                // Resize L if necessary
                if (cc >= IDELEMS(L)) {
                    // Create a temporary ideal tmpL with space for one more element
                    ideal tmpL = idInit(cc + 1, 1);

                    // Copy elements from L to tmpL
                    for (int i = 0; i < cc; i++) {
                        tmpL->m[i] = pCopy(L->m[i]);  // Copy elements
                        L->m[i] = NULL;  // Clear L after copying
                    }

                    // Delete old L and assign the resized tmpL to L
                    idDelete(&L);  // Delete old L
                    L = tmpL;  // Assign resized tmpL to L
                }

                // Add t to L at the next available position
                L->m[cc] = t;
                cc++;  // Increment counter after adding t
            } 
        }
    }

       // Debug output
    // std::cout << "Final second-Leadsyz  size: " << IDELEMS(L) << std::endl;
    // for (int k = 0; k < cc; k++) {
    //     std::cout << "Generator " << k << ": " << pString((poly)L->m[k]) << std::endl;
    // }
    // return (L);
    return L;
}












std::pair<int, lists> leadSyz_GpI(leftv args) {
    // Extract Token
    lists Token = (lists)(args->Data());
    
    // Extract the first element (f)
    lists tmp = (lists)(Token->m[3].Data());
    // ideal f = (ideal)(tmp->m[0].Data());
     //std::cout << "ideal from tokesn:" << IDELEMS(f)<< std::endl;
    int counter = 0;
    int r0 = 0;
    int r=0;
    // ideal LT=NULL;
    ideal LT = idInit(0, 1);
    
    // Check if f is an ideal or a vector
    if (tmp->m[0].Typ() == IDEAL_CMD) {
       ideal f_copy = idCopy((ideal)(tmp->m[0].Data()));  // Create a deep copy of f
       //std::cout << "ideal:" <<tmp->m[0].Typ()<< std::endl;
        // If it's an ideal, call leadSyz
        LT = leadSyz(f_copy);
     
        r = IDELEMS(LT); 


        r0 = IDELEMS(f_copy); 
        r0=r0; // Set row count to the number of elements in ideal
        idDelete(&f_copy);  // Clean up copy
    // for(int k=0; k< IDELEMS(LT); k++){
    //          std::cout << "leadsyz:" << pString((poly)LT->m[k])<< std::endl;
    //      }


        counter = 1;//counter=1
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        // If it's a vector, handle it using Sec_leadSyz
         //std::cout << "ideal in vector case" <<tmp->m[0].Typ()<< std::endl;
        ideal mM = (ideal)(tmp->m[4].Data()); 
         ideal mM_copy =idCopy(mM); 
        //    for(int k=0; k< IDELEMS(mM); k++){
        //      std::cout << "First_leadsyz:from GPI=" << pString((poly)mM->m[k])<< std::endl;
        //  }
        r0 =IDELEMS(mM_copy);  // Set row count for matrix
      
        r0=r0;
        LT = Sec_leadSyz(mM_copy);
        r = IDELEMS(LT); 
          idDelete(&mM_copy);
        //   for(int k=0; k< IDELEMS(LT); k++){
        //      std::cout << "Second_leadsyz:" << pString((poly)LT->m[k])<< std::endl;
        //  }

        counter = (int)(long)tmp->m[6].Data();
    }


    // Prepare the LLT token
    lists LLT = (lists)omAlloc0Bin(slists_bin);
    LLT->Init(4); // Initialize with 4 fields
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);

    // Iterate to fill in data
    for (int k = 0; k < r; k++) {
        // Create a new token Ld
        lists Ld = (lists)omAlloc0Bin(slists_bin);
        Ld->Init(4); // Initialize with 4 fields

        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = strdup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = strdup("Sparse_matrix_Leadsyz");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

        matrix sM = mpNew(r0, c);
        poly s_lift = (poly)LT->m[k]; // Retrieve the lifted polynomial
        int l_k = p_GetComp(s_lift, currRing);

        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);

        MATELEM(sM, l_k, colmn) = pCopy(lm);

        // Prepare Ld data
        t = (lists)omAlloc0Bin(slists_bin);
        t->Init(7);
        t->m[0].rtyp = VECTOR_CMD; 
        t->m[0].data = pCopy(s_lift);
        t->m[1].rtyp = MATRIX_CMD; 
        t->m[1].data = sM;
        t->m[2].rtyp = INT_CMD; 
        t->m[2].data = (void*)(long)l_k;
        t->m[3].rtyp = INT_CMD; 
        t->m[3].data = (void*)(long)(k+1);

      
            t->m[4].rtyp = MODUL_CMD; 
            t->m[4].data = idCopy(LT); // J[2]
       

        t->m[5].rtyp = INT_CMD;  
        t->m[5].data = (void*)(long)counter;
        t->m[6].rtyp = INT_CMD; 
        t->m[6].data = (void*)(long)(counter);

        Ld->m[3].rtyp = LIST_CMD; 
        Ld->m[3].data = t;

        // Set fieldnames[k] to "generator"
        lists field_names = (lists)omAlloc0Bin(slists_bin);
        field_names->Init(r);
        for (int s = 0; s < r; s++) {
            field_names->m[s].rtyp = STRING_CMD;
            field_names->m[s].data = strdup("generator");
        }

        LLT->m[0].rtyp = RING_CMD; 
        LLT->m[0].data = currRing;

        // Assign fieldnames to LLT
        LLT->m[1].rtyp = LIST_CMD;
        LLT->m[1].data = field_names;
        LLT->m[2].rtyp = RING_CMD; 
        LLT->m[2].data = currRing;

        // Set data for LLT
        lists t0 = (lists)omAlloc0Bin(slists_bin);
        t0->Init(r);
        for (int s = 0; s < r; s++) {
            t0->m[s].rtyp = LIST_CMD;
            t0->m[s].data = lCopy(Ld);
        }
        temp->m[k].rtyp = LIST_CMD;
        temp->m[k].data = lCopy(Ld);
    }

    // Prepare the final field names
    lists final_field_names = (lists)omAlloc0Bin(slists_bin);
    final_field_names->Init(r + 1);
    for (int s = 0; s < r; s++) {
        final_field_names->m[s].rtyp = STRING_CMD;
        final_field_names->m[s].data = omStrDup((const char*)LLT->m[1].data);
    }

    // Append "total_number_generator"
    final_field_names->m[r].rtyp = STRING_CMD;
    final_field_names->m[r].data = strdup("total_number_generator");
    LLT->m[1].rtyp = LIST_CMD;  
    LLT->m[1].data = final_field_names;

    // Final data preparation
    lists final_data = (lists)omAlloc0Bin(slists_bin);
    final_data->Init(r + 1);
    for (int k = 0; k < r; k++) {
        final_data->m[k].rtyp = LIST_CMD;
        final_data->m[k].data = temp->m[k].data;  // Transfer data from temp
    }

    final_data->m[r].rtyp = INT_CMD;
    final_data->m[r].data = (void*)(long)(r + 1); // Set total count

    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;

    return {r, LLT};  // Return success state and LLT
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
  
    auto start_computation = std::chrono::high_resolution_clock::now();
    out =  leadSyz_GpI(args.leftV());
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
      std::cout<<"GPI_LEADSYZ_tokens="<<lSize(u)<< std::endl;
     
    for(int i (0); i<lSize(u); i++)

  { 
    //std::cout<<"size of u:"<<lSize(u)+1<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    //std::cout<<"LEADSYZ:="<<Outname<< std::endl;
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
  
    matrix A;
    matrix B;
    //leftv L2=(ideal)tmpL1->m[1];
    lists tmpl=(lists)(tok->m[3].Data()); //tok.data
    //leftv l=(ideal)(tmpl->m[1]);
    A = (matrix)tmpL1->m[1].Data(); // Tok.data[2]
    B = (matrix)tmpl->m[1].Data(); // tok.data[2]
    //smatrix A0=A;
    ideal A0=id_Matrix2Module(mp_Copy(A,currRing),currRing);
    //smatrix A0=A;
    ideal B0=id_Matrix2Module(mp_Copy(B,currRing),currRing);
    // Perform the matrix addition using Singular's API function
    ideal C0 = sm_Add(A0, B0, currRing);
    idDelete(&A0);idDelete(&B0);
    matrix C=id_Module2Matrix(C0,currRing);
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
    t->m[1].rtyp=MATRIX_CMD; t->m[1].data=C;
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




lists allLeadSyz(ideal f) {
    // Allocate memory for the lists structure
    lists J = (lists)omAlloc0Bin(slists_bin);

    
    J->m[0].rtyp = IDEAL_CMD;  
    J->m[0].data = f;          

    
    ideal F = leadSyz(f);

    
    J->m[1].rtyp = MODUL_CMD;  
    J->m[1].data = F;

    int n=nvars(currRing);
    int level = 1;
    for(int t=0;t<n;t++) {
        // Compute the next syzygy ideal using Sec_leadSyz
        ideal temp = Sec_leadSyz(F);

        // Check if the result is empty
        if (nrows(temp)!=0 && IDELEMS(temp)!= 0) {
          F=idCopy(temp);
        }
          if(F==poly(0)){
            break;
          }
       J->m[level+1].rtyp==MODUL_CMD;
       J->m[level+1].data=idCopy(F);
       level++;

    // Return the list of syzygy ideals
   
}
 return J;
}

 
proc all_leadsyz_GpI(token Token) 
"USAGE: all_leadsyz_GpI(Token); Token token
RETURN: a token LLT containing a list of leading syzygies
NOTE: The types token are defined . This procedure is 
intended to be called as a transition procedure inside the
worker process (that is, the "inner" instances of SINGULAR).
"

{
  def f= Token.data[1];
 
    list J=allLeadSyz(f);
    int r = size(J);
    token LLT;
 
   int k;
  
  for (k = 1; k <=r; k++) {
    token Ld;
    Ld.fieldnames = list("generator", "SchFrame");  
  
    
    Ld.data[1]= J;
    //print(s_lift);
    Ld.data[2]= r;
    LLT.fieldnames[k] = "generator";  
    LLT.data[k] =Ld; 

  }
      LLT.fieldnames[size(LLT.fieldnames) + 1] = "total_number_generator";  
      LLT.data[size(LLT.data) + 1] = r;  
    return (LLT);  
}



lists allLeadSyz(ideal f) {
    // Allocate memory for the lists structure
    lists J = (lists)omAlloc0Bin(slists_bin);
    J->Init(2); // Initialize the list with two elements

    // Initialize the first two elements
    J->m[0].rtyp = IDEAL_CMD;  
    J->m[0].data = f;          

    ideal F = leadSyz(f);

    J->m[1].rtyp = MODUL_CMD;  
    J->m[1].data = F;

    int cc = 2; // Current count of elements in the list

    while (TRUE) {
        // Compute the next syzygy ideal using Sec_leadSyz
        ideal temp = Sec_leadSyz(F);

        // Check if the result is empty
        if (temp == NULL || IDELEMS(temp) == 0) {
            break; // Terminate if the ideal is empty
        }

        // Update F with the new ideal
        F = idCopy(temp);

        // Resize the list if necessary
        if (cc >= lSize(J)) {
            // Allocate a new list with additional space
            int newSize = cc + 1; // Increase size by 1
            lists tmpL = (lists)omAlloc0Bin(slists_bin);
            tmpL->Init(newSize);

            // Copy elements from J to tmpL
            for (int i = 0; i < cc; i++) {
                tmpL->m[i].rtyp = J->m[i].rtyp;
                tmpL->m[i].data = J->m[i].data;
            }

          
            omFreeBin(J, slists_bin);

            // Assign the resized list
            J = tmpL;
        }

        // Append the new ideal to the list
        J->m[cc].rtyp = MODUL_CMD;
        J->m[cc].data = F;
        cc++; // Increment the count of elements
    }

    // Return the list of syzygy ideals
    return J;
}




 std::pair<int, lists> all_leadsyz_GpI(leftv args) {
    // Extract Token
    lists Token = (lists)(args->Data());
    
    // Extract Token.data
     lists tmp = (lists)(Token->m[3].Data());
     //Extract def f=Token.data[1]
    ideal f=(ideal)tmp->m[0].Data();
    ideal f_copy=idCopy(f);
    lists J=allLeadSyz(f_copy);
    idDelete(&f_copy);
    int r=lSize(J)+1;
    int k=0;
     lists LLT=(lists)omAlloc0Bin(slists_bin);
    LLT->Init(4);// type token
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);

    for (k = 0; k < r; k++) {
       // Create a new token Ld
      lists Ld=(lists)omAlloc0Bin(slists_bin);
      Ld->Init(4);// type token 

      
      lists t=(lists)omAlloc0Bin(slists_bin);
      t->Init(2);
      t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
      t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("SchFrame");
    
      Ld->m[1].rtyp=LIST_CMD; Ld->m[1].data=t;
      Ld->m[0].rtyp=RING_CMD; Ld->m[0].data=currRing;
      Ld->m[2].rtyp=RING_CMD; Ld->m[2].data=currRing;
        t=(lists)omAlloc0Bin(slists_bin);
     t->Init(2);
     t->m[0].rtyp=LIST_CMD;  t->m[0].data=lCopy(J);
     t->m[2].rtyp=INT_CMD;  t->m[2].data = (void*)(long)r;
     Ld->m[3].rtyp=LIST_CMD; Ld->m[3].data=t;   
    // Set fieldnames[k] to "generator"
   lists field_names = (lists)omAlloc0Bin(slists_bin);
   field_names->Init(r);
 for (int s = 0; s < r; s++) 
 {
    field_names->m[s].rtyp = STRING_CMD;
    field_names->m[s].data = strdup("generator"); 
   // std::cout << "LLT[k]:generator "<<k<< std::endl;
 }
     LLT->m[0].rtyp=RING_CMD; LLT->m[0].data=currRing;
 // Assign fieldnames to LLT
  LLT->m[1].rtyp = LIST_CMD;
  LLT->m[1].data = field_names;
  
  
  LLT->m[2].rtyp=RING_CMD; LLT->m[2].data=currRing;

     //LLT.data[k] = Ld; 
     
    
   // Set data for LLT
  lists t0 = (lists)omAlloc0Bin(slists_bin);
  t0->Init(r);
  for (int s = 0; s < r; s++) 
  {
    t0->m[s].rtyp = LIST_CMD;
    t0->m[s].data = lCopy(Ld);  
  }

  // LLT->m[3].rtyp = LIST_CMD;
  // LLT->m[3].data = t0;
 
    temp->m[k].rtyp = LIST_CMD;
    temp->m[k].data = lCopy(Ld); 
 
    }

   
lists field_names = (lists)omAlloc0Bin(slists_bin);
field_names->Init(r + 1);  

// Copy existing "generator" field names
for (int s = 0; s < r; s++) 
{
    field_names->m[s].rtyp = STRING_CMD;
    field_names->m[s].data =omStrDup((const char*)LLT->m[1].data);  // Add existing field names
}

// Append "total_number_generator" at position r
field_names->m[r].rtyp = STRING_CMD;
field_names->m[r].data = strdup("total_number_generator");

// Set field names in LLT
LLT->m[1].rtyp = LIST_CMD;  
LLT->m[1].data = field_names;



     // Append "total_number_generator" at the end
    lists final_data = (lists)omAlloc0Bin(slists_bin);
    final_data->Init(r + 1);

    // Copy generator data into final_data
    for (int s = 0; s < r; s++) {
        final_data->m[s].rtyp = LIST_CMD;
        final_data->m[s].data = lCopy(static_cast<lists>(temp->m[s].data));
        
    }

    // Append total_number_generator
    final_data->m[r].rtyp = INT_CMD;
    final_data->m[r].data = (void*)(long)(r+1);

    // Set final_data as LLT->m[3] (LLT.data)
    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;




    return {r, LLT};  // Return r (number of generators) and LLT
}
