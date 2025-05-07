
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
   
   
    lists J = aLL_LEAD(f_copy);
    int r = lSize(J) + 1;

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
            field_names->m[s].data = omStrDup("generator");
           
        }

        LLT->m[0].rtyp = RING_CMD; LLT->m[0].data = currRing;

        // Assign fieldnames to LLT
        LLT->m[1].rtyp = LIST_CMD;
        LLT->m[1].data = field_names;

        LLT->m[2].rtyp = RING_CMD; LLT->m[2].data = currRing;

        temp->m[k].rtyp = LIST_CMD;
        temp->m[k].data = lCopy(Ld);
    }

    // Prepare the final field names
     lists t= (lists)omAlloc0Bin(slists_bin);
     t->Init(r+1);

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
    auto start_computation = std::chrono::high_resolution_clock::now();

	// out = call_user_proc(function_name, needed_library, args);
     out =  ALL_LEAD_GPI(args.leftV());
    lists u = (lists)out.second->m[3].Data();
     std::vector<std::string> vec;
     int total_generator;

    // // Iterate through each element of the outer list
    for(int i (0); i<lSize(u); i++)

  {
    
    auto Outname=serialize((lists)u->m[i].Data(), base_filename);
    
    
    vec.push_back(Outname);
  } 
    auto end_computation = std::chrono::high_resolution_clock::now();
   auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
   total_generator = lSize(u); // Assuming u contains the computed generators
   auto total_runtime=computation_time;

  return {vec, total_generator,total_runtime};
}


NO_NAME_MANGLING
std::pair<int, lists> LEAD_GPI(leftv args) {
    // Extract Token
    lists Token = (lists)(args->Data());
    
    // Extract the first element (f)
    lists tmp = (lists)(Token->m[3].Data());
    int counter =0;
    int r0 =0;
    int r=0;
    int c=0;
    // ideal LT=NULL;
    ideal LT = nullptr;
    
    // Check if f is an ideal or a vector
    if (tmp->m[0].Typ() == IDEAL_CMD) {
       ideal f_copy = idCopy((ideal)(tmp->m[0].Data()));  // Create a deep copy of f
      
        // If it's an ideal, call leadSyz
        LT = leadSyz(f_copy);
     
        r = IDELEMS(LT); 
         c=r;

        r0 = IDELEMS(f_copy); 
       
        idDelete(&f_copy);  // Clean up copy
    
      counter = 1;//counter=1
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        
        ideal mM = (ideal)(tmp->m[4].Data()); 
        ideal mM_copy =idCopy(mM); 
        
        r0 =IDELEMS(mM_copy);  // Set row count for matrix
      
       
        LT = Sec_leadSyz(mM_copy);
        r = IDELEMS(LT); 
        c=r;
        idDelete(&mM_copy);
      

        counter = (int)(long)tmp->m[6].Data();
    }

   

    // Prepare the LLT token
    lists LLT = (lists)omAlloc0Bin(slists_bin);
    LLT->Init(4); // Initialize with 4 fields
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);

    ideal sM = idInit(c, r0);  // Initialize the smatrix
    lists Ld =NULL; //(lists)omAlloc0Bin(slists_bin);  // Initialize Ld
    
    for (int k = 0; k < r; k++) {
        // Create a new token Ld
        id_Delete(&sM, currRing);  // Delete the existing sM
        sM = idInit(c, r0); 
      
    
        Ld = (lists)omAlloc0Bin(slists_bin);  // Reinitialize Ld
        Ld->Init(4);  // Initialize with 4 fields

        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = omStrDup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = omStrDup("Sparse_LeadSyz_matrix");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;
         
        // matrix sM = mpNew(r0, c);
        poly s_lift = (poly)LT->m[k]; // Retrieve the lifted polynomial
    
        int l_k = p_GetComp(s_lift, currRing);

        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);
        poly C=sM->m[k];
        poly Ci=p_Vec2Poly(C,l_k,currRing);
       
        C=p_Sub(C,Ci,currRing);
    
        poly C1= pCopy(lm);
                   p_SetComp(C1,l_k,currRing);
                   p_SetmComp(C1,currRing);
                   

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
    LLT->m[1].rtyp = LIST_CMD;  
    LLT->m[1].data = final_field_names;

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

    // Debugging: Print worker ID
    std::string ids = worker();


    // Deserialize input data
    std::pair<int, lists> input;

    input = deserialize(input_filename, ids);

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));

    // Call Singular procedure
    std::pair<int, lists> out;

   auto start_computation = std::chrono::high_resolution_clock::now();
   
    out =  LEAD_GPI(args.leftV());
    // Extract list from the output
    lists u = (lists)out.second->m[3].Data();

     std::vector<std::string> vec;
     int total_generator;
      total_generator  = lSize(u); 
    for(int i (0); i<total_generator; i++)

  { 
    
    auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    

    vec.push_back(Outname);
  }  // Free memory after usage
 u->Clean(currRing);
