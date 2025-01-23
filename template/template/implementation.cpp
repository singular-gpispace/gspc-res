
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
matrix lcm_mod(ideal G) { //ideal G is Singular module

    int a=0;
    int b=0;
    int i=0;
    int j=0;
    ideal G_copy=idCopy(G);
    int r = IDELEMS(G_copy);
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
                //  std::cout << "m[a.b] in lcm_mod: " << pString(MATELEM(l, a, b)) << std::endl;

            } else {
                // If i is not equal to j, set l[a, b] to 0
                MATELEM(l,a,b)= NULL;
            }
        }
    }
    

    return l;
}




NO_NAME_MANGLING
ideal leadSyz(ideal f) {
    int a = 0, b = 0, j = 0, k = 0;  // Loop variables
    poly s = NULL;  // Temporary polynomial to store elements of L
    poly t = NULL;  // Temporary polynomial to store the current syzygy candidate
    ideal f_copy = idCopy(f);  // Copy the input ideal to avoid modifying it
    int r = IDELEMS(f_copy);  // Number of elements in the ideal
    ideal L = idInit(0, 1);  // Initialize an empty ideal (syzygy module)
    matrix M = mpNew(r, r);  // Matrix to store LCM-based computations

    // Fill the matrix M with LCM computations
    for (a = 0; a < r; a++) {
        for (b = 0; b < r; b++) {
            // Compute LCM of leading monomials of f[a] and f[b]
            poly lcm = p_Lcm(pHead(f_copy->m[b]), pHead(f_copy->m[a]), currRing);
            pSetCoeff0(lcm, nInit(1));  // Normalize the LCM (set coefficient to 1)
            MATELEM(M, a, b) = pp_Divide(lcm, pHead(f_copy->m[b]), currRing); // Store the quotient
        }
    }

    int cc = 0;  // Counter for the number of generators in L

    // Iterate through pairs of indices to construct syzygies
    for (int i = 1; i < r; i++) {
        for (j = 0; j < i; j++) {
            // Generate the initial syzygy candidate from matrix M
            poly t0 = pCopy(MATELEM(M, j, i));
            p_SetComp(t0, i + 1, currRing);  // Assign the component index
            p_SetmComp(t0, currRing);       // Normalize the component
            t = pCopy(t0);  // Copy t0 to t as the current syzygy candidate

            // Check divisibility conditions for t against the elements in L
            for (k = 0; k < IDELEMS(L); k++) {
                s = (poly)L->m[k];  // Retrieve the k-th generator of L
                bool c = p_DivisibleBy(pHead(s), t, currRing);  // Check if s divides t
                if (c == TRUE) {
                    // If s divides t, discard t
                    t = NULL;
                    break;
                } else {
                    // Check if t divides s
                    bool d = p_DivisibleBy(pHead(t), s, currRing);
                    if (d == TRUE) {
                        // If t divides s, remove s from L
                        // std::cout << "t divides L[" << k << "], removing L[" << k << "]." << std::endl;
                        ideal tmp = id_Delete_Pos(L, k, currRing);  // Remove s from L
                        L = NULL;  // Set L to NULL before reassigning
                        idDelete(&L);  // Free the old L
                        L = tmp;  // Assign the updated L
                        k--;  // Adjust index to account for the removed element
                        cc--;  // Decrease the counter
                    }
                }
            }

            // If t survives all checks, add it to L
            if (t != NULL) {
                if (cc >= IDELEMS(L)) {
                    // Resize L if necessary
                    ideal tmpL = idInit(cc + 1, 1);
                    for (int i = 0; i < cc; i++) {
                        tmpL->m[i] = pCopy(L->m[i]);  // Copy elements from L to tmpL
                        L->m[i] = NULL;  // Clear the old L
                    }
                    idDelete(&L);  // Free the old L
                    L = tmpL;  // Update L to point to the resized ideal
                }
                L->m[cc] = pCopy(t);  // Add t to L
                cc++;  // Increment the counter
                p_Delete(&t, currRing);  // Free t after adding it to L
            }
        }
    }

    // Debug output: Print the final size and contents of L
    // std::cout << "Final first leadsyz size: " << IDELEMS(L) << std::endl;
    // for (int k = 0; k < cc; k++) {
    //     std::cout << "Generator " << k << ": " << pString((poly)L->m[k]) << std::endl;
    // }

 

    return L;  // Return the computed syzygy module
}






