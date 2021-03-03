//Zach Kauffman and David Kalcic
//CS-440 Wi21

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_BLOCK_SIZE 11

struct EmpBlock{
    int eid;
    string ename;
    int age;
    double salary;

    bool operator()(EmpBlock const& a, EmpBlock const& b) const{
        return a.eid < b.eid;
    }
};

struct DeptBlock{
    int did;
    string dname;
    double budget;
    int managerid;

    bool operator()(DeptBlock const& a, DeptBlock const& b) const{
        return a.managerid < b.managerid;
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

// Grab a single block from the dept.csv file, in theory if a block was larger than
// one tuple, this function would return an array of DeptBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
DeptBlock grabDept(fstream& deptin){
    string line, word;
    DeptBlock dept;
    if(getline(deptin, line, '\n')){
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
    } else{
        dept.did = -1;
        return dept;
    }
}

//Print out the attributes from emp and dept when a join condition is met
void printJoin(EmpBlock emp, DeptBlock dept, fstream& fout){
    fout << dept.did << ',' << dept.dname << ',' << dept.budget << ','
         << dept.managerid << ',' << emp.eid << ',' << emp.ename << ','
         << emp.age << ',' << emp.salary << "\n";
}

void mergeJoin(vector<vector<EmpBlock> > empList, vector<vector<DeptBlock> > deptList){
    fstream joinout;
    joinout.open("join.csv", ios::out | ios::trunc);

    int empIdx1 = 0, empIdx2 = 0, deptIdx1 = 0, deptIdx2 = 0;
    while(true){

        if(empList[empIdx1][empIdx2].eid == deptList[deptIdx1][deptIdx2].managerid){
            printJoin(empList[empIdx1][empIdx2], deptList[deptIdx1][deptIdx2], joinout);
            deptIdx2++;
        } else if(empList[empIdx1][empIdx2].eid < deptList[deptIdx1][deptIdx2].managerid){
            empIdx2++;
        } else if(empList[empIdx1][empIdx2].eid > deptList[deptIdx1][deptIdx2].managerid){
            deptIdx2++;
        }
        if(empIdx2 == MAX_BLOCK_SIZE){
            empIdx1++;
            empIdx2 = 0;
        }
        if(deptIdx2 == MAX_BLOCK_SIZE){
            deptIdx1++;
            deptIdx2 = 0;
        }
        if(empIdx1 == empList.size() || empIdx2 == empList[empIdx1].size()){
            break;
        }
        if(deptIdx1 == empList.size() || deptIdx2 == deptList[deptIdx1].size()){
            break;
        }
    }
    joinout.close();
}

vector<vector<EmpBlock> > sortAndStoreEmps(vector<vector<EmpBlock> > empList){
    fstream empin;
    vector<EmpBlock> tempEmp = {};
    vector<EmpBlock> smallTempEmp = {};

    
    empin.open("Emp.csv", ios::in);
    while(true){
        // FOR BLOCK IN RELATION EMP
        // grabs a block
        EmpBlock empBlock = grabEmp(empin);
        // checks if filestream is empty
        if(empBlock.eid == -1){
            break;
        } else{
            tempEmp.push_back(empBlock);
        }
    }
    // grab all employees and sort by eid
    sort(tempEmp.begin(), tempEmp.end(), EmpBlock());
    int runs = 0;

    // split employees into groups of 11 per memory constraints
    for(int ii = 0; ii < tempEmp.size() % MAX_BLOCK_SIZE; ii++){
        for(int jj = 0; jj < MAX_BLOCK_SIZE; jj++){
            runs++;
            if(runs < tempEmp.size() + 1){
                smallTempEmp.push_back(tempEmp[jj + ii * MAX_BLOCK_SIZE]);
            } else{
                break;
            }
        }
        empList.push_back(smallTempEmp);
        smallTempEmp.clear();
    }
    empin.close();
    return empList;

}

vector<vector<DeptBlock> > sortAndStoreDepts(vector<vector<DeptBlock> > deptList){
    fstream deptin;
    vector<DeptBlock> tempDept = {};
    vector<DeptBlock> smallTempDept = {};

    deptin.open("Dept.csv", ios::in);

    while(true){
        // FOR BLOCK IN RELATION DEPT
        // grabs a block
        DeptBlock deptBlock = grabDept(deptin);
        // checks if filestream is empty
        if(deptBlock.did == -1){
            break;
        } else{
            tempDept.push_back(deptBlock);
        }
    }


    // grab all departments and sort by managerid
    sort(tempDept.begin(), tempDept.end(), DeptBlock());
    int runs = 0;

    // split departments into groups of 11 per memory constraints

    for(int ii = 0; ii < tempDept.size() % MAX_BLOCK_SIZE; ii++){
        for(int jj = 0; jj < MAX_BLOCK_SIZE; jj++){
            runs++;
            if(runs < tempDept.size() + 1){
                smallTempDept.push_back(tempDept[jj + ii * MAX_BLOCK_SIZE]);
            } else{
                break;
            }
        }
        deptList.push_back(smallTempDept);
        smallTempDept.clear();
    }

    deptin.close();
    return deptList;

}

int main(){
    vector<vector<EmpBlock> > empList;
    empList = sortAndStoreEmps(empList);

    vector<vector<DeptBlock> > deptList;
    deptList = sortAndStoreDepts(deptList);

    mergeJoin(empList, deptList);

    return 0;
}
