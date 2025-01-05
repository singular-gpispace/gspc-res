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
lists pOLY_List(poly f0) {
    int r = pLength(f0);  // Get the number of elements in f0
    int k = 0;

    // Allocate and initialize a list S with r elements
    slists* S = (slists*)omAlloc0Bin(slists_bin);
    S->Init(r);

    // Iterate over the elements of f0 and add the leading monomial of each to S
    for (k = 0; k < r; k++) {
       S->m[k].rtyp =POLY_CMD;
       S->m[k].data= pHead(f0);  // Add the leading monomial of f0 to S
       f0 = pNext(f0); 
        
    }

   


     // Allocate memory for the final result list
    slists* res = (slists*)omAllocBin(slists_bin);
    res->Init(r);  // Initialize the result list with cl elements
   int i=0;
    // Copy S contents to res
    for (i = 0; i < r; i++) {
        res->m[i] = S->m[i];
    }

    // Free S
    omFreeBin(S, slists_bin);

    return res;
}
//for vectors
lists vector_List(poly f0) { //poly f0 is singular vector
    int r = pLength(f0); 
    lists S = (lists)omAlloc0Bin(slists_bin); // Allocate memory for the list
    S->Init(r); // Initialize the list to hold r elements

    int cc = 0; // Counter for the elements added to S

    for (int i = 0; i < r; i++) { // Loop through each element of the vector f0
        poly term = pCopy(f0); // Start with the current polynomial

        while (term != NULL) { // Traverse through each monomial in the polynomial
            poly term_copy = p_Copy(pHead(term), currRing); // Copy the leading monomial

            if (term_copy != NULL) {
                S->m[cc].rtyp = VECTOR_CMD; // Set the type of the entry
                S->m[cc].data = term_copy; // Store the copied term in the list
                cc++; // Increment the counter
            }

            term = pNext(term); // Move to the next monomial
        }

        f0 = pNext(f0); // Move to the next polynomial in the vector
    }

    // Adjust the list size to the number of elements added
    S->Init(cc); // Initialize the list with the count of elements added
    return S;
}








poly LOT(poly g0,ideal S1 ) {
  
    
    poly t = NULL;
    int i = 0;
    poly LOT = NULL;

    while (g0 != 0) {
        for (i = 0; i < IDELEMS(S1) ; i++) {
            t = pHead(g0); // Leading term of g0
           ideal Gb_t=idInit(1,1);
           Gb_t->m[0]=t;
            if (kNF(Gb_t, NULL,pHead(S1->m[i])) == NULL) { // Check normal form
            // Accumulate LOT
            LOT= p_Add_q(LOT,t,currRing);
            //  LOT  = LOT + t; 
                break;
            }
        }
        //g0 = g0 - t; // Reduce g0
       g0= p_Sub(g0,t, currRing);
    }
   return LOT;
}

//returns singular vector
poly LOT_2(poly g0, ideal S1, lists J, int level) {
    // Convert g0 to a list of polynomials (terms)
    lists termList = vector_List(g0);
    
    poly LOT = NULL;  // Resulting polynomial to hold non-divisible terms

    int r = IDELEMS(S1);  // Get the number of generators in F0
    ideal F0 = (ideal)J->m[level].Data(); // Get the module F0 from J[level]

    int termCount = lSize(termList);  // Get the number of terms in termList
   ideal Gb_t=NULL;
    // Iterate over each term in termList
    for (int i = 0; i < termCount; i++) {
        poly term = (poly)termList->m[i].Data();  // Get the i-th term from the list
        bool isDivisible = false;

        // Check divisibility of the term against each leading term in F0
        for (int j = 0; j < r; j++) {
            poly leadingTermF0 = pHead(F0->m[j]);  // Get the leading term of F0[j]

            Gb_t = idInit(1, 1); // Initialize a new ideal Gb_t
            Gb_t->m[0] = term; // Set the polynomial term in Gb_t

            // Check if the term is divisible by the leading term of F0[j]
            if ((Gb_t!=NULL)&& kNF(Gb_t, NULL, leadingTermF0) == NULL) {
                isDivisible = true;  // Mark as divisible
                omFreeBin(Gb_t, ideal_bin); // Free memory for Gb_t
                break;  // No need to check further generators
            }
                 omFreeBin(Gb_t, ideal_bin); 
          
        }

        // If the term is not divisible by any generator, add it to LOT
        if (!isDivisible) {
          
           // poly termCopy = p_Copy(term, currRing);  // Copy the term
            LOT = p_Add_q(LOT, term, currRing);  // Add the term to LOT
        }
    }

    // Free memory allocated for termList
    
    omFreeBin(termList, slists_bin);
    

    return LOT;  // Return the list of non-divisible terms
}








