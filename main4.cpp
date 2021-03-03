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

void printLine(EmpBlock& emp, fstream& fout) {
    fout << emp.eid << ',' << emp.ename << ',' << emp.age << ',' << emp.salary << '\n';
}

//sort and Print out the attributes from emp and dept when a join condition is met
void printEmpBlock(vector<EmpBlock>& emp, fstream& fout){
    for(int ii = 0; ii < emp.size(); ii ++) {
        printLine(emp[ii], fout);
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
            tempout.close();
            break;
        } else{
            blockCount++;
            tempEmps.push_back(empBlock);
        }
    }
    empin.close();
    return numTempfiles;
}

//finds smallest eid out of all employees at current index, returns index in vector
int getLowestEmp(vector<EmpBlock> blocks) {
    int minvalue = blocks[0].eid;
    int index = 0;
    for(int ii = 1; ii < blocks.size(); ii ++) {
        if(blocks[ii].eid < minvalue) {
            minvalue = blocks[ii].eid;
            index = ii;
        }
    }
    return index;
}

//has a vector of empblocks and of ints, one for each temp file
//grabs one employee at a time from each file, finds the smallest one, writes to file,
//and increments the index. when a file is exhausted, it is removed from the vectors
void compareAndMerge(int numTempFiles) {

    vector<EmpBlock> emps = {};
    vector<int> mergePointers = {};
    int index;

    fstream f, out;

    //populate vectors with initial values
    for(int ii = 0; ii < numTempFiles; ii ++) {
        f.open(("temp" + to_string(ii + 1) + ".csv"), ios::in);
        emps.push_back(grabEmp(f));
        f.close();
        mergePointers.push_back(0);
    }

    //open output file
    out.open("EmpSorted.csv", ios::out | ios::trunc);

    while(true) {
        index = getLowestEmp(emps);
        printLine(emps[index], out);
        
        //mergePointers tracks how many lines deep in the file we are
        mergePointers[index] ++;
        
        //reads mergePointers[ii] lines
        f.open(("temp" + to_string(index + 1) + ".csv"), ios::in);
        for(int ii = 0; ii <= mergePointers[index]; ii ++) {
            emps[index] = grabEmp(f);
        }
        f.close();

        //if eid=-1 (aka end of file), remove elements from vectors at that index
        if(emps[index].eid == -1) {
            emps.erase(emps.begin() + index);
            mergePointers.erase(mergePointers.begin() + index);
            if(emps.empty()) {
                break;
            }
        } 
    }

    out.close();
}

int main(){
    compareAndMerge(sortAndStoreEmps());
    return 0;
}
