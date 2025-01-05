matrix lcm_mod(ideal G) { //ideal G is Singular module

    int a=0;
    int b=0;
    int i=0;
    int j=0;
    int r = IDELEMS(G);
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
                 //std::cout << "m[a.b] in lcm_mod: " << pString(MATELEM(l, a, b)) << std::endl;

            } else {
                // If i is not equal to j, set l[a, b] to 0
                MATELEM(l,a,b)= NULL;
            }
        }
    }
    

    return l;
}




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
    std::cout << "Final first leasyz size: " << IDELEMS(L) << std::endl;
    for (int k = 0; k < cc; k++) {
        std::cout << "Generator " << k << ": " << pString((poly)L->m[k]) << std::endl;
    }
  

    return L;  // Return the ideal L
}



ideal Sec_leadSyz(ideal f0) {
    int r = IDELEMS(f0);  // Get the number of elements in the ideal f0
    poly s = NULL;  // Polynomial s is a singular vector
    poly t = NULL;  // Polynomial t is a singular vector
    int cc = 0; // Counter for elements in L
    
    // Initialize ideal L with initial size 0
    ideal L = idInit(0, 1);

    // Create a matrix M using lcm_mod for the input ideal f0
    matrix M = lcm_mod(f0);  // Ensure lcm_mod returns a valid matrix

    // Loop through pairs (i, j) in the matrix
    for (int i = 1; i < r; i++) {
        for (int j = 0; j < i; j++) {
            // Fetch the matrix element at (j, i)
            poly t0 = MATELEM(M, j, i);

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

       // Debug output
    std::cout << "Final second-Leadsyz  size: " << IDELEMS(L) << std::endl;
    for (int k = 0; k < cc; k++) {
        std::cout << "Generator " << k << ": " << pString((poly)L->m[k]) << std::endl;
    }
    return (L);
   
}
lists allLeadSyz(ideal f) {
   
    ideal F = leadSyz(f);
    
    int level = 0;
    ideal temp=idInit(0, 1);;
    int t=0;
    
     lists J = (lists)omAlloc0Bin(slists_bin);
    // J[1]=F;
     J->m[0].rtyp = MODUL_CMD;
     J->m[0].data = idCopy(F);
  
    int n = NVars(GetBasering());

    for(t=0; t<n; t++) {
       
        
        temp = Sec_leadSyz(F);
        
      
        
        if ((NROWS(temp) !=0) && (NCOLS(temp) != 0)) {
           
             F = idCopy(temp);
        }
        if(IDELEMS(F)==-1){
         break;
        }
        
        J->m[level+1].rtyp  = MODUL_CMD;
        J->m[level+1].data = idCopy(F);
        level++;
    }  
    
    return (J);
}

std::pair<int, lists> all_leadsyz_GpI(leftv args) {

   // Extract Token
    lists Token = (lists)(args->Data());
    
    // Extract the first element (f)
    lists tmp = (lists)(Token->m[3].Data());
    ideal f = (ideal)(tmp->m[0].Data());
  
 
 
  int k=0;
  lists LT= allLeadSyz(f);
 
   int r = lSize(LT)+1;
      // Prepare the LLT token
    lists LLT=(lists)omAlloc0Bin(slists_bin);
    LLT->Init(4);// type token
     lists temp = (lists)omAlloc0Bin(slists_bin);
    temp->Init(r);
  for (k = 0; k <r; k++) {

     lists Ld=(lists)omAlloc0Bin(slists_bin);
      Ld->Init(4);// type token 

      
      lists t=(lists)omAlloc0Bin(slists_bin);
      t->Init(2);
      t->m[0].rtyp=STRING_CMD; t->m[0].data=strdup("generators");
      t->m[1].rtyp=STRING_CMD; t->m[1].data=strdup("Sparse_matrix_LeadSyz");
    
      Ld->m[1].rtyp=LIST_CMD; Ld->m[1].data=t;
      Ld->m[0].rtyp=RING_CMD; Ld->m[0].data=currRing;
      Ld->m[2].rtyp=RING_CMD; Ld->m[2].data=currRing;
      t=(lists)omAlloc0Bin(slists_bin);
     t->Init(2);
      t->m[0].rtyp=MODUL_CMD;  t->m[0].data=LT->m[k].CopyD();
      //Ld.data[2]= sM;
     t->m[1].rtyp=INT_CMD; t->m[1].data=(void*)(long)(k+1);

     
    
     //Ld.data //data is in postion 4
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

  // LLT->m[3].rtyp = LIST_CMD;
  // LLT->m[3].data = t0;
 
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
    final_data->m[r].data = (void*)(long)r;

    // Set final_data as LLT->m[3] (LLT.data)
    LLT->m[3].rtyp = LIST_CMD;
    LLT->m[3].data = final_data;




    return {r, LLT};  // Return r (number of generators) and LLT
}