lists oneSublift(ideal f, poly s) { //poly s is singular vector
    
    int r = IDELEMS(f);
    poly h = NULL;
    poly g = NULL;

    int i = 0;
    int k = 0;
    int q = 0;
   

    poly lm_s = pHead(s); // Get the leading monomial of s including coefficient
    int g1= p_GetComp(s,currRing);
     
    for (i = 1; i <= r; i++) {
    if (g1== i) {
   h=pp_Mult_qq(lm_s,f->m[g1],currRing);
      break;
    }
  }
     
    

    //g = h - pHead(h); // Leading term of h
    g=p_Sub(h,pHead(h),currRing);
    
    poly T0=LOT(g, f);//poly T0 = (poly)LOT(g, f)->Data(); 

    lists T=pOLY_List(T0);  //lists T = (lists)pOLY_List(T0)->Data(); 

    

    //poly s_hat = pCopy(s); // vector s_hat=s
    
    poly m1 = NULL;
    poly m = NULL;
    poly s_v = NULL;//vector s_v
    int C = 0;
    int cl = 0;
    int t_size = lSize(T);

    lists TT = (lists)omAlloc0Bin(slists_bin); // Allocate memory for list TT
    TT->Init(t_size); // Initialize TT with size t_size
     


    for (k = 0; k < t_size; k++) {
        poly t = (poly)T->m[k].Data();// poly t=T[k]

        for (q = 0; q < r; q++) {
           
           ideal Gb_t=idInit(1,1);
           Gb_t->m[0]=t;
           C = (kNF(Gb_t,NULL, pHead(f->m[q])) == NULL); // Use -> to access Data


            if (C == 1) {
                // Perform polynomial division
                //m1 = p_PolyDiv(t, lead_terms[q], currRing); // Multivariate division
                m1=pp_Divide(t,pHead(f->m[q]), currRing);
                 
                    m = pCopy(m1);//m=m1;
                   p_SetComp(m,lambda,currRing);
                   p_SetmComp(m,currRing);
                   s_v=m;


             
               
                 
                  // Store the result in TT
                TT->m[cl].rtyp = VECTOR_CMD;
                TT->m[cl].data=s_v;//TT[cl]=m*f[lambda]
                cl++;
                break;
            }
        }
    }
  
 // Allocate memory for the final result list
    slists* res = (slists*)omAllocBin(slists_bin);
    res->Init(cl);  // Initialize the result list with cl elements

    // Copy TT contents to res
    for (i = 0; i < cl; i++) {
        res->m[i] = TT->m[i];
    }

    // Free TT if no longer needed
    omFreeBin(TT, slists_bin);

    return res;
}