NO_NAME_MANGLING
ideal Sec_leadSyz(ideal f0) {
    int r = IDELEMS(f0);  // Get the number of elements in the ideal f0
    poly s = NULL;  // Polynomial s is a singular vector
    poly t = NULL;  // Polynomial t is a singular vector
    int cc = 0; // Counter for elements in L
    
    // Initialize ideal L with initial size 0
    ideal L = idInit(0, 1);
    ideal f_copy=idCopy(f0);
    // Create a matrix M using lcm_mod for the input ideal f0
    matrix M = lcm_mod(f_copy);  // Ensure lcm_mod returns a valid matrix

    // Loop through pairs (i, j) in the matrix
    for (int i = 1; i < r; i++) {
        for (int j = 0; j < i; j++) {
            // Fetch the matrix element at (j, i)
            poly t0 = pCopy(MATELEM(M, j, i));

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

      //  Debug output
    // std::cout << "Final second-Leadsyz  size: " << IDELEMS(L) << std::endl;
    // for (int k = 0; k < cc; k++) {
    //     std::cout << "Generator " << k << ": " << pString((poly)L->m[k]) << std::endl;
    // }

    // // return (L);
    return L;
}



NO_NAME_MANGLING
lists aLL_LEAD(ideal f) {
    // Allocate memory for the lists structure
    lists J = (lists)omAlloc0Bin(slists_bin);
    J->Init(2); // Initialize the list with two elements
    ideal f_copy = f;
    
    // Initialize the first two elements
    J->m[0].rtyp = IDEAL_CMD;  
    J->m[0].data = f_copy;          
    
    int n = rVar(currRing); // Get the number of variables in the current ring
    ideal F = leadSyz(f_copy);
    // int g=IDELEMS(F);
   
    // for(int k=0; k<g;k++){
    //   std::cout << "First_LeadSyz :at" <<""<<k << ": " << pString((poly)F->m[k]) << std::endl;
    // }
    ideal F_copy =F;
   
    J->m[1].rtyp = MODUL_CMD;  
    J->m[1].data = F_copy;

    int cc = 2; // Current count of elements in the list

    // Debugging output to print the number of variables
    // std::cout << "Number of variables: " << n << std::endl;

    // Iterate through the variables in the ring to compute subsequent syzygy ideals
    for (int t = 0; t < n; t++) {
        // std::cout << "Processing variable: " << t + 1 << std::endl;

        
        ideal temp = Sec_leadSyz(F_copy);
        bool b=idIs0(temp);
       if(b==FALSE){
        // idDelete(&F_copy);
         F_copy = temp; 
         temp=NULL;
       }
    else
    {
            std::cout << "In break: the syzygy ideal is zero or empty." << std::endl;
            break;  // Exit the loop
    }

        // Resize the list if necessary to accommodate new elements
        if (cc >= lSize(J)+1) {
            int newSize = cc+1; // Increase size by 1
            lists tmpL = (lists)omAlloc0Bin(slists_bin); // Allocate a new list
            tmpL->Init(newSize); // Initialize the new list with the updated size

            // Copy elements from the old list `J` to the new list `tmpL`
            for (int i = 0; i < cc; i++) {
                //  std::cout << "variable:i " << i<< std::endl;
                tmpL->m[i].rtyp = J->m[i].rtyp;
                tmpL->m[i].data = J->m[i].data;
            }

            // Free memory associated with the old list
            omFreeBin(J, slists_bin);

            // Assign the resized list to `J`
            J = tmpL;
        }
     std::cout << "Counter in the loop:= " << cc << std::endl;
        // Append the new syzygy ideal to the list
        J->m[cc].rtyp = MODUL_CMD;
        J->m[cc].data = F_copy;
            cc++;
        
    }
       for(int k=0;k<cc;k++){
      ideal l=(ideal)J->m[k].Data();
      for(int s=0; s <IDELEMS(l);s++){
      std::cout << "Sch FRame as list " << k << ": " << pString((poly)l->m[s]) << std::endl;
      }
 
    }
    return J;
}








NO_NAME_MANGLING
 std::pair<int, lists> ALL_LEAD_GPI(leftv args) {
    // Extract Token
    lists Token = (lists)(args->Data());
    
    // Extract Token.data
     lists tmp = (lists)(Token->m[3].Data());
     //Extract def f=Token.data[1]
    ideal f=(ideal)tmp->m[0].Data();
    ideal f_copy=idCopy(f);
   
   
    for(int k=0;k<IDELEMS(f_copy);k++){
 std::cout << "Generator input" << k << ": " << pString((poly)f_copy->m[k]) << std::endl;
    }
   
    lists J=aLL_LEAD(f_copy);
       
    for(int k=0;k<IDELEMS(f_copy);k++){
 std::cout << "aLL_LEAD input" << k << ": " << pString((poly)f_copy->m[k]) << std::endl;
    }
     int r=lSize(J)+1;
       std::cout << "lSize(J):="<<r<< std::endl;
    // idDelete(&f_copy);
     for(int k=0;k<r;k++){
      ideal l=(ideal)J->m[k].Data();
      for(int s=0; s <IDELEMS(l);s++){
      std::cout << "Generator in all_lead " << k << ": " << pString((poly)l->m[s]) << std::endl;
      }
 
    }

    
    int counter=r;
     lists LLT=(lists)omAlloc0Bin(slists_bin);
    LLT->Init(4);// type token
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);
  std::cout << "size of Sch frame: " << "" << r << std::endl;
    for (int k = 0; k < r; k++) {
       // Create a new token Ld
      lists Ld=(lists)omAlloc0Bin(slists_bin);
      Ld->Init(4);// type token 

      
      lists t=(lists)omAlloc0Bin(slists_bin);
      t->Init(2);
      t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
      t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("SchFrame");
    
      Ld->m[0].rtyp=RING_CMD; Ld->m[0].data=currRing;
      Ld->m[1].rtyp=LIST_CMD; Ld->m[1].data=t;
    
      Ld->m[2].rtyp=RING_CMD; Ld->m[2].data=currRing;

     t=(lists)omAlloc0Bin(slists_bin);
     t->Init(2);
     t->m[0].rtyp=LIST_CMD;  t->m[0].data=lCopy(J);
     t->m[1].rtyp=INT_CMD;  t->m[1].data = (void*)(long)(counter);
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

  LLT->m[3].rtyp = LIST_CMD;
  LLT->m[3].data = t0;
 
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
    final_data->m[r].data = (void*)(long)(counter+1);

    // Set final_data as LLT->m[3] (LLT.data)
    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;




    return {r, LLT};  // Return r (number of generators) and LLT
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
  std::string function_name = "all_leadsyz_GpI";
 auto start_computation = std::chrono::high_resolution_clock::now();

	out = call_user_proc(function_name, needed_library, args);
  auto end_computation = std::chrono::high_resolution_clock::now();
  auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
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
    
 std::cout << "#LeadSyz_GPI:=" <<r<< std::endl;
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
        t->m[1].rtyp = STRING_CMD; t->m[1].data = strdup("Sparse_LeadSyz_matrix");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

        matrix sM = mpNew(r0, c);
        poly s_lift = (poly)LT->m[k]; // Retrieve the lifted polynomial
        int l_k = p_GetComp(s_lift, currRing);

        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);

        MATELEM(sM, l_k, k+1) =pCopy(lm);

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
  //  std::string function_name = "leadsyz_GpI";
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




 std::cout << "total_runtime_LeadSYZ:=" << total_runtime<<" "<<"nanoseconds"<< std::endl;
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
   
    
    poly lm_s = pHead(s); // Get the leading monomial of s including coefficient
   
    pSetComp(lm_s,0);
    pSetmComp(lm_s);
      
    int g1= p_GetComp(s,currRing);
     
  
    g=pp_Mult_qq(lm_s,f->m[g1-1],currRing);//g:=psi(s)
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

 
   
   // number lc_s = leadcoef(s);
    poly lm_s =  pHead(s); //leading monomial including coefficient
   
   
    pSetComp(lm_s,0);
    pSetmComp(lm_s);
    int g1= p_GetComp(s,currRing);

   
        
    
  g = pp_Mult_qq(lm_s, f->m[g1-1],currRing);//g:=psi(s)

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

    // Iterate to fill in data
    for (int k = 0; k < r; k++) {
        // Create a new token Ld
        lists Ld = (lists)omAlloc0Bin(slists_bin);
        Ld->Init(4); // Initialize with 4 fields

        lists t = (lists)omAlloc0Bin(slists_bin);
        t->Init(2);
        t->m[0].rtyp = STRING_CMD; t->m[0].data = strdup("generators");
        t->m[1].rtyp = STRING_CMD; t->m[1].data = strdup("Sparse_matrix_Lift");

        Ld->m[1].rtyp = LIST_CMD; Ld->m[1].data = t;
        Ld->m[0].rtyp = RING_CMD; Ld->m[0].data = currRing;
        Ld->m[2].rtyp = RING_CMD; Ld->m[2].data = currRing;

        matrix sM = mpNew(r0, c);
        poly s_lift = (poly)lL->m[k].Data(); // Retrieve the lifted polynomial
        int l_k = p_GetComp(s_lift, currRing);

        poly lm = pHead(s_lift);
        pSetComp(lm, 0);
        pSetmComp(lm);

        MATELEM(sM, l_k, colmn) = p_Mult_q(pISet(-1), pCopy(lm), currRing);

        // Prepare Ld data
        t = (lists)omAlloc0Bin(slists_bin);
        t->Init(7);
        t->m[0].rtyp = VECTOR_CMD; 
        t->m[0].data = pCopy(p_Mult_q(pISet(-1), s_lift, currRing));
        t->m[1].rtyp = MATRIX_CMD; 
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
  // std::string function_name = "LIFT_GPI";
  //    //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
    // out = call_user_proc(function_name, needed_library, args);
   
   out = LIFT_GPI(args.leftV()); 
  

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
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time =std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
    auto total_runtime=computation_time;
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
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}

















NO_NAME_MANGLING
std::pair<int, lists> reduce_GPI(leftv arg1) {

    // std::cout << "Type of 1.arg:" << arg1->Typ() <<std::endl;
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
    // ideal A0=id_Matrix2Module(mp_Copy(A,currRing),currRing);
    // //smatrix A0=A;
    // ideal B0=id_Matrix2Module(mp_Copy(B,currRing),currRing);
    // Perform the matrix addition using Singular's API function
    // ideal C0 = sm_Add(A0, B0, currRing);
    // idDelete(&A0);idDelete(&B0);
    matrix C=mp_Add(A, B, currRing);
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
      

        // std::cout << "input.reduce:-= " << Gb.second<< std::endl;


    // Prepare arguments
    ScopedLeftv arg1(Gb.first, lCopy(Gb.second));
    ScopedLeftv args(arg1, input.first, lCopy(input.second));
    ScopedLeftv arg(args, lift_syz.first, lCopy(lift_syz.second));
    

    std::string function_name = "reduce_GPI";
    // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();

    // out = call_user_proc(function_name, needed_library, arg1);
    out = reduce_GPI(arg1.leftV());  // Call reduce_GPI with the raw pointer




  //   lists Token1 = (lists)(arg1.leftV()->data);
   
  // int L_si = lSize(Token1)+1;
  // std::cout << "Size of J in the transition: " << L_si << std::endl;
  // for (int i = 0; i < L_si; i++) {
  //         sleftv& listElement = Token1->m[i];  // Access each element as `leftv`
  //     std::cout << "Input: " << listElement.String()  << std::endl;
  // }

  //   lists Token2 = (lists)(args.leftV()->data);
   
  // int L = lSize(Token2)+1;
  // std::cout << "Size of J in the transition: " << L << std::endl;
  // for (int i = 0; i < L; i++) {
  //         sleftv& listElement = Token2->m[i];  // Access each element as `leftv`
  //     std::cout << "LHS: " << listElement.String()  << std::endl;
  // }


  //   lists Token3 = (lists)(arg.leftV()->data);
   
  // int L0 = lSize(Token3)+1;
  // std::cout << "Size of J in the transition: " << L0 << std::endl;
  // for (int i = 0; i < L0; i++) {
  //         sleftv& listElement = Token3->m[i];  // Access each element as `leftv`
  //     std::cout << "RHS: " << listElement.String()  << std::endl;
  // }



    
    out_filename = serialize(out.second, base_filename);
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_computation - start_computation).count();
    auto total_runtime = computation_time;
    
  //  std::cout << " total gen " << total_generator<< std::endl;
// std::cout << " total time " << total_runtime<< std::endl;
//   std::cout << arg1.leftV()->String() <<std::endl;
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