auto end_computation = std::chrono::high_resolution_clock::now();
auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();

auto total_runtime=computation_time;


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

        // Get the current leading term (monomial)
        poly headTerm = pHead(f0);
        if (headTerm != NULL) {
            S->m[k].data = headTerm;
            
        } else {
            std::cout << "Warning: pHead(f0) returned NULL at index " << k << std::endl;
        }

        // Move to the next term in the polynomial
        f0 = pNext(f0);

        // Check if f0 is NULL before accessing it
        if (f0 == NULL) 
        {
    
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
    
    lists S = (lists)omAlloc0Bin(slists_bin);

    if (r == 0) 
    {
        S->Init(0); 
        return S;
    }

    S->Init(r);

    for (int k = 0; k < r; k++) 
    {

        S->m[k].rtyp = VECTOR_CMD;

        // Get the current leading term (monomial)
        poly headTerm = pHead(f0);
        if (headTerm != NULL) 
        {
            S->m[k].data = headTerm;
        
        } else {
            std::cout << "Warning: pHead(f0) returned NULL at index " << k << std::endl;
        }

        // Move to the next term in the polynomial
        f0 = pNext(f0);

        // Check if f0 is NULL before accessing it
        if (f0 == NULL) 
        {
    
            break;
        }
    }

    return S;
}








NO_NAME_MANGLING
poly LOT(poly g0, ideal S1) 
{
    
    poly t = NULL;
    int i = 0;
    poly LOT = pCopy(g0);
    
    while (g0!= NULL) 
    {
        for (i = 0; i < IDELEMS(S1); i++) 
        {
            t = pHead(g0); // Leading term of g0
          
            bool c = p_DivisibleBy(S1->m[i], t, currRing);
        
          
            if (c == TRUE)
             { 

                LOT = p_Sub(LOT, pCopy(t), currRing);
                break;
            }
        }
        
        g0 = p_Sub(g0, pCopy(t), currRing);
    
    }

    return LOT;
}

NO_NAME_MANGLING
poly LOT_2(poly g0, ideal S1, lists J, int level) 
{

    int r = IDELEMS(S1);  // Get the number of generators in F0
    ideal F0 = (ideal)J->m[level-1].Data(); // Get the module F0 from J[level]
    poly LOT = pCopy(g0);
    poly t = NULL;
    int i = 0;    

    while (g0!= NULL) 
    {
        for (i = 0; i < r; i++) 
        {
            t = pHead(g0); // Leading term of g0
          
            bool c = p_DivisibleBy(pHead(F0->m[i]), t, currRing);
        
           
            if (c == TRUE)
             { 

                LOT = p_Sub(LOT, pCopy(t), currRing);
                break;
            }
        }
        
        g0 = p_Sub(g0, pCopy(t), currRing);
    
    }

    return LOT;
}