lists subLiftTree2(ideal f, poly s, lists J, int level) {//poly s is singular vector  
    //ideal f is singular module
    int r = IDELEMS(f);
    poly g=NULL;  //vector g
    poly h=NULL;  //vector h
    poly H=NULL; //vector H
    int i=0;
  
    //module f0 = J[level];
      ideal f0=(ideal)J->m[level].Data();//.data?
   
   // number lc_s = leadcoef(s);
    poly lm_s =  pHead(s); //leading monomial including coefficient

    int g1= p_GetComp(s,currRing);


        
    for (i = 1; i <= r; i++) {
    if (g1== i) {
        h = pp_Mult_qq(lm_s, f->m[g1],currRing);
        H=pp_Mult_qq(lm_s, f0->m[g1],currRing);
      break;
    }
  }
     
  

    g = p_Sub(h, pHead(H),currRing); // g = h - lead(H)


// vector t0=LOT_2(g,f,J,level)
    poly t0=LOT_2(g,f,J,level);//poly t0 = (poly)LOT_2(g, f, J, level)->next; 


    
    poly T0 = p_Sub(g, t0,currRing);//vector T0=g - LOT_2(g, f, J, level)

   
      lists T = vector_List(T0); //lists T = (lists)vector_List(T0)->next;

    
    int t_size = lSize(T);//??
    lists TT = (lists)omAlloc0Bin(slists_bin);
    TT->Init(t_size);

    
    //poly s_hat = pCopy(s); // vector s_hat:=s
    
    //poly s_v=NULL; //vector s_v
     poly m=NULL;
     poly m1=NULL;
    int lambda=0;
    int counter = 0;
    int k=0;
    for (k = 0; k < lSize(T); k++) {
        poly t = (poly)T->m[k].Data(); //vector t=T[k]
        for (lambda = 0; lambda <= r-1; lambda++) {
            int a = p_GetComp(t,currRing);//?
            int b = p_GetComp(f0->m[lambda],currRing);//p_Head for leadsxp
           ideal Gb_t=idInit(1,1);
           Gb_t->m[0]=t;
            if (a == b) {
                int C = (kNF(Gb_t,NULL, pHead(f0->m[lambda])) == 0);
                if (C == 1) {
                    // Compute m1 and update TT   
                    
                    m1 = pp_Divide(pHead(t), pHead(f0->m[lambda]), currRing);
                     m = pCopy(m1);//m=m1;
                   p_SetComp(m,lambda,currRing);
                   p_SetmComp(m,currRing);
                   s_v=m;
                   // s_v = pp_Mult_qq(m1, poly(lambda),currRing);
               
                  
                      // Store the result in TT
                  TT->m[counter].rtyp =VECTOR_CMD;
                  TT->m[counter].data= s_v;//TT[cl]=m*f[lambda]
              
                    counter++;
                    break;
                }
            }
        }
    }

      
 // Allocate memory for the final result list
    slists* res = (slists*)omAllocBin(slists_bin);
    res->Init(counter);  // Initialize the result list with cl elements

    // Copy TT contents to res
    for (i = 0; i < counter; i++) {
        res->m[i] = TT->m[i];
    }

    // Free TT if no longer needed
    omFreeBin(TT, slists_bin);

    return res;
}

