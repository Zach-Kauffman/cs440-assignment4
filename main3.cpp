#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct EmpBlock
{
    int eid;
    string ename;
    int age;
    double salary;

    bool operator()(EmpBlock const & a, EmpBlock const & b) const
    {
        return a.eid < b.eid;
    }
};

struct DeptBlock
{
    int did;
    string dname;
    double budget;
    int managerid;

    bool operator()(DeptBlock const & a, DeptBlock const & b) const
    {
        return a.did < b.did;
    }
};

// Grab a single block from the emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpBlock grabEmp(fstream &empin)
{
    string line, word;
    EmpBlock emp;
    // grab entire line
    if (getline(empin, line, '\n'))
    {
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
    }
    else
    {
        emp.eid = -1;
        return emp;
    }
}

// Grab a single block from the dept.csv file, in theory if a block was larger than
// one tuple, this function would return an array of DeptBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
DeptBlock grabDept(fstream &deptin)
{
    string line, word;
    DeptBlock dept;
    if (getline(deptin, line, '\n'))
    {
        stringstream s(line);
        getline(s, word, ',');
        dept.did = stoi(word);
        getline(s, word, ',');
        dept.dname = word;
        getline(s, word, ',');
        dept.budget = stod(word);
        getline(s, word, ',');
        dept.managerid = stoi(word);
        return dept;
    }
    else
    {
        dept.did = -1;
        return dept;
    }
}

//Print out the attributes from emp and dept when a join condition is met
void printJoin(EmpBlock emp, DeptBlock dept, fstream &fout)
{
    fout << emp.eid << ',' << emp.ename << ',' << emp.age << ','
         << emp.salary << ',' << dept.did << ',' << dept.dname << ','
         << dept.budget << "\n";
}

void mergeJoin(vector<EmpBlock> empList, vector<DeptBlock> deptList) {
    fstream joinout;
    joinout.open("Join.csv", ios::out | ios::app);

    int empIdx, deptIdx = 0;
    while(empIdx < empList.size() || deptIdx < deptList.size()) {

        if(empList[empIdx].eid == deptList[deptIdx].did) {
            printJoin(empList[empIdx], deptList[deptIdx], joinout);
            empIdx ++;
            deptIdx ++;
        } else if (empList[empIdx].eid < deptList[deptIdx].did) {
            empIdx ++;
        } else {
            deptIdx ++;
        }
    } 

    joinout.close();
}

int main() {
    // open file streams to read and write
    fstream empin;
    fstream deptin;

    vector<EmpBlock> tempEmp = {};
    
    vector< vector< EmpBlock > > empList = {};
    vector< DeptBlock > deptList = {};

    // flags check when relations are done being read
    empin.open("Emp.csv", ios::in);
    while (true) {
        // FOR BLOCK IN RELATION EMP
        // grabs a block
        EmpBlock empBlock = grabEmp(empin);

        // checks if filestream is empty
        if (empBlock.eid == -1) {
            sort(tempEmp.begin(), tempEmp.end(), EmpBlock());
            empList.push_back(tempEmp);
            tempEmp.clear();
            break;
        } else {
            tempEmp.push_back(empBlock);
            //can only read 22 blocks into memory at a time
            if(tempEmp.size() == 22) {
                sort(tempEmp.begin(), tempEmp.end(), EmpBlock());
                empList.push_back(tempEmp);
                tempEmp.clear();
            }
        }
    }
    
    int numVectors = empList.size();
    int index1, index2 = 0;
    bool flag = true;
    while(flag) {
        int smallestID = 2147483647;
        int smallestIndex1, smallestIndex2;
        for(int ii = index1; ii < numVectors; ii ++) {
            for(int jj = index2; jj < 22; jj ++) {
                if(empList[ii][jj].eid == -1) {
                    flag = false;
                    break;
                }
                if(empList[ii][jj].eid < smallestID) {
                    smallestID = empList[ii][jj].eid;
                    smallestIndex1 = ii;
                    smallestIndex2 = jj;
                }
                
            }
        }
        if(flag) {
            EmpBlock temp = empList[index1][index2];
            empList[index1][index2] = empList[smallestIndex1][smallestIndex2];
            empList[smallestIndex1][smallestIndex2] = temp;

            index2 ++;
            if(index2 == 22) {
                index2 = 0;
                index1 ++;
            }
        }  
    }

    for(int ii = 0; ii < numVectors; ii ++) {
        for(int jj = 0; jj < 22; jj ++) {
            cout << empList[ii][jj].eid << endl;
        }
    }

    deptin.open("Dept.csv", ios::in);
    while (true) {
        // FOR BLOCK IN RELATION EMP
        // grabs a block
        DeptBlock deptBlock = grabDept(deptin);

        // checks if filestream is empty
        if (deptBlock.did == -1) {
            break;
        } else {
            deptList.push_back(deptBlock);
        }
    }

    //sort(empList.begin(), empList.end(), EmpBlock());
    sort(deptList.begin(), deptList.end(), DeptBlock());

    //mergeJoin(empList, deptList);
    
    return 0;
}
