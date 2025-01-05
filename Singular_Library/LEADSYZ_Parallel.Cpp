

proc append(token s, token t) {
    def t0 = t.data[1];  
    list L;              
    
    // Check if t0 is a vector
    if (t0 == vector) {
        // Append s to L only if it is not zero and meets the conditions
        if ((s == t0) || (s | t0) || (t0 | s)) {
            if (s != 0) {  
                L[1] = s;  
            }
        }
        return L;  // Return the list
    }
    
    // If t0 is a list
    if (t0 == list) {
        // Iterate over the elements of the list
        for (int i = 1; i <= size(t0); i++) {
            vector vi = t0[i];  // Get the i-th vector from the list
            
            // Check if vi divides s
            if (vi | s) 
            {
                return L;  // If vi divides s, return the current list L (without s)
            }
            
            // Check if s divides vi
            if (s | vi) {
                L = delete(L, i);  // If s divides vi, delete vi from list L
                i=i-1;
                continue;  // Move to the next element
            }
        }
        
        // If s is non-zero and no conditions were met, append s to the list
        if (s != 0) {
            L[size(L) + 1] = s;  
        }
    }
    
    return L;  // Return the updated list
}
