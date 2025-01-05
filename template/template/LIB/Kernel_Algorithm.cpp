
lists pOLY_List(leftv u) {
    poly f0 = (poly)u->Data(); // Extract polynomial f0 from u
    int r = pLength(f0); // Get the number of elements in f0
    int k = 0;

    lists S = (lists)omAlloc0Bin(slists_bin);
    S->Init(r); // Initialize list S with size r
    for (k = 0; k <= r-1; k++) {
        S->m[k] = pHead(f0); // Add leading monomial of f0 to S
        f0 = pNext(f0); // Move to the next polynomial
    }

    lists res = (lists)omAllocBin(sleftv_bin); // Allocate memory for result
    res->rtyp = LIST_CMD; // Set the type of the result
    res->Data() = (void *)S;
    return res;
}


poly LOT(leftv u) {
    poly g0 = (poly)u->Data(); 
    ideal S1 = (ideal)u->next->Data();
    
    poly t = NULL;
    int i = 0;
    poly LOT = NULL;

    while (g0 != 0) {
        for (i = 0; i <= IDELEMS(S1) - 1; i++) {
            t = pHead(g0); // Leading term of g0

            if (kNF(t, pHead(S1[i])) == NULL) { // Check normal form
            // Accumulate LOT
            LOT= p_Add_q(LOT,t,currRing);
            //  LOT  = LOT + t; 
                break;
            }
        }
        //g0 = g0 - t; // Reduce g0
       g0= pp_Sub_qq(g0,t, currRing);
    }
    poly res = (poly)omAllocBin(sleftv_bin); // Allocate memory for result
    res->rtyp = POLY_CMD; // Set result type
    res->Data() = (void *)LOT;
    return res;
}



lists oneSublift(leftv res, leftv u) {
    ideal f = (ideal)u->Data(); // Extract ideal f from u
    vector s = (vector)u->next->Data(); // Extract vector s from v
    
    int r = IDELEMS(f);
    poly h = NULL;
    poly g = NULL;

    int i = 0;
    int k = 0;
    int q = 0;
    int i0 = 0;

    poly lm_s = pHead(s); // Get the leading monomial of s
    poly e_i = NULL;

    for (int i = 0; i < IDELEMS(f); i++) {
        e_i = p_One(currRing); // Initialize e_i as the monomial 1
        p_SetComp(e_i, i + 1, currRing); // Set e_i to the i-th position

        poly tempPoly = pp_Mul_qq(lm_s, e_i, currRing); // Multiply m by e_i

        // Check if tempPoly equals the vector s
        if (p_EqualPolys(tempPoly, s, currRing)) {
            h = pp_Mul_qq(lm_s, f[i], currRing); // Compute h = m * f[i]
            p_Delete(&tempPoly, currRing); // Clean up temporary polynomial
            p_Delete(&e_i, currRing); // Clean up unit vector
            break; // Exit loop once found
        }
        
        p_Delete(&tempPoly, currRing); // Clean up temporary polynomial if not needed
        p_Delete(&e_i, currRing); // Clean up unit vector if not needed
    }

    g = h - pHead(h); // Leading term of h
    lists T0 = (lists)LOT(g, f)->Data(); // Compute LOT

    lists T = (lists)pOLY_List(T0)->Data(); // Convert T0 into a list

    // Precompute leading terms of f
    lists lead_terms = (lists)omAlloc0Bin(slists_bin);
    lead_terms->Init(r);
    for (i0 = 0; i0 <= r - 1; i0++) {
        lead_terms[i0] = pHead(f[i0]);
    }

    poly s_hat = pCopy(s); // Copy s
    poly t = NULL, m1 = NULL;
    poly m = NULL;
    vector s_v = NULL;
    int C = 0;
    int cl = 0;
    int t_size = IDELEMS(T);

    lists TT = (lists)omAlloc0Bin(slists_bin); // Allocate memory for list TT
    TT->Init(t_size); // Initialize TT with size t_size

    for (k = 0; k <= t_size - 1; k++) {
        t = T[k];

        for (q = 0; q <= r - 1; q++) {
            pSetComp(h1, q); // Set component of h1

            C = (kNF(t, lead_terms[q]) == NULL); // Check if normal form is NULL

            if (C == 1) {
                // Perform polynomial division
                //m1 = p_PolyDiv(t, lead_terms[q], currRing); // Multivariate division
                m1= p_MDivide(t,lead_term[q], currRing);
                s_v = pp_Mul_qq(m1, h1, currRing);


                m = pCopy(m1);
                TT[cl] = p_Mul_qq(m, h1, currRing);
                cl++;
                break;
            }
        }
    }
   // Clean up allocated memory
    p_Delete(&h, currRing);
    p_Delete(&h1, currRing);
    for (i0 = 0; i0 <= r - 1; i0++) {
        p_Delete(&lead_terms[i0], currRing);
    }
    omFreeBin(lead_terms);

    for (k = 0; k < t_size; k++) {
        p_Delete(&TT[k], currRing);
    }
    omFreeBin(TT);

    res->rtyp = LIST_CMD;
    res->Data() = (void *)TT;
    
    return res;
}




