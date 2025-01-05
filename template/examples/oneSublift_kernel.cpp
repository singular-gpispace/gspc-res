
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









// For vectors
lists vector_List(poly f0) 
{
    int r = pLength(f0); 
    std::cout << "vector_LIST= " <<pString(pCopy(f0))<< std::endl;
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
        std::cout << "Entry " << k << " rtyp: " << S->m[k].rtyp << std::endl;

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


poly LOT_2(poly g0, ideal S1, lists J, int level) 
{
    // std::cout << "LOT_g= " << pString(g0) << std::endl;
    // std::cout << "LOT_S1= " << IDELEMS(S1) << std::endl;
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
           poly lm=(poly)F0->m[i];
            bool c = p_DivisibleBy(pHead(F0->m[i]), t, currRing);
            // std::cout << "Divisibility check result: " << c << std::endl;
            std::cout << "Second_LeadSyz:" << pString(lm) << std::endl;
            if (c == TRUE)
             { // Check normal form
                std::cout << "LOT_found _t :" << pString(t) << std::endl;

                LOT = p_Sub(LOT, pCopy(t), currRing);
                break;
            }
        }
        
        g0 = p_Sub(g0, pCopy(t), currRing);
        // std::cout << "LOT_return :" << pString(LOT) << std::endl;
    }

    return LOT;
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

    //std::cout<<"after LOT g=" << pString(g) <<std::endl;
    //std::cout << "lOT_onesubLift: " << pString(lOT) << std::endl;
    //poly T0 = g-LOT(g, f); 
    poly T0=p_Sub(pCopy(g),pCopy(lOT),currRing);

    //std::cout << "Input_pOLY_LIST: " << pString(T0) << std::endl;

    lists T=pOLY_List(pCopy(T0)); 
   
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
      std::cout << "TT is empty, stopping algorithm." << std::endl;
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
            TT->m[k].data=s_v;  //TT[cl]=m*f[lambda]
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
     std::cout << "vector in SubLIFT2:"<<pString(s)<< std::endl;
    // Get the ideal f0 from the list J at the current level
    ideal f0 = (ideal)J->m[level-1].Data();
    
    // Get the leading monomial of s, including the coefficient
    poly lm_s = pHead(s);
    pSetComp(lm_s,0);
    pSetmComp(lm_s);
    // Get the component of the vector s
    int g1 = p_GetComp(s, currRing);

    // Multiply the leading monomial by the corresponding elements in f and f0
    h = pp_Mult_qq(lm_s, f->m[g1-1], currRing);    // h = lm_s * f[g1]
    H = pp_Mult_qq(lm_s, f0->m[g1-1], currRing);   // H = lm_s * f0[g1]

    // Subtract the head of H from h
    g = p_Sub(h, pHead(H), currRing); // g = h - lead(H)
    poly g_copy = pCopy(g);
    // Compute the LOT (List of Terms) of g with respect to f and J at the given level
    poly t0 = LOT_2(g_copy, f, J, level);

    // Subtract the LOT from g to get T0
    poly T0 = p_Sub(g, pCopy(t0), currRing);            // T0 = g - LOT_2(g, f, J, level)

    // Convert T0 into a list of vectors
    lists T = vector_List(T0);

    // Initialize the result list TT with the size of T
    int t_size = lSize(T)+1;                       // Get the size of T
    lists TT = (lists)omAlloc0Bin(slists_bin);   // Allocate memory for TT
    if (t_size==0) 
    {
     TT->Init(0);
      std::cout << "TT is empty, stopping algorithm." << std::endl;
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
                    TT->m[k].data = s_v;

                   // counter++;  // Increment the counter
                    break;
                }
            }
        }
    }

    // Return the result list TT
    return TT;
}

