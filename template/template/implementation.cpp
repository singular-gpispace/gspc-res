
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
#include <sstream>
#include <string>

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

matrix lcm_mod(ideal G) {
    int a = 0, b = 0, i = 0, j = 0;
    ideal G_copy = idCopy(G);
    int r = IDELEMS(G_copy);
    // for (int k = 0; k < r; k++) {
    //     std::cout << "ideal G: " << pString((poly)G->m[k]) << std::endl;
    // }
    // std::cout << "r in lcm_mod:= " << r << std::endl;
    // omUpdateInfo();
    // std::cout << "used mem: " << om_Info.UsedBytes << std::endl;

    matrix l = mpNew(r, r);
    // std::cout << "pointer l: " << (long)l << std::endl;
    // std::cout << "Row of l: " << MATROWS(l) << std::endl;
    // std::cout << "Cols of l: " << MATCOLS(l) << std::endl;

    poly s10 = NULL;
    poly t10 = NULL;

    for (a = 1; a < r+1; a++) {
        for (b = 1; b < r+1; b++) {
            i = p_GetComp(G->m[a-1], currRing);
            j = p_GetComp(G->m[b-1], currRing);

            s10 = pHead(G->m[a-1]);
            pSetComp(s10, 0);
            pSetmComp(s10);

            t10 = pHead(G->m[b-1]);
            pSetComp(t10, 0);
            pSetmComp(t10);

            poly lcm_poly = p_Lcm(s10, t10, currRing);
            pSetCoeff0(lcm_poly, nInit(1));

            if (i == j) {
                MATELEM(l, a, b) = pp_Divide(lcm_poly, t10, currRing);
            } else {
                MATELEM(l, a, b) = NULL;
            }
        }
    }

    p_Delete(&s10, currRing);
    p_Delete(&t10, currRing);

    return l;
}

ideal leadSyz(ideal f) {
    int a = 0, b = 0, j = 0, k = 0;
    poly s = NULL, t = NULL;
    ideal f_copy = idCopy(f);
    int r = IDELEMS(f_copy);
    ideal L = idInit(0, 1);
    matrix M = mpNew(r, r);

    for (a = 1; a < r+1; a++) {
        for (b = 1; b < r+1; b++) {
            poly lcm = p_Lcm(pHead(f_copy->m[b-1]), pHead(f_copy->m[a-1]), currRing);
            pSetCoeff0(lcm, nInit(1));
            MATELEM(M, a, b) = pp_Divide(lcm, pHead(f_copy->m[b-1]), currRing);
        }
    }

    int cc = 0;
    for (int i = 2; i < r+1; i++) {
        for (j = 1; j < i; j++) {
            poly t0 = pCopy(MATELEM(M, j, i));
            p_SetComp(t0, i, currRing);
            p_SetmComp(t0, currRing);
            t = pCopy(t0);

            for (k = 0; k < IDELEMS(L); k++) {
                s = (poly)L->m[k];
                bool c = p_DivisibleBy(pHead(s), t, currRing);
                if (c == TRUE) {
                    t = NULL;
                    break;
                } else {
                    bool d = p_DivisibleBy(pHead(t), s, currRing);
                    if (d == TRUE) {
                        ideal tmp = id_Delete_Pos(L, k, currRing);
                        idDelete(&L);
                        L = tmp;
                        k--;
                        cc--;
                    }
                }
            }

            if (t != NULL) {
                if (cc >= IDELEMS(L)) {
                    ideal tmpL = idInit(cc + 1, 1);
                    for (int i = 0; i < cc; i++) {
                        tmpL->m[i] = pCopy(L->m[i]);
                        L->m[i] = NULL;
                    }
                    idDelete(&L);
                    L = tmpL;
                }
                L->m[cc] = pCopy(t);
                cc++;
                p_Delete(&t, currRing);
            }
        }
    }

    // std::cout << "Final first leadsyz size: " << IDELEMS(L) << std::endl;

    return L;
}

ideal Sec_leadSyz(ideal f0) {
    int r = IDELEMS(f0);
    poly s = NULL, t = NULL;
    int cc = 0;
    ideal L = idInit(0, 1);
    ideal f_copy = idCopy(f0);
    matrix M = lcm_mod(f_copy);

    for (int i = 2; i < r+1; i++) {
        for (int j = 1; j < i; j++) {
            poly t0 = MATELEM(M, j, i);
            if (t0 != NULL) {
                p_SetComp(t0, i, currRing);
                p_SetmComp(t0, currRing);
                t = pCopy(t0);
            }

            for (int k = 0; k < IDELEMS(L); k++) {
                s = (poly)L->m[k];
                if (s != NULL && t != NULL) {
                    if (p_DivisibleBy(pHead(s), t, currRing)) {
                        t = NULL;
                        break;
                    } else if (p_DivisibleBy(t, pHead(s), currRing)) {
                        ideal tmp = id_Delete_Pos(L, k, currRing);
                        idDelete(&L);
                        L = tmp;
                        k--;
                        cc--;
                    }
                }
            }

            if (t != NULL) {
                if (cc >= IDELEMS(L)) {
                    ideal tmpL = idInit(cc + 1, 1);
                    for (int i = 0; i < cc; i++) {
                        tmpL->m[i] = pCopy(L->m[i]);
                        L->m[i] = NULL;
                    }
                    idDelete(&L);
                    L = tmpL;
                }
                L->m[cc] = t;
                cc++;
            }
        }
    }

    idDelete((ideal*)&M);

    // std::cout << "Final second-Leadsyz size: " << IDELEMS(L) << std::endl;

    return L;
}

lists aLL_LEAD(ideal f) {
    lists J = (lists)omAlloc0Bin(slists_bin);
    J->Init(2);
    ideal f_copy = idCopy(f);
    int n = rVar(currRing);
    ideal F = leadSyz(f_copy);
    // int g = IDELEMS(F);
    // idDelete(&f);
    ideal F_copy = idCopy(F);

    J->m[0].rtyp = IDEAL_CMD;
    J->m[0].data = f_copy;

    J->m[1].rtyp = MODUL_CMD;
    J->m[1].data = F_copy;

    int cc = 2;

    for (int t = 0; t < n; t++) {
        ideal temp = Sec_leadSyz(F_copy);

        bool b = idIs0(temp);
        if (b == FALSE) {
            F_copy = idCopy(temp);
            temp = NULL;
        } else {
            break;
        }

        if (cc >= lSize(J) + 1) {
            int newSize = cc + 1;
            lists tmpL = (lists)omAlloc0Bin(slists_bin);
            tmpL->Init(newSize);

            for (int i = 0; i < cc; i++) {
                tmpL->m[i].rtyp = J->m[i].rtyp;
                tmpL->m[i].data = J->m[i].data;
            }

            omFreeBin(J, slists_bin);
            J = tmpL;
        }

        J->m[cc].rtyp = MODUL_CMD;
        J->m[cc].data = F_copy;
        cc++;
    }

    return J;
}








