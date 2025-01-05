


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
                 //std::cout << "s=L[k] " << k << ": " << pString(s) << std::endl;
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
  

    return L;  // Return the ideal L
}










std::pair<int, lists> leadsyz_GpI(leftv args) {
    // Extract Token
    lists Token = (lists)(args->Data());
    
    // Extract the first element (f)
    lists tmp = (lists)(Token->m[3].Data());
    ideal f = (ideal)(tmp->m[0].Data());
     //std::cout << "ideal from tokesn:" << IDELEMS(f)<< std::endl;
   
    int r0 = 0;
    int r=0;
    
    
   
      ideal f_copy = idCopy(f);  // Create a deep copy of f
      ideal LT = leadSyz(f_copy );
       
        r = IDELEMS(LT); 
    //            for (int k = 0; k < r; k++) {
    //     std::cout << "leadsyz in GPI=" << k << ": " << pString((poly)LT->m[k+1]) << std::endl;
    // }

        r0 = IDELEMS(f_copy); 
         idDelete(&f_copy);  // Clean up copy
        r0=r0; // Set row count to the number of elements in ideal


    

   //int r = IDELEMS(LT); 
     std::cout << "#LEADSYZ_GPI tokens:" << r<< std::endl;
       // Prepare the LLT token
    lists LLT=(lists)omAlloc0Bin(slists_bin);
    LLT->Init(4);// type token
    lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);
    //std::cout << "SubLIFT_GPI_token: "<< std::endl;
    int k=0;
     int k1=1;
    for (k = 0; k < r; k++) {
       // Create a new token Ld
      lists Ld=(lists)omAlloc0Bin(slists_bin);
      Ld->Init(4);// type token 

      
      lists t=(lists)omAlloc0Bin(slists_bin);
      t->Init(2);
      t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
      t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("Sparse_matrix_LeadSyz");
    
      Ld->m[1].rtyp=LIST_CMD; Ld->m[1].data=t;
      Ld->m[0].rtyp=RING_CMD; Ld->m[0].data=currRing;
      Ld->m[2].rtyp=RING_CMD; Ld->m[2].data=currRing;

        // Create the matrix sM
        matrix sM = mpNew(r0, r);
        poly s_lift = (poly)LT->m[k];
        std::cout<< "Leadsyz at k:" <<k<< pString(s_lift) << std::endl;
         poly lm = pHead(s_lift);
         pSetComp(lm,0);
         pSetmComp(lm);
         std::cout<< "Leadsyz _matrix=" << pString(lm) << std::endl;
        // Extract the lead exponent l_k
        int l_k = p_GetComp(s_lift, currRing);
          std::cout<< "Row :" <<k<<":"<<l_k<< std::endl;
        // sM[l_k, k] = leadcoef(s_lift) * leadmonomial(s_lift);
        MATELEM(sM,l_k,k+1) = pCopy(lm);

        //data  Ld.data
    lists t0=(lists)omAlloc0Bin(slists_bin);
     t0->Init(5);
      t0->m[0].rtyp=VECTOR_CMD;  t0->m[0].data=pCopy(s_lift);
      //Ld.data[2]= sM;
     t0->m[1].rtyp=MATRIX_CMD; t0->m[1].data=sM;
       //Ld.data[3] = l_k;
     t0->m[2].rtyp=INT_CMD;  t0->m[2].data = (void*)(long)l_k;
      // Ld.data[4]=colmn;
     t0->m[3].rtyp=INT_CMD;  t0->m[3].data = (void*)(long)(k+1);
          // std::cout << "#LEADSYZ_GPI column:" <<k+1<< std::endl;
     t0->m[4].rtyp = INT_CMD;t0->m[4].data = (void*)(long)k1; // Safely cast integer to a pointer

     // std::cout << "LT in token:" << IDELEMS(LT) << std::endl;
        // Ld.data[6]=counter;
   
  
    
     //Ld.data //data is in postion 4
    Ld->m[3].rtyp=LIST_CMD; Ld->m[3].data=t0;
        
      
    // Set fieldnames[k] to "generator"
   lists field_names = (lists)omAlloc0Bin(slists_bin);
   field_names->Init(r);
 for (int s = 0; s < r; s++) 
 {
    field_names->m[s].rtyp = STRING_CMD;
    field_names->m[s].data = strdup("generator"); 
   std::cout << "LLT[k]:generator "<<s<< std::endl;
 }
     LLT->m[0].rtyp=RING_CMD; LLT->m[0].data=currRing;
 // Assign fieldnames to LLT
  LLT->m[1].rtyp = LIST_CMD;
  LLT->m[1].data = field_names;
  
  
  LLT->m[2].rtyp=RING_CMD; LLT->m[2].data=currRing;

     //LLT.data[k] = Ld; 
     
    
   // Set data for LLT
  // lists t1 = (lists)omAlloc0Bin(slists_bin);
  // t1->Init(r);
  // for (int s = 0; s < r; s++) 
  // {
  //   t1->m[s].rtyp = LIST_CMD;
  //   t1->m[s].data = lCopy(Ld);  
  //   std::cout << "LLT[k]:data= "<<s<< std::endl;
  // }

  // LLT->m[3].rtyp = LIST_CMD;
  // LLT->m[3].data = t0;
 
    temp->m[k].rtyp = LIST_CMD;
     std::cout << "temp->m[k].data: "<<k<< std::endl;
    temp->m[k].data =  lCopy(Ld);
   
 
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
     std::cout << "LLT[k]:generator "<<lSize(LLT)<< std::endl;
      std::cout << "Total_leadSyz= "<<r<< std::endl;
    return {r, LLT};  // Return success state and LLT
}





NO_NAME_MANGLING
std::pair<std::vector<std::string>, int>  singular_template_leadSyzGPI(const std::string& input_filename,
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
    input = deserialize(input_filename, ids);

    // Debugging: Print input data
    //std::cout << "Input Data:" << input.second << std::endl;

    // Prepare arguments
    ScopedLeftv args(input.first, lCopy(input.second));

    // Call Singular procedure
    std::pair<int, lists> out;
    // std::string function_name = "leadsyz_GpI";
    // out = call_user_proc(function_name, needed_library, args);
     out = leadsyz_GpI(args.leftV());
    //std::cout<<"ListOutside_proc:"<<function_name<< std:: endl;
    
    // Extract list from the output
   
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
    
    //std::cout<<"Check the output_LEADSYZ:"<<Outname<< std::endl;
    vec.push_back(Outname);
  } 
   total_generator = lSize(u); // Assuming u contains the computed generators

  return {vec, total_generator};

}