lists subLiftTree2(module f, vector s, lists J, int level) {
    int r = size(f);
    vector g, h, H;
    lists T;
    ideal I_Gr = (ideal)J->m[1].Data(); // Extract ideal from J
    module f0 = (module)J->m[level].Data(); // Extract module from J

    // Extract leading coefficient and monomial
    number lc_s = leadcoef(s);
    poly lm_s = leadmonomial(s);

    // Finding h and H based on conditions
    for (int i = 1; i <= r; i++) {
        if (p_EqualPolys(s, pp_Mul_qq(lc_s, lm_s, gen(i)), currRing)) {
            h = pp_Mul_qq(lc_s, lm_s, f[i]);
            H = pp_Mul_qq(lc_s, lm_s, f0[i]);
            break;
        }
    }

    g = p_Sub_qq(h, lead(H)); // g = h - lead(H)

    // Compute T0 as g - LOT_2(g, f, J, level)
    vector T0 = p_Sub_qq(g, LOT_2(g, f, J, level));
    T = vector_List(T0); // Convert vector T0 to list T

    // Initialize list TT
    int t_size = size(T);
    lists TT = (lists)omAlloc0Bin(slists_bin);
    TT->Init(t_size);

    // Initialize s_hat to s
    vector s_hat = pCopy(s);

    int counter = 1;
    for (int k = 1; k <= size(T); k++) {
        vector t = T[k];
        for (int lambda = 1; lambda <= r; lambda++) {
            int a = leadexp(t)[nvars(basering) + 1];
            int b = leadexp(f0[lambda])[nvars(basering) + 1];

            if (a == b) {
                int C = (NF(t, lead(f0[lambda]), 1) == 0);
                if (C == 1) {
                    // Compute m1 and update TT
                    poly m1 = p_Div_qq(p_Mul_qq(leadcoef(t), leadmonomial(t)), p_Mul_qq(leadcoef(f0[lambda]), leadmonomial(f0[lambda])), currRing);
                    vector s_v = p_Mul_qq(m1, gen(lambda));
                    poly m = p_Mul_qq(m1, gen(lambda));
                    TT->m[counter].rtyp = VECTOR_CMD;
                    TT->m[counter].data = s_v;
                    counter++;
                    break;
                }
            }
        }
    }

    // Clean up allocated memory if necessary
    for (int i = 0; i < t_size; i++) {
        p_Delete(&T[i], currRing);
    }
    omFreeBin(T0);

    return TT;
}





int success = iiAddCproc("template.lib", "oneSublift", FALSE, oneSublift);

// Check if the registration was successful
if (success) {
    printf("oneSublift has been successfully registered.\n");
} else {
    printf("Failed to register oneSublift.\n");
}