NO_NAME_MANGLING
std::pair<int, lists> ALL_LEAD_GPI(leftv args) {
    // Extract Token
    lists Token = (lists)(args->Data());
    
    // Extract Token.data
    lists tmp = (lists)(Token->m[3].Data());
    // Extract def f=Token.data[1]
    ideal f = (ideal)tmp->m[0].Data();
    ideal f_copy = idCopy(f);
   
    // for(int k = 0; k < IDELEMS(f_copy); k++) {
    //     std::cout << "Generator input aLL_LEAD" << k << ": " << pString((poly)f_copy->m[k]) << std::endl;
    // }
   
    lists J = aLL_LEAD(f_copy);
    int r = lSize(J) + 1;
    // std::cout << "lSize(J):=" << r << std::endl;

    // for(int k = 0; k < r; k++) {
    //     ideal l = (ideal)J->m[k].Data();
    //     for(int s = 0; s < IDELEMS(l); s++) {
    //         std::cout << "Generator in all_lead " << k << ": " << pString((poly)l->m[s]) << std::endl;
    //     }
    // }

    lists LLT = (lists)omAlloc0Bin(slists_bin);
   
    LLT->Init(4); // type token
    
    lists temp = (lists)omAlloc0Bin(slists_bin);
   
    temp->Init(r);

    for(int k = 0; k < r; k++) {
        lists Ld = (lists)omAlloc0Bin(slists_bin);
       
        Ld->Init(4); // type token
        
        lists t = (lists)omAlloc0Bin(slists_bin);
      
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = strdup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = strdup("SchFrame");

        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;

        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

        t = (lists)omAlloc0Bin(slists_bin);
      
        t->Init(2);
        t->m[0].rtyp = LIST_CMD; t->m[0].data = lCopy(J);
        t->m[1].rtyp = INT_CMD; t->m[1].data = (void*)(long)(r);
        Ld->m[3].rtyp = LIST_CMD; Ld->m[3].data = t;

        lists field_names = (lists)omAlloc0Bin(slists_bin);
      
        field_names->Init(r);
        for (int s = 0; s < r; s++) {
            field_names->m[s].rtyp = STRING_CMD;
            field_names->m[s].data = strdup("generator");
           
        }

        LLT->m[0].rtyp = RING_CMD; LLT->m[0].data = currRing;

        // Assign fieldnames to LLT
        LLT->m[1].rtyp = LIST_CMD;
        LLT->m[1].data = field_names;

        LLT->m[2].rtyp = RING_CMD; LLT->m[2].data = currRing;

        lists t0 = (lists)omAlloc0Bin(slists_bin);
      
        t0->Init(r);
        for (int s = 0; s < r; s++) {
            t0->m[s].rtyp = LIST_CMD;
            t0->m[s].data = lCopy(Ld);
        }

        LLT->m[3].rtyp = LIST_CMD;
        LLT->m[3].data = t0;

        temp->m[k].rtyp = LIST_CMD;
        temp->m[k].data = lCopy(Ld);
    }

    // std::cout << "size of token Sch frame: After loop" << r << std::endl;
    // std::cout << "Allocating memory for final_field_names with size: " << r + 1 << std::endl;

    // Prepare the final field names
    lists t= (lists)omAlloc0Bin(slists_bin);
//   std::cout << "Before Init: r = " << r << std::endl;
t->Init(r+1);
// std::cout << "After Init: r = " << r << std::endl;


    for (int s = 0; s < r; s++) {
        t->m[s].rtyp = STRING_CMD;
       t->m[s].data = omStrDup((const char*)LLT->m[1].data);
       
    }

    // Append "total_number_generator"
   t->m[r].rtyp = STRING_CMD;
   t->m[r].data = strdup("total_number_generator");
 
    LLT->m[1].rtyp = LIST_CMD;  
    LLT->m[1].data =t;

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
std::tuple<std::vector<std::string>, int, long> singular_template_ALL_LEAD(std::string const& input_filename 
										, std::string const& needed_library
            							, std::string const& base_filename)
{
	init_singular (config::singularLibrary().string());
	load_singular_library(needed_library);
	std::pair<int,lists> input;
	std::pair<int, lists> out;
	std::string ids;
	
	ids = worker();
	//std::cout << ids << " in singular_..._compute" << std::endl;
	input = deserialize(input_filename,ids);
  
	ScopedLeftv args( input.first, lCopy(input.second));
//   std::string function_name = "all_leadsyz_GpI";
 auto start_computation = std::chrono::high_resolution_clock::now();

	// out = call_user_proc(function_name, needed_library, args);
     out =  ALL_LEAD_GPI(args.leftV());

  // std::cout << "SchFrame_Runtime:_implementation " << computation_time << " milliseconds" << std::endl;
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
    auto end_computation = std::chrono::high_resolution_clock::now();
  auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
   total_generator = lSize(u); // Assuming u contains the computed generators
   auto total_runtime=computation_time;
    // std::cout << "total_runtime_SchFrame:=" << total_runtime<<" "<<"nanoseconds"<< std::endl;
  return {vec, total_generator,total_runtime};
}


NO_NAME_MANGLING
std::pair<int, lists> LEAD_GPI(leftv args) {
    // Extract Token
    lists Token = (lists)(args->Data());
    
    // Extract the first element (f)
    lists tmp = (lists)(Token->m[3].Data());
    // ideal f = (ideal)(tmp->m[0].Data());
     //std::cout << "ideal from tokesn:" << IDELEMS(f)<< std::endl;
    int counter =0;
    int r0 =0;
    int r=0;
    int c=0;
    // ideal LT=NULL;
    ideal LT = nullptr;
    
    // Check if f is an ideal or a vector
    if (tmp->m[0].Typ() == IDEAL_CMD) {
       ideal f_copy = idCopy((ideal)(tmp->m[0].Data()));  // Create a deep copy of f
      //  std::cout << "ideal:" <<tmp->m[0].Typ()<< std::endl;
        // If it's an ideal, call leadSyz
        LT = leadSyz(f_copy);
     
        r = IDELEMS(LT); 
         c=r;

        r0 = IDELEMS(f_copy); 
       
        idDelete(&f_copy);  // Clean up copy
    // for(int k=0; k< IDELEMS(LT); k++){
    //          std::cout << "leadsyz:" << pString((poly)LT->m[k])<< std::endl;
    //      }


        counter = 1;//counter=1
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        // If it's a vector, handle it using Sec_leadSyz
        std::cout << "vector:" <<tmp->m[0].Typ()<< std::endl;
        ideal mM = (ideal)(tmp->m[4].Data()); 
         ideal mM_copy =idCopy(mM); 
        //    for(int k=0; k< IDELEMS(mM); k++){
        //      std::cout << "First_leadsyz:from GPI=" << pString((poly)mM->m[k])<< std::endl;
        //  }
        r0 =IDELEMS(mM_copy);  // Set row count for matrix
      
       
        LT = Sec_leadSyz(mM_copy);
        r = IDELEMS(LT); 
        c=r;
          idDelete(&mM_copy);
        //   for(int k=0; k< IDELEMS(LT); k++){
        //      std::cout << "Second_leadsyz:" << pString((poly)LT->m[k])<< std::endl;
        //  }

        counter = (int)(long)tmp->m[6].Data();
    }
    
//  std::cout << "#LeadSyz_GPI:=" <<r<< std::endl;
    // Prepare the LLT token
    lists LLT = (lists)omAlloc0Bin(slists_bin);
    LLT->Init(4); // Initialize with 4 fields
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);

    ideal sM = idInit(c, r0);  // Initialize the smatrix
    lists Ld = NULL; //(lists)omAlloc0Bin(slists_bin);  // Initialize Ld

    for (int k = 0; k < r; k++) {
        // Create a new token Ld
      id_Delete(&sM, currRing);  // Delete the existing sM
        sM = idInit(c, r0);        // Reinitialize sM

        // omUpdateInfo();
        // std::cout << "used mem: " << om_Info.UsedBytes << std::endl;
    
    
       Ld = (lists)omAlloc0Bin(slists_bin);  // Reinitialize Ld
       Ld->Init(4);  // Initialize with 4 fields

        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = strdup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = strdup("Sparse_LeadSyz_matrix");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;
         
         
       
        // matrix sM = mpNew(r0, c);
        poly s_lift = (poly)LT->m[k]; // Retrieve the lifted polynomial
        //  std::cout << "#s_lift:=" <<pString(s_lift)<< std::endl;
        int l_k = p_GetComp(s_lift, currRing);

        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);
        poly C=sM->m[k];
        //  std::cout << "#poly C:=" <<pString(C)<< std::endl;
        poly Ci=p_Vec2Poly(C,l_k,currRing);
       
        C=p_Sub(C,Ci,currRing);
        //  std::cout << "after C-Ci:=" <<pString(C)<< std::endl;
         poly C1= pCopy(lm);
                   p_SetComp(C1,l_k,currRing);
                   p_SetmComp(C1,currRing);
                   
//        std::cout << "Before addition C: " << pString(C) << std::endl;
// std::cout << "Before addition C1: " << pString(C1) << std::endl;

C=p_Add_q(C, pCopy(C1), currRing);

// std::cout << "After addition C: " << pString(C) << std::endl;
sM->m[k]=C;
        // Prepare Ld data
        t = (lists)omAlloc0Bin(slists_bin);
        t->Init(7);
        t->m[0].rtyp = VECTOR_CMD; 
        t->m[0].data = pCopy(s_lift);
        t->m[1].rtyp = SMATRIX_CMD; 
        t->m[1].data = sM;
        t->m[2].rtyp = INT_CMD; 
        t->m[2].data = (void*)(long)l_k;
        t->m[3].rtyp = INT_CMD; 
        t->m[3].data = (void*)(long)(k+1);

    
            t->m[4].rtyp = MODUL_CMD; 
            t->m[4].data = idCopy(LT);
        

        t->m[5].rtyp = INT_CMD;  
        t->m[5].data = (void*)(long)(counter);
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
        // lists t0 = (lists)omAlloc0Bin(slists_bin);
        // t0->Init(r);
        // for (int s = 0; s < r; s++) {
        //     t0->m[s].rtyp = LIST_CMD;
        //     t0->m[s].data = lCopy(Ld);
        // }

        temp->m[k].rtyp = LIST_CMD;
        temp->m[k].data = lCopy(Ld);
         // Clean up temporary lists
        //  omFreeBin(t, slists_bin);
        //  omFreeBin(field_names, slists_bin);
        //  omFreeBin(t0, slists_bin);
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
        temp->m[k].data = NULL; 

    }

    final_data->m[r].rtyp = INT_CMD;
    final_data->m[r].data = (void*)(long)(r + 1); // Set total count

    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;

       // Clean up sM,Ld and temp
    id_Delete(&sM, currRing);
    omFreeBin(Ld, slists_bin);
    temp->Clean(currRing);

    return {r, LLT};  // Return success state and LLT
}





NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long> singular_template_LEAD(const std::string& input_filename,
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
//    std::string function_name = "leadsyz_GpI";
 auto start_computation = std::chrono::high_resolution_clock::now();

	// out = call_user_proc(function_name, needed_library, args);
   
    out =  LEAD_GPI(args.leftV());
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
  
  //std::cout << "LEADSYZ_Runtime: " << computation_time << " milliseconds" << std::endl;
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    
   
     std::vector<std::string> vec;
     int total_generator;

    // // Iterate through each element of the outer list
     //auto start_serialize = std::chrono::high_resolution_clock::now();
      // std::cout<<"GPI_LEADSYZ_tokens="<<lSize(u)<< std::endl;
     
    for(int i (0); i<lSize(u); i++)

  { 
    //std::cout<<"size of u:"<<lSize(u)+1<<std::endl;
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    // std::cout<<"LEADSYZ:="<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
  auto end_computation = std::chrono::high_resolution_clock::now();
   auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
  //auto end_serialize = std::chrono::high_resolution_clock::now();
   //auto  serialization_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count(); // Convert to milliseconds
 //auto total_runtime= computation_time_deserialize+computation_time+ serialization_time;
auto total_runtime=computation_time;




//  std::cout << "total_runtime_LeadSYZ:=" << total_runtime<<" "<<"nanoseconds"<< std::endl;
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}





NO_NAME_MANGLING

lists pOLY_List(poly f0) 
{
    int r = pLength(f0); 
    lists S = (lists)omAlloc0Bin(slists_bin);

    if (r == 0) 
    {
        S->Init(0); 
        return S;
    }

    S->Init(r);

    for (int k = 0; k < r; k++) 
    {
        // Set the type of the entry
        S->m[k].rtyp = POLY_CMD;
        //std::cout << "Entry " << k << " rtyp: " << S->m[k].rtyp << std::endl;

        // Ensure rtyp matches POLY_CMD
        // bool bl = (S->m[k].Typ() == POLY_CMD);
        // std::cout << "rtyp matches POLY_CMD: " << bl << std::endl;

        // Get the current leading term (monomial)
        poly headTerm = pHead(f0);
        if (headTerm != NULL) {
            S->m[k].data = headTerm;
            //std::cout << "Adding term: " << pString((poly)S->m[k].data) << " at index " << k << std::endl;
        } else {
            std::cout << "Warning: pHead(f0) returned NULL at index " << k << std::endl;
        }

        // Move to the next term in the polynomial
        f0 = pNext(f0);

        // Check if f0 is NULL before accessing it
        if (f0 == NULL) 
        {
           // std::cout << "No more terms, breaking loop." << std::endl;
            break;
        }
    }

    return S;
}








NO_NAME_MANGLING
// For vectors
lists vector_List(poly f0) 
{
    int r = pLength(f0); 
    //std::cout << "vector_LIST= " <<pString(pCopy(f0))<< std::endl;
    lists S = (lists)omAlloc0Bin(slists_bin);

    if (r == 0) 
    {
        S->Init(0); 
        return S;
    }

    S->Init(r);

    for (int k = 0; k < r; k++) 
    {
        // Set the type of the entry
        S->m[k].rtyp = VECTOR_CMD;
        //std::cout << "Entry " << k << " rtyp: " << S->m[k].rtyp << std::endl;

        // Ensure rtyp matches POLY_CMD
        // bool bl = (S->m[k].Typ() == POLY_CMD);
        // std::cout << "rtyp matches POLY_CMD: " << bl << std::endl;

        // Get the current leading term (monomial)
        poly headTerm = pHead(f0);
        if (headTerm != NULL) 
        {
            S->m[k].data = headTerm;
            //std::cout << "Adding term: " << pString((poly)S->m[k].data) << " at index " << k << std::endl;
        } else {
            std::cout << "Warning: pHead(f0) returned NULL at index " << k << std::endl;
        }

        // Move to the next term in the polynomial
        f0 = pNext(f0);

        // Check if f0 is NULL before accessing it
        if (f0 == NULL) 
        {
           // std::cout << "No more terms, breaking loop." << std::endl;
            break;
        }
    }

    return S;
}








NO_NAME_MANGLING
poly LOT(poly g0, ideal S1) 
{
    // std::cout << "LOT_g= " << pString(g0) << std::endl;
    // std::cout << "LOT_S1= " << IDELEMS(S1) << std::endl;
    poly t = NULL;
    int i = 0;
    poly LOT = pCopy(g0);

    // std::cout << "LOT_initialize: " << pString(LOT) << std::endl;
    
    while (g0!= NULL) 
    {
        for (i = 0; i < IDELEMS(S1); i++) 
        {
            t = pHead(g0); // Leading term of g0
          
            bool c = p_DivisibleBy(S1->m[i], t, currRing);
            // std::cout << "Divisibility check result: " << c << std::endl;
          
            if (c == TRUE)
             { // Check normal form
                // std::cout << "LOT_found _t :" << t << std::endl;

                LOT = p_Sub(LOT, pCopy(t), currRing);
                break;
            }
        }
        
        g0 = p_Sub(g0, pCopy(t), currRing);
        // std::cout << "LOT_return :" << pString(LOT) << std::endl;
    }

    return LOT;
}

NO_NAME_MANGLING
poly LOT_2(poly g0, ideal S1, lists J, int level) 
{
   // std::cout << "levelin LOT_2= " << level<< std::endl;
    //std::cout << "Size of FirstSyz= " << IDELEMS(S1) << std::endl;
    int r = IDELEMS(S1);  // Get the number of generators in F0
    ideal F0 = (ideal)J->m[level-1].Data(); // Get the module F0 from J[level]
    poly LOT = pCopy(g0);
    poly t = NULL;
    int i = 0;
   

    // std::cout << "LOT_initialize: " << pString(LOT) << std::endl;
    

    while (g0!= NULL) 
    {
        for (i = 0; i < r; i++) 
        {
            t = pHead(g0); // Leading term of g0
          
            bool c = p_DivisibleBy(pHead(F0->m[i]), t, currRing);
            // std::cout << "Divisibility check result: " << c << std::endl;
           
            if (c == TRUE)
             { // Check normal form
                //std::cout << "LOT_found _t :" << pString(t) << std::endl;

                LOT = p_Sub(LOT, pCopy(t), currRing);
                break;
            }
        }
        
        g0 = p_Sub(g0, pCopy(t), currRing);
        // std::cout << "LOT_return :" << pString(LOT) << std::endl;
    }

    return LOT;
}





NO_NAME_MANGLING
int coM(ideal f, poly s, poly t) {//poly s and poly t are singular vectors
  
  //std::cout << "s in cOM:"<<pString(s)<< std::endl;
  //std::cout << "t in cOM:"<<pString(t)<< std::endl;
  int i=0;
  int j=0;
  int isTrue = 0;
  //i = leadexp(s)[nvars(basering) + 1];
 i= p_GetComp(s,currRing);
  //std::cout << "component of s in cOM:"<<i<< std::endl;
  //j = leadexp(t)[nvars(basering) + 1];
 j= p_GetComp(t,currRing);
  //std::cout << "component of t in cOM:"<<j<< std::endl;
  //poly s10 = leadmonomial(s);
   
    poly s10= pHead(s);
    pSetComp(s10,0);
    pSetmComp(s10);
    pSetCoeff(s10,nInit(1));
  //poly t10 = leadmonomial(t);
   poly t10= pHead(t);
    pSetComp(t10,0);
    pSetmComp(t10);
    pSetCoeff(t10,nInit(1));
 //std::cout << " s="<<pString(s)<<":lead(s) ="<<pString(s10)<<":t="<<pString(t)<<":lead of t="<<pString(t10) <<std::endl;
  //std::cout << "lead(t) in cOM:"<<pString(t10)<< std::endl;
  //poly sfi = leadmonom(s10 * f[i]);
  poly sfi=pp_Mult_qq(s10,pHead(f->m[i-1]),currRing);//should leadmonom

  //poly tfj = leadmonom(t10 * f[j]);
 poly tfj=pp_Mult_qq(t10,pHead(f->m[j-1]),currRing); //should leadmonom
 
  int c= p_Cmp(sfi,tfj,currRing);

 // std::cout << "sfi>tfjin cOM:"<<c<< std::endl;
  if (c==1) {
     //std::cout << "sfi>tfjin cOM:"<<c<< std::endl;
    isTrue = 1;
  } else if ((c==0) && (i > j)) 
   {
      isTrue = 1;
   }
 //std::cout << "Leadsyz s="<<pString(s)<<":coM(s,s_v)="<<isTrue<<" 0f s= "<<pString(s)<<" and s_v="<<pString(t)<< std::endl;
  
  return isTrue;
}



NO_NAME_MANGLING
int coM2(ideal f, poly s, poly t, lists J, int k) { //poly s and poly t are singular vectors
//ideal f is singular module

          // std::cout << "Level in coM2="<<k<< std::endl;                   
  
    int i=0;
    int j=0;
    int isTrue = 0;
    poly f1=NULL;
    poly g1=NULL;
    poly F1=NULL;
     poly G1=NULL;  // Declare vectors for transformations
    int l=0;
    int i1=0;
    int i2=0;  // Loop and index variables
    f=f;
                      
    //ideal I_Gr=(ideal)J->m[0].Data();

    // Compute initial indices and monomials for the vectors s and t
     i= p_GetComp(s,currRing);
     j= p_GetComp(t,currRing);
      //  std::cout << "get comp of s="<<i<< std::endl; 
      //    std::cout << "get comp of t="<<j<< std::endl;    
    poly s10= pHead(s);
    pSetComp(s10,0);
    pSetmComp(s10);
    pSetCoeff(s10,nInit(1));
    poly t10= pHead(t);
    pSetComp(t10,0);
    pSetmComp(t10);
    pSetCoeff(t10,nInit(1));
    ideal M=(ideal)J->m[k-1].Data();// M =J[2]
    f1 = pp_Mult_qq(s10, M->m[i-1],currRing);//should be leadmonomial
    g1 = pp_Mult_qq(t10, M->m[j-1],currRing);
    
    // Iterate from level k down to 2
    for (l = k; l >= 2; l--) {
        // Print current level for debugging
       
           i1 = p_GetComp(f1,currRing);
           i2 = p_GetComp(g1,currRing);
        //  std::cout << "get comp of f1="<<i1<< std::endl; 
        //  std::cout << "get comp of g1="<<i2<< std::endl; 
        // Compute indices for current level
        if (l == k) {
            // First iteration: use f1 and g1
           
            ideal N=(ideal)J->m[l-2].Data();
            // Update F1 and G1 based on f1 and g1
            poly f10= pHead(f1);
            pSetComp(f10,0);
            pSetmComp(f10);
            pSetCoeff(f10,nInit(1));
            poly g10= pHead(g1);
            pSetComp(g10,0);
            pSetmComp(g10);
            pSetCoeff(g10,nInit(1));
            F1 = pp_Mult_qq(pHead(f10),N->m[i1-1],currRing);//should leadmonomial
            G1 = pp_Mult_qq(pHead(g10), N->m[i2-1],currRing);//should leadmonomial
        } else {
            ideal K=(ideal)J->m[l-2].Data();
            // Subsequent iterations: use F1 and G1 from the previous iteration
           i1 = p_GetComp(F1,currRing);
           i2 = p_GetComp(G1,currRing);

                  poly F10= pHead(F1);
                  pSetComp(F10,0);
                  pSetmComp(F10);
                  pSetCoeff(F10,nInit(1));
                  poly G10= pHead(G1);
                  pSetComp(G10,0);
                  pSetmComp(G10); 
                  pSetCoeff(G10,nInit(1));
            F1 = pp_Mult_qq(pHead(F10), K->m[i1-1],currRing); //chnage from f1 to F1
            //should leadmonomial
            G1 = pp_Mult_qq(pHead(G10), K->m[i2-1],currRing); //change from g1 to G1
            
        //should monomial
           
        }
    }
    //std::cout << "Leadsyz s="<<pString(s)<<":reduced s to f1"<<pString(f1)<<" lead t="<<pString(t)<<" reduce  t to g1="<<pString(g1)<< std::endl;
   
   // bool d0= p_ComparePolys(f1,g1,currRing);
   F1=pHead(F1);
   G1=pHead(G1);
    //std::cout << "Leadsyz s="<<pString(s)<<":reduced final s to F1"<<pString(F1)<<" lead t="<<pString(t)<<":reduce final t to G1="<<pString(G1)<< std::endl;
    //bool d1=p_ComparePolys(F1,G1,currRing);
    //bool d=p_EqualPolys(F1,G1,currRing);
   int d= p_Cmp(F1,G1,currRing);
    // Perform final comparison using coM
   //std::cout << "Leadsyz s="<<pString(s)<<":reduced final s to F1"<<pString(F1)<<" lead t="<<pString(t)<<":reduce final t to G1="<<pString(G1)<< ":coM2"<<d<<std::endl;
        // General case: compare F1 and G1 after iteration
        
        if (d == 1) {
            isTrue = 1;
        } else {
            if ((d==0) && (i > j)) {  //it was i1>i2
                isTrue = 1;
            } else {
              isTrue=1;
            }
        
        }
      
    
  
    //std::cout << "Leadsyz s="<<pString(s)<<":coM2(s,t, f, j)="<<isTrue<<" 0f s= "<<pString(s)<<" and t="<<pString(t)<< std::endl;
  
    return isTrue;
}


poly phi(poly s, ideal f)
{
 
 poly g = NULL;
  poly lm_s = pHead(s);
  pSetComp(lm_s,0);
  pSetmComp(lm_s);
  int g1= p_GetComp(s,currRing);
     
  
  g=pp_Mult_qq(lm_s,f->m[g1-1],currRing);//g:=psi(s)
  return(g);
}

NO_NAME_MANGLING
//First Level LiftTree
lists  liftTree(ideal f, poly s) { //poly s is singular vector
    
    int r = IDELEMS(f);
    //std::cout << "lift_ideal: " << r << std::endl;
    //std::cout<<"lift_vector s=" << pString(s) <<std::endl;
    poly g = NULL;

   // int i = 0;
    int k = 0;
    int q = 0;
   
    
  
   g= phi(pCopy(s),idCopy(f));//g:=psi(s)
  
  
     //std::cout<<"g=psi(s)" << pString(g) <<": s="<<pString(s)<<std::endl;
    poly g_copy = pCopy(g);
    poly lOT=LOT(g_copy, f);//poly T0 = LOT(g, f); 
     //std::cout<<"LOT=" << pString(lOT) <<std::endl;
     //poly T0 = g-LOT(g, f); 
    poly T0=p_Sub(pCopy(g),pCopy(lOT),currRing);
    //std::cout << "g-LOT_in LIFT: " << pString(T0) << std::endl;
    lists T=pOLY_List(pCopy(T0));  //lists T = (lists)pOLY_List(T0)->Data(); 
    
    poly m1 = NULL;
    poly m = NULL;
  
    
    int C = 0;
    //int cl = 0;
    int t_size = lSize(T)+1;

  //  std::cout << "see T: for vector=" <<pString(s)<< std::endl;
  //  for(int k=0;k< t_size; k++){
  //   std::cout << "T[k]=: " <<pString((poly)T->m[k].Data())<< std::endl;
  //  }
    lists TT = (lists)omAlloc0Bin(slists_bin); // Allocate memory for empty list TT
  


      TT->Init(t_size); // Initialize TT with size t_size
    
    
  
    for ( k = 0; k < t_size; k++) {
        poly t = (poly)T->m[k].Data();// poly t=T[k]
       poly s_v = NULL;//vector s_v
       //std::cout << "t in LIFT:"<<pString(t)<< ": given leadsyz:s="<<pString(s)<<std::endl;
        for (q = 0; q < r; q++) {
        
            bool c = p_DivisibleBy(pHead(f->m[q]), t, currRing);


           //ideal Gb_t=idInit(1,1);
           //Gb_t->m[0]=t;
           //C = (kNF(Gb_t,NULL, pHead(f->m[q])) == NULL); 

            if (c == TRUE) {
                  m1=pp_Divide(t,pHead(f->m[q]), currRing);
                 
                   m = pCopy(m1);//m=m1;
                   p_SetComp(m,q+1,currRing);
                   p_SetmComp(m,currRing);
                   s_v=pCopy(m);
               //std::cout << "s_v in LIFT_COM:"<<pString(s_v)<<":corresponding t="<<pString(t)<< ": given leadsyz:s="<<pString(s)<<std::endl;
                C = coM(f, s, s_v);
                 //std::cout << "s_v in LIFT_COM:"<<pString(s_v)<<":corresponding t="<<pString(t)<< ": given leadsyz:s="<<pString(s)<<"com c="<<C<<std::endl;
                if(C == 1) {
                  
                   TT->m[k].rtyp = VECTOR_CMD;
                   TT->m[k].data=pCopy(s_v);  //TT[cl]=m*f[lambda]
                  
                    
                    break;
                }
            }
             
        } 
            
        
            
    }   
       
    
 

    return TT;
}



NO_NAME_MANGLING
//iterative liftTree

lists liftTree2(ideal f, poly s, lists J, int level) {

//ideal f is singular module
// poly s is singular vector

  //std::cout<<"lift2_vector s=" << pString(s) <<std::endl;

   //ideal f is singular module
    int r = IDELEMS(f);
    poly g=NULL;  //vector g
  
    //int i=0;

 
   
    g= phi(pCopy(s),idCopy(f));//g:=psi(s)

 // std::cout<<"g=psi(s)in LIFT2" << pString(pCopy(g)) <<std::endl;
  poly g_copy = pCopy(g);

// vector t0=LOT_2(g,f,J,level)
   poly lOT=LOT_2(g_copy,f,J,level);

    

   
   //std::cout<<"LOT=" << pString(lOT) <<std::endl;
     //poly T0 = g-LOT(g, f); 
    poly T0=p_Sub(pCopy(g),pCopy(lOT),currRing);
    //poly T0 = p_Sub(g_copy, pCopy(t0),currRing);//vector T0=g - LOT_2(g, f, J, level)

   //std::cout<<"g-LOT=" << pString(T0) <<std::endl;
      lists T = vector_List(T0); //list T =vector_List(T0)

    
      int t_size = lSize(T)+1;
  
  //     std::cout << "see T: for vector=" <<pString(s)<< std::endl;
  //  for(int k=0;k< t_size; k++){
  //   std::cout << "T[k]=: " <<pString((poly)T->m[k].Data())<< std::endl;
  //  }

     poly s_v=NULL; //vector s_v
     poly m=NULL;
     poly m1=NULL;
    int lambda=0;
   
    int k=0;
    lists TT = (lists)omAlloc0Bin(slists_bin);
  
   
       TT->Init(t_size); 
     
   
    //ideal I_Gr=J[1];
    //module f0= leadSyz(I_Gr);//changing to lead syz
      ideal f0=(ideal)J->m[level-1].Data();//
  
     

    for (k = 0; k < t_size; k++){
           poly t = (poly)T->m[k].Data(); //vector t=T[k]
           //std::cout << "t in LIFT2:"<<pString(t)<< std::endl;
            
          for (lambda = 0; lambda <= r; lambda++) {
           
            int a = p_GetComp(t,currRing);
            int b = p_GetComp(f0->m[lambda],currRing);
           //ideal Gb_t=idInit(1,1);
          // Gb_t->m[0]=t;
            
    

            if(a==b){
                   
              //int C = (kNF(Gb_t,NULL, pHead(f0->m[lambda])) == 0);
                bool d = p_DivisibleBy(pHead(f0->m[lambda]), t, currRing);
                 //std::cout << "bool d in LIFT2:"<<d<< std::endl;
               if (d==TRUE) {
                   //std::cout << "bool d in LIFT2:"<<d<< std::endl;
                   m1 = pp_Divide(pHead(t), pHead(f0->m[lambda]), currRing);
                   m = pCopy(m1);//m=m1;
                   p_SetComp(m,lambda+1,currRing);
                   p_SetmComp(m,currRing);
                   s_v=pCopy(m);
   
              
                      int c= coM2(f0, s, s_v, J,level);
                      if (c==1)
                       {
                     //std::cout <<"in LIFT2 s="<<pString(s)<<":correspond t="<<pString(t)<< ":found s_v=:"<<pString(s_v)<< std::endl;
                      TT->m[k].rtyp = VECTOR_CMD;
                      TT->m[k].data=pCopy(s_v);;  //TT[cl]=m*f[lambda]
                   // std::cout <<"in LIFT2 s="<<pString(s)<<":correspond t="<<pString(t)<< ":found s_v=:"<<pString(s_v)<< std::endl;
  
                    break;
                }
                
              
            } 
        }
        
      }    
                
                
    
           

          
            
            
   }   
 

    return TT;

}


NO_NAME_MANGLING
std::pair<int, lists> LIFT_GPI(leftv args) {
    // Extract the Token
    lists Token = (lists)(args->Data());
  

    // Extract tok
    lists tok = (lists)(args->next->Data());
    // Extract TT
    lists TT = (lists)(args->next->next->Data());

    // Token.data
    lists tmp = (lists)(Token->m[3].Data());
    // tok.data
    lists tmp1 = (lists)(tok->m[3].Data());
    // TT.data
    lists tmp2 = (lists)(TT->m[3].Data());

    // TT.data[6]
    int counter = (int)(long)tmp2->m[5].Data();
    int colmn = (int)(long)tmp2->m[3].Data();
   
    // J = tok.data[1]
    lists J = (lists)(tmp1->m[0].Data());
  

    // vector s = TT.data[1]
    poly s = (poly)tmp2->m[0].Data();
    
    
    ideal l = nullptr;
    ideal LL = nullptr;
   int r0 = 0, c = 0, l_size = 0;
    // Determine the ideal or vector type and get the corresponding data
    if (tmp->m[0].Typ() == IDEAL_CMD) {
        l = (ideal)J->m[1].Data();
        // for(int k=0;k<IDELEMS(l);k++){
        //     std::cout <<"ideal l in LIFT:"<<""<<pString((poly)l->m[k])<< std::endl;
        // }
         l_size = IDELEMS(l);
        r0 = IDELEMS((ideal)(tmp->m[0].Data()));
        c = l_size;
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        int level = counter;
        //Since J in C++; level should be counter -1??
         l = (ideal)J->m[level - 1].Data();
        l_size = IDELEMS(l);
        r0 = l_size;
        LL = (ideal)J->m[level].Data();
        c = IDELEMS(LL);
    } else {
        std::cerr << "Error: Unsupported command type." << std::endl;
        return {0, nullptr};
    }

    lists lL = nullptr;
    int r = 0;

    if (tmp->m[0].Typ() == IDEAL_CMD) {
        ideal f_copy = idCopy((ideal)(tmp->m[0].Data()));  // Create a deep copy of f
        poly s_copy = pCopy(s);  // Copy polynomial s to prevent modification
          // std::cout << "vector s:="<<pString(s_copy)<<"-"<<"at"<<"-"<<colmn<<std::endl;
        lL = liftTree(f_copy, s_copy); // Call liftTree with copies
        idDelete(&f_copy);  // Clean up copy
        pDelete(&s_copy);    // Clean up polynomial copy

        r = lSize(lL) + 1;
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        matrix A = (matrix)tmp->m[1].Data();  // Deep copy matrix A
        ideal M_copy = id_Matrix2Module(mp_Copy(A,currRing), currRing);
        poly s_copy = pCopy(s);  // Copy polynomial s
        lists J_copy = lCopy(J); // Deep copy of lists J

        lL = liftTree2(M_copy, s_copy, J_copy, counter);

        // Clean up copies
        idDelete(&M_copy);
        pDelete(&s_copy);
        //lDelete(&J_copy);

        r = lSize(lL) + 1;
    }

    // Prepare the LLT token
    lists LLT = (lists)omAlloc0Bin(slists_bin);
    LLT->Init(4); // Initialize with 4 fields
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);

    lists Ld = NULL;
    // Iterate to fill in data
    ideal sM = idInit(c, r0);
    for (int k = 0; k < r; k++) {

        id_Delete(&sM, currRing);  // Delete the existing sM
        sM = idInit(c, r0);  
        // Create a new token Ld
        // omFreeBin(Ld, slists_bin);  // Free the existing Ld
        Ld = (lists)omAlloc0Bin(slists_bin);  // Reinitialize Ld
        Ld->Init(4);  // Initialize with 4 fields


        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = strdup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = strdup("Sparse_matrix_Lift");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

      
  
      
        // matrix sM = mpNew(r0, c);
       poly s_lift = (poly)lL->m[k].Data(); // Retrieve the lifted polynomial
        int l_k = p_GetComp(s_lift, currRing);

        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);
        //  std::cout << "#colmn:=" <<colmn<< std::endl;
        poly C=sM->m[colmn-1];
        //  std::cout << "#poly C:=" <<pString(C)<< std::endl;
        poly Ci=p_Vec2Poly(C,l_k,currRing);
       
      C= p_Sub(C,Ci,currRing);
        //  std::cout << "after C-Ci:=" <<pString(C)<< std::endl;
         poly C1= pCopy(p_Mult_q(pISet(-1), pCopy(lm), currRing));
                   p_SetComp(C1,l_k,currRing);
                   p_SetmComp(C1,currRing);
                   
//        std::cout << "Before addition C: " << pString(C) << std::endl;
// std::cout << "Before addition C1: " << pString(C1) << std::endl;

C=p_Add_q(C, pCopy(C1), currRing);

// std::cout << "After addition C: " << pString(C) << std::endl;
   sM->m[colmn-1]=C;








        // Prepare Ld data
        t = (lists)omAlloc0Bin(slists_bin);
        t->Init(7);
        t->m[0].rtyp = VECTOR_CMD; 
        t->m[0].data = pCopy(p_Mult_q(pISet(-1), s_lift, currRing));
        t->m[1].rtyp = SMATRIX_CMD; 
        t->m[1].data = sM;
        t->m[2].rtyp = INT_CMD; 
        t->m[2].data = (void*)(long)l_k;
        t->m[3].rtyp = INT_CMD; 
        t->m[3].data = (void*)(long)colmn;

        // Assign data to Ld based on the command type
        if (tmp->m[0].Typ() == IDEAL_CMD) {
            t->m[4].rtyp = MODUL_CMD; 
            t->m[4].data = J->m[1].Data(); // J[2]
        } else if (tmp->m[0].Typ() == VECTOR_CMD) {
            int level = counter;
            t->m[4].rtyp = MODUL_CMD; 
            t->m[4].data = J->m[level].Data(); // Ld.data[5]=J[level+1];
        }

        t->m[5].rtyp = INT_CMD;  
        t->m[5].data = (void*)(long)counter;
        t->m[6].rtyp = INT_CMD; 
        t->m[6].data = (void*)(long)counter;

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
        // lists t0 = (lists)omAlloc0Bin(slists_bin);
        // t0->Init(r);
        // for (int s = 0; s < r; s++) {
        //     t0->m[s].rtyp = LIST_CMD;
        //     t0->m[s].data = lCopy(Ld);
        // }
        temp->m[k].rtyp = LIST_CMD;
        temp->m[k].data = lCopy(Ld);
        //  // Clean up temporary lists
        //  omFreeBin(t, slists_bin);
        //  omFreeBin(field_names, slists_bin);
        //  omFreeBin(t0, slists_bin);
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
        temp->m[k].data=NULL;  
    }

    final_data->m[r].rtyp = INT_CMD;
    final_data->m[r].data = (void*)(long)(r + 1); // Set total count

    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;

       // Clean up sM,Ld and temp
    id_Delete(&sM, currRing);
    omFreeBin(Ld, slists_bin);
    temp->Clean(currRing);

    return {r, LLT};  // Return success state and LLT
}



NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int, long> singular_template_LIFT(const std::string& input_filename,
                                                                       const std::string& all_lead,
                                                                       const std::string& lead,
                                                                       const std::string& needed_library,
                                                                       const std::string& base_filename, unsigned long N)
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
   
//   std::string function_name = "LIFT_GPI";
  //    //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
    // out = call_user_proc(function_name, needed_library, args);
   
    // Direct call to LIFT_GPI 
   out = LIFT_GPI(args.leftV());  
  

    // Extract the result list from the output
    lists u = (lists)out.second->m[3].Data();
    std::filesystem::path oldPath(lead);

if (std::filesystem::exists(oldPath)) {
    std::filesystem::path newPath = oldPath.parent_path() / (std::to_string(N) + ".ssi");

    if (!std::filesystem::exists(newPath)) {
        try {
            std::filesystem::rename(oldPath, newPath);
            // std::cout << "Renamed " << lead << " to " << newPath.string() << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error renaming file: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "File with new name already exists: " << newPath.string() << std::endl;
    }
} else {
    std::cerr << "File does not exist: " << lead << std::endl;
}

    
    std::vector<std::string> vec;
    int total_generator;

    // Iterate through each element of the outer list and serialize it
    for(int i = 0; i < lSize(u); i++) {
    
        auto Outname = serialize((lists)u->m[i].Data(), base_filename);
        // std::cout << " Outname:LIFT2 " << Outname<< std::endl;
        vec.push_back(Outname);
    }

    total_generator = lSize(u);  // Assuming u contains the computed generators
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
    auto total_runtime=computation_time;
// std::cout << " total gen " << total_generator<< std::endl;
// std::cout << " total time_LIFT:= " << total_runtime<< std::endl;
  //std::cout << arg1.leftV()->String() <<std::endl;

    return {vec, total_generator, total_runtime};
}