std::pair<int, lists> SubLIFT_GPI(leftv args)
{

  
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
     //std::cout << "SubLIFT_GPI_Counter: " << counter << std::endl;
   //J=tok.data[1]
    lists J=(lists)(tmp1->m[0].Data());
   //std::cout << "SubLIFT_GPI_LIST: " << J << std::endl;
  
    poly s=(poly)tmp2->m[0].Data(); //  vector s = TT.data[1];
    //std::cout << "SubLIFT_GPI_vector: " << s << std::endl;
  
    int r0=0; int c=0;int l_size=0;
    int L_size=lSize(J);
    std::cout << "Size of J: " << L_size << std::endl;
    ideal l=NULL;
    ideal LL=NULL;

  if (tmp->m[0].Typ() == IDEAL_CMD) 
  {
     l =(ideal)J->m[1].Data(); //module l=J[2]
    //std::cout << "SubLIFT_GPI_Module: " << l << std::endl;
     l_size=IDELEMS(l);
    //std::cout << "SubLIFT_GPI_Module_size: " << l_size << std::endl;
     r0=IDELEMS((ideal)(tmp->m[0].Data())); 
     c=l_size;
     c=c;
     r0=r0;


}
else if (tmp->m[0].Typ() == VECTOR_CMD) 
{
    int level=counter;
    l=(ideal)J->m[counter-1].Data(); //module l=J[counter]
    std::cout << "SubLIFT_GPI_Second_Level: " << counter<< std::endl;
    int l_size = IDELEMS(l); 
    l_size=l_size;
    int r0= IDELEMS(l); 
    r0=r0;
    LL=(ideal)J->m[level].Data(); //module LL=J[counter+1]
    int c=IDELEMS(LL);
    c=c;


}
 lists lL=NULL;
 int r=0;
if(tmp->m[0].Typ() == IDEAL_CMD){
   ideal f=(ideal)(tmp->m[0].Data());
   // std::cout << "SubLIFT_GPI_Input: " << f << std::endl;
     //std::cout << "SubLIFT_GPI_Size(f): " << IDELEMS(f) << std::endl;
      lL=oneSublift(f,s); // lists lL=(lists)oneSublift(f,s).Data();
       
       //std::cout << "SubLIFT_GPI_oneSublift: " << IDELEMS(f) << std::endl;
         
         
            // **Early Termination Check**
           // Handle empty output from oneSublift
        

        if (lL == NULL) {
            std::cerr << "Error: oneSublift returned a null list." << std::endl;
            return { 0, NULL };
        }
         r = lSize(lL)+1;
         r=r;
          // std::cout << "SubLIFT_GPI_oneSublift_size: " << r << std::endl;
        } else if(tmp->m[0].Typ() == VECTOR_CMD){
           std::cout << "SubLIFT_GPI_Second_Level_sublift2: " << pString((poly)tmp->m[0].Data())<< std::endl;
           // matrix A[r0][c] =Token.data[2];
           matrix A= (matrix)tmp->m[1].Data(); //A= Tok.data[2]
           bool m0=(tmp->m[1].Typ()==MATRIX_CMD);
             std::cout << "m0_matrix:? " << m0<< std::endl;
            int mc=MATCOLS(A);
             std::cout << "column_Syz_matrix: " << mc<< std::endl;
             int mr=MATROWS(A);
              std::cout << "row_Syz_matrix: " << mr<< std::endl;
           // ideal M = id_Copy((matrix)A);//module M=A
            ideal M=id_Matrix2Module(A,currRing);
        
            int level=counter;
          
            lL=subLiftTree2(M,s,J,level);// lists lL=(lists)subLiftTree2(M,s,J,level).Data(); 
             if (lL == NULL) {
            std::cerr << "Error: subLiftTree2 returned a null list." << std::endl;
            return {0, NULL };
        }
            r = lSize(lL)+1;
          
      
    
            r=r;
           
           
          }
        



     
        // Prepare the LLT token
    lists LLT=(lists)omAlloc0Bin(slists_bin);
    LLT->Init(4);// type token
    //std::cout << "SubLIFT_GPI_token: "<< std::endl;
    int k=0;


  for (k = 0; k <r; k++) {
    
      // Create a new token Ld
      lists Ld=(lists)omAlloc0Bin(slists_bin);
      Ld->Init(4);// type token 
      // fieldnames 
      //Ld.fieldnames = list("generator", "Sparse_matrix_Lift"); 

      lists t=(lists)omAlloc0Bin(slists_bin);
      t->Init(2);
      t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
      t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("Sparse_matrix_SubLift");
    
      Ld->m[1].rtyp=LIST_CMD; Ld->m[1].data=t;
      Ld->m[0].rtyp=RING_CMD; Ld->m[0].data=currRing;
      Ld->m[2].rtyp=RING_CMD; Ld->m[2].data=currRing;


     //matrix sM[r0][c];
     matrix sM= mpNew(r0,c);
  
     //std::cout << "SubLIFT_GPI_matrix:row= "<<r0<< "column:"<<c<< std::endl;
     //vector s_lift =lL[k];
     poly s_lift=(poly)lL->m[k].Data(); //vector s_lift=lL[k]
     //std::cout << "SubLIFT_GPI_token_poly: "<<pString(s_lift)<< std::endl;
    poly lm = pHead(s_lift);
    pSetComp(lm,0);
    pSetmComp(lm);
     int l_k = p_GetComp(s_lift, currRing);
    // std::cout << "SubLIFT_GPI_token_comp: "<<l_k<< std::endl;
     //sM[l_k,colmn] =pCopy(pp_Mult_qq(-1, pHead(s_lift), currRing)); 
   
     MATELEM(sM,l_k,colmn)=p_Mult_q(pISet(-1), lm,currRing); //should include -1
     //std::cout << "SubLIFT_GPI_token_poly_sparse: "<<pString(MATELEM(sM,l_k,colmn))<< std::endl;
     //data  Ld.data
     t=(lists)omAlloc0Bin(slists_bin);
     t->Init(7);

     //Ld.data[1]=(-1)*s_lift; 
     //t->m[0].rtyp=VECTOR_CMD;  t->m[0].data=pp_Mult_qq(poly(-1),s_lift, currRing);
     t->m[0].rtyp=VECTOR_CMD;  t->m[0].data=pCopy(p_Mult_q(pISet(-1), pHead(s_lift),currRing));
   
     //Ld.data[2]= sM;
     t->m[1].rtyp=MATRIX_CMD; t->m[1].data=sM;

     //Ld.data[3] = l_k;
     t->m[2].rtyp=INT_CMD;  t->m[2].data = (void*)(long)l_k;
    
    
      // Ld.data[4]=colmn;
     t->m[3].rtyp=INT_CMD;  t->m[3].data = (void*)(long)colmn;
   

    if (tmp->m[0].Typ() == IDEAL_CMD)
     {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[1].Data();//J[2]
          //std::cout << "SubLIFT_GPI_J[2]: "<<IDELEMS((ideal)t->m[4].data)<< std::endl;
     //data  Ld.data
     } 
     else if (tmp->m[0].Typ() == VECTOR_CMD) 
     {
         int level=counter;
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[level].Data(); // Ld.data[5]=J[level+1];
         std::cout << "SubLIFT_GPI_token: "<<IDELEMS((ideal)t->m[4].data)<< std::endl;
     }
   
    // Ld.data[6]=counter;
    t->m[5].rtyp=INT_CMD;  t->m[5].data = (void*)(long)counter;
    
   //Ld.data[7] = counter;
    t->m[6].rtyp=INT_CMD; t->m[6].data = (void*)(long)(counter + 1);
    
     //data  Ld.data
    Ld->m[3].rtyp=LIST_CMD; Ld->m[3].data=t;//data is in postion 4
   
    // Set fieldnames[k] to "generator"
   lists field_names = (lists)omAlloc0Bin(slists_bin);
   field_names->Init(r);
 for (int s = 0; s < r; s++) 
 {
    field_names->m[s].rtyp = STRING_CMD;
    field_names->m[s].data = strdup("generator"); 
   // std::cout << "LLT[k]:generator "<<k<< std::endl;
 }
 // Assign fieldnames to LLT
  LLT->m[1].rtyp = LIST_CMD;
  LLT->m[1].data = field_names;
  
  LLT->m[0].rtyp=RING_CMD; LLT->m[0].data=currRing;
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
  // Assign fieldnames to LLT
  LLT->m[3].rtyp = LIST_CMD;
  LLT->m[3].data = t0;

   
  }
    LLT->m[r].rtyp = STRING_CMD;
    LLT->m[r].data = strdup("total_number_generator");
   // std::cout << "Appending 'total_number_generator' to LLT." << std::endl;

    LLT->m[r + 1].rtyp = INT_CMD;
    LLT->m[r + 1].data = (void*)(long)r;
    //std::cout << "Appending 'r' (" << r << ") to LLT." << std::endl;
 
   
   
     
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
    out = SubLIFT_GPI(args.leftV());  // Call  SubLIFT_GPI with the raw pointer
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
    std::cout << "SubLIFT_Runtime: " << computation_time << " milliseconds" << std::endl;
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    // Check if out.second is valid and non-null

     std::vector<std::string> vec;
     int total_generator;
    // Extract list from the output
    lists u=NULL;
    u = (lists)out.second->m[3].Data();
    //std::cout<<"m[3]:"<< out.second->m[3].Data()<< std::endl;
    //std::cout<<"ListOutside:"<<lSize(u)<< std::endl;
    if (u==NULL)
     {
    std::cerr << "Error: SubLIFT_GPI returned a null list." << std::endl;
    return {{}, 0, 0 }; // Early exit with default values
    }
    
   //auto start_serialize = std::chrono::high_resolution_clock::now();


    // Iterate through each element of the list
    for (int i = 0; i < lSize(u)+1; i++)
     {
        auto  Outname=serialize((lists)u->m[i].Data(), base_filename);
    
      // std::cout<<"serialized:"<<Outname<< std::endl;
        vec.push_back(Outname);
    }

    
  // End timing for serialization
   //auto end_serialize = std::chrono::high_resolution_clock::now();
   //auto  serialization_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_serialize - start_serialize).count(); // Convert to milliseconds
   //auto total_runtime=computation_time_deserialize+computation_time+serialization_time;

   auto total_runtime=computation_time;
   total_generator = lSize(u)+1; // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}


// some changes


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









// For vectors
lists vector_List(poly f0) 
{
    int r = pLength(f0); 
    std::cout << "vector_LIST= " <<pString(pCopy(f0))<< std::endl;
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
        std::cout << "Entry " << k << " rtyp: " << S->m[k].rtyp << std::endl;

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


poly LOT_2(poly g0, ideal S1, lists J, int level) 
{
    // std::cout << "LOT_g= " << pString(g0) << std::endl;
    std::cout << "Size of FirstSyz= " << IDELEMS(S1) << std::endl;
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

    std::cout << "g-LOT: " << pString(T0) << std::endl;

    lists T=pOLY_List(pCopy(T0)); 
   
    std::cout << "Input_pOLY_LIST: "<< std::endl;
    for(int k=0; k<lSize(T)+1; k++){
        poly lm=(poly)T->m[k].Data();
        std::cout << "polY_List:size" << lSize(T)+1
        <<"pOLY_LIST;"<<pString(lm) << std::endl;

    }

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
      std::cout << "TT is empty, stopping algorithm." << std::endl;
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
            TT->m[k].data=s_v;  //TT[cl]=m*f[lambda]
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
    std::cout << "vector in SubLIFT2:"<<pString(s)<< std::endl;
    // Get the ideal f0 from the list J at the current level
    ideal f0 = (ideal)J->m[level-1].Data();
    
    // Get the leading monomial of s, including the coefficient
    poly lm_s = pHead(s);
    pSetComp(lm_s,0);
    pSetmComp(lm_s);
    // Get the component of the vector s
    int g1 = p_GetComp(s, currRing);

    // Multiply the leading monomial by the corresponding elements in f and f0
    h = pp_Mult_qq(lm_s, f->m[g1-1], currRing);    // h = lm_s * f[g1]
    H = pp_Mult_qq(lm_s, f0->m[g1-1], currRing);   // H = lm_s * f0[g1]

    // Subtract the head of H from h
    g = p_Sub(h, pHead(H), currRing); // g = h - lead(H)
    poly g_copy = pCopy(g);
    // Compute the LOT (List of Terms) of g with respect to f and J at the given level
    poly t0 = LOT_2(g_copy, f, J, level);

    // Subtract the LOT from g to get T0
    poly T0 = p_Sub(g, pCopy(t0), currRing);            // T0 = g - LOT_2(g, f, J, level)

    // Convert T0 into a list of vectors
    lists T = vector_List(T0);


     std::cout << "Input_Vector_LIST: "<< std::endl;
    for(int k=0; k<lSize(T)+1; k++){
        poly lm=(poly)T->m[k].Data();
        std::cout << "vector_List:size" << lSize(T)+1
        <<"pOLY_LIST;"<<pString(lm) << std::endl;

    }

    // Initialize the result list TT with the size of T
    int t_size = lSize(T)+1;                       // Get the size of T
    lists TT = (lists)omAlloc0Bin(slists_bin);   // Allocate memory for TT
    if (t_size==0) 
    {
     TT->Init(0);
      std::cout << "TT is empty, stopping algorithm." << std::endl;
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
                    TT->m[k].data = s_v;
                    std::cout << "vector s_v in SubLIFT:2"<<pString(s_v)<< std::endl;
                   // counter++;  // Increment the counter
                    break;
                }
            }
        }
    }

    // Return the result list TT
    return TT;
}

