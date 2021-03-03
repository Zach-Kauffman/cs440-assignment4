#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

#define BLOCK_SIZE 11

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
        return a.managerid < b.managerid;
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

void mergeJoin(vector< vector< EmpBlock > > empList, vector< vector< DeptBlock > > deptList) {
    fstream joinout;
    joinout.open("Join.csv", ios::out | ios::app);

    // int empIdx, deptIdx = 0;
    // while(empIdx < empList.size() || deptIdx < deptList.size()) {

    //     if(empList[empIdx].eid == deptList[deptIdx].managerid) {
    //         printJoin(empList[empIdx], deptList[deptIdx], joinout);
    //         empIdx ++;
    //         deptIdx ++;
    //     } else if (empList[empIdx].eid < deptList[deptIdx].managerid) {
    //         empIdx ++;
    //     } else {
    //         deptIdx ++;
    //     }
    // } 

    joinout.close();
}

vector< vector< EmpBlock > > sortAndStoreEmps(vector< vector< EmpBlock > > empList) {
    fstream empin;
    vector<EmpBlock> tempEmp = {};
    vector<EmpBlock> smallTempEmp = {};

    empin.open("Emp.csv", ios::in);
    while (true) {
        // FOR BLOCK IN RELATION EMP
        // grabs a block
        EmpBlock empBlock = grabEmp(empin);
        // checks if filestream is empty
        if (empBlock.eid == -1) {
            break;
        } else {
            tempEmp.push_back(empBlock);
        }
    }

    // grab all employees and sort by eid
    sort(tempEmp.begin(), tempEmp.end(), EmpBlock());
    int runs = 0;

    // split employees into groups of 11 per memory constraints
    for(int ii = 0; ii < tempEmp.size() % BLOCK_SIZE; ii ++) {
        for(int jj = 0; jj < BLOCK_SIZE; jj ++) {
            runs ++;
            if(runs < tempEmp.size()) {
                smallTempEmp.push_back(tempEmp[jj + ii * BLOCK_SIZE]);
            } else {
                break;
            }
        }
        empList.push_back(smallTempEmp);
        smallTempEmp.clear();
    }
    empin.close();
    return empList;

}

vector< vector< DeptBlock > > sortAndStoreDepts(vector< vector< DeptBlock > > deptList) {
    
    cout << "i made it" << endl;
    fstream deptin;
    vector<DeptBlock> tempDept = {};
    vector<DeptBlock> smallTempDept = {};

    deptin.open("Dept.csv", ios::in);
    while (true) {
        // FOR BLOCK IN RELATION DEPT
        // grabs a block
        DeptBlock deptBlock = grabDept(deptin);
        // checks if filestream is empty
        if (deptBlock.did == -1) {
            break;
        } else {
            tempDept.push_back(deptBlock);
        }
    }


    // grab all departments and sort by managerid
    sort(tempDept.begin(), tempDept.end(), DeptBlock());
    int runs = 0;

    // split departments into groups of 11 per memory constraints

    for(int ii = 0; ii < tempDept.size() % BLOCK_SIZE; ii ++) {
        for(int jj = 0; jj < BLOCK_SIZE; jj ++) {
            runs ++;
            if(runs < tempDept.size()) {
                smallTempDept.push_back(tempDept[jj + ii * BLOCK_SIZE]);
            } else {
                break;
            }
        }
        deptList.push_back(smallTempDept);
        smallTempDept.clear();
    }

    deptin.close();
    return deptList;
    
}

int main() {
    vector< vector< EmpBlock > > empList = sortAndStoreEmps(empList);
        cout << "swagger man ultimate" << endl;

    vector< vector< DeptBlock > > deptList = sortAndStoreDepts(deptList);

    // flags check when relations are done being read
    

    //REMOVE THIS WHEN UR DONE KIDDO
    for(int ii = 0; ii < empList.size(); ii ++) {
        for(int jj = 0; jj < empList[ii].size(); jj ++) {
            cout << empList[ii][jj].eid << endl;
        }
    }
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    //REMOVE THIS WHEN UR DONE KIDDO
    for(int ii = 0; ii < deptList.size(); ii ++) {
        for(int jj = 0; jj < deptList[ii].size(); jj ++) {
            cout << deptList[ii][jj].managerid << endl;
        }
    }

    mergeJoin(empList, deptList);
    
    return 0;
}
