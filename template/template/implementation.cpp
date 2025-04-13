
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
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>






NO_NAME_MANGLING
std::string singular_template_compute_StdBasis(std::string const& res
    , std::string const& needed_library
    , std::string const& base_filename)
{
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    std::pair<int, lists> Res;
    std::pair<int, lists> out;
    std::string ids;
    std::string out_filename;
    ids = worker();
    //std::cout << ids << " in singular_..._compute" << std::endl;
    Res = deserialize(res, ids);

    ScopedLeftv args(Res.first, lCopy(Res.second));
    std::string function_name = "stdBasis";
    out = call_user_proc(function_name, needed_library, args);
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
  

    matrix l = mpNew(r, r);
  

    poly s10 = NULL;
    poly t10 = NULL;

    for (a = 1; a < r+1; a++) {
        for (b = 1; b < r+1; b++) {
          poly tmp1=p_Head(G->m[a-1],currRing);
          poly tmp2=p_Head(G->m[b-1],currRing);
            i = p_GetComp(tmp1, currRing);
            j = p_GetComp(tmp2, currRing);

            s10 = tmp1;
            pSetComp(s10, 0);
            pSetmComp(s10);
            
            t10 = tmp2;
            pSetComp(t10, 0);
            pSetmComp(t10);
          
            poly lcm_poly = p_Lcm(s10, t10, currRing);
            pSetCoeff0(lcm_poly, nInit(1));
            // std::cout << "polyLCM:" << pString(lcm_poly)<< std::endl;
            if (i == j) {
              // std::cout << "poly t10:" << pString(t10)<< "j:="<<j<<std::endl;
              // std::cout << "poly s10:" << pString(s10)<< "i:="<<i<<std::endl;
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





NO_NAME_MANGLING
//Answer match with Singular
ideal leadSyz_ext(ideal f0) {
  int r = IDELEMS(f0);
  poly s = NULL, t = NULL;
  int cc = 0;
  ideal L = idInit(0, 1);
  ideal f_copy = idCopy(f0);
  matrix M = mpNew(r, r);
 M= lcm_mod(f_copy);

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
                // std::cout << "t survives:=" << pString(t)<< std::endl;
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
    J->Init(1);
    ideal F_copy = idCopy(f);
    int n = rVar(currRing);
   

   

    J->m[0].rtyp = MODUL_CMD;
    J->m[0].data = F_copy;

    int cc = 1;

    for (int t = 0; t < n; t++) {
        ideal temp = leadSyz_ext(F_copy);

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
        //  return size based on hom_degree
   

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
    lists Ld = NULL; //(lists)omAlloc0Bin(slists_bin);  // Initialize Ld
    for(int k = 0; k < r; k++) {

        
         Ld = (lists)omAlloc0Bin(slists_bin);
       
        Ld->Init(4); // type token
        
        lists t = (lists)omAlloc0Bin(slists_bin);
      
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = omStrDup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = omStrDup("SchFrame");

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
            field_names->m[s].data = omStrDup("generator");
           
        }

        LLT->m[0].rtyp = RING_CMD; LLT->m[0].data = currRing;

        // Assign fieldnames to LLT
        LLT->m[1].rtyp = LIST_CMD;
        LLT->m[1].data = field_names;

        LLT->m[2].rtyp = RING_CMD; LLT->m[2].data = currRing;

        // lists t0 = (lists)omAlloc0Bin(slists_bin);
      
        // t0->Init(r);
        // for (int s = 0; s < r; s++) {
        //     t0->m[s].rtyp = LIST_CMD;
        //     t0->m[s].data = lCopy(Ld);
        // }
//         omUpdateInfo();
      
//   std::cout << "used mem_SchFrame: " << om_Info.UsedBytes << std::endl;
        
        // LLT->m[3].rtyp = LIST_CMD;
        // LLT->m[3].data = t0;

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
   t->m[r].data = omStrDup("total_number_generator");
 
    LLT->m[1].rtyp = LIST_CMD;  
    LLT->m[1].data =t;

    // Final data preparation
    lists final_data = (lists)omAlloc0Bin(slists_bin);
  
    final_data->Init(r + 1);
    for (int k = 0; k < r; k++) {
        final_data->m[k].rtyp = LIST_CMD;
        final_data->m[k].data = temp->m[k].data;  // Transfer data from temp
        temp->m[k].data = NULL; temp->m[k].rtyp=DEF_CMD;
    }

    final_data->m[r].rtyp = INT_CMD;
    final_data->m[r].data = (void*)(long)(r + 1); // Set total count

    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;
       // Clean up sM,Ld and temp
     
       omFreeBin(Ld, slists_bin);
       temp->Clean(currRing);

    return {r, LLT};  // Return success state and LLT
}




NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int,  long>  singular_template_ALL_LEAD( std::string const& input_filename 
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
	// std::cout << degree << " homological degree" << std::endl;
	input = deserialize(input_filename,ids);
   
	ScopedLeftv args( input.first, lCopy(input.second));
   
//   std::string function_name = "all_leadsyz_GpI_Ext";


  auto start_computation = std::chrono::high_resolution_clock::now();
	// out = call_user_proc(function_name, needed_library, args);

    out =  ALL_LEAD_GPI(args.leftV());


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

  return {vec, total_generator, total_runtime};
}


// poly lcm_mod_EXT(poly v, poly w) {
//     int i = 0, j = 0;
//     poly answer = NULL;
//     poly tmp1 = p_Head(v, currRing);
//     poly tmp2 = p_Head(w, currRing);

//     // std::cout << "tmp1:=" << pString(tmp1) << std::endl;
//     // std::cout << "tmp2:=" << pString(tmp2) << std::endl;

//     if (tmp1 == NULL || tmp2 == NULL) {
//         if (tmp1 != NULL) p_Delete(&tmp1, currRing);
//         if (tmp2 != NULL) p_Delete(&tmp2, currRing);
//         return NULL;
//     }

//     i = p_GetComp(tmp1, currRing);
//     j = p_GetComp(tmp2, currRing);

//     if (i == j) {
//         poly answer1 = p_Lcm(tmp1, tmp2, currRing);
//         if (answer1 != NULL) {
//             // std::cout << "answer1:=" << pString(answer1) << std::endl;
//             pSetCoeff0(answer1, nInit(1));
//             // std::cout << "answer1:=" << pString(answer1) << std::endl;

//             poly tmp = p_CopyPowerProduct0(answer1, NULL, currRing);
//             if (tmp != NULL) {
//                 p_ExpVectorSub(tmp, tmp1, currRing);
//                 p_SetCoeff0(tmp, nDiv(pGetCoeff(tmp1), pGetCoeff(answer1)), currRing);
//                 answer = tmp;
//                 // std::cout << "LCM:=" << pString(answer) << std::endl;
//             }

//             p_Delete(&answer1, currRing);
//         }
//     }

//     p_Delete(&tmp1, currRing);
//     p_Delete(&tmp2, currRing);

//     return answer;
// }

// ideal lead_syz_ext(ideal f0) {
   
//     poly s = NULL, t = NULL;
//     int cc = 0;
//     ideal L = idInit(0, 1);  // Initialize the ideal L
//     ideal f_copy = idCopy(f0);  // Copy the input ideal f0
//     int r = IDELEMS(f_copy);
//     poly pi = NULL, pj = NULL;
//     poly Li = NULL, Lj = NULL, t0 = NULL;
//     // std::cout << "r:=" <<r<< std::endl;
//     std::cout << "Starting lead_syz_ext" << std::endl;

//     // Loop over i from 2 to r + 1
//     for (int i = 2; i < r + 1; i++) {
//         pi = (poly)f_copy->m[i-1];
//         if (pi == NULL) {
//             // std::cout << "Skipping pi at index " << i << " (NULL)" << std::endl;
//             continue;
//         }
//         // std::cout << "Processing pi at index " << i << ": " << pString(pi) << std::endl;

//         // Loop over j from 1 to i - 1
//         for (int j = 1; j < i; j++) {
//             pj = (poly)f_copy->m[j-1];
//             if (pj == NULL) {
//                 // std::cout << "Skipping pj at index " << j << " (NULL)" << std::endl;
//                 continue;
//             }
//             // std::cout << "Processing pj at index " << j << ": " << pString(pj) << std::endl;

//             // Create Li and Lj
//             Li = p_Head(pi, currRing);
//             Lj = p_Head(pj, currRing);
//             // std::cout << "Li: " << pString(Li) << ", Lj: " << pString(Lj) << std::endl;

//             // Check if Li and Lj are valid
//             if (Li == NULL || Lj == NULL) {
//                 // std::cout << "Li or Lj is NULL, skipping this pair." << std::endl;
//                 if (Li != NULL) p_Delete(&Li, currRing);
//                 if (Lj != NULL) p_Delete(&Lj, currRing);
//                 continue;  // Skip this iteration if Li or Lj is NULL
//             }

//             int a = p_GetComp(Li, currRing);
//             int b = p_GetComp(Lj, currRing);
//             // std::cout << "Comparing components: a = " << a << ", b = " << b << std::endl;
            

//             if (a == b) {
//                 // std::cout << "a = " << a << "= " << b <<"b"<< std::endl;
//                 poly lcm_poly = lcm_mod_EXT(Li, Lj);

//                 if (lcm_poly != NULL) {
//                     // std::cout << "lcm_poly: " << pString(lcm_poly) << std::endl;

//                     t0 = lcm_poly;
//                     p_SetComp(t0, i, currRing);
//                     p_SetmComp(t0, currRing);
//                     t = pCopy(t0);
//                     // std::cout << "t: " << pString(t) << std::endl;

//                     // Loop through elements of L and handle divisibility
//                     for (int k = 0; k < IDELEMS(L); k++) {
//                         s = (poly)L->m[k];
//                         if (s != NULL && t != NULL) {
//                             if (p_DivisibleBy(pHead(s), t, currRing)) {
//                                 t = NULL;  // Break if divisibility conditions are met
//                                 break;
//                             } else if (p_DivisibleBy(t, pHead(s), currRing)) {
//                                 ideal tmp = id_Delete_Pos(L, k, currRing);
//                                 idDelete(&L);
//                                 L = tmp;
//                                 k--;
//                                 cc--;
//                             }
//                         }
//                     }

//                     // Add t to L if it survives the checks
//                     if (t != NULL) {
//                         if (cc >= IDELEMS(L)) {
//                             ideal tmpL = idInit(cc + 1, 1);
//                             for (int i = 0; i < cc; i++) {
//                                 tmpL->m[i] = pCopy(L->m[i]);
//                                 L->m[i] = NULL;
//                             }
//                             idDelete(&L);
//                             L = tmpL;
//                         }
//                         L->m[cc] = t;
//                         // std::cout << "if t survives: " << pString(t) << std::endl;
//                         cc++;
//                     }

//                     p_Delete(&lcm_poly, currRing);  // Safely delete LCM poly
//                 }
//             }

//             // Delete Li and Lj at the end of each inner loop iteration
//             if (Li != NULL) p_Delete(&Li, currRing);
//             if (Lj != NULL) p_Delete(&Lj, currRing);
//         }
//     }

//     // idDelete(&f_copy); // Free f_copy to avoid memory leaks

//     std::cout << "Exiting lead_syz_ext" << std::endl;
//     return L;  // Return the ideal L
// }



NO_NAME_MANGLING
std::pair<int, lists> LEAD_GPI(leftv args) {
    // Extract Token
    lists Token = (lists)(args->Data());

    // Extract the first element (f)
    lists tmp = (lists)(Token->m[3].Data());
   
    ideal LT=nullptr;
    // Call leadSyz_ext
    // auto start_computation = std::chrono::high_resolution_clock::now();
   
    // auto end_computation = std::chrono::high_resolution_clock::now();
    // auto computation_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
    // std::cout << "lead_syz_ext: " << computation_time << " nanoseconds" << std::endl;
    int level=0;
    int r0 =0;
    int r=0;
    int c=0;
    if(tmp->m[0].Typ()==MODUL_CMD){
        
        ideal f_copy = idCopy((ideal)(tmp->m[0].Data()));  // Create a deep copy of f
        LT = leadSyz_ext(f_copy);
        r = IDELEMS(LT); 
        c=r;
        r0 = IDELEMS(f_copy); 
        level=1;
        idDelete(&f_copy);
    } else if(tmp->m[0].Typ()==VECTOR_CMD){
        ideal mM =(ideal)tmp->m[3].Data();
        ideal mM_copy =idCopy(mM); 
        r0 =IDELEMS(mM_copy);
        LT = leadSyz_ext(mM_copy);
        r = IDELEMS(LT);
        level= (int)(long)tmp->m[4].Data();
        c=r;
        idDelete(&mM_copy);
    } 
  
  
    // Debugging: Print the last element of LT
    // std::cout << "last leadSyz_GPI: " << pString((poly)LT->m[r - 1]) << ", r: " << r << std::endl;

    // Prepare the LLT token
    lists LLT = (lists)omAlloc0Bin(slists_bin);
    LLT->Init(4);  // Initialize with 4 fields
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);  // Initialize with r fields

   




    // Allocate sM and Ld outside the loop
    ideal sM = idInit(c, r0);  // Initialize the smatrix
    lists Ld = NULL; //(lists)omAlloc0Bin(slists_bin);  // Initialize Ld
    //Ld->Init(4);  // Initialize with 4 fields

    // Process each element in LT
    for (int k = 0; k < r; k++) {
       
        id_Delete(&sM, currRing);  // Delete the existing sM
        sM = idInit(c, r0);        // Reinitialize sM

        // omUpdateInfo();
        // if(k==494){
        //     std::cout << "used mem: " << om_Info.UsedBytes << std::endl;
        // }
    
       // Reset Ld for the current iteration (without reallocating)
       //omFreeBin(Ld, slists_bin);  // Free the existing Ld
       Ld = (lists)omAlloc0Bin(slists_bin);  // Reinitialize Ld
       Ld->Init(4);  // Initialize with 4 fields

        // Retrieve the lifted polynomial
        poly s_lift = (poly)LT->m[k];  // s_lift is reassigned in each iteration

       
        int l_k = p_GetComp(s_lift, currRing);
        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);
        poly C = sM->m[k];
        poly Ci = p_Vec2Poly(C, l_k, currRing);
        C = p_Sub(C, Ci, currRing);
        poly C1 = pCopy(lm);
        p_SetComp(C1, l_k, currRing);
        p_SetmComp(C1, currRing);
        C = p_Add_q(C, pCopy(C1), currRing);
        sM->m[k] = C;

     
        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = omStrDup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = omStrDup("Sparse_LeadSyz_matrix");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

        t = (lists)omAlloc0Bin(slists_bin);
        t->Init(5);
        t->m[0].rtyp = VECTOR_CMD; t->m[0].data = pCopy(s_lift);
        t->m[1].rtyp = SMATRIX_CMD; t->m[1].data = sM;
        t->m[2].rtyp = INT_CMD; t->m[2].data = (void*)(long)(k + 1);
        t->m[3].rtyp = MODUL_CMD; t->m[3].data = idCopy(LT);
        // std::cout<<"size of LT:="<<IDELEMS(LT)<<std::endl;
        t->m[4].rtyp = INT_CMD; t->m[4].data = (void*)(long)(level);  // counter = 1

        Ld->m[3].rtyp = LIST_CMD; Ld->m[3].data = t;

        // Set fieldnames[k] to "generator"
        lists field_names = (lists)omAlloc0Bin(slists_bin);
        field_names->Init(r);
        for (int s = 0; s < r; s++) {
            field_names->m[s].rtyp = STRING_CMD;
            field_names->m[s].data = omStrDup("generator");
        }

        LLT->m[0].rtyp = RING_CMD; LLT->m[0].data = currRing;
        LLT->m[1].rtyp = LIST_CMD; LLT->m[1].data = field_names;
        LLT->m[2].rtyp = RING_CMD; LLT->m[2].data = currRing;

        // // // Set data for LLT
        // lists t0 = (lists)omAlloc0Bin(slists_bin);
        // t0->Init(r);
        // for (int s = 0; s < r; s++) {
        //     t0->m[s].rtyp = LIST_CMD;
        //     t0->m[s].data = Ld;
        // }
        temp->m[k].rtyp = LIST_CMD;
        temp->m[k].data = lCopy(Ld);

        pDelete(&Ci);
        pDelete(&C1);
      
         // Clean up temporary lists
         omFreeBin(t, slists_bin);
         omFreeBin(field_names, slists_bin);
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
    final_field_names->m[r].data = omStrDup("total_number_generator");
    LLT->m[1].rtyp = LIST_CMD; LLT->m[1].data = final_field_names;

    // Final data preparation
    lists final_data = (lists)omAlloc0Bin(slists_bin);
    final_data->Init(r + 1);
    for (int k = 0; k < r; k++) {
        final_data->m[k].rtyp = LIST_CMD;
        final_data->m[k].data = temp->m[k].data;  // Transfer data from temp
        temp->m[k].data=NULL; temp->m[k].rtyp=DEF_CMD;

    }

    final_data->m[r].rtyp = INT_CMD;
    final_data->m[r].data = (void*)(long)(r + 1);  // Set total count

    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;

    // Clean up sM and Ld
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
 
 auto start_computation = std::chrono::high_resolution_clock::now();
//  std::string function_name = "lead_syz_GpI";
	// out = call_user_proc(function_name, needed_library, args);
   
    out =  LEAD_GPI(args.leftV());
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    // auto end_computation1 = std::chrono::high_resolution_clock::now();
    // auto computation_time1 =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation1 - start_computation).count();
    // std::cout << "LEADSYZ_Runtime: " << computation_time1 << " nanoseconds" << std::endl;
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();
    // std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    // std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    // lists Token1 = (lists)out.second->m[3].Data();
   
    // int L_si = lSize(Token1)+1;
    // std::cout << "Size of J in the transition: " << L_si << std::endl;
    // for (int i = 0; i < L_si; i++) {
    //         sleftv& listElement = Token1->m[i];  // Access each element as `leftv`
    //     std::cout << "LEAD: " << listElement.String()  << std::endl;
    // }
   
     std::vector<std::string> vec;
     int total_generator;

    // // Iterate through each element of the outer list
     //auto start_serialize = std::chrono::high_resolution_clock::now();
    //   std::cout<<"GPI_LEADSYZ_tokens="<<lSize(u)<< std::endl;
     
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
// std::cout << "#LeadSyz_GPI:=" <<total_generator<< std::endl;
  return {vec, total_generator, total_runtime};

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
    ideal F0 = (ideal)J->m[level].Data(); // Get the module F0 from J[level]
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
        std::cout << "LOT_return :" << pString(LOT) << std::endl;
    }

    return LOT;
}



NO_NAME_MANGLING
int coM(ideal f0, poly s, poly t,lists J,int level) {//poly s and poly t are singular vectors
  
  //std::cout << "s in cOM:"<<pString(s)<< std::endl;
  //std::cout << "t in cOM:"<<pString(t)<< std::endl;
  int i=0;
  int j=0;
  int isTrue = 0;
  f0=f0;
  //i = leadexp(s)[nvars(basering) + 1];
 i= p_GetComp(s,currRing);
  //std::cout << "component of s in cOM:"<<i<< std::endl;
  //j = leadexp(t)[nvars(basering) + 1];
 j= p_GetComp(t,currRing);
  //std::cout << "component of t in cOM:"<<j<< std::endl;
  //poly s10 = leadmonomial(s);
   ideal f=(ideal)J->m[level-1].Data();
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
  } else if ((c<=0) && (i > j)) 
   {
      isTrue = 1;
   }else{
    isTrue=0;
   }

   p_Delete(&s10, currRing);
   p_Delete(&t10, currRing);
   p_Delete(&sfi, currRing);
   p_Delete(&tfj, currRing);


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


    //If t=constant*gen(k), then s>t
    int c0=p_IsConstantPoly(t,currRing);
    if(c0==1){
     return 1;
    }
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
    k=1;
   int d= coM(f, F1, G1, J,k);
//    int d= p_Cmp(F1,G1,currRing);
    // Perform final comparison using coM
   //std::cout << "Leadsyz s="<<pString(s)<<":reduced final s to F1"<<pString(F1)<<" lead t="<<pString(t)<<":reduce final t to G1="<<pString(G1)<< ":coM2"<<d<<std::endl;
        // General case: compare F1 and G1 after iteration
        
        if (d == 1) {
            isTrue = 1;
        } else {
            if ((d==0) && (i > j)) {  //it was i1>i2
                isTrue = 1;
            } else {
              isTrue=0;
            }
        
        }
      
     // Clean memory
  p_Delete(&s10, currRing);
  p_Delete(&t10, currRing);
  p_Delete(&f1, currRing);
  p_Delete(&g1, currRing);
  p_Delete(&F1, currRing);
  p_Delete(&G1, currRing);
  
    //std::cout << "Leadsyz s="<<pString(s)<<":coM2(s,t, f, j)="<<isTrue<<" 0f s= "<<pString(s)<<" and t="<<pString(t)<< std::endl;
  
    return isTrue;
}











//This is working for Example 1,2,3
// int coM2(ideal f, poly s, poly t, lists J, int k) {
//     int i = p_GetComp(s, currRing);  // Get component index for s
//     int j = p_GetComp(t, currRing);  // Get component index for t
   
    
//     // Handle constant polynomials first
//     if (p_IsConstantPoly(t, currRing)) {
//         return 1;  // t is constant, so it is always "less than" s in the ordering
//     }

//     f = f;

//     // Setup s10 and t10 to isolate the lead terms
//     poly s10 = pHead(s); 
//     pSetComp(s10, 0); 
//     pSetmComp(s10); 
//     pSetCoeff(s10, nInit(1)); 
    
//     poly t10 = pHead(t); 
//     pSetComp(t10, 0); 
//     pSetmComp(t10); 
//     pSetCoeff(t10, nInit(1));

//     // Start with the current level's lead terms
//     ideal M = (ideal)J->m[k - 1].Data();
//     poly f1 = pp_Mult_qq(s10, M->m[i - 1], currRing);
//     poly g1 = pp_Mult_qq(t10, M->m[j - 1], currRing);

//     //std::cout << "com2 s=" << pString(s) << ":correspond f1=" << pString(f1) << " and: " << pString(t) << " corr g1=" << pString(g1) << std::endl;

//     // Iterate down through the syzygy levels
//     poly F1 = NULL, G1 = NULL;  // Initialize F1 and G1
//     for (int l = k; l >= 1; l--) {
//         int i1 = p_GetComp(f1, currRing);
//         int i2 = p_GetComp(g1, currRing);

//         if (l == k) {
//             // At the current level k, multiply with corresponding syzygy generators
//             ideal N = (ideal)J->m[k - 1].Data();

//             poly f10 = pHead(f1); 
//             pSetComp(f10, 0); 
//             pSetmComp(f10); 
//             pSetCoeff(f10, nInit(1));
            
//             poly g10 = pHead(g1); 
//             pSetComp(g10, 0); 
//             pSetmComp(g10); 
//             pSetCoeff(g10, nInit(1));

//             F1 = pp_Mult_qq(f10, N->m[i1 - 1], currRing);
//             G1 = pp_Mult_qq(g10, N->m[i2 - 1], currRing);
//             //std::cout << "l:=" <<l<<"level:=" <<k<<std::endl;
//         } else {
//             // At level < k, multiply further down with syzygy generators
//             ideal K = (ideal)J->m[l - 1].Data();
//             //std::cout << "l:=" <<l<<"level:=" <<k<<std::endl;
//             i1 = p_GetComp(F1, currRing);
//             i2 = p_GetComp(G1, currRing);

//             poly F10 = pHead(F1); 
//             pSetComp(F10, 0); 
//             pSetmComp(F10); 
//             pSetCoeff(F10, nInit(1));
            
//             poly G10 = pHead(G1); 
//             pSetComp(G10, 0); 
//             pSetmComp(G10); 
//             pSetCoeff(G10, nInit(1));

//             F1 = pp_Mult_qq(F10, K->m[i1 - 1], currRing);
//             G1 = pp_Mult_qq(G10, K->m[i2 - 1], currRing);
//         }
//     }

    

//     F1 = pHead(F1);
//     G1 = pHead(G1);

   

//     int cmp = p_Cmp(F1, G1, currRing);
//     //std::cout << "s=" << pString(s) << ":reduced F1=" << pString(F1) << "  t=" << pString(t) << ":reduce G1=" << pString(G1) << "SchOrdr"<<cmp<< std::endl;
//     if (cmp > 0) {
//         return 1; // F1 > G1
//     } else if (cmp == 0) {
//         if (i > j) return 1; // Tie breaker: component index
//     } else { // cmp < 0
//         if (i > j) return 1; // Even if F1 < G1, use syzygy index as fallback
//     }
//     return 0;
    
    
//  // Otherwise, s <= t in Schreyer order
// }







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


lists liftTree2(ideal f, poly s,lists J, int level) { //poly s is singular vector
    
    int r = IDELEMS(f);
    //std::cout << "lift_ideal: " << r << std::endl;
    //std::cout<<"lift_vector s=" << pString(s) <<std::endl;
    poly g = NULL;

  
   
   
    g= phi(pCopy(s),idCopy(f));//g:=psi(s)
    //  std::cout<<"g=psi(s)" << pString(g) <<": s="<<pString(s)<<std::endl;
    poly g_copy = pCopy(g);
    // poly lOT=LOT_2(g_copy, f);//poly T0 = LOT(g, f); 
    poly lOT=LOT(g_copy,idCopy((ideal)J->m[level-1].Data()));
    //  std::cout<<"LOT=" << pString(lOT) <<std::endl;
     //poly T0 = g-LOT(g, f); 
    poly T0=p_Sub(pCopy(g),pCopy(lOT),currRing);
    // std::cout << "g-LOT_in LIFT: " << pString(T0) << std::endl;
    lists T=vector_List(pCopy(T0));  //lists T = (lists)pOLY_List(T0)->Data(); 
    
    int t_size = lSize(T)+1;

//    std::cout << "see T: for vector=" <<pString(s)<< std::endl;
//    for(int k=0;k< t_size; k++){
//     std::cout << "T["<<k<<"]=: " <<pString((poly)T->m[k].Data())<< std::endl;
//    }
  
      poly s_v=NULL; //vector s_v
      poly m=NULL;
      poly m1=NULL;
      int k = 0;
     int lambda=0;
    
    
     lists TT = (lists)omAlloc0Bin(slists_bin);
  
     if (t_size==0) 
     {
      TT->Init(0);
       std::cout << "TT is empty, stopping algorithm." << std::endl;
       return TT;
    
     
     } else
     {
        TT->Init(t_size); 
      
     }
      
    
     //ideal I_Gr=J[1];
    //  module f0= leadSyz(I_Gr);//changing to lead syz
       ideal f0=(ideal)J->m[level-1].Data();//
   
    //   std::cout<<"size:="<<t_size<<"at level:="<<level<<"for s="<<pString(s)<<std::endl;
    int c=0;
     for (k = 0; k < t_size; k++){
            poly t = (poly)T->m[k].Data(); //vector t=T[k]
            // std::cout <<"in LIFT2 s="<<pString(s)<< "t in LIFT2:"<<pString(t)<< std::endl;
             
           for (lambda = 0; lambda <= r; lambda++) {
            
             int a = p_GetComp(t,currRing);
             int b = p_GetComp(f0->m[lambda],currRing);
            //ideal Gb_t=idInit(1,1);
           // Gb_t->m[0]=t;
             
     
 
             if(a==b){
                // std::cout <<"a=b s="<<pString(s)<< "t in LIFT2:"<<pString(t)<< std::endl;  
               //int C = (kNF(Gb_t,NULL, pHead(f0->m[lambda])) == 0);
                 bool d = p_DivisibleBy(pHead(f0->m[lambda]), t, currRing);
                  //std::cout << "bool d in LIFT2:"<<d<< std::endl;
                if (d==TRUE) {
                    //std::cout << "bool d in LIFT2:"<<d<< std::endl;
                    m1 = pp_Divide(pHead(t), pHead(f0->m[lambda]), currRing);
                    m = m1;//m=m1;
                    p_SetComp(m,lambda+1,currRing);
                    p_SetmComp(m,currRing);
                    s_v=m;
                      if(level==1){
                        c= coM(f0, s, s_v, J,level);
                      }
                    // std::cout <<"d true s="<<pString(s)<< "t in LIFT2:"<<pString(t)<<":=Found at"<<pString(s_v)<<std::endl;  // std::cout << "bool d in LIFT2:"<<d<< std::endl;
                       else{
                        c= coM2(f0, s, s_v, J,level);
                       } 
                    //    std::cout << "bool c in LIFT2:"<<c<< std::endl;
                       if (c==1)
                        {
                      //std::cout <<" s="<<pString(s)<<":correspond t="<<pString(t)<< "found lift s_v=:"<<pString(s_v)<< std::endl;
                       TT->m[k].rtyp = VECTOR_CMD;
                       TT->m[k].data=pCopy(p_Mult_q(pISet(-1), s_v, currRing));
                    //std::cout <<"in LIFT2 s="<<pString(s)<<":correspond t="<<pString(t)<< ":com detect s_v=:"<<pString(s_v)<< std::endl;
   
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
    // vector s = tok.data[1]
    poly s = (poly)tmp1->m[0].Data();
    // tmp1.data[5]
    int level = (int)(long)tmp1->m[4].Data();
    // std::cout<<" at"<<"level:="<<level<<std::endl;
     // tmp1.data[3]
    int colmn = (int)(long)tmp1->m[2].Data();
    
        // J = tok.data[1]
        lists J = (lists)(tmp2->m[0].Data());
        //  for(int k=0;k<=lSize(J)+1; k++){
        //     ideal l=(ideal)J->m[k].Data();
        //     for (int s=0; s<IDELEMS(l);s++){
        //         std::cout<<"ideal l:="<<pString((poly)l->m[s])<<std::endl;
        //     }
        //  }
//    ideal LT=(ideal)tmp1->m[3].Data();
   
  

   
    
ideal M_copy=nullptr;
  lists lL=nullptr;
  
   int r0 = 0, c = 0;
    
 
 
      
       
      if (tmp->m[0].Typ() == MODUL_CMD){
        r0 = IDELEMS((ideal)(tmp->m[0].Data()));
        M_copy = idCopy((ideal)tmp->m[0].Data()); 
      
        // for(int s=0; s<IDELEMS(M_copy);s++){
        //     std::cout<<"ideal  M_copy module case :="<<pString((poly) M_copy->m[s])<<std::endl;
        // }
      
      } else if(tmp->m[0].Typ() == VECTOR_CMD) {
        r0 = IDELEMS((ideal)(tmp->m[1].Data()));
        M_copy = idCopy((ideal)tmp->m[1].Data()); 
        // for(int s=0; s<IDELEMS(M_copy);s++){
        //     std::cout<<"ideal  M_copy smatrix case :="<<pString((poly) M_copy->m[s])<<std::endl;
        // }
    }
   //M_copy is smatrix
   poly s_copy = pCopy(s);  // Copy polynomial s
   lists J_copy = lCopy(J); // Deep copy of lists J


        // Clean up polynomial copy
        lL = liftTree2(M_copy, s_copy, J_copy, level);
        // std::cout<<" at"<<"level:="<<level<<std::endl;
       ideal LL = (ideal)J->m[level].Data();
       c = IDELEMS(LL);

    
       
        
      
       int r = lSize(lL) + 1;

    //    std::cout<<"r:=lSize(lL) + 1:="<<r<< "at level:="<<level<<std::endl;

    //    for (int i = 0; i < lSize(lL)+1; ++i) {
    //     if (lL->m[i].data ==nullptr) {
    //     std::cout<<"DEBUG: NULL index"<< i<<std::endl;
    //     } else {
    //         std::cout<<"DEBUG: element at index"<< i<<std::endl;
    //     }
    // }
       idDelete(&M_copy);  // Clean up copy
       pDelete(&s_copy);

    lists LLT = (lists)omAlloc0Bin(slists_bin);
    LLT->Init(4); // Initialize with 4 fields
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);
    lists Ld = NULL;
    poly s_lift=NULL;
    // Iterate to fill in data
    ideal sM = idInit(c, r0);
    // std::cout<<"c:="<<c<<" at"<<"level:="<<level<<std::endl;
    for (int k = 0; k < r; k++) {
        id_Delete(&sM, currRing);  // Delete the existing sM
        sM = idInit(c, r0);  
        // Create a new token Ld
     
            s_lift = (poly)lL->m[k].Data(); // Retrieve the lifted polynomial
           
     
        int l_k = p_GetComp(s_lift, currRing);
        // std::cout << "s_lift in LIFT:="<<pString(s_lift)<<std::endl;
       

      
      
        Ld = (lists)omAlloc0Bin(slists_bin);  // Reinitialize Ld
        Ld->Init(4);  // Initialize with 4 fields



        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = omStrDup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = omStrDup("Sparse_matrix_Lift");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

       
      
        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);
        //  std::cout << "#colmn:=" <<colmn<< std::endl;
        poly C=sM->m[colmn-1];
        //  std::cout << "#poly C:=" <<pString(C)<< std::endl;
        poly Ci=p_Vec2Poly(C,l_k,currRing);
       
      C= p_Sub(C,Ci,currRing);
        //  std::cout << "pCopy(s_lift):=" <<pString(s_lift)<< std::endl;
         poly C1= pCopy(lm);
                   p_SetComp(C1,l_k,currRing);
                   p_SetmComp(C1,currRing);
                   


    C=p_Add_q(C, pCopy(C1), currRing);


       sM->m[colmn-1]=C;


        // Prepare Ld data
        t = (lists)omAlloc0Bin(slists_bin);
        t->Init(5);
        t->m[0].rtyp = VECTOR_CMD; 
        t->m[0].data = pCopy(s_lift);
        // std::cout << "After changing sign : " << pString((poly)t->m[0].data) << std::endl;
        t->m[0].data = pCopy(s_lift);
        t->m[1].rtyp = SMATRIX_CMD; 
        t->m[1].data = sM ;
        t->m[2].rtyp = INT_CMD; 
        t->m[2].data = (void*)(long)colmn;
   

        // Assign data to Ld based on the command type
      
            t->m[3].rtyp = MODUL_CMD; 
            t->m[3].data = idCopy(LL);
       
            // std::cout<<"size of LL LIFT:="<<IDELEMS(LL)<<std::endl;
        t->m[4].rtyp = INT_CMD;  
        t->m[4].data = (void*)(long)level;
     

        Ld->m[3].rtyp = LIST_CMD; 
        Ld->m[3].data = t;

        // Set fieldnames[k] to "generator"
        lists field_names = (lists)omAlloc0Bin(slists_bin);
        field_names->Init(r);
        for (int s = 0; s < r; s++) {
            field_names->m[s].rtyp = STRING_CMD;
            field_names->m[s].data = omStrDup("generator");
        }

        LLT->m[0].rtyp = RING_CMD; 
        LLT->m[0].data = currRing;

        // Assign fieldnames to LLT
        LLT->m[1].rtyp = LIST_CMD;
        LLT->m[1].data = field_names;
        LLT->m[2].rtyp = RING_CMD; 
        LLT->m[2].data = currRing;

        // // Set data for LLT
        // lists t0 = (lists)omAlloc0Bin(slists_bin);
        // t0->Init(r);
        // for (int s = 0; s < r; s++) {
        //     t0->m[s].rtyp = LIST_CMD;
        //     t0->m[s].data = lCopy(Ld);
        // }
        temp->m[k].rtyp = LIST_CMD;
        temp->m[k].data = lCopy(Ld);

        pDelete(&Ci);
        pDelete(&C1);
     
     
  
        //  // Clean up temporary lists
         omFreeBin(t, slists_bin);
         omFreeBin(field_names, slists_bin);
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
    final_field_names->m[r].data = omStrDup("total_number_generator");
    LLT->m[1].rtyp = LIST_CMD;  
    LLT->m[1].data = final_field_names;

    // Final data preparation
    lists final_data = (lists)omAlloc0Bin(slists_bin);
    final_data->Init(r + 1);
    for (int k = 0; k < r; k++) {
        final_data->m[k].rtyp = LIST_CMD;
        final_data->m[k].data = temp->m[k].data;  // Transfer data from temp
        temp->m[k].data=NULL; temp->m[k].rtyp=DEF_CMD;
    }

    final_data->m[r].rtyp = INT_CMD;
    final_data->m[r].data = (void*)(long)(r + 1); // Set total count

    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;
    
       // Clean up sM and Ld
    id_Delete(&sM, currRing);
    omFreeBin(Ld, slists_bin);
    temp->Clean(currRing);
    lL->Clean(currRing);

    return {r, LLT};  // Return success state and LLT
}






NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int, long> singular_template_LIFT(const std::string& input_filename,
                                                                      
                                                                       const std::string& lead,
                                                                       const std::string& all_lead,
                                                                       const std::string& needed_library,
                                                                       const std::string& base_filename,unsigned long N)
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
   
    Lead = deserialize(lead, ids);
    All_Lead = deserialize(all_lead, ids);
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
  
    ScopedLeftv arg1(args, Lead.first, lCopy(Lead.second));
    ScopedLeftv arg(arg1, All_Lead.first, lCopy(All_Lead.second));
    //std::cout << arg1.leftV()->String() <<std::endl;
   

//          lists Token = (lists)(args.leftV()->data);
   
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
//       std::cout << "LEAD: " << listElement.String()  << std::endl;
//   }




    // Call the Singular procedure directly (LIFT_GPI) without using name mangling
    std::pair<int, lists> out;
    
    // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
     // Direct call to LIFT_GPI 
//   std::string function_name = "LIFT_Ext_GPI";
//   //   //  std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
//     out = call_user_proc(function_name, needed_library, args);
   
  
      // Direct call to LIFT_GPI 
      out = LIFT_GPI(args.leftV());  

   
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
    int total_generator=0;
   // Extract list from the output
   lists u=NULL;
   // u = (lists)out.second->m[3].CopyD();
  
       u = (lists)out.second->m[3].CopyD();

    if (lSize(u)==0)
    {
       u->Clean(currRing);
   //std::cerr << "Error: SubLIFT_GPI returned a null list." << std::endl;
  ///return std::make_tuple(std::vector<std::string>(), 0, 0);// Early exit with default values
  return{{},0,0};
   } 
 

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
// std::cout << " total time " << total_runtime<< std::endl;
  //std::cout << arg1.leftV()->String() <<std::endl;

    return {vec, total_generator, total_runtime};
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
    poly t0 = LOT(g_copy, idCopy(f0));

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
    // poly s_v = NULL;   // Temporary vector for storing the result
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
                    m = m1;
                    p_SetComp(m, lambda+1, currRing);
                    p_SetmComp(m, currRing);

                    // Set s_v as m and store in TT
                    // s_v =pCopy(m);
                    TT->m[k].rtyp = VECTOR_CMD;
                    TT->m[k].data = m;
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
  

    // Extract tok
    lists tok = (lists)(args->next->Data());
      // Extract TT
    lists TT = (lists)(args->next->next->Data());

    // Token.data
    lists tmp = (lists)(Token->m[3].Data());
    // tok.data
    lists tmp1 = (lists)(tok->m[3].Data());
  
 
 
   // vector s = tok.data[1]
   poly s = (poly)tmp1->m[0].Data();
//    std::cout<<"poly s:="<<pString(s)<<std::endl;
   // tmp1.data[5]
   int level = (int)(long)tmp1->m[4].Data();
    // tmp1.data[3]
   int colmn = (int)(long)tmp1->m[2].Data();
   


       // TT.data
   lists tmp2 = (lists)(TT->m[3].Data());
       // J = TT.data[1]
       lists J = (lists)(tmp2->m[0].Data());

//    ideal LT=(ideal)tmp1->m[3].Data();
   
//   for(int k=0; k<lSize(J)+1; k++){
//     ideal l=(ideal)J->m[k].Data();
//     for(int s=0;s<IDELEMS(l);s++){
//         std::cout<<"ideal:="<<pString((poly)l->m[s])<<std::endl;
//     }
//   }

   
    
    
  ideal M_copy=NULL;
  
   int r0 = 0, c = 0;
    
 
  
 
  
           
      if (tmp->m[0].Typ() == MODUL_CMD){
        r0 = IDELEMS((ideal)(tmp->m[0].Data()));
        M_copy = idCopy((ideal)tmp->m[0].Data()); 
      } else if(tmp->m[0].Typ() == VECTOR_CMD) {
        r0 = IDELEMS((ideal)(tmp->m[1].Data()));
        M_copy = idCopy((ideal)tmp->m[1].Data()); 
    }
        poly s_copy = pCopy(s);  // Copy polynomial s to prevent modification
        lists J_copy = lCopy(J); // Deep copy of lists J
          // std::cout << "vector s:="<<pString(s_copy)<<"-"<<"at"<<"-"<<colmn<<std::endl;
       lists lL = subLiftTree2(M_copy, s_copy,J_copy, level); // Call liftTree with copies
        idDelete(&M_copy);  // Clean up copy
        pDelete(&s_copy);    // Clean up polynomial copy

       int r = lSize(lL) + 1;
       ideal LL = (ideal)J->m[level].Data();
       c = IDELEMS(LL);
    
       if (r == 0) {
        // std::cerr << "Warning: lL is empty. Returning empty result." << std::endl;
        
        lists empty_list = (lists)omAlloc0Bin(slists_bin);
        empty_list->Init(4);

        empty_list->m[0].rtyp = RING_CMD;
        empty_list->m[0].data = currRing;

        // Initialize field names
        lists field_names = (lists)omAlloc0Bin(slists_bin);
        field_names->Init(1); // Initialize with 1 field
        field_names->m[0].rtyp = STRING_CMD;
        field_names->m[0].data = omStrDup("empty");

        empty_list->m[1].rtyp = LIST_CMD;
        empty_list->m[1].data = field_names;

        empty_list->m[2].rtyp = RING_CMD;
        empty_list->m[2].data = currRing;

        // Create an empty list for `final_data`
        lists final_data = (lists)omAlloc0Bin(slists_bin);
        final_data->Init(1);
        final_data->m[0].rtyp = INT_CMD;
        final_data->m[0].data = (void*)(long)0;  // Indicate no generators found

        empty_list->m[3].rtyp = LIST_CMD;
        empty_list->m[3].data = final_data;

        return {0, empty_list};
    }

    // Prepare the LLT token
    lists LLT = (lists)omAlloc0Bin(slists_bin);
    LLT->Init(4); // Initialize with 4 fields
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);
       // Allocate sM and Ld outside the loop
       ideal sM = idInit(c, r0);  // Initialize the submodule
       lists Ld = NULL; // Initialize Ld

  
    // Iterate to fill in data
    for (int k = 0; k < r; k++) {
        
        id_Delete(&sM, currRing);  // Delete the existing sM
        sM = idInit(c, r0);  
    
        // Reset Ld for the current iteration (without reallocating)
     
        Ld = (lists)omAlloc0Bin(slists_bin);  // Reinitialize Ld
        Ld->Init(4);  // Initialize with 4 fields

        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = omStrDup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = omStrDup("Sparse_matrix_SubLift");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

       
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
                   


    C=p_Add_q(C, pCopy(C1), currRing);

// std::cout << "After addition C: " << pString(C) << std::endl;
       sM->m[colmn-1]=C;


        // Prepare Ld data
        t = (lists)omAlloc0Bin(slists_bin);
        t->Init(5);
        t->m[0].rtyp = VECTOR_CMD; 
        // t->m[0].data = pCopy(p_Mult_q(pISet(-1), s_lift, currRing));
        t->m[0].data = pCopy(s_lift);
        t->m[1].rtyp = SMATRIX_CMD; 
        t->m[1].data = sM ;
        t->m[2].rtyp = INT_CMD; 
        t->m[2].data = (void*)(long)colmn;
   

        // Assign data to Ld based on the command type
      
            t->m[3].rtyp = MODUL_CMD; 
            t->m[3].data = idCopy(LL); //J[level]
    //    std::cout<<"size of LT SubLIFT:="<<IDELEMS((ideal)J->m[level].Data())<<std::endl;

        t->m[4].rtyp = INT_CMD;  
        t->m[4].data = (void*)(long)level;
     

        Ld->m[3].rtyp = LIST_CMD; 
        Ld->m[3].data = t;

        // Set fieldnames[k] to "generator"
        lists field_names = (lists)omAlloc0Bin(slists_bin);
        field_names->Init(r);
        for (int s = 0; s < r; s++) {
            field_names->m[s].rtyp = STRING_CMD;
            field_names->m[s].data = omStrDup("generator");
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
        pDelete(&Ci);
        pDelete(&C1);
     
      
  
        //  // Clean up temporary lists
         omFreeBin(t, slists_bin);
         omFreeBin(field_names, slists_bin);
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
    final_field_names->m[r].data = omStrDup("total_number_generator");
    LLT->m[1].rtyp = LIST_CMD;  
    LLT->m[1].data = final_field_names;

    // Final data preparation
    lists final_data = (lists)omAlloc0Bin(slists_bin);
    final_data->Init(r + 1);
    for (int k = 0; k < r; k++) {
        final_data->m[k].rtyp = LIST_CMD;
        final_data->m[k].data = temp->m[k].data;  // Transfer data from temp
        temp->m[k].data=NULL; temp->m[k].rtyp=DEF_CMD;
    }

    final_data->m[r].rtyp = INT_CMD;
    final_data->m[r].data = (void*)(long)(r + 1); // Set total count

    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;
      // Clean up sM and Ld
      id_Delete(&sM, currRing);
      omFreeBin(Ld, slists_bin);
      temp->Clean(currRing);
      lL->Clean(currRing);

    return {r, LLT};  // Return success state and LLT
}

NO_NAME_MANGLING
std::tuple<std::vector<std::string>, int, long> singular_template_SUBLIFT(const std::string& input_filename,
                                                              
                                                                const std::string& lead,
                                                                const std::string& all_lead,
                                                  const std::string& needed_library,
                                                   const std::string& base_filename,unsigned long N)
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
  
    Lead = deserialize(lead, ids);
    All_Lead = deserialize(all_lead, ids);
  //auto end_deserialize = std::chrono::high_resolution_clock::now();
  //auto computation_time_deserialize = std::chrono::duration_cast<std::chrono::milliseconds>( end_deserialize - start_deserialize).count();

    
    // Debugging: Print input data
    // std::cout << "Input Data_LIFT:" << input.second << std::endl;
    
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
 
    ScopedLeftv arg1(args,Lead.first, lCopy(Lead.second));
    ScopedLeftv arg(arg1,All_Lead.first, lCopy(All_Lead.second));
    
//          lists Token = (lists)(args.leftV()->data);
   
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
    // Call Singular procedure
    std::pair<int, lists> out;
    
    //std::cout<<"function_name_SubLIFT:"<< std:: endl;
     // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
     
    // std::string function_name = "SubLIFT_Ext_GPI";
    //  //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
    // out = call_user_proc(function_name, needed_library, args);// Call  SubLIFT_GPI with the raw pointer
  
    out = SubLIFT_GPI(args.leftV());  



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
        u->Clean(currRing);
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
   total_generator = lSize(u); // Assuming u contains the computed generators
   u->Clean(currRing);
  return {vec, total_generator, total_runtime};

}










void sum_InplaceAdd(ideal a, ideal b, const ring R)
{
   
    if (a == nullptr || b == nullptr) {
        std::cerr << "Error: Invalid ideal data!" << std::endl;
        return;
    }
    // Perform the addition in place, modifying 'a'
    for (int k = IDELEMS(a) - 1; k >= 0; k--)
    {
        // Add corresponding elements of 'a' and 'b' and store the result back into 'a'
        a->m[k] = p_Add_q(a->m[k], p_Copy(b->m[k], R), R);
    }
}







NO_NAME_MANGLING
std::pair<std::string, long> singular_template_reduce(const std::string& Red, 
    const std::pair<int, int>& ranges,
    int N,
    const std::string& needed_library,
    const std::string& base_filename) 
{
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    std::filesystem::path basePath = std::filesystem::path(Red).parent_path();
    std::string ids = worker();
    std::pair<int, lists> Gb = deserialize(Red, ids);

    lists Gb_data = (lists)(Gb.second);
    lists tmpL1 = (lists)(Gb_data->m[3].Data());
    // std::cout <<N<< "=:reduce" << std::endl;
    ideal C = nullptr;
    int counter = 0;
    int r=0; int c=0;
    poly vec = NULL;
    leftv L = nullptr;
    std::pair<int, lists> input_part;
    std::string filename;

    // std::string logFilePath = "/scratch/gnawali/Try/gspc-res/example_dir/Smatrix_timing.log";
    // std::ofstream logFile(logFilePath, std::ios::app);
    // if (!logFile.is_open()) {
    //     std::cerr << "Error: Failed to open log file at " << logFilePath << "!" << std::endl;
    //     return {"", 0};
    // }
    
    auto start_computation = std::chrono::high_resolution_clock::now();
    int start = ranges.first;
    int end = ranges.second; 
    bool foundValidFile = false;

    for (int i = start; i <= end; ++i) {  
        filename = (basePath / (std::to_string(i) + ".ssi")).string();
        if (!std::filesystem::exists(filename)) {
            continue;
        }
        foundValidFile = true;
        // Deserialize using the full path
        input_part = deserialize(filename, ids);

        lists token = (lists)(input_part.second);
        lists tmpL = (lists)(token->m[3].Data());
     

        if (L == nullptr) {
            L=&tmpL->m[3];
        }
        if (vec == NULL) {
            vec = (poly)tmpL->m[0].Data();
        }

       
        counter = (int)(long)tmpL->m[4].Data();
      
         
        if (tmpL1->m[0].Typ() == MODUL_CMD) {
            // Handle case where `f` is an ideal
            r = IDELEMS((ideal)(tmpL1->m[0].Data()));
            // std::cout <<r<< "=:r" << std::endl;
          
           
           c = IDELEMS((ideal)L->Data());
        //    std::cout <<c<< "=:c" << std::endl;
         
        } else if (tmpL1->m[0].Typ() == VECTOR_CMD) {
            // Handle case where `f` is a vector
            ideal t=(ideal)(tmpL1->m[1].Data());
            r = IDELEMS(t);
          
          
           c = IDELEMS((ideal)L->Data());
        }
         
      
        
           ideal A = (ideal)tmpL->m[1].Data();
        if (C == nullptr) {
            C = idCopy(A);
        } else {
           
            sum_InplaceAdd(C, A, currRing);
           
        }

        idDelete(&A);

        try {
            std::filesystem::remove(filename);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Failed to delete " << filename << ": " << e.what() << std::endl;
        }
    }

      // If no valid files were found in the range, return an empty list structure
      if (!foundValidFile) {
        // std::cerr << "Warning: No valid files found in range [" << start << "," << end << "]!" << std::endl;

        // Create an empty list
        lists empty_list = (lists)omAlloc0Bin(slists_bin);
        empty_list->Init(4);

        empty_list->m[0].rtyp = RING_CMD;
        empty_list->m[0].data = currRing;

        // Initialize field names
        lists field_names = (lists)omAlloc0Bin(slists_bin);
        field_names->Init(1);  // Initialize with 1 field
        field_names->m[0].rtyp = STRING_CMD;
        field_names->m[0].data = omStrDup("empty");

        empty_list->m[1].rtyp = LIST_CMD;
        empty_list->m[1].data = field_names;

        empty_list->m[2].rtyp = RING_CMD;
        empty_list->m[2].data = currRing;

        // Create an empty list for `final_data`
        lists final_data = (lists)omAlloc0Bin(slists_bin);
        final_data->Init(1);
        final_data->m[0].rtyp = INT_CMD;
        final_data->m[0].data = (void*)(long)0;  // Indicate no generators found

        empty_list->m[3].rtyp = LIST_CMD;
        empty_list->m[3].data = final_data;

        std::string empty_filename = "empty_data";
        return {empty_filename, 0};  // Return the empty list with 0 time
    }
    
 
    
    r=r;
    //   std::cout<<"Smatrix"<<pString((poly)C->m[0])<<std::endl;
    lists output = (lists)omAlloc0Bin(slists_bin);
    output->Init(4);

    lists t = (lists)omAlloc0Bin(slists_bin);
    t->Init(2);
    t->m[0].rtyp = STRING_CMD; t->m[0].data = omStrDup("generators");
    t->m[1].rtyp = STRING_CMD; t->m[1].data = omStrDup("FirstSyz_smatrix");

    output->m[1].rtyp = LIST_CMD; output->m[1].data = t;
    output->m[0].rtyp = RING_CMD; output->m[0].data = currRing;
    output->m[2].rtyp = RING_CMD; output->m[2].data = currRing;

    t = (lists)omAlloc0Bin(slists_bin);
    t->Init(5);
    t->m[0].rtyp = VECTOR_CMD; t->m[0].data = vec;
    t->m[1].rtyp = SMATRIX_CMD; t->m[1].data = C;
    
    t->m[2].rtyp = INT_CMD; t->m[2].data = (void*)(long)c;


  
        t->m[3].rtyp=MODUL_CMD; t->m[3].data= L->CopyD();//Tok.data[5]

    
   
    t->m[4].rtyp = INT_CMD; t->m[4].data = (void*)(long)(counter);
   

    output->m[3].rtyp = LIST_CMD; output->m[3].data = t;

    int cmd_nr;
    blackboxIsCmd("token", cmd_nr);

    std::string out_filename = serialize_with_N(output, base_filename,N);

    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();

    return {out_filename, computation_time};
}



NO_NAME_MANGLING
std::pair<std::string, long> singular_template_ADD_Seq(const std::string& Red, 
    int N,int Nplus,
    const std::string& needed_library,
    const std::string& base_filename) 
{
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);

    // Extract the folder path from the Red file's location
    std::filesystem::path basePath = std::filesystem::path(Red).parent_path();
    // std::cout << base_filename<< "base_filename" << std::endl;
    // std::cout <<Red<< "" << std::endl;
    // std::cout <<N<< "=:ADD_Seq N" << std::endl;
    // std::cout <<Nplus<< "=:ADD_Seq Nplus" << std::endl;
    std::string ids = worker();
    std::pair<int, lists> Gb = deserialize(Red, ids);

    auto start_computation = std::chrono::high_resolution_clock::now();

    lists Gb_data = (lists)(Gb.second);
    lists tmpL1 = (lists)(Gb_data->m[3].Data());
   
    ideal C = nullptr;
    int counter = 0;
   int c=0; int r=0;
    poly vec = NULL;
    leftv L = nullptr;
    std::string logFilePath = "/home/santosh/gspc-syz-ext/example_dir/Smatrix_timing.log";
    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: Failed to open log file at " << logFilePath << "!" << std::endl;
        return {"", 0};
    }


    for (int i = Nplus+1; i <= N+Nplus; ++i) { // Iterate from 1 to N to match "1.ssi", "2.ssi", etc.
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
     

        if (L != nullptr) {
            L = nullptr;
        }
        if (vec != NULL) {
            vec = NULL;
        }

        vec = (poly)tmpL->m[0].Data();
        counter = (int)(long)tmpL->m[4].Data();
      
         
            
        if (tmpL1->m[0].Typ() == MODUL_CMD) {
            // Handle case where `f` is an ideal
            r = IDELEMS((ideal)(tmpL1->m[0].Data()));
          
            L=&tmpL->m[3];
          
           c = IDELEMS((ideal)L->Data());
         
        } else if (tmpL1->m[0].Typ() == VECTOR_CMD) {
            // Handle case where `f` is a vector
            ideal t=(ideal)(tmpL1->m[1].Data());
            r = IDELEMS(t);
            L=&tmpL->m[3];
          
           c = IDELEMS((ideal)L->Data());
        }
      
        
           ideal A = (ideal)tmpL->m[1].Data();
        if (C == nullptr) {
            C = idCopy(A);
        } else {
           
            sum_InplaceAdd(C, A, currRing);
           
        }
        idDelete(&A);

        try {
            std::filesystem::remove(filename);
            // std::cout << "Deleted file: " << filename << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Failed to delete " << filename << ": " << e.what() << std::endl;
        }
    }
   r=r;
    //   std::cout<<"Smatrix"<<pString((poly)C->m[0])<<std::endl;
    lists output = (lists)omAlloc0Bin(slists_bin);
    output->Init(4);

    lists t = (lists)omAlloc0Bin(slists_bin);
    t->Init(2);
    t->m[0].rtyp = STRING_CMD; t->m[0].data = omStrDup("generators");
    t->m[1].rtyp = STRING_CMD; t->m[1].data = omStrDup("FirstSyz_smatrix");

    output->m[1].rtyp = LIST_CMD; output->m[1].data = t;
    output->m[0].rtyp = RING_CMD; output->m[0].data = currRing;
    output->m[2].rtyp = RING_CMD; output->m[2].data = currRing;

    t = (lists)omAlloc0Bin(slists_bin);
    t->Init(5);
    t->m[0].rtyp = VECTOR_CMD; t->m[0].data = vec;
    t->m[1].rtyp = SMATRIX_CMD; t->m[1].data = C;
    
    t->m[2].rtyp = INT_CMD; t->m[2].data = (void*)(long)c;


  
        t->m[3].rtyp=MODUL_CMD; t->m[3].data= L->CopyD();//Tok.data[5]

    
   
    t->m[4].rtyp = INT_CMD; t->m[4].data = (void*)(long)(counter + 1);
   

    output->m[3].rtyp = LIST_CMD; output->m[3].data = t;

    int cmd_nr;
    blackboxIsCmd("token", cmd_nr);

    std::string out_filename = serialize(output, base_filename);

    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
    logFile << "Total Sequential Addition Time: " << computation_time << " ms\n";
    logFile.flush();
    logFile.close();
    return {out_filename, computation_time};
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