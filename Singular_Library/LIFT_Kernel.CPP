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
      //std::cout << "lead(s) in cOM:"<<pString(s10)<< std::endl;
  //poly t10 = leadmonomial(t);
   poly t10= pHead(t);
    pSetComp(t10,0);
    pSetmComp(t10);
 
  //std::cout << "lead(t) in cOM:"<<pString(t10)<< std::endl;
  //poly sfi = leadmonom(s10 * f[i]);
  poly sfi=pp_Mult_qq(s10,pHead(f->m[i-1]),currRing);//should leadmonom

  //poly tfj = leadmonom(t10 * f[j]);
 poly tfj=pp_Mult_qq(t10,pHead(f->m[j-1]),currRing); //should leadmonom
  bool c= p_ComparePolys(sfi,tfj,currRing);
 // std::cout << "sfi>tfjin cOM:"<<c<< std::endl;
  if (c==TRUE) {
     //std::cout << "sfi>tfjin cOM:"<<c<< std::endl;
    isTrue = 1;
  } else if ((sfi == tfj) && (i > j)) 
   {
      isTrue = 1;
    }
  
  return (isTrue);
}




int coM2(ideal f, poly s, poly t, lists J, int k) { //poly s and poly t are singular vectors
//ideal f is singular module

                            
  
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
                      
    ideal I_Gr=(ideal)J->m[0].Data();

    // Compute initial indices and monomials for the vectors s and t
     i= p_GetComp(s,currRing);
     j= p_GetComp(t,currRing);
    
    poly s10= pHead(s);
    pSetComp(s10,0);
    pSetmComp(s10);
  
    poly t10= pHead(t);
    pSetComp(t10,0);
    pSetmComp(t10);
    ideal M=(ideal)J->m[k-1].Data();
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
  
            poly g10= pHead(g1);
            pSetComp(g10,0);
            pSetmComp(g10);
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
  
                  poly G10= pHead(G1);
                  pSetComp(G10,0);
                  pSetmComp(G10); 
            F1 = pp_Mult_qq(pHead(F10), K->m[i1-1],currRing); //chnage from f1 to F1
            //should leadmonomial
            G1 = pp_Mult_qq(pHead(G10), K->m[i2-1],currRing); //change from g1 to G1
            
        //should monomial
           
        }
    }

    // Perform final comparison using coM
    if (k == 2) {
        int c=coM(I_Gr, f1, g1);
        if ( c== 1) {
            isTrue = 1;
        } else {
            if ((f1 == g1) && (i > j)) {  
                isTrue = 1;
            }
        }
    } else {
        // General case: compare F1 and G1 after iteration
        int  d=coM(I_Gr, F1, G1);
        if (d == 1) {
            isTrue = 1;
        } else {
            if ((F1 == G1) && (i > j)) {  //it was i1>i2
                isTrue = 1;
            }
        
        }
      
    }
  

  
    return (isTrue);
}





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
 
    poly g_copy = pCopy(g);
    poly lOT=LOT(g_copy, f);//poly T0 = LOT(g, f); 
     //std::cout<<"LOT=" << pString(lOT) <<std::endl;
     //poly T0 = g-LOT(g, f); 
    poly T0=p_Sub(pCopy(g),pCopy(lOT),currRing);
    //std::cout << "g-LOT_in LIFT: " << pString(T0) << std::endl;
    lists T=pOLY_List(pCopy(T0));  //lists T = (lists)pOLY_List(T0)->Data(); 
    
    poly m1 = NULL;
    poly m = NULL;
  
    poly s_v = NULL;//vector s_v
    //int C = 0;
    //int cl = 0;
    int t_size = lSize(T)+1;


    lists TT = (lists)omAlloc0Bin(slists_bin); // Allocate memory for empty list TT
    if (t_size == 0) 
    {
    std::cout << "T is empty, stopping algorithm." << std::endl;
    
    TT->Init(0); // Initialize TT with size 0
    return TT; // Return an empty list
    } else 
    {
      TT->Init(t_size); // Initialize TT with size t_size
    }
   
    
  
    for ( k = 0; k < t_size; k++) {
        poly t = (poly)T->m[k].Data();// poly t=T[k]
    
       // std::cout << "t in LIFT:"<<pString(t)<< std::endl;
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
               // std::cout << "s_v in LIFT:"<<pString(s_v)<< std::endl;
                int c = coM(f, s, s_v);

                if(c == 1) {
                  //std::cout << "s_v in LIFT_COM:"<<pString(s_v)<< std::endl;
                   TT->m[k].rtyp = VECTOR_CMD;
                   TT->m[k].data=pCopy(s_v);  //TT[cl]=m*f[lambda]
                    
                    
                    break;
                }
            }
             
        } 
            
        
            
    }   
       
    
 

    return TT;
}




