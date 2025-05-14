
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
                poly s0=MATELEM(l, a, b) ;
                for(int ii=1;ii<=rVar(currRing);ii++)
                {
                    if(p_GetExp(s0,ii,currRing)==2) { pLmDelete(&s0); break;}
                }
                MATELEM(l, a, b) = s0;
                
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
  matrix M= lcm_mod(f_copy);

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
	//std::cout << ids << " in singular_..._compute" << std::endl;
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
std::pair<int, lists> LEAD_ExT_GPI(leftv args) {
    // Extract Token
    lists Token = (lists)(args->Data());

    // Extract the first element (f)
    lists tmp = (lists)(Token->m[3].Data());
    ideal f_copy = idCopy((ideal)(tmp->m[0].Data()));  // Create a deep copy of f

    // Call leadSyz_ext
    // auto start_computation = std::chrono::high_resolution_clock::now();
    ideal LT = leadSyz_ext(f_copy);
    // auto end_computation = std::chrono::high_resolution_clock::now();
    // auto computation_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
    // std::cout << "lead_syz_ext: " << computation_time << " nanoseconds" << std::endl;

    int r = IDELEMS(LT);  // Number of elements in LT
    int r0 = IDELEMS(f_copy);
    int c = r;

    idDelete(&f_copy);  // Free f_copy

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
//   r=300;
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
        t->m[4].rtyp = INT_CMD; t->m[4].data = (void*)(long)(1);  // counter = 1

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
// 	out = call_user_proc(function_name, needed_library, args);
   
    out =  LEAD_ExT_GPI(args.leftV());
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



lists  liftTree(ideal f, poly s) { //poly s is singular vector
    
    int r = IDELEMS(f);
    //std::cout << "lift_ideal: " << r << std::endl;
    //std::cout<<"lift_vector s=" << pString(s) <<std::endl;
    poly g = NULL;

   // int i = 0;
    int k = 0;
    int q = 0;
   
    
    poly lm_s = pHead(s); // Get the leading monomial of s including coefficient
    // std::cout<<"lm_s=pHeads)"<<pString(lm_s)<<std::endl;
    pSetComp(lm_s,0);
    pSetmComp(lm_s);
      // std::cout<<"lm_s=pHeads)"<<pString(lm_s)<<std::endl;
    int g1= p_GetComp(s,currRing);
     
  
    g=pp_Mult_qq(lm_s,f->m[g1-1],currRing);//g:=psi(s)
    //  std::cout<<"g=psi(s)" << pString(g) <<": s="<<pString(s)<<std::endl;
    poly g_copy = pCopy(g);
    poly lOT=LOT(g_copy, f);//poly T0 = LOT(g, f); 
     //std::cout<<"LOT=" << pString(lOT) <<std::endl;
     //poly T0 = g-LOT(g, f); 
    poly T0=p_Sub(pCopy(g),pCopy(lOT),currRing);
    //std::cout << "g-LOT_in LIFT: " << pString(T0) << std::endl;
    lists T=vector_List(pCopy(T0));  //lists T = (lists)pOLY_List(T0)->Data(); 
    
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
  
        for (q = 0; q < r; q++) {
    

            bool c = p_DivisibleBy(f->m[q], t, currRing);


          

            if (c == TRUE) {
            
               poly L=pHead(f->m[q]);
         
              //  m1 =t/lead(L); 
                poly tmp=p_CopyPowerProduct0(t,NULL,currRing);
                p_ExpVectorSub(tmp,L, currRing);
                p_SetCoeff0(tmp,nDiv(pGetCoeff(L),pGetCoeff(t)),currRing);
          
                  m1=tmp;
                //     if(p_Mult_q(pCopy(m1),L,currRing)==t){
                //     m=pCopy(m1);
                
                //  } else{
                //     m=pCopy(p_Mult_q(pISet(-1), pCopy(m1), currRing));
                  
                
                //  }
                   
                 
               poly t1=pCopy(p_Mult_q(pCopy(m1),L,currRing));
               BOOLEAN sign1=p_EqualPolys(t1,t,currRing);
                    // std::cout << "sign1:"<<sign1<< std::endl;
                 if(sign1==FALSE){
                    m=pCopy(p_Mult_q(pISet(-1), pCopy(m1), currRing));
                   
                 } 
                  else{
                    m=pCopy(m1);
                 } 
                   p_SetComp(m,q+1,currRing);
                   p_SetmComp(m,currRing);
                   s_v=pCopy(m);
               //std::cout << "s_v in LIFT_COM:"<<pString(s_v)<<":corresponding t="<<pString(t)<< ": given leadsyz:s="<<pString(s)<<std::endl;
                C = coM(f, s, s_v);
                 //std::cout << "s_v in LIFT_COM:"<<pString(s_v)<<":corresponding t="<<pString(t)<< ": given leadsyz:s="<<pString(s)<<"com c="<<C<<std::endl;
                if(C == 1) {
                  
                   TT->m[k].rtyp = VECTOR_CMD;
                   TT->m[k].data=pCopy(s_v);  //TT[cl]=m*f[lambda]
                //    std::cout << "s_v in LIFT_COM:="<<pString(s_v)<<std::endl;
                    
                    break;
                }
            }
             
        } 
            
      
            
    }   
       
    
 

    return TT;
}