lists oneSublift(ideal f, poly s) 
{ //poly s is singular vector
    
    int r = IDELEMS(f);
    // std::cout << "oneSublift_ideal: " << r << std::endl;
    // std::cout<<"oneSublift_vector s=" << pString(s) <<std::endl;
    poly h = NULL;
    poly g = NULL;

   // int i = 0;
    int k = 0;
    int q = 0;
    
    h= phi(pCopy(s),idCopy(f));//h:=psi(s)
    //std::cout << "psi(s): " << h << std::endl;
     //std::cout<<"After _f[g1]=" << pString(f->m[g1-1]) <<std::endl;

    //g = h - pHead(h); // Leading term of h
    //std::cout<<"h=" << pString(h) <<std::endl;
    g=p_Sub(h,pHead(h),currRing);
    //std::cout<<"g=" << pString(g) <<std::endl;
    poly g_copy = pCopy(g);
    poly lOT = LOT(g_copy, f);

    //std::cout<<"g=g-LT(psi(s))" << pString(g) <<std::endl;
    //std::cout << "lOT_onesubLift: " << pString(lOT) << std::endl;
    //poly T0 = g-LOT(g, f); 
    poly T0=p_Sub(pCopy(g),pCopy(lOT),currRing);

    //std::cout << "g-LOT: " << pString(T0) << std::endl;

    lists T=pOLY_List(pCopy(T0)); 
   
    // std::cout << "Input_pOLY_LIST: "<< std::endl;
    // for(int k=0; k<lSize(T)+1; k++){
    //     poly lm=(poly)T->m[k].Data();
    //     std::cout << "polY_List:size" << lSize(T)+1
    //     <<"pOLY_LIST;"<<pString(lm) << std::endl;

    // }

    //std::cout << "output_pOLY_List: " << lSize(T) << std::endl;
    //poly s_hat = pCopy(s); // vector s_hat=s
    
    poly m1 = NULL;
    poly m = NULL;
    poly s_v = NULL;//vector s_v
    //int C = 0;
    //int cl = 0;
    int t_size =lSize(T) + 1;
    // std::cout << "size of T: " << t_size << std::endl;
    //  std::cout << "Manual inspection of list_Sublift:" << std::endl;
    // for (int k = 0; k < t_size; k++) {
    //    std::cout << "Element " << k << ": rtyp=" << T->m[k].rtyp 
    //               << ", data=" << pString((poly)T->m[k].data) << std::endl;
    // }

    lists TT = (lists)omAlloc0Bin(slists_bin);
  
    if (t_size==0) 
    {
     TT->Init(0);
      //std::cout << "TT is empty, stopping algorithm." << std::endl;
      return TT;
   
    
    } else
    {
       TT->Init(t_size); 
     
    }

  for (k = 0; k < t_size; k++) 
  {
      poly t = (poly)T->m[k].Data();// poly t=T[k]
         //std::cout << "t in SubLIFT:"<<pString(t)<< std::endl;
      for (q = 0; q < r; q++)
      {
           
        bool c = p_DivisibleBy(pHead(f->m[q]), t, currRing);

          //std::cout << "c in SubLIFT:"<<c<< std::endl;
        // ideal Gb_t=idInit(1,1);
        // Gb_t->m[0]=t;
        // C = (kNF(Gb_t,NULL, pHead(f->m[q])) == NULL); 


          if (c == TRUE) 
          {
            // Perform polynomial division
            //m1 = p_PolyDiv(t, lead_terms[q], currRing); 
            m1=pp_Divide(t,pHead(f->m[q]), currRing);
                 
            m = pCopy(m1); //m=m1;
            p_SetComp(m,q+1,currRing);
            p_SetmComp(m,currRing);
            s_v=pCopy(m);
            //std::cout << "vector s_v in SubLIFT:"<<pString(s_v)<< std::endl;
            TT->m[k].rtyp = VECTOR_CMD;
            TT->m[k].data = pCopy(s_v);
            //cl++;
              break;
          }
      }
  }
  


 return TT;
}