//iterative liftTree

lists liftTree2(ideal f, poly s, lists J, int level) {

//ideal f is singular module
// poly s is singular vector

  std::cout<<"lift2_vector s=" << pString(s) <<std::endl;

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

  std::cout<<"g=psi(s)in LIFT2" << pString(pCopy(g)) <<std::endl;
  poly g_copy = pCopy(g);

// vector t0=LOT_2(g,f,J,level)
    poly t0=LOT_2(g_copy,f,J,level);

      std::cout<<"g-LOT in LIFT2" << pString(pCopy(t0)) <<std::endl;

    
     poly T0 = p_Sub(g_copy, pCopy(t0),currRing);//vector T0=g - LOT_2(g, f, J, level)

   
      lists T = vector_List(T0); //list T =vector_List(T0)

    
      int t_size = lSize(T)+1;
  
   

     poly s_v=NULL; //vector s_v
     poly m=NULL;
     poly m1=NULL;
    int lambda=0;
   
    int k=0;
    lists TT = (lists)omAlloc0Bin(slists_bin);
  
    if (t_size ==0) 
    {
     TT->Init(0);
      std::cout << "TT is empty, stopping algorithm." << std::endl;
      return TT;
   
    
    } else
    {
       TT->Init(t_size); 
     
    }
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
                   std::cout << "bool d in LIFT2:"<<d<< std::endl;
                   m1 = pp_Divide(pHead(t), pHead(f0->m[lambda]), currRing);
                   m = pCopy(m1);//m=m1;
                   p_SetComp(m,lambda+1,currRing);
                   p_SetmComp(m,currRing);
                  s_v=pCopy(m);
   
               
                      int c= coM2(f0, s, s_v, J,level);
                      if (c==1)
                       {
                    std::cout << "vector s_v in LiftTREE2:"<<pString(s_v)<< std::endl;
                      TT->m[k].rtyp = VECTOR_CMD;
                      TT->m[k].data=pCopy(s_v);  //TT[cl]=m*f[lambda]
  
                    break;
                }
                
              
            } 
        }
        
      }    
                
                
    
           

          
            
            
   }   
 

    return TT;

}



