#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_BLOCK_SIZE 22

struct EmpBlock{
    int eid;
    string ename;
    int age;
    double salary;

    bool operator()(EmpBlock const& a, EmpBlock const& b) const{
        return a.eid < b.eid;
    }
};

// Grab a single block from the emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpBlock grabEmp(fstream& empin){
    string line, word;
    EmpBlock emp;
    // grab entire line
    if(getline(empin, line, '\n')){
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word, ',');
        emp.eid = stoi(word);
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word);
        return emp;
    } else{
        emp.eid = -1;
        return emp;
    }
}

//sort and Print out the attributes from emp and dept when a join condition is met
void printEmpBlock(vector<EmpBlock>& emp, fstream& fout){
    for(int ii = 0; ii < emp.size(); ii ++) {
        fout << emp[ii].eid << ',' << emp[ii].ename << ',' << emp[ii].age << ',' << emp[ii].salary << '\n';
    }
    
}

// Reads data until max block space has been reached, sorts, then writes to a temp file.
// Repeats making temp files until the entire emp file is read.
int sortAndStoreEmps(){
    fstream empin, tempout;
    int numTempfiles = 1, blockCount = 0;
    vector<EmpBlock> emps = {}, tempEmps = {};

    empin.open("Emp.csv", ios::in);
    tempout.open("temp1.csv", ios::out | ios::trunc);
    while(true){
        // FOR BLOCK IN RELATION EMP
        // grabs a block
        if(tempEmps.size() == MAX_BLOCK_SIZE){
            numTempfiles++;
            sort(tempEmps.begin(), tempEmps.end(), EmpBlock());
            printEmpBlock(tempEmps, tempout);
            tempout.close();
            tempEmps.clear();
            tempout.open("temp" + to_string(numTempfiles) + ".csv", ios::out | ios::trunc);
        }

        EmpBlock empBlock = grabEmp(empin);
        // checks if filestream is empty
        if(empBlock.eid == -1){
            sort(tempEmps.begin(), tempEmps.end(), EmpBlock());
            printEmpBlock(tempEmps, tempout);
            break;
        } else{
            blockCount++;
            tempEmps.push_back(empBlock);
        }
    }
    empin.close();
    return numTempfiles;
}

int main(){
    int numTempfiles = sortAndStoreEmps();
    cout << numTempfiles << endl;


// Read data until there is no space left.
// Sort it.
// Write to a temporary file.
// Repeat from 1 until there is no data left to read.
// You know know how many temporary files you have, N.
// You need to determine how many of those files you can read at one time, M.
// If N > M, then you design your merging phase so that the last phase will merge M files.
// You merge sets of M files into new temporary files until you reach the last merge.
// You merge the final set of M files (or N if N < M) writing to final destination.


    return 0;
}
