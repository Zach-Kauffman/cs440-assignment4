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

void compareAndMerge(int numTempFiles) {

    vector<fstream> fstreams = {};
    vector<EmpBlock> emps = {};
    int index;

    fstream f, out;
    for(int ii = 0; ii < numTempFiles; ii ++) {
        f.open( ("temp" + to_string(numTempFiles) + ".csv"), ios::in);
        fstreams.push_back(f);
        emps.push_back(grabEmp(f));
        f.close();
    }

    out.open("EmpSorted.csv", ios::out | ios::trunc);

    while(true) {
        index = getLowestEmp(emps);
        printLine(emps[index], out);
        fstreams[index].open("temp" + to_string(index) + ".csv", ios::in);
        emps[index] = grabEmp(fstreams[index]);
        fstreams[index].close();
        if(emps[index].eid == -1) {
            emps.erase(emps.begin() + index);
            fstreams.erase(fstreams.begin() + index);
            if(emps.size() == 0) {
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