std::pair<int, lists> LIFT_GPI(leftv args) {
    // Extract the Token
    lists Token = (lists)(args->Data()); 
    // Extract tok
    lists tok =  (lists)(args->next->Data()); 
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
    // Column
    int colmn = (int)(long)tmp2->m[3].Data(); 
   // std::cout << "LIFT_GPI Counter: " << counter << std::endl;

    // J = tok.data[1]
    lists J = (lists)(tmp1->m[0].Data());
   

    // vector s = TT.data[1]
    poly s = (poly)tmp2->m[0].Data(); 
    //std::cout << "LIFT_GPI vector: " << pString(s) << std::endl;

    int r0 = 0, c = 0, l_size = 0;
    int L_size = lSize(J);
    std::cout << "Size of J: " << L_size << std::endl;

    ideal l = NULL;
    ideal LL = NULL;

    if (tmp->m[0].Typ() == IDEAL_CMD) {
        // l = J[2]
        l = (ideal)J->m[1].Data(); 
        //std::cout << "LIFT_GPI Module: " << l << std::endl;
        l_size = IDELEMS(l);
        //std::cout << "LIFT_GPI Module size: " << l_size << std::endl;
        r0 = IDELEMS((ideal)(tmp->m[0].Data())); 
        c = l_size;
    } else if (tmp->m[0].Typ() == VECTOR_CMD) {
        int level = counter;
        l = (ideal)J->m[counter - 1].Data(); 
        l_size = IDELEMS(l); 
        r0 = IDELEMS(l); 
        LL = (ideal)J->m[level].Data(); 
        c = IDELEMS(LL);
    }

    lists lL = NULL;
    int r = 0;

    if (tmp->m[0].Typ() == IDEAL_CMD) {
        // Use liftTree for IDEAL_CMD
        ideal f = (ideal)(tmp->m[0].Data());
        //std::cout << "LIFT_GPI Input: " << f << std::endl;
        //std::cout << "LIFT_GPI Size(f): " << IDELEMS(f) << std::endl;
        lL = liftTree(f, s); 
       // std::cout << "LIFT_GPI liftTree: " << IDELEMS(f) << std::endl;
       
        if (lL == NULL) {
            std::cerr << "Error: liftTree returned a null list." << std::endl;
            return {0, NULL};
        }
         r = lSize(lL)+1;
         r=r;
        //std::cout << "LIFT_GPI liftTree size: " << r << std::endl;
    } else if (tmp->m[0].Typ() == VECTOR_CMD)
     {
        // Use liftTree2 for VECTOR_CMD
            matrix A= (matrix)tmp->m[1].Data(); //A= Tok.data[2]
            ideal M = id_Matrix2Module(A, currRing);
         
            bool m0=(tmp->m[1].Typ()==MATRIX_CMD);
             std::cout << "m0_matrix:? " << m0<< std::endl;

        int level = counter;
        lL = liftTree2(M, s, J, level); 
        if (lL == NULL) {
            std::cerr << "Error: liftTree2 returned a null list." << std::endl;
            return {0, NULL};
        }
         r = lSize(lL)+1;
         r=r;
    }

    // Prepare the LLT token
    lists LLT = (lists)omAlloc0Bin(slists_bin);
    LLT->Init(4); // type token
    
    int k = 0;
 

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
  
    
     //vector s_lift =lL[k];
     poly s_lift=(poly)lL->m[k].Data(); //vector s_lift=lL[k]
    
     // p_Head for leadexp 
     int l_k = p_GetComp(s_lift, currRing);
   
     poly lm = pHead(s_lift);
    pSetComp(lm,0);
    pSetmComp(lm);
    
     MATELEM(sM,l_k,colmn)=p_Mult_q(pISet(-1), pHead(lm),currRing); //should include -1
    
     //data  Ld.data
     t=(lists)omAlloc0Bin(slists_bin);
     t->Init(7);

     //Ld.data[1]=(-1)*s_lift; 
    
     t->m[0].rtyp=VECTOR_CMD;  t->m[0].data=pCopy(p_Mult_q(pISet(-1), s_lift,currRing));
   
     //Ld.data[2]= sM;
     t->m[1].rtyp=MATRIX_CMD; t->m[1].data=sM;

     //Ld.data[3] = l_k;
     t->m[2].rtyp=INT_CMD;  t->m[2].data = (void*)(long)l_k;
    
    
      // Ld.data[4]=colmn;
     t->m[3].rtyp=INT_CMD;  t->m[3].data = (void*)(long)colmn;
   

    if (tmp->m[0].Typ() == IDEAL_CMD)
     {
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[1].Data();//J[2]
        
     //data  Ld.data
     } 
     else if (tmp->m[0].Typ() == VECTOR_CMD) 
     {
         int level=counter;
         t->m[4].rtyp=MODUL_CMD; t->m[4].data= J->m[level].Data(); // Ld.data[5]=J[level+1];
         std::cout << "LIFT_GPI_token_lift2: "<<IDELEMS((ideal)t->m[4].data)<< std::endl;
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
  // Assign data to LLT
  LLT->m[3].rtyp = LIST_CMD;
  LLT->m[3].data = t0;

   
  }
    LLT->m[r].rtyp = STRING_CMD;
    LLT->m[r].data = strdup("total_number_generator");
   // std::cout << "Appending 'total_number_generator' to LLT." << std::endl;

    LLT->m[r + 1].rtyp = INT_CMD;
    LLT->m[r + 1].data = (void*)(long)r;
   
 
   
   
     
     return {r,LLT};
  }




std::tuple<std::vector<std::string>, int, long> singular_template_LIFT(const std::string& input_filename,
                                                                       const std::string& all_lead,
                                                                       const std::string& lead,
                                                                       const std::string& needed_library,
                                                                       const std::string& base_filename)
{
    // Initialize Singular and load the necessary library
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    
    // Debugging: Optionally print out function parameters
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

    // Call the Singular procedure directly (LIFT_GPI) without using name mangling
    std::pair<int, lists> out;
    
    // Measure Computation Time
    auto start_computation = std::chrono::high_resolution_clock::now();
    
    out = LIFT_GPI(args.leftV());  // Direct call to LIFT_GPI (similar to SubLIFT_GPI)
    
    auto end_computation = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_computation - start_computation).count();
    
    // std::cout << "LIFT Runtime: " << computation_time << " milliseconds" << std::endl;

    // Extract the result list from the output
    lists u=NULL;
    u = (lists)out.second->m[3].Data();
     if (u==NULL)
     {
    std::cerr << "Error: LIFT_GPI returned a null list." << std::endl;
    return {{}, 0, 0 }; // Early exit with default values
    }
    
    std::vector<std::string> vec;
    int total_generator;

    // Iterate through each element of the outer list and serialize it
    for(int i = 0; i < lSize(u)+1; i++) {
        auto Outname = serialize((lists)u->m[i].Data(), base_filename);
        vec.push_back(Outname);
    }

    total_generator = lSize(u)+1;  // Assuming u contains the computed generators
    auto total_runtime = computation_time;

    return {vec, total_generator, total_runtime};
}