NO_NAME_MANGLING
std::pair<int, lists> LIFT_Ext_GPI(leftv args) {
    // Extract the Token
    lists Token = (lists)(args->Data());
  

    // Extract tok
    lists tok = (lists)(args->next->Data());
   

    // Token.data
    lists tmp = (lists)(Token->m[3].Data());
    // tok.data
    lists tmp1 = (lists)(tok->m[3].Data());
  
 
    // vector s = tok.data[1]
    poly s = (poly)tmp1->m[0].Data();
    // tmp1.data[5]
    int counter = (int)(long)tmp1->m[4].Data();
     // tmp1.data[3]
    int colmn = (int)(long)tmp1->m[2].Data();
   ideal LT=(ideal)tmp1->m[3].Data();
   
  

   
    
    
  
  
   int r0 = 0, c = 0;
    
 
   r0 = IDELEMS((ideal)(tmp->m[0].Data()));
   c = IDELEMS(LT);
  
        ideal f_copy = idCopy((ideal)(tmp->m[0].Data()));  // Create a deep copy of f
        poly s_copy = pCopy(s);  // Copy polynomial s to prevent modification
          // std::cout << "vector s:="<<pString(s_copy)<<"-"<<"at"<<"-"<<colmn<<std::endl;
       lists lL = liftTree(f_copy, s_copy); // Call liftTree with copies
        idDelete(&f_copy);  // Clean up copy
        pDelete(&s_copy);    // Clean up polynomial copy

       int r = lSize(lL) + 1;
  

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
      
   
       

      
      
        Ld = (lists)omAlloc0Bin(slists_bin);  // Reinitialize Ld
        Ld->Init(4);  // Initialize with 4 fields



        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = omStrDup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = omStrDup("Sparse_matrix_Lift");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

       
        poly s_lift = (poly)lL->m[k].Data(); // Retrieve the lifted polynomial
        int l_k = p_GetComp(s_lift, currRing);
        // std::cout << "s_lift in LIFT:="<<pString(s_lift)<<std::endl;
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
        t->m[0].data = pCopy(p_Mult_q(pISet(-1), s_lift, currRing));
        t->m[1].rtyp = SMATRIX_CMD; 
        t->m[1].data = sM ;
        t->m[2].rtyp = INT_CMD; 
        t->m[2].data = (void*)(long)colmn;
   

        // Assign data to Ld based on the command type
      
            t->m[3].rtyp = MODUL_CMD; 
            t->m[3].data = LT; //LT
       

        t->m[4].rtyp = INT_CMD;  
        t->m[4].data = (void*)(long)counter;
     

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


    input = deserialize(input_filename, ids);
   
    Lead = deserialize(lead, ids);

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
  
    ScopedLeftv arg1(args, Lead.first, lCopy(Lead.second));
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
    //    std::string function_name = "LIFT_Ext_GPI";
	// out = call_user_proc(function_name, needed_library, args);
   
  
      // Direct call to LIFT_GPI 
      out = LIFT_Ext_GPI(args.leftV());  

    // Extract the result list from the output
    lists u = (lists)out.second->m[3].Data();
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
    
   //For s= c*x^a*gen(i);
   //poly lm_s=c*x^a;
    poly lm_s = pHead(s); // Get the leading monomial of s including coefficient
    pSetComp(lm_s,0);
    pSetmComp(lm_s);

    int g1= p_GetComp(s,currRing);
     
    //std::cout << "GetComp: " << g1 << std::endl;
    //std::cout<<"f[g1]=" << pString(f->m[g1-1]) <<std::endl;
    h=pp_Mult_qq(lm_s,f->m[g1-1],currRing); //h=lm_s*f[g1]
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

    lists T=vector_List(pCopy(T0)); 
   
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
            poly L=pHead(f->m[q]);
              //  std::cout << "c="<<c<< "t in SubLIFT:"<<pString(t)<< "L="<<pString(L)<<std::endl;
              //  m1 =t/lead(L); 
                poly tmp=p_CopyPowerProduct0(t,NULL,currRing);
                p_ExpVectorSub(tmp,L, currRing);
                p_SetCoeff0(tmp,nDiv(pGetCoeff(L),pGetCoeff(t)),currRing);
                  // std::cout << "tmp "<<pString(tmp)<<std::endl;
                  m1=tmp;
                 
            //  if(p_Mult_q(pCopy(m1),L,currRing)==t){
            //         m=pCopy(m1);
                
            //      } else{
            //         m=pCopy(p_Mult_q(pISet(-1), pCopy(m1), currRing));
                  
                
            //      }

                //    if(m*L!=t){
                //     m=-m1;
                //    }    
                poly t1=pCopy(p_Mult_q(pCopy(m1),L,currRing));
               BOOLEAN sign1=p_EqualPolys(t1,t,currRing);
                    // std::cout << "sign1:"<<sign1<< std::endl;
                 if(sign1==FALSE){
                    m=pCopy(p_Mult_q(pISet(-1), pCopy(m1), currRing));
                   
                 } 
                  else{
                    m=pCopy(m1);
                 } 
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




NO_NAME_MANGLING
std::pair<int, lists> SubLIFT_Ext_GPI(leftv args) {
    // Extract the Token
    lists Token = (lists)(args->Data());
  

    // Extract tok
    lists tok = (lists)(args->next->Data());
   

    // Token.data
    lists tmp = (lists)(Token->m[3].Data());
    // tok.data
    lists tmp1 = (lists)(tok->m[3].Data());
  
 
    // vector s = tok.data[1]
    poly s = (poly)tmp1->m[0].Data();
    // tmp1.data[5]
    int counter = (int)(long)tmp1->m[4].Data();
     // tmp1.data[3]
    int colmn = (int)(long)tmp1->m[2].Data();
   ideal LT=(ideal)tmp1->m[3].Data();
   
  

   
    
    
  
  
   int r0 = 0, c = 0;
    
 
   r0 = IDELEMS((ideal)(tmp->m[0].Data()));
   c = IDELEMS(LT);
  
        ideal f_copy = idCopy((ideal)(tmp->m[0].Data()));  // Create a deep copy of f
        poly s_copy = pCopy(s);  // Copy polynomial s to prevent modification
          // std::cout << "vector s:="<<pString(s_copy)<<"-"<<"at"<<"-"<<colmn<<std::endl;
       lists lL = oneSublift(f_copy, s_copy); // Call liftTree with copies
        idDelete(&f_copy);  // Clean up copy
        pDelete(&s_copy);    // Clean up polynomial copy

       int r = lSize(lL) + 1;
  
    
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
        t->m[0].data = pCopy(p_Mult_q(pISet(-1), s_lift, currRing));
        t->m[1].rtyp = SMATRIX_CMD; 
        t->m[1].data = sM ;
        t->m[2].rtyp = INT_CMD; 
        t->m[2].data = (void*)(long)colmn;
   

        // Assign data to Ld based on the command type
      
            t->m[3].rtyp = MODUL_CMD; 
            t->m[3].data = LT; //LT
       

        t->m[4].rtyp = INT_CMD;  
        t->m[4].data = (void*)(long)counter;
     

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
    
    
    //auto start_deserialize = std::chrono::high_resolution_clock::now();
    input = deserialize(input_filename, ids);
  
    Lead = deserialize(lead, ids);
  //auto end_deserialize = std::chrono::high_resolution_clock::now();
  //auto computation_time_deserialize = std::chrono::duration_cast<std::chrono::milliseconds>( end_deserialize - start_deserialize).count();

    
    // Debugging: Print input data
    // std::cout << "Input Data_LIFT:" << input.second << std::endl;
    
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
 
    ScopedLeftv arg1(args,Lead.first, lCopy(Lead.second));
    
    
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
     
    //  std::string function_name = "SubLIFT_Ext_GPI";
	// out = call_user_proc(function_name, needed_library, args);
  
    out = SubLIFT_Ext_GPI(args.leftV());  



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
     

        if (L != nullptr) {
            L = nullptr;
        }
        if (vec != NULL) {
            vec = NULL;
        }

        vec = (poly)tmpL->m[0].Data();
        counter = (int)(long)tmpL->m[4].Data();
      
         
            r = IDELEMS((ideal)(tmpL1->m[0].Data()));
          
            L=&tmpL->m[3];
          
           c = IDELEMS((ideal)L->Data());
         
      
        
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
    // std::cout <<N<< "=:Reduce" << std::endl;
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
    std::string logFilePath = "/scratch/gnawali/Try/gspc-res/example_dir/Smatrix_timing.log";
    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: Failed to open log file at " << logFilePath << "!" << std::endl;
        return {"", 0};
    }


    for (int i = Nplus+1; i <= N+Nplus; i++) { // Iterate from 1 to N to match "1.ssi", "2.ssi", etc.
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
      
         
            r = IDELEMS((ideal)(tmpL1->m[0].Data()));
          
            L=&tmpL->m[3];
          
           c = IDELEMS((ideal)L->Data());
         
      
        
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