lists subLiftTree2(ideal f, poly s, lists J, int level) {
    // Initialize variables
    int r = IDELEMS(f);      
    poly g = NULL;                    
    poly h = NULL;                    
    poly H = NULL;                    
    //std::cout << "vector in SubLIFT2:"<<pString(s)<< std::endl;
    // Get the ideal f0 from the list J at the current level
    ideal f0 = (ideal)J->m[level-1].Data();
    
  
  

   
    h= phi(pCopy(s),idCopy(f));//h:=psi(s)

    H= phi(pCopy(s),idCopy(f0));//H:=psi(s)
    // Subtract the head of H from h
    g = p_Sub(h, pHead(H), currRing); // g = h - lead(H)
    poly g_copy = pCopy(g);
    // Compute the LOT (List of Terms) of g with respect to f and J at the given level
    poly t0 = LOT_2(g_copy, f, J, level);

    // Subtract the LOT from g to get T0
    poly T0 = p_Sub(g, pCopy(t0), currRing);            // T0 = g - LOT_2(g, f, J, level)

    // Convert T0 into a list of vectors
    lists T = vector_List(T0);


    //  std::cout << "Input_Vector_LIST: "<< std::endl;
    // for(int k=0; k<lSize(T)+1; k++){
    //     poly lm=(poly)T->m[k].Data();
    //     std::cout << "vector_List:size" << lSize(T)+1
    //     <<"pOLY_LIST;"<<pString(lm) << std::endl;

    // }

    // Initialize the result list TT with the size of T
    int t_size = lSize(T)+1;                       // Get the size of T
    lists TT = (lists)omAlloc0Bin(slists_bin);   // Allocate memory for TT
    if (t_size==0) 
    {
     TT->Init(0);
     // std::cout << "TT is empty, stopping algorithm." << std::endl;
      return TT;
   
    
    } else
    {
       TT->Init(t_size); 
     
    }                          // Initialize TT with size t_size

    // Initialize temporary variables
    poly s_v = NULL;   // Temporary vector for storing the result
    poly m = NULL;
    poly m1 = NULL;
    //int counter = 0;   // Counter to track the number of terms in TT

    // Main loop for sub-lifting each term in T
    for (int k = 0; k < lSize(T)+1; k++) {
        poly t = (poly)T->m[k].Data();           // Get the k-th term in T

        for (int lambda = 0; lambda < r; lambda++) {
            // Get the components of the current term t and f0[lambda]
            int a = p_GetComp(t, currRing);
            int b = p_GetComp(f0->m[lambda], currRing);
            
            // // Initialize Gb_t for normal form check
            // ideal Gb_t = idInit(1, 1);
            // Gb_t->m[0] = t;

            // Check if the components match and if the normal form is 0
            if (a == b) {
                //int C = (kNF(Gb_t, NULL, pHead(f0->m[lambda])) == 0);
                 bool c = p_DivisibleBy(pHead(f0->m[lambda]), t, currRing);
                if (c==TRUE) {
                    // Compute m1 as the quotient of leading terms
                    m1 = pp_Divide(pHead(t), pHead(f0->m[lambda]), currRing);

                    // Copy m1 to m and set its component
                    m = pCopy(m1);
                    p_SetComp(m, lambda+1, currRing);
                    p_SetmComp(m, currRing);

                    // Set s_v as m and store in TT
                    s_v =pCopy(m);
                    TT->m[k].rtyp = VECTOR_CMD;
                    TT->m[k].data = pCopy(s_v);
                    //std::cout << "vector s_v in SubLIFT:2"<<pString(s_v)<< std::endl;
                   // counter++;  // Increment the counter
                    break;
                }
            }
        }
    }

    // Return the result list TT
    return TT;
}