NO_NAME_MANGLING
int coM(ideal f, poly s, poly t) {//poly s and poly t are singular vectors
  
    int i=0;
    int j=0;
    int isTrue = 0;
    //i = leadexp(s)[nvars(basering) + 1];
    i= p_GetComp(s,currRing);

    //j = leadexp(t)[nvars(basering) + 1];
    j= p_GetComp(t,currRing);
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
 
    //poly sfi = leadmonom(s10 * f[i]);
    poly sfi=pp_Mult_qq(s10,pHead(f->m[i-1]),currRing);//should leadmonom

    //poly tfj = leadmonom(t10 * f[j]);
    poly tfj=pp_Mult_qq(t10,pHead(f->m[j-1]),currRing); //should leadmonom
 
    int c= p_Cmp(sfi,tfj,currRing);

 
  if (c==1) {
    
    isTrue = 1;
  } else if ((c==0) && (i > j)) 
   {
      isTrue = 1;
   }else{
    isTrue=0;
   }

   p_Delete(&s10, currRing);
   p_Delete(&t10, currRing);
   p_Delete(&sfi, currRing);
   p_Delete(&tfj, currRing);
  
  return isTrue;
}



NO_NAME_MANGLING
int coM2(ideal f, poly s, poly t, lists J, int k) {                  
  
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
            
       
           
        }
    }

   
   // bool d0= p_ComparePolys(f1,g1,currRing);
   F1=pHead(F1);
   G1=pHead(G1);

   int d= p_Cmp(F1,G1,currRing);
    // Perform final comparison using coM
   
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
lists liftTree(ideal f, poly s) { //poly s is singular vector
    
    int r = IDELEMS(f);
    
    poly g = NULL;

   // int i = 0;
    int k = 0;
    int q = 0;
    g= phi(pCopy(s),idCopy(f));//g:=psi(s)
  
    poly g_copy = pCopy(g);
    poly lOT=LOT(g_copy, f);//poly T0 = LOT(g, f); 
    
     //poly T0 = g-LOT(g, f); 
    poly T0=p_Sub(pCopy(g),pCopy(lOT),currRing);
    //std::cout << "g-LOT_in LIFT: " << pString(T0) << std::endl;
    lists T=pOLY_List(T0);  //lists T = (lists)pOLY_List(T0)->Data(); 
    
    poly m1 = NULL;
    poly m = NULL;
    int C = 0;
    //int cl = 0;
    int t_size = lSize(T)+1;

    lists TT = (lists)omAlloc0Bin(slists_bin); // Allocate memory for empty list TT
  


    TT->Init(t_size); // Initialize TT with size t_size
    
    poly s_v = NULL;//vector s_v
  
    for ( k = 0; k < t_size; k++) {
        poly t = (poly)T->m[k].Data();// poly t=T[k]
   
        for (q = 0; q < r; q++) {
        
            bool c = p_DivisibleBy(pHead(f->m[q]), t, currRing);

            if (c == TRUE) {
                  m1=pp_Divide(t,pHead(f->m[q]), currRing);
                 
                   m = m1;//m=m1;
                   p_SetComp(m,q+1,currRing);
                   p_SetmComp(m,currRing);
                   s_v=m;
    
                   C = coM(f, s, s_v);
                 
                if(C == 1) {
                  
                   TT->m[k].rtyp = VECTOR_CMD;
                   TT->m[k].data=s_v;  //TT[cl]=m*f[lambda]
                  
                    
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
    int r = IDELEMS(f);
    poly g=NULL;  //vector g   
    g= phi(pCopy(s),idCopy(f));//g:=psi(s)
    poly g_copy = pCopy(g);

    // vector t0=LOT_2(g,f,J,level)
    poly lOT=LOT_2(g_copy,f,J,level);
     //poly T0 = g-LOT(g, f); 
    poly T0=p_Sub(pCopy(g),pCopy(lOT),currRing);
    
    lists T = vector_List(T0); //list T =vector_List(T0)

    
    int t_size = lSize(T)+1;
    poly s_v=NULL; //vector s_v
    poly m=NULL;
    poly m1=NULL;
    int lambda=0;
   
    int k=0;
    lists TT = (lists)omAlloc0Bin(slists_bin);
  
   
       TT->Init(t_size); 
     
   
    //ideal I_Gr=J[1];
    //module f0= leadSyz(I_Gr);//changing to lead syz
      ideal f0=(ideal)J->m[level-1].Data();
  
     

    for (k = 0; k < t_size; k++){
           poly t = (poly)T->m[k].Data(); //vector t=T[k]
        
            
          for (lambda = 0; lambda <= r; lambda++) {
           
            int a = p_GetComp(t,currRing);
            int b = p_GetComp(f0->m[lambda],currRing);
         
            if(a==b){
                   
            
                   bool d = p_DivisibleBy(pHead(f0->m[lambda]), t, currRing);
                
               if (d==TRUE) {
                
                   m1 = pp_Divide(pHead(t), pHead(f0->m[lambda]), currRing);
                   m = m1;//m=m1;
                   p_SetComp(m,lambda+1,currRing);
                   p_SetmComp(m,currRing);
                   s_v=m;
   
              
                      int c= coM2(f0, s, s_v, J,level);
                      if (c==1)
                       {

                      TT->m[k].rtyp = VECTOR_CMD;
                      TT->m[k].data=s_v;
  
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
    
    if (tmp->m[0].Typ() == IDEAL_CMD) {
        l = (ideal)J->m[1].Data();
        
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
    
        lL = liftTree(f_copy, s_copy); // Call liftTree with copies
        idDelete(&f_copy);  // Clean up copy
        pDelete(&s_copy);    // Clean up polynomial copy

        r = lSize(lL) + 1;
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        ideal M_copy = idCopy((ideal)tmp->m[1].Data());  //M_copy is smatrix
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

        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);
    
        poly C=sM->m[colmn-1];
    
        poly Ci=p_Vec2Poly(C,l_k,currRing);
       
        C= p_Sub(C,Ci,currRing);
    
        poly C1= pCopy(p_Mult_q(pISet(-1), pCopy(lm), currRing));
                   p_SetComp(C1,l_k,currRing);
                   p_SetmComp(C1,currRing);
                   


        C=p_Add_q(C, pCopy(C1), currRing);
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
  
         // Clean up temporary lists
         omFreeBin(t, slists_bin);
         omFreeBin(field_names, slists_bin);
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
    final_field_names->m[r].data = omStrDup("total_number_generator");
    LLT->m[1].rtyp = LIST_CMD;  
    LLT->m[1].data = final_field_names;

    // Final data preparation
    lists final_data = (lists)omAlloc0Bin(slists_bin);
    final_data->Init(r + 1);
    for (int k = 0; k < r; k++) {
        final_data->m[k].rtyp = LIST_CMD;
        final_data->m[k].data = temp->m[k].data;  // Transfer data from temp
        temp->m[k].data=NULL;  temp->m[k].rtyp=DEF_CMD;
    }

    final_data->m[r].rtyp = INT_CMD;
    final_data->m[r].data = (void*)(long)(r + 1); // Set total count

    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;

       // Clean up sM,Ld and temp
    id_Delete(&sM, currRing);
    omFreeBin(Ld, slists_bin);
    temp->Clean(currRing);
    lL->Clean(currRing);
  
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
    

    // Debugging: Print worker ID
    std::string ids = worker();

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
   out = LIFT_GPI(args.leftV());  
  

    // Extract the result list from the output
    lists u = (lists)out.second->m[3].Data();
    std::filesystem::path oldPath(lead);

if (std::filesystem::exists(oldPath)) {
    std::filesystem::path newPath = oldPath.parent_path() / (std::to_string(N) + ".ssi");

    if (!std::filesystem::exists(newPath)) {
        try {
            std::filesystem::rename(oldPath, newPath);
        
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
    
        vec.push_back(Outname);
    }
    // Free memory after usage

    total_generator = lSize(u);  // Assuming u contains the computed generators
    u->Clean(currRing);
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
    auto total_runtime=computation_time;

    return {vec, total_generator, total_runtime};
}






lists oneSublift(ideal f, poly s) 
{ //poly s is singular vector
    
    int r = IDELEMS(f);

    poly h = NULL;
    poly g = NULL;

   // int i = 0;
    int k = 0;
    int q = 0;
    
    h= phi(pCopy(s),idCopy(f));//h:=psi(s)
    
    g=p_Sub(h,pHead(h),currRing);
    
    poly g_copy = pCopy(g);
    poly lOT = LOT(g_copy, f);
    poly T0=p_Sub(pCopy(g),pCopy(lOT),currRing);

    lists T=pOLY_List(T0); 
   
    poly m1 = NULL;
    poly m = NULL;
    int t_size =lSize(T) + 1;

    lists TT = (lists)omAlloc0Bin(slists_bin);
  
    if (t_size==0) 
    {
     TT->Init(0);
    
      return TT;
   
    
    } else
    {
       TT->Init(t_size); 
     
    }

  for (k = 0; k < t_size; k++) 
  {
      poly t = (poly)T->m[k].Data();// poly t=T[k]

      for (q = 0; q < r; q++)
      {
           
        bool c = p_DivisibleBy(pHead(f->m[q]), t, currRing);


          if (c == TRUE) 
          {
        
            m1=pp_Divide(t,pHead(f->m[q]), currRing);
                 
            m = m1; //m=m1;
            p_SetComp(m,q+1,currRing);
            p_SetmComp(m,currRing);
        
            TT->m[k].rtyp = VECTOR_CMD;
            TT->m[k].data = m;
            
              break;
          }
      }
  }
  


 return TT;
}





lists subLiftTree2(ideal f, poly s, lists J, int level) {
    int r = IDELEMS(f);      
    poly g = NULL;                    
    poly h = NULL;                    
    poly H = NULL;                    
    ideal f0 = (ideal)J->m[level-1].Data();
    
    h= phi(pCopy(s),idCopy(f));//h:=psi(s)

    H= phi(pCopy(s),idCopy(f0));//H:=psi(s)

    g = p_Sub(h, pHead(H), currRing); // g = h - lead(H)
    poly g_copy = pCopy(g);
    
    poly t0 = LOT_2(g_copy, f, J, level);

    // Subtract the LOT from g to get T0
    poly T0 = p_Sub(g, pCopy(t0), currRing);       

    // Convert T0 into a list of vectors
    lists T = vector_List(T0);

    int t_size = lSize(T)+1;                       // Get the size of T
    lists TT = (lists)omAlloc0Bin(slists_bin);   // Allocate memory for TT
    if (t_size==0) 
    {
     TT->Init(0);
    
      return TT;
   
    
    } else
    {
       TT->Init(t_size); 
     
    }                     

    
    poly m = NULL;
    poly m1 = NULL;
    

    // Main loop for sub-lifting each term in T
    for (int k = 0; k < lSize(T)+1; k++) {
        poly t = (poly)T->m[k].Data();           // Get the k-th term in T

        for (int lambda = 0; lambda < r; lambda++) {
            // Get the components of the current term t and f0[lambda]
            int a = p_GetComp(t, currRing);
            int b = p_GetComp(f0->m[lambda], currRing);

        
            if (a == b) {
            
                 bool c = p_DivisibleBy(pHead(f0->m[lambda]), t, currRing);
                if (c==TRUE) {
                    // Compute m1 as the quotient of leading terms
                    m1 = pp_Divide(pHead(t), pHead(f0->m[lambda]), currRing);
                    m = m1;
                    p_SetComp(m, lambda+1, currRing);
                    p_SetmComp(m, currRing);
                    TT->m[k].rtyp = VECTOR_CMD;
                    TT->m[k].data = m;
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

        lL = oneSublift(f_copy, s_copy); // Call liftTree with copies
        idDelete(&f_copy);  // Clean up copy
        pDelete(&s_copy);    // Clean up polynomial copy

        r = lSize(lL) + 1;
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        ideal M_copy = idCopy((ideal)tmp->m[1].Data());  //M_copy is smatrix

        poly s_copy = pCopy(s);  // Copy polynomial s
        lists J_copy = lCopy(J); // Deep copy of lists J

        lL = subLiftTree2(M_copy, s_copy, J_copy, counter);

        // Clean up copies
        idDelete(&M_copy);
        pDelete(&s_copy);
        //lDelete(&J_copy);

        r = lSize(lL) + 1;
    }

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
    lists Ld = NULL;  // Initialize Ld
    // std::cout << "#SubLIFT: " << r << std::endl;
    for (int k = 0; k < r; k++) {
        id_Delete(&sM, currRing);  // Delete the existing sM
        sM = idInit(c, r0);  
    
        Ld = (lists)omAlloc0Bin(slists_bin);  // Reinitialize Ld
        Ld->Init(4);  // Initialize with 4 fields

        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = omStrDup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = omStrDup("Sparse_matrix_SubLIFT");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

        // matrix sM = mpNew(r0, c);
        poly s_lift = (poly)lL->m[k].Data(); // Retrieve the lifted polynomial
        int l_k = p_GetComp(s_lift, currRing);

        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);

        poly C = sM->m[colmn - 1];
        poly Ci = p_Vec2Poly(C, l_k, currRing);
        C = p_Sub(C, Ci, currRing);

        poly C1 = pCopy(p_Mult_q(pISet(-1), pCopy(lm), currRing));
        p_SetComp(C1, l_k, currRing);
        p_SetmComp(C1, currRing);

        C = p_Add_q(C, pCopy(C1), currRing);
        sM->m[colmn - 1] = C;

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
        temp->m[k].rtyp = LIST_CMD;
        temp->m[k].data = lCopy(Ld);
        pDelete(&Ci);
        pDelete(&C1);
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
        temp->m[k].data=NULL;  temp->m[k].rtyp=DEF_CMD;
    }

    final_data->m[r].rtyp = INT_CMD;
    final_data->m[r].data = (void*)(long)(r + 1); // Set total count

    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;

    // Clean up sM and Ld
    id_Delete(&sM, currRing);
   
    omFreeBin(Ld, slists_bin);
    temp->Clean(currRing);  // Clean up temp list
    lL->Clean(currRing);

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


    
    std::pair<int, lists> input;
    std::pair<int, lists> Lead;
    std::pair<int, lists> All_Lead;
    // Deserialize input data
    input = deserialize(input_filename, ids);
    All_Lead = deserialize(all_lead, ids);
    Lead = deserialize(lead, ids);
 
    
    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));
    ScopedLeftv arg(args,All_Lead.first, lCopy(All_Lead.second));
    ScopedLeftv arg1(arg,Lead.first, lCopy(Lead.second));
    
    
    // Call Singular procedure
    std::pair<int, lists> out;
     // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
     
     out = SubLIFT_GPI(args.leftV());  // Call  SubLIFT_GPI with the raw pointer
 
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
      u = (lists)out.second->m[3].CopyD();
    
    
    if (lSize(u)==0)
     {
        u->Clean(currRing);

   return{{},0,0};
    } 
    
       //std::cerr << "Size of SubLIFT Token:" << lSize(u)+1<<std::endl;
       total_generator = lSize(u);
       for (int i = 0; i < total_generator; i++) {
           vec.push_back(serialize((lists)u->m[i].Data(), base_filename));
       }
   
    
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
      
    auto total_runtime=computation_time;

 
  // Free memory after usage
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
    
    auto start_addition = std::chrono::high_resolution_clock::now();
    int start = ranges.first;
    int end = ranges.second; 
    bool foundValidFile = false;

    for (int i = start; i <= end; ++i) {  
        filename = (basePath / (std::to_string(i) + ".ssi")).string();
        if (!std::filesystem::exists(filename)) {
            continue;
        }
        foundValidFile = true;
        input_part = deserialize(filename, ids);
        lists token = (lists)input_part.second;
        lists tmpL = (lists)(token->m[3].Data());
        
        if (L != nullptr) {
            L = nullptr;
        }
        if (vec != NULL) {
            vec = NULL;
        }
        vec = (poly)tmpL->m[0].Data();
        counter = (int)(long)tmpL->m[5].Data();

        if (tmpL1->m[0].Typ() == IDEAL_CMD) {
            r = IDELEMS((ideal)(tmpL1->m[0].Data()));
            L = &tmpL->m[4];
            c = IDELEMS((ideal)L->Data());
        } else if (tmpL1->m[0].Typ() == VECTOR_CMD) {
            ideal t = (ideal)(tmpL1->m[4].Data());
            r = IDELEMS(t);
            L = &tmpL->m[4];
            c = IDELEMS((ideal)L->Data());
        }

        ideal A = (ideal)tmpL->m[1].Data();

        if (C == nullptr) {
            C = idCopy(A);
            id_Normalize(C, currRing);
        } else {
            sum_InplaceAdd(C, A, currRing);
            id_Normalize(C, currRing);
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

    if (tmpL1->m[0].Typ() == IDEAL_CMD || tmpL1->m[0].Typ() == VECTOR_CMD) {
        t->m[4].rtyp = MODUL_CMD; t->m[4].data = L->CopyD();
    }
    t->m[5].rtyp = INT_CMD; t->m[5].data = (void*)(long)counter;
    t->m[6].rtyp = INT_CMD; t->m[6].data = (void*)(long)(counter + 1);

    output->m[3].rtyp = LIST_CMD; output->m[3].data = t;

    int cmd_nr;
    blackboxIsCmd("token", cmd_nr);
    std::string out_filename = serialize_with_N(output, base_filename,N);
    auto end_addition = std::chrono::high_resolution_clock::now();
    auto addition_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_addition - start_addition).count();
    // logFile << "Total Addition Time: " << addition_time << " ms\n";
    // logFile.flush();
    // logFile.close();
   
    return {out_filename, addition_time};
}









NO_NAME_MANGLING
std::pair<std::string, long> singular_template_ADD_Seq(const std::string& Red, 
    int N,int Nplus,
    const std::string& needed_library,
    const std::string& base_filename) 
{
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    std::filesystem::path basePath = std::filesystem::path(Red).parent_path();

    std::string ids = worker();
    std::pair<int, lists> Gb = deserialize(Red, ids);

    auto start_computation = std::chrono::high_resolution_clock::now();

    lists Gb_data = (lists)(Gb.second);
    lists tmpL1 = (lists)(Gb_data->m[3].Data());
   
    ideal C = nullptr;
    int counter = 0;
    int r=0;int c=0;
    poly vec = NULL;
    leftv L = nullptr;
    std::pair<int, lists> input_part;
    std::string logFilePath = "/home/santosh/gspc-res/example_dir/Smatrix_timing.log";
    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: Failed to open log file at " << logFilePath << "!" << std::endl;
        return {"", 0};
    }
    
   

    for (int i = Nplus+1; i <= N+Nplus; i++) {  
     
    // Iterate from 1 to N to match "1.ssi", "2.ssi", etc.
        // Construct the full path for i.ssi files within the same folder as Red
        std::string filename = (basePath / (std::to_string(i) + ".ssi")).string();
        
          
            if (!std::filesystem::exists(filename)) {
                // std::cerr << "File not found: " << filename << ". Skipping this iteration." << std::endl;
                continue;  // Skip to next iteration
            }
            
        input_part = deserialize(filename, ids);

        lists token = (lists)input_part.second;
        lists tmpL = (lists)(token->m[3].Data());

         


          // At this point, L might still point to tmpL or token, so  need to ensure it's safe to free it
          if (L != nullptr) {
            L = nullptr;  // Reset L to prevent any dangling pointers
        }
        if(vec!=NULL){
            vec=NULL;
        }

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
        ideal A = (ideal)tmpL->m[1].Data();
      
    if (C == nullptr) {
            C = idCopy(A);
            id_Normalize(C, currRing);
        }
  
    else {
        sum_InplaceAdd(C, A, currRing);  
        id_Normalize(C, currRing);
        
     
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
        t->m[4].rtyp=MODUL_CMD; t->m[4].data= L->CopyD();//Tok.data[5]
    }
    t->m[5].rtyp = INT_CMD; t->m[5].data = (void*)(long)counter;
    t->m[6].rtyp = INT_CMD; t->m[6].data = (void*)(long)(counter + 1);

    output->m[3].rtyp = LIST_CMD; output->m[3].data = t;

    int cmd_nr;
    blackboxIsCmd("token", cmd_nr);

   
    std::string out_filename=serialize(output,base_filename);
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
    logFile << "Total Sequential Addition Time: " << computation_time << " ms\n";
    logFile.flush();
    logFile.close();

    rKill(currRing); 
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