std::pair<int, lists> SubLIFT_GPI(leftv arg1){
  lists Token = (lists)(arg1->Data()); //extract Token
  lists tok =  (lists)(arg1->next->Data()); // extract tok
  lists TT = (lists)(arg1->next->next->Data()); //extract TT
  lists tmp = (lists)(Token->m[3].Data());//Token.data
  lists tmp1 = (lists)(tok->m[3].Data());//Token.data
   lists tmp2 = (lists)(TT->m[3].Data());//TT.data
  int counter=(int)(long)tmp2->m[5].Data();//TT.data[6]
   int colmn = (int)(long)tmp2->m[3].Data(); //column
   lists J=(lists)(tok->m[0].Data());//J=tok.data[1]
  vector s=pCopy(tmp2->m[0].Data()); //  vector s = TT.data[1];

  leftv L = nullptr; leftv LL = nullptr;
   int r0=0; int c=0;int l_size=0;
   //f=tmp->m[0]
  if (tmp->m[0].Typ() == IDEAL_CMD) {
    auto l=leadSyz(f);// Should Implement in C++
    l_size=sizeof(l);
    r0= IDELEMS((ideal)(tmp->m[0].Data()));
    c=l_size;


}
else if (tmp->m[0].Typ() == VECTOR_CMD) {
  int level=counter;
  auto l=J[counter];
    
    int l_size = size(l); 
    
    int r0= size(l);

     def LL=J[level+1];
     int c=size(LL);


}
 
if(tmp->m[0].Typ() == IDEAL_CMD){
          def lL=oneSublift(f,s);// Write C++ here
          int r=size(lL);
        } else if(tmp->m[0].Typ() == VECTOR_CMD){
           // matrix A[r0][c] =Token.data[2];
           matrix A;
            A = (matrix)tmp1->m[1].Data(); //A= Tok.data[2]
            module M=A;
            int level=counter;
           
            def lL=subLiftTree2(M,s,J,level);//write C++ function
            int r=size(lL);
           
          }
        
        // Prepare the LLT token
    lists LLT=(lists)omAlloc0Bin(slists_bin);
    LLT->Init(4);// type token
    
    int k=0;


  for (k = 0; k <=r-1; k++) {
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


    matrix sM[r0][c];
    
           
    //vector s_lift =lL[k];//added c_n
    vector s_lift=lL[k];
     
    int l_k = leadexp(s_lift)[nvars(basering) + 1];//see the function
  
    sM[l_k, colmn] =pp_Mul_qq(-1, pHead(s_lift), currRing); 
    
     //data  Ld.data
    t=(lists)omAlloc0Bin(slists_bin);
    t->Init(7);
    //Ld.data[1]=(-1)*s_lift; 
    t->m[0].rtyp=VECTOR_CMD;  t->m[0].data=pp_Mul_qq(-1,s_lift, currRing);
    

    //Ld.data[2]= sM;
     t->m[1].rtyp=MATRIX_CMD; t->m[1].data=sM;

     //Ld.data[3] = l_k;
    t->m[2].rtyp=INT_CMD;  t->m[2].data = (void*)(long)l_k;
    
    
      // Ld.data[4]=colmn;
    t->m[3].rtyp=INT_CMD;  t->m[3].data = (void*)(long)colmn;
   

    if (tmp->m[0].Typ() == IDEAL_CMD) {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J[2];//Ask Hans

     } 
     else if (tmp->m[0].Typ() == VECTOR_CMD) {
      int level=counter;
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J[level+1]; //Ask Hans
     }
   
   // Ld.data[6]=counter;
    t->m[5].rtyp=INT_CMD;  t->m[5].data = (void*)(long)counter;
//Ld.data[7] = counter;
   t->m[6].rtyp=INT_CMD; t->m[6].data = (void*)(long)(counter + 1);
   Ld->m[3].rtyp=LIST_CMD; output->m[3].data=t;//data is in postion 4
     // fieldnames in token LLT
    lists t0=(lists)omAlloc0Bin(slists_bin);
    t0->Init(r);
    // LLT.fieldnames[k] = "generator"; 
     t0->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
     LLT->m[1].rtyp=LIST_CMD; output->m[1].data=t0;
     LLT->m[0].rtyp=RING_CMD; output->m[0].data=currRing;
     LLT->m[2].rtyp=RING_CMD; output->m[2].data=currRing;



     //LLT.data[k] = Ld; 
     
    lists t0 = (lists)LLT->m[1].data;
    if (t0 == nullptr) {
        t0 = (lists)omAlloc0Bin(slists_bin);
        t0->Init(r); // Initialize with r elements to store each Ld
        LLT->m[1].rtyp = LIST_CMD;
        LLT->m[1].data = t0;
    }
   
    t0->m[k].rtyp=LIST_CMD; t->m[k].data=Ld;
   
  }
     //int cmd_nr;
     //blackboxIsCmd("token",cmd_nr); // get the type id of "token"
     
     return {r,LLT};
  }