NO_NAME_MANGLING
std::pair<int, lists> SubLIFT_GPI(leftv args) {
    // Extract the Token
    lists Token = (lists)(args->Data());
    // if (Token == nullptr) {
    //     std::cerr << "Error: Token is null." << std::endl;
    //     return {0, nullptr};
    // }

    // Extract tok
    lists tok = (lists)(args->next->Data());
    // Extract TT
    lists TT = (lists)(args->next->next->Data());

    // Token.data
    lists tmp = (lists)(Token->m[3].Data());
    // tok.data
    lists tmp1 = (lists)(tok->m[3].Data());
    // TT.data
    lists tmp2 = (lists)(TT->m[3].Data());

    // TT.data[6]
    int counter = (int)(long)tmp2->m[5].Data();
    int colmn = (int)(long)tmp2->m[3].Data();
    
    // J = tok.data[1]
    lists J = (lists)(tmp1->m[0].Data());
  

    // vector s = TT.data[1]
    poly s = (poly)tmp2->m[0].Data();
    
    ideal l = nullptr;
    ideal LL = nullptr;
   int r0 = 0, c = 0, l_size = 0;
    // Determine the ideal or vector type and get the corresponding data
    if (tmp->m[0].Typ() == IDEAL_CMD) {
        l = (ideal)J->m[1].Data();
         l_size = IDELEMS(l);
        r0 = IDELEMS((ideal)(tmp->m[0].Data()));
        c = l_size;
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        int level = counter;
         l = (ideal)J->m[level - 1].Data();
        l_size = IDELEMS(l);
        r0 = l_size;
        LL = (ideal)J->m[level].Data();
        c = IDELEMS(LL);
    } else {
        std::cerr << "Error: Unsupported command type." << std::endl;
        return {0, nullptr};
    }

    lists lL = nullptr;
    int r = 0;

    if (tmp->m[0].Typ() == IDEAL_CMD) {
        ideal f_copy = idCopy((ideal)(tmp->m[0].Data()));  // Create a deep copy of f
        poly s_copy = pCopy(s);  // Copy polynomial s to prevent modification

        lL =oneSublift(f_copy, s_copy); // Call liftTree with copies
        idDelete(&f_copy);  // Clean up copy
        pDelete(&s_copy);    // Clean up polynomial copy

        r = lSize(lL) + 1;
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        matrix A = (matrix)tmp->m[1].Data();  // Deep copy matrix A
        ideal M_copy = id_Matrix2Module(mp_Copy(A,currRing), currRing);
        poly s_copy = pCopy(s);  // Copy polynomial s
        lists J_copy = lCopy(J); // Deep copy of lists J

        lL = subLiftTree2(M_copy, s_copy, J_copy, counter);

        // Clean up copies
        idDelete(&M_copy);
        pDelete(&s_copy);
        //lDelete(&J_copy);

        r = lSize(lL) + 1;
    }

    // Prepare the LLT token
    lists LLT = (lists)omAlloc0Bin(slists_bin);
    LLT->Init(4); // Initialize with 4 fields
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);

      // Allocate sM and Ld outside the loop
      ideal sM = idInit(c, r0);  // Initialize the submodule
      lists Ld = (lists)omAlloc0Bin(slists_bin);  // Initialize Ld
      Ld->Init(4);  // Initialize with 4 fields
      int k=0;
    for (k = 0; k < r; k++) {

        id_Delete(&sM, currRing);  // Delete the existing sM
        sM = idInit(c, r0);  
    
      
        omFreeBin(Ld, slists_bin);  // Free the existing Ld
        Ld = (lists)omAlloc0Bin(slists_bin);  // Reinitialize Ld
        Ld->Init(4);  // Initialize with 4 fields

        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = strdup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = strdup("Sparse_matrix_SubLIFT");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

      

        ideal sM = idInit(c, r0);
        // matrix sM = mpNew(r0, c);
       poly s_lift = (poly)lL->m[k].Data(); // Retrieve the lifted polynomial
        int l_k = p_GetComp(s_lift, currRing);

        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);
        //  std::cout << "#colmn:=" <<colmn<< std::endl;
        poly C=sM->m[colmn-1];
        //  std::cout << "#poly C:=" <<pString(C)<< std::endl;
        poly Ci=p_Vec2Poly(C,l_k,currRing);
       
      C= p_Sub(C,Ci,currRing);
        //  std::cout << "after C-Ci:=" <<pString(C)<< std::endl;
         poly C1= pCopy(p_Mult_q(pISet(-1), pCopy(lm), currRing));
                   p_SetComp(C1,l_k,currRing);
                   p_SetmComp(C1,currRing);
                   
//        std::cout << "Before addition C: " << pString(C) << std::endl;
// std::cout << "Before addition C1: " << pString(C1) << std::endl;

C=p_Add_q(C, pCopy(C1), currRing);