std::pair<int, lists> SubLIFT_GPI(leftv args){
  //extract Token
  lists Token = (lists)(args->Data()); 
// extract tok
  lists tok =  (lists)(args->next->Data()); 
//extract TT
  lists TT = (lists)(args->next->next->Data()); 
  //Token.data
  lists tmp = (lists)(Token->m[3].Data());
  //tok.data
  lists tmp1 = (lists)(tok->m[3].Data());
  //TT.data
   lists tmp2 = (lists)(TT->m[3].Data());
   //TT.data[6]
  int counter=(int)(long)tmp2->m[5].Data();
  //column
   int colmn = (int)(long)tmp2->m[3].Data(); 

   //J=tok.data[1]
   lists J=(lists)(tok->m[0].Data());

  
  poly s=(poly)tmp2->m[0].Data(); //  vector s = TT.data[1];

  //leftv L = nullptr; leftv LL = nullptr;
   int r0=0; int c=0;int l_size=0;
    //def f = Token.data[1]; 
  if (tmp->m[0].Typ() == IDEAL_CMD) {
    ideal l=(ideal)J->m[1].Data(); //module l=J[2]
    l_size=IDELEMS(l);
    r0= IDELEMS((ideal)(tmp->m[0].Data())); 
    c=l_size;


}
else if (tmp->m[0].Typ() == VECTOR_CMD) {
  int level=counter;
  ideal l=(ideal)J->m[counter].Data(); //module l=J[counter]
    
    int l_size = IDELEMS(l); 
    l_size=l_size;
    int r0= IDELEMS(l); 
     r0=r0;
     ideal LL=(ideal)J->m[level+1].Data(); //module LL=J[counter+1]
     int c=IDELEMS(LL);
     c=c;


}
 lists lL=NULL;
 int r=0;
if(tmp->m[0].Typ() == IDEAL_CMD){
   ideal f=(ideal)(tmp->m[0].Data());
        lists lL=oneSublift(f,s); // lists lL=(lists)oneSublift(f,s).Data();
          r = lSize(lL);
          r=r;
        } else if(tmp->m[0].Typ() == VECTOR_CMD){
           // matrix A[r0][c] =Token.data[2];
           matrix A;
            A = (matrix)tmp1->m[1].Data(); //A= Tok.data[2]
           // ideal M = id_Copy((matrix)A);//module M=A
            ideal M=id_Matrix2Module(A,currRing);
            int level=counter;
          
           lists lL=subLiftTree2(M,s,J,level);// lists lL=(lists)subLiftTree2(M,s,J,level).Data(); 
            r = lSize(lL);
            r=r;
           
          }
        
        // Prepare the LLT token
    lists LLT=(lists)omAlloc0Bin(slists_bin);
    LLT->Init(4);// type token
    
    int k=0;


  for (k = 0; k <r; k++) {
      // Create a new token Ld
      lists Ld=(lists)omAlloc0Bin(slists_bin);
     Ld->Init(4);// type token 
      // fieldnames //Ld.fieldnames = list("generator", "Sparse_matrix_Lift"); 
    lists t=(lists)omAlloc0Bin(slists_bin);
    t->Init(2);
     t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
    t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("Sparse_matrix_SubLift");
     
     Ld->m[1].rtyp=LIST_CMD; Ld->m[1].data=t;
     Ld->m[0].rtyp=RING_CMD; Ld->m[0].data=currRing;
     Ld->m[2].rtyp=RING_CMD; Ld->m[2].data=currRing;


    //matrix sM[r0][c];
    matrix sM= mpNew(r0,c);
  //sM = (matrix)omAllocBin(smatrix_bin); // Allocate memory for the matrix

    //vector s_lift =lL[k];//added c_n
    poly s_lift=(poly)lL->m[k].Data(); //vector s_lift=lL[k]
     
    //int l_k = p_Head(s_lift)[nvars(basering) + 1];// p_Head for leadexp 
    int l_k = p_GetComp(s_lift, currRing);
  
    //sM[l_k,colmn] =pCopy(pp_Mult_qq(-1, pHead(s_lift), currRing)); 
    MATELEM(sM,l_k,colmn)=pCopy(pp_Mult_qq(poly(-1),pHead(s_lift), currRing)); //should include -1
    
     //data  Ld.data
    t=(lists)omAlloc0Bin(slists_bin);
    t->Init(7);

    //Ld.data[1]=(-1)*s_lift; 
    t->m[0].rtyp=VECTOR_CMD;  t->m[0].data=pp_Mult_qq(poly(-1),s_lift, currRing);//should include -1
    

    //Ld.data[2]= sM;
     t->m[1].rtyp=MATRIX_CMD; t->m[1].data=sM;

     //Ld.data[3] = l_k;
    t->m[2].rtyp=INT_CMD;  t->m[2].data = (void*)(long)l_k;
    
    
      // Ld.data[4]=colmn;
    t->m[3].rtyp=INT_CMD;  t->m[3].data = (void*)(long)colmn;
   

    if (tmp->m[0].Typ() == IDEAL_CMD) {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[1].Data();//J[2]

     } 
     else if (tmp->m[0].Typ() == VECTOR_CMD) {
      int level=counter;
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[level].Data(); // Ld.data[5]=J[level+1];
     }
   
   // Ld.data[6]=counter;
    t->m[5].rtyp=INT_CMD;  t->m[5].data = (void*)(long)counter;
//Ld.data[7] = counter;
   t->m[6].rtyp=INT_CMD; t->m[6].data = (void*)(long)(counter + 1);
   Ld->m[3].rtyp=LIST_CMD; Ld->m[3].data=t;//data is in postion 4
     // fieldnames in token LLT
  // Set field names for LLT
lists fieldnames = (lists)omAlloc0Bin(slists_bin);
fieldnames->Init(r);
for (int s = 0; s < r; s++) {
    fieldnames->m[s].rtyp = STRING_CMD;
    fieldnames->m[s].data = strdup("generator");  // Set fieldnames[k] to "generator"
}
 // Assign fieldnames to LLT
LLT->m[1].rtyp = LIST_CMD;
LLT->m[1].data = fieldnames;

LLT->m[0].rtyp=RING_CMD; LLT->m[0].data=currRing;
LLT->m[2].rtyp=RING_CMD; LLT->m[2].data=currRing;

     //LLT.data[k] = Ld; 
     
    
   // Set data for LLT
lists t0 = (lists)omAlloc0Bin(slists_bin);
t0->Init(r);
for (int s = 0; s < r; s++) {
    t0->m[s].rtyp = STRING_CMD;
    t0->m[s].data = Ld;  // Set fieldnames[k] to "generator"
}
 // Assign fieldnames to LLT
LLT->m[3].rtyp = LIST_CMD;
LLT->m[3].data = t0;

   
  }
     //int cmd_nr;
     //blackboxIsCmd("token",cmd_nr); // get the type id of "token"
     
     return {r,LLT};
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
    
    //std::cout<<"function_name_LIFT:"<<function_name<< std:: endl;
     // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
    out = SubLIFT_GPI(args.leftV());  // Call reduce_GPI with the raw pointer
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