proc SubLIFT_GPI(token Token,token tok, token TT) {
    def J=tok.data[1];
    def f = Token.data[1]; 
    int counter=TT.data[6];
    vector s = TT.data[1];
   // matrix B= TT.data[2]; //smatrix
    int colmn = TT.data[4]; //column
    
if(typeof(f)=="ideal"){
     def l = leadSyz(f);
   
      int l_size = size(l); 
      int r0= size(f);
      int c=l_size;
    }else{
      if(typeof(f)=="vector"){
        int level=counter;
        def l=J[level];
         
         int l_size = size(l); 
    
       int r0= size(l);

     def LL=J[level+1];
     int c=size(LL);
  
      }
    }
    

 
if(typeof(f)=="ideal"){
          def lL=oneSublift(f,s);// I must use C++ here
          int r=size(lL);
        } else{
          if(typeof(f)=="vector"){
            matrix A[r0][c] =Token.data[2];
            module M=A;
            int level=counter;
           
            def lL=subLiftTree2(M,s,J,level);
            int r=size(lL);
           
          }
        }


    token LLT;               
   

   

  int k;

  for (k = 1; k <=r; k++) {
     token Ld;
    Ld.fieldnames = list("generator", "Sparse_matrix_Lift");  
    matrix sM[r0][c];
    
           
    vector s_lift =lL[k];//added c_n
     
    int l_k = leadexp(s_lift)[nvars(basering) + 1];
  
    sM[l_k, colmn] = (-1)*leadcoef(s_lift) * leadmonomial(s_lift);//removed_sign
    
    
    Ld.data[1]=(-1)*s_lift; //added c_n
    //print(s_lift);
    Ld.data[2]= sM;
    
     //print("Smatrix");
    
   
    //print("row");

    Ld.data[3] = l_k;
    
    //print("q:column");
    Ld.data[4]=colmn;
    
  
    
   if(typeof(f)=="ideal"){
   Ld.data[5]=J[2];
   
   }else {
    if(typeof(f)=="vector"){
      int level=counter;
      Ld.data[5]=J[level+1];
      
    }
   }
    Ld.data[6]=counter;
    Ld.data[7] = counter;
    LLT.fieldnames[k] = "generator";  
    LLT.data[k] = Ld; 

  }
    LLT.fieldnames[size(LLT.fieldnames) + 1] = "total_number_generator";  
    LLT.data[size(LLT.data) + 1] = r;  
    return (LLT);  
}
















int iiAddCproc(const char *libname, const char *procname, BOOLEAN pstatic,
               BOOLEAN(*func)(leftv res, leftv v))
{
  procinfov pi;
  idhdl h;

  #ifndef SING_NDEBUG
  int dummy;
  if (IsCmd(procname,dummy))
  {
    Werror(">>%s< is a reserved name",procname);
    return 0;
  }
  #endif

  h=IDROOT->get(procname,0);
  if ((h!=NULL)
  && (IDTYP(h)==PROC_CMD))
  {
    pi = IDPROC(h);
    #if 0
    if ((pi->language == LANG_SINGULAR)
    &&(BVERBOSE(V_REDEFINE)))
      Warn("extend `%s`",procname);
    #endif
  }
  else
  {
    h = enterid(procname,0, PROC_CMD, &IDROOT, TRUE);
  }
  if ( h!= NULL )
  {
    pi = IDPROC(h);
    if((pi->language == LANG_SINGULAR)
    ||(pi->language == LANG_NONE))
    {
      omfree(pi->libname);
      pi->libname = omStrDup(libname);
      omfree(pi->procname);
      pi->procname = omStrDup(procname);
      pi->language = LANG_C;
      pi->ref = 1;
      pi->is_static = pstatic;
      pi->data.o.function = func;
    }
    else if(pi->language == LANG_C)
    {
      if(pi->data.o.function == func)
      {
        pi->ref++;
      }
      else
      {
        omfree(pi->libname);
        pi->libname = omStrDup(libname);
        omfree(pi->procname);
        pi->procname = omStrDup(procname);
        pi->language = LANG_C;
        pi->ref = 1;
        pi->is_static = pstatic;
        pi->data.o.function = func;
      }
    }
    else
      Warn("internal error: unknown procedure type %d",pi->language);
    if (currPack->language==LANG_SINGULAR) currPack->language=LANG_MIX;
    return(1);
  }
  else
  {
    WarnS("iiAddCproc: failed.");
  }
  return(0);
}
