#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
//#include <cstring>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

int main () {

    ofstream examFile("examm.txt", ios::app);
    ofstream respFile("Respexamm.txt", ios::app);
    int counter = 1;
    int numQ =  3 ;

    while (numQ > 0) {
        string question;
        string response;


        cout << "Enter question number title " << counter << ": ";
        getline(cin, question);
        examFile << question << endl  ;


        cout << "Enter response for question  number " << counter << ": ";
        getline(cin, response);
        respFile << response << endl;


        numQ--;
        counter++;
    }

    examFile.close();
    respFile.close();



    string srcExamPath = "examm.txt";
    string dstExamPath = "./students/99/exams/examm.txt";
    string srcRespPath = "Respexamm.txt";
    string dstRespPath = "./students/examm/examans/Respexamm.txt";
    ifstream srcExamFile(srcExamPath, ios::binary);
    ofstream dstExamFile(dstExamPath, ios::binary);
    ifstream srcRespFile(srcRespPath, ios::binary);
    ofstream dstRespFile(dstRespPath, ios::binary);

    dstExamFile << srcExamFile.rdbuf();
    dstRespFile << srcRespFile.rdbuf();

    srcExamFile.close();
    dstExamFile.close();
    srcRespFile.close();
    dstRespFile.close();











return 0 ;

}