std::pair<int, lists> SubLIFT_GPI(leftv args)
{

  
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
     //std::cout << "SubLIFT_GPI_Counter: " << counter << std::endl;
   //J=tok.data[1]
    lists J=(lists)(tmp1->m[0].Data());
   //std::cout << "SubLIFT_GPI_LIST: " << J << std::endl;
  
    poly s=(poly)tmp2->m[0].Data(); //  vector s = TT.data[1];
    //std::cout << "SubLIFT_GPI_vector: " << s << std::endl;
  
    int r0=0; int c=0;int l_size=0;
    int L_size=lSize(J);
    std::cout << "Size of J: " << L_size << std::endl;
    ideal l=NULL;
    ideal LL=NULL;

  if (tmp->m[0].Typ() == IDEAL_CMD) 
  {
     l =(ideal)J->m[1].Data(); //module l=J[2]
    //std::cout << "SubLIFT_GPI_Module: " << l << std::endl;
     l_size=IDELEMS(l);
    //std::cout << "SubLIFT_GPI_Module_size: " << l_size << std::endl;
     r0=IDELEMS((ideal)(tmp->m[0].Data())); 
     c=l_size;
     c=c;
     r0=r0;


}
else if (tmp->m[0].Typ() == VECTOR_CMD) 
{
    int level=counter;
    l=(ideal)J->m[counter-1].Data(); //module l=J[counter]
    std::cout << "SubLIFT_GPI_Second_Level: " << counter<< std::endl;
    int l_size = IDELEMS(l); 
    l_size=l_size;
    int r0= IDELEMS(l); 
    r0=r0;
    LL=(ideal)J->m[level].Data(); //module LL=J[counter+1]
    int c=IDELEMS(LL);
    c=c;


}
 //lists lL=NULL;
 lists lL = (lists)omAlloc0Bin(slists_bin); 
 int r=0;
if(tmp->m[0].Typ() == IDEAL_CMD){
   ideal f=(ideal)(tmp->m[0].Data());
   // std::cout << "SubLIFT_GPI_Input: " << f << std::endl;
     //std::cout << "SubLIFT_GPI_Size(f): " << IDELEMS(f) << std::endl;
     lL=oneSublift(f,s); 
     
       
       //std::cout << "SubLIFT_GPI_oneSublift: " << IDELEMS(f) << std::endl;
         
         
            // **Early Termination Check**
           // Handle empty output from oneSublift
        

        // if (lSize(lL) ==0) {
        //     std::cerr << "Error: oneSublift returned a null list." << std::endl;
        //     //return std::make_pair(0, lL);
        //     return {0, lL}; 
        // }
         r = lSize(lL)+1;
         
         //r=r;
         //lL->Init(r); 
          // std::cout << "SubLIFT_GPI_oneSublift_size: " << r << std::endl;
        } else if(tmp->m[0].Typ() == VECTOR_CMD){
           std::cout << "SubLIFT_GPI_Second_Level_sublift2: " << pString((poly)tmp->m[0].Data())<< std::endl;
           // matrix A[r0][c] =Token.data[2];
           matrix A= (matrix)tmp->m[1].Data(); //A= Tok.data[2]
           bool m0=(tmp->m[1].Typ()==MATRIX_CMD);
             std::cout << "m0_matrix:? " << m0<< std::endl;
            int mc=MATCOLS(A);
             std::cout << "column_Syz_matrix: " << mc<< std::endl;
             int mr=MATROWS(A);
              std::cout << "row_Syz_matrix: " << mr<< std::endl;
           // ideal M = id_Copy((matrix)A);//module M=A
            ideal M=id_Matrix2Module(A,currRing);
        
            int level=counter;
          
            lL=subLiftTree2(M,s,J,level);
           
        //      if (lSize(lL) == 0) {
        //     std::cerr << "Error: subLiftTree2 returned a null list." << std::endl;
        //   //return std::make_pair(0, lL); 
        //   return {0, lL}; 
        // }
            r = lSize(lL)+1;
          
           
    
            //r=r;
           
           
          }
        



      std::cout << "SubLIFT_GPI_r:= "<<r<< std::endl;
        // Prepare the LLT token
    lists LLT=(lists)omAlloc0Bin(slists_bin);
    if(r==0){
        std::cout << "SubLIFT_GPI_empty_token: "<< std::endl;
         LLT->Init(0);
        return std::make_pair(0,LLT);
    } else{



 LLT->Init(4);// type token
    //std::cout << "SubLIFT_GPI_token: "<< std::endl;
    int k=0;


  for (k = 0; k <r; k++) {
    
      // Create a new token Ld
      lists Ld=(lists)omAlloc0Bin(slists_bin);
      Ld->Init(4);// type token 
      // fieldnames 
      //Ld.fieldnames = list("generator", "Sparse_matrix_Lift"); 

      lists t=(lists)omAlloc0Bin(slists_bin);
      t->Init(2);
      t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
      t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("Sparse_matrix_SubLift");
    
      Ld->m[1].rtyp=LIST_CMD; Ld->m[1].data=t;
      Ld->m[0].rtyp=RING_CMD; Ld->m[0].data=currRing;
      Ld->m[2].rtyp=RING_CMD; Ld->m[2].data=currRing;


     //matrix sM[r0][c];
     matrix sM= mpNew(r0,c);
  
     //std::cout << "SubLIFT_GPI_matrix:row= "<<r0<< "column:"<<c<< std::endl;
     //vector s_lift =lL[k];
     poly s_lift=(poly)lL->m[k].Data(); //vector s_lift=lL[k]
     //std::cout << "SubLIFT_GPI_token_poly: "<<pString(s_lift)<< std::endl;
    poly lm = pHead(s_lift);
    pSetComp(lm,0);
    pSetmComp(lm);
     int l_k = p_GetComp(s_lift, currRing);
    // std::cout << "SubLIFT_GPI_token_comp: "<<l_k<< std::endl;
     //sM[l_k,colmn] =pCopy(pp_Mult_qq(-1, pHead(s_lift), currRing)); 
   
     MATELEM(sM,l_k,colmn)=p_Mult_q(pISet(-1), lm,currRing); //should include -1
     //std::cout << "SubLIFT_GPI_token_poly_sparse: "<<pString(MATELEM(sM,l_k,colmn))<< std::endl;
     //data  Ld.data
     t=(lists)omAlloc0Bin(slists_bin);
     t->Init(7);

     //Ld.data[1]=(-1)*s_lift; 
     //t->m[0].rtyp=VECTOR_CMD;  t->m[0].data=pp_Mult_qq(poly(-1),s_lift, currRing);
     t->m[0].rtyp=VECTOR_CMD;  t->m[0].data=pCopy(p_Mult_q(pISet(-1), pHead(s_lift),currRing));
   
     //Ld.data[2]= sM;
     t->m[1].rtyp=MATRIX_CMD; t->m[1].data=sM;

     //Ld.data[3] = l_k;
     t->m[2].rtyp=INT_CMD;  t->m[2].data = (void*)(long)l_k;
    
    
      // Ld.data[4]=colmn;
     t->m[3].rtyp=INT_CMD;  t->m[3].data = (void*)(long)colmn;
   

    if (tmp->m[0].Typ() == IDEAL_CMD)
     {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[1].Data();//J[2]
          //std::cout << "SubLIFT_GPI_J[2]: "<<IDELEMS((ideal)t->m[4].data)<< std::endl;
     //data  Ld.data
     } 
     else if (tmp->m[0].Typ() == VECTOR_CMD) 
     {
         int level=counter;
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[level].Data(); // Ld.data[5]=J[level+1];
         std::cout << "SubLIFT_GPI_token: "<<IDELEMS((ideal)t->m[4].data)<< std::endl;
     }
   
    // Ld.data[6]=counter;
    t->m[5].rtyp=INT_CMD;  t->m[5].data = (void*)(long)counter;
    
   //Ld.data[7] = counter;
    t->m[6].rtyp=INT_CMD; t->m[6].data = (void*)(long)(counter + 1);
    
     //data  Ld.data
    Ld->m[3].rtyp=LIST_CMD; Ld->m[3].data=t;//data is in postion 4
   
    // Set fieldnames[k] to "generator"
   lists field_names = (lists)omAlloc0Bin(slists_bin);
   field_names->Init(r);
 for (int s = 0; s < r; s++) 
 {
    field_names->m[s].rtyp = STRING_CMD;
    field_names->m[s].data = strdup("generator"); 
   // std::cout << "LLT[k]:generator "<<k<< std::endl;
 }
 // Assign fieldnames to LLT
  LLT->m[1].rtyp = LIST_CMD;
  LLT->m[1].data = field_names;
  
  LLT->m[0].rtyp=RING_CMD; LLT->m[0].data=currRing;
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
  // Assign fieldnames to LLT
  LLT->m[3].rtyp = LIST_CMD;
  LLT->m[3].data = t0;

   
  }
    LLT->m[r].rtyp = STRING_CMD;
    LLT->m[r].data = strdup("total_number_generator");
   // std::cout << "Appending 'total_number_generator' to LLT." << std::endl;

    LLT->m[r + 1].rtyp = INT_CMD;
    LLT->m[r + 1].data = (void*)(long)r;
    //std::cout << "Appending 'r' (" << r << ") to LLT." << std::endl;
 
   
   
     
     return {r, LLT};  
  }



        
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
    out = SubLIFT_GPI(args.leftV());  // Call  SubLIFT_GPI with the raw pointer
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
    std::cout << "SubLIFT_Runtime: " << computation_time << " milliseconds" << std::endl;
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    // Check if out.second is valid and non-null

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
    std::cerr << "Error: SubLIFT_GPI returned a null list." << std::endl;
   ///return std::make_tuple(std::vector<std::string>(), 0, 0);// Early exit with default values
   return{{},0,0};
    } 
    
       std::cerr << "Size of SubLIFT Token:" << lSize(u)+1<<std::endl;
    // Iterate through each element of the list
      for (int i = 0; i < lSize(u)+1; i++)
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
   total_generator = lSize(u)+1; // Assuming u contains the computed generators

  return {vec, total_generator, total_runtime};

}