// std::cout << "After addition C: " << pString(C) << std::endl;
   sM->m[colmn-1]=C;






        // Prepare Ld data
        t = (lists)omAlloc0Bin(slists_bin);
        t->Init(7);
        t->m[0].rtyp = VECTOR_CMD; 
        t->m[0].data = pCopy(p_Mult_q(pISet(-1), s_lift, currRing));
        t->m[1].rtyp = SMATRIX_CMD; 
        t->m[1].data = sM;
        t->m[2].rtyp = INT_CMD; 
        t->m[2].data = (void*)(long)l_k;
        t->m[3].rtyp = INT_CMD; 
        t->m[3].data = (void*)(long)colmn;

        // Assign data to Ld based on the command type
        if (tmp->m[0].Typ() == IDEAL_CMD) {
            t->m[4].rtyp = MODUL_CMD; 
            t->m[4].data = J->m[1].Data(); // J[2]
        } else if (tmp->m[0].Typ() == VECTOR_CMD) {
            int level = counter;
            t->m[4].rtyp = MODUL_CMD; 
            t->m[4].data = J->m[level].Data(); // Ld.data[5]=J[level+1];
        }

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
          // Clean up temporary lists
          omFreeBin(t, slists_bin);
          omFreeBin(field_names, slists_bin);
          omFreeBin(t0, slists_bin);
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

      // Clean up sM and Ld
      id_Delete(&sM, currRing);
      omFreeBin(Ld, slists_bin);

    return {r, LLT};  // Return success state and LLT
}







NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int, long> singular_template_SUBLIFT(const std::string& input_filename,
                                                                 const std::string& all_lead,
                                                                const std::string& lead,
                                                               
                                                  const std::string& needed_library,
                                                   const std::string& base_filename,unsigned long N)
{ // Initialize Singular and load library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    // Debugging: Print worker ID
    std::string ids = worker();
    // std::cout << "Path"<<lead << std::endl;

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
     
    // std::string function_name = "SubLIFT_GPI";
     //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
    // out = call_user_proc(function_name, needed_library, args);
     out = SubLIFT_GPI(args.leftV());  // Call  SubLIFT_GPI with the raw pointer
  //std::cout << "SubLIFT_Runtime: " << computation_time << " milliseconds" << std::endl;
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
  //   lists Token = (lists)(args.leftV()->data);
   
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

  //   lists Token1 = (lists)(arg.leftV()->data);
   
  // int L_si = lSize(Token1)+1;
  // std::cout << "Size of J in the transition: " << L_si << std::endl;
  // for (int i = 0; i < L_si; i++) {
  //         sleftv& listElement = Token1->m[i];  // Access each element as `leftv`
  //     std::cout << "LIFT: " << listElement.String()  << std::endl;
  // }



  //   lists Token2 = (lists)(arg1.leftV()->data);
   
  // int L = lSize(Token2)+1;
  // std::cout << "Size of J in the transition: " << L << std::endl;
  // for (int i = 0; i < L; i++) {
  //         sleftv& listElement = Token2->m[i];  // Access each element as `leftv`
  //     std::cout << "All_lead: " << listElement.String()  << std::endl;
  // }
//   std::cout << "Renamed " << lead <<"" << std::endl;
  std::filesystem::path oldPath(lead);

  if (std::filesystem::exists(oldPath)) {
      std::filesystem::path newPath = oldPath.parent_path() / (std::to_string(N) + ".ssi");
  
      if (!std::filesystem::exists(newPath)) {
          try {
              std::filesystem::rename(oldPath, newPath);
            //   std::cout << "Renamed " << lead << " to " << newPath.string() << std::endl;
          } catch (const std::filesystem::filesystem_error& e) {
              std::cerr << "Error renaming file: " << e.what() << std::endl;
          }
      } else {
          std::cerr << "File with new name already exists: " << newPath.string() << std::endl;
      }
  } else {
      std::cerr << "File does not exist: " << lead << std::endl;
  }
  

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
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
    
   //auto start_serialize = std::chrono::high_resolution_clock::now();

  

    
  // End timing for serialization
   //auto end_serialize = std::chrono::high_resolution_clock::now();
   //auto  serialization_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count(); // Convert to milliseconds
   //auto total_runtime=computation_time_deserialize+computation_time+serialization_time;
  
   auto total_runtime=computation_time;
//    std::cout << " total time_SubLIFT:= " << total_runtime<< std::endl;
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}







NO_NAME_MANGLING
std::pair<std::string, long> singular_template_reduce(const std::string& Red, 
    unsigned long N,
    const std::string& needed_library,
    const std::string& base_filename) 
{
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);

    // Extract the folder path from the Red file's location
    std::filesystem::path basePath = std::filesystem::path(Red).parent_path();
    // std::cout << base_filename<< "base_filename" << std::endl;
    // std::cout <<Red<< "" << std::endl;
    // std::cout <<N<< "=:Reduce" << std::endl;
    std::string ids = worker();
    std::pair<int, lists> Gb = deserialize(Red, ids);

    auto start_computation = std::chrono::high_resolution_clock::now();

    lists Gb_data = (lists)(Gb.second);
    lists tmpL1 = (lists)(Gb_data->m[3].Data());
   
    ideal C = nullptr;
    int counter = 0;
    int r=0;int c=0;
    poly vec = nullptr;
    leftv L = nullptr; leftv LL = nullptr;
    for (unsigned long  i = 1; i <= N; ++i) { // Iterate from 1 to N to match "1.ssi", "2.ssi", etc.
        // Construct the full path for i.ssi files within the same folder as Red
        std::string filename = (basePath / (std::to_string(i) + ".ssi")).string();
            // std::cout << filename<< "" << std::endl;
          
            if (!std::filesystem::exists(filename)) {
                // std::cerr << "File not found: " << filename << ". Skipping this iteration." << std::endl;
                continue;  // Skip to next iteration
            }
            
        // Deserialize using the full path
        std::pair<int, lists> input_part = deserialize(filename, ids);

        lists token = (lists)(input_part.second);
        lists tmpL = (lists)(token->m[3].Data());
     
        vec = (poly)tmpL->m[0].Data();
        counter = (int)(long)tmpL->m[5].Data();
        if (tmpL1->m[0].Typ() == IDEAL_CMD) {
            // Handle case where `f` is an ideal
            r = IDELEMS((ideal)(tmpL1->m[0].Data()));
          
            L=&tmpL->m[4];
          
           c = IDELEMS((ideal)L->Data());
         
        } else if (tmpL1->m[0].Typ() == VECTOR_CMD) {
            // Handle case where `f` is a vector
            ideal t=(ideal)(tmpL1->m[4].Data());
            r = IDELEMS(t);
            L=&tmpL->m[4];
          
           c = IDELEMS((ideal)L->Data());
        }
        ideal A = idCopy((ideal)tmpL->m[1].Data());
        if (C == nullptr) {
            C = idCopy(A);
        } else {
            ideal temp = idInit(IDELEMS(C), r);
            temp = sm_Add(C, A, currRing);
            idDelete(&C);
            C = idCopy(temp);
            idDelete(&temp);
        }
        idDelete(&A);

        try {
            std::filesystem::remove(filename);
            // std::cout << "Deleted file: " << filename << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Failed to delete " << filename << ": " << e.what() << std::endl;
        }
    }

    lists output = (lists)omAlloc0Bin(slists_bin);
    output->Init(4);

    lists t = (lists)omAlloc0Bin(slists_bin);
    t->Init(2);
    t->m[0].rtyp = STRING_CMD; t->m[0].data = strdup("generators");
    t->m[1].rtyp = STRING_CMD; t->m[1].data = strdup("FirstSyz_smatrix");

    output->m[1].rtyp = LIST_CMD; output->m[1].data = t;
    output->m[0].rtyp = RING_CMD; output->m[0].data = currRing;
    output->m[2].rtyp = RING_CMD; output->m[2].data = currRing;

    t = (lists)omAlloc0Bin(slists_bin);
    t->Init(7);
    t->m[0].rtyp = VECTOR_CMD; t->m[0].data = vec;
    t->m[1].rtyp = SMATRIX_CMD; t->m[1].data = C;
    t->m[2].rtyp = INT_CMD; t->m[2].data = (void*)(long)r;
    t->m[3].rtyp = INT_CMD; t->m[3].data = (void*)(long)c;

    if (tmpL1->m[0].Typ() == IDEAL_CMD) {
        t->m[4].rtyp=MODUL_CMD; t->m[4].data= L->CopyD();//Tok.data[5]

    } 
    else if (tmpL1->m[0].Typ() == VECTOR_CMD) {
        t->m[4].rtyp=MODUL_CMD; t->m[4].data= LL->CopyD();//Tok.data[5]
    }
    t->m[5].rtyp = INT_CMD; t->m[5].data = (void*)(long)counter;
    t->m[6].rtyp = INT_CMD; t->m[6].data = (void*)(long)(counter + 1);

    output->m[3].rtyp = LIST_CMD; output->m[3].data = t;

    int cmd_nr;
    blackboxIsCmd("token", cmd_nr);

    std::string out_filename = serialize(output, base_filename);

    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();

    return {out_filename, computation_time};
}












