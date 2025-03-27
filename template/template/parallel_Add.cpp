#include <iostream>
#include <vector>
#include <sstream>
#include <string>

// Function to divide the range into smaller ranges
void divideIntoRanges(int N, int r) {
    int n = N / r; // Base size of each range
    int remainder = N % r; // Remainder to distribute

    std::vector<std::string> range_strings; // To store the range as strings

    int start = 1; // Starting index for the first range
    for (int i = 0; i < r; ++i) {
        int end = start + n - 1 + (i < remainder ? 1 : 0); // Adjust the end for remainder
        std::string range_str = "[" + std::to_string(start) + "," + std::to_string(end) + "]";
        range_strings.push_back(range_str); // Store the range as string
        start = end + 1; // Update start for the next range
    }

    // Display the generated ranges as strings
    std::cout << "Generated ranges as strings:" << std::endl;
    for (const auto& range : range_strings) {
        std::cout << range << " ";
    }
    std::cout << std::endl;

    // Parsing ranges back from string to integer pairs
    std::vector<std::pair<int, int>> range_pairs;
    for (const auto& range : range_strings) {
        int start, end;
        char discard;
        
        // Parse the range string "[n,n+k]"
        std::istringstream range_stream(range);
        range_stream >> discard >> start >> discard >> end >> discard; // Extract start and end

        // Store the parsed pair
        range_pairs.push_back({start, end});
    }

    // Display the parsed ranges as pairs
    std::cout << "Parsed ranges back to pairs:" << std::endl;
    for (const auto& range : range_pairs) {
        std::cout << "[" << range.first << "," << range.second << "] ";
    }
    std::cout << std::endl;
}

int main() {
    int N, r;
    std::cout << "Enter N and r: ";
    std::cin >> N >> r;

    // Input validation
    if (r <= 0 || N <= 0 || r > N) {
        std::cout << "Invalid input. Ensure 0 < r <= N." << std::endl;
        return 1;
    }

    // Call the function to divide ranges and parse them
    divideIntoRanges(N, r);
    
    return 0;
}




NO_NAME_MANGLING
std::pair<std::string, long> singular_template_reduce(const std::string& Red, 
    const std::pair<int, int>& ranges,
    const std::string& needed_library,
    const std::string& base_filename) 
{
    init_singular(config::singularLibrary().string());
    load_singular_library(needed_library);
    // std::cout << "Reduce:= " <<N << std::endl;
    // Extract the folder path from the Red file's location
    std::filesystem::path basePath = std::filesystem::path(Red).parent_path();
    // std::cout << base_filename<< "base_filename" << std::endl;
    // std::cout <<Red<< "" << std::endl;
    // std::cout <<N<< "=:Reduce" << std::endl;
    std::string ids = worker();
    std::pair<int, lists> Gb = deserialize(Red, ids);

   

    lists Gb_data = (lists)(Gb.second);
    lists tmpL1 = (lists)(Gb_data->m[3].Data());
   
    ideal C = nullptr;
    int counter = 0;
    int r=0;int c=0;
    poly vec = NULL;
    leftv L = nullptr;
    std::pair<int, lists> input_part;
    std::string filename;


          // Open a log file for timing information
  // Full path to the log file
  std::string logFilePath = "/scratch/gnawali/Try/gspc-res/example_dir/Smatrix_timing.log";
 
  // Open the log file
  std::ofstream logFile(logFilePath, std::ios::app);
  if (!logFile.is_open()) {
      std::cerr << "Error: Failed to open log file at " << logFilePath << "!" << std::endl;
      return {"", 0};  // Exit early if file cannot be opened
  } else {
      std::cout << "Log file opened successfully at " << logFilePath << "." << std::endl;
  }
  // Start timing before the loop
auto start_addition = std::chrono::high_resolution_clock::now();
int start = ranges.first;  // Get the start value
int end = ranges.second; 
for (int i = start; i <=end; ++i) {  
    filename = (basePath / (std::to_string(i) + ".ssi")).string();
    
    if (!std::filesystem::exists(filename)) {
        continue;  // Skip to next iteration
    }

    input_part = deserialize(filename, ids);
    lists token = (lists)input_part.second;
    lists tmpL = (lists)(token->m[3].Data());

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

// Stop timing after the loop
auto end_addition = std::chrono::high_resolution_clock::now();
auto addition_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_addition - start_addition).count();

// Log total addition time
logFile << "Total Addition Time: " << addition_time << " ns\n";
logFile.flush();  // Ensure immediate write


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

    std::string out_filename = serialize(output, base_filename);

    logFile.close();
    rKill(currRing); 
    return {out_filename, addition_time};
}
