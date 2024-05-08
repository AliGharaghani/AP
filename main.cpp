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

class Student {
public:
    string id, password;

    Student(string id, string password) : id(id), password(password) {}

    void showExam() {
        string path = "./students/" + id + "/exams/";
        for (const auto& entry : fs::directory_iterator(path))
            cout << "You should do this exam: " << entry.path().filename() << endl;
    }

    bool studentAuthentication() {
        string path = "./students/" + id + "/password.txt";
        ifstream file(path);
        string filePassword;
        file >> filePassword;
        if (filePassword == password) {
            cout << "Login successful." << endl;
            return true;
        } else {
            cout << "Invalid credentials." << endl;
            return false;
        }
    }

    void doExam() {
        string examName;
        cout << "Enter exam name: ";
        cin >> examName;

        ifstream examFile(examName);
        ofstream answerFile("./students/" + id + "/answers/" + examName);

        string line;
        while (getline(examFile, line)) {
            cout << line << endl;
            string answer;
            cout << "Enter your answer: ";
            cin >> answer;
            answerFile << line << " " << answer << endl;
        }
    }

    void seeResults() {
        string examName;
        cout << "Enter exam name: ";
        cin >> examName;

        ifstream examFile("./students/" + id + "/examans/Resp" + examName);
        string line;
        int count = 1;
        while (getline(examFile, line)) {
            cout << "Response to question " << count << ": " << line << endl;
            count++;
        }
    }

    void seeComments() {
        string examName;
        cout << "Enter exam name: ";
        cin >> examName;

        ifstream commentsFile("./students/" + id + "/comments/" + examName);
        string line;
        cout << "Teacher's comments:" << endl;
        while (getline(commentsFile, line)) {
            cout << line << endl;
        }
    }

    void seeGrades() {
        string examName;
        cout << "Enter exam name: ";
        cin >> examName;

        ifstream gradesFile("./students/" + id + "/Grades/" + examName);
        string line;
        cout << "Your grades:" << endl;
        while (getline(gradesFile, line)) {
            cout << line << endl;
        }
    }
};


class Teacher {
private:


public:
    string user;
    string password;
    static bool LoginStatus;


    Teacher(const string& user, const string& password) : user(user), password(password) {}

    void teacherAuthentication() {
        LoginStatus = false ;
        ifstream file("teacher.txt");
        string line;

        if (file.is_open()) {
            while (getline(file, line)) {
                string delimiter = ":";
                size_t pos = line.find(delimiter);
                string username = line.substr(0, pos);
                string pass = line.substr(pos + 1);

                if (username == user && pass == password) {
                    cout << "Login successful." << endl;
                    LoginStatus = true;
                    return ;
                }
            }
            cout << "Invalid credentials." << endl;
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    static void addExam(const string& examName, int numQ, const vector<string>& students) {
        ofstream examFile(examName + ".txt", ios::app);
        ofstream respFile("Resp" + examName + ".txt", ios::app);
        int counter = 1;

        while (numQ > 0) {
            string question;
            cout << "Enter question number title " << counter << ": ";
            getline(cin, question);
            examFile << question << endl;

            string response;
            cout << "Enter response for question number " << counter << ": ";
            getline(cin, response);
            respFile << response << endl;

            numQ--;
            counter++;
        }

        examFile.close();
        respFile.close();

        for (const auto& i : students) {
            string srcExamPath = examName + ".txt";
            string dstExamPath = "./students/" + i + "/exams/" + examName + ".txt";
            string srcRespPath = "Resp" + examName + ".txt";
            string dstRespPath = "./students/" + i + "/examans/Resp" + examName + ".txt";
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
        }
    }

    static void addStudent(const string& id, const string& password) {
        string basePath = "./students/" + id;
        mkdir(basePath.c_str(), 0777);
        mkdir((basePath + "/exams").c_str(), 0777);
        mkdir((basePath + "/answers").c_str(), 0777);
        mkdir((basePath + "/comments").c_str(), 0777);
        mkdir((basePath + "/examans").c_str(), 0777);
        mkdir((basePath + "/Grades").c_str(), 0777);

        ofstream passwordFile(basePath + "/password.txt", ios::app);
        passwordFile << password;
        passwordFile.close();
    }

    void add_comment(const string& id) {
        string exam;
        cout << "Enter exam name: ";
        cin >> exam;

        ifstream examFile("./students/" + id + "/answers/" + exam);
        ofstream commentFile("./students/" + id + "/comments/" + exam);

        if (examFile.is_open() && commentFile.is_open()) {
            string question;
            while (getline(examFile, question)) {
                string comment;
                cout << question << ": ";
                getline(cin, comment);
                commentFile << question << " " << comment << endl;
            }
        } else {
            cout << "Error: Unable to open files." << endl;
        }

        examFile.close();
        commentFile.close();
    }





    void add_grade(const string& id) {
        string examName;
        cout << "Enter exam name: ";
        getline(cin, examName);

        string answersPath = "./students/" + id + "/answers/" + examName;
        string gradesPath = "./students/" + id + "/Grades/" + examName;

        ifstream answersFile(answersPath);
        ofstream gradesFile(gradesPath);

        if (!answersFile.is_open() || !gradesFile.is_open()) {
            cerr << "Failed to open file(s)." << endl;
            return;
        }

        string line;
        while (getline(answersFile, line)) {
            cout << line << endl;
            string grade;
            cout << "Enter grade for this question: ";
            getline(cin, grade);
            gradesFile << line << " " << grade << endl;
        }

        answersFile.close();
        gradesFile.close();
    }
};




bool Teacher::LoginStatus = false;


int main ( ) {
    string userMode;
    cout << "T:Teacher\n";
    cout << "S:Student\n";
    cout << "Enter your job type: ";
    cin >> userMode;
    if ( userMode == "T" )
    {

        string username ;
        string pass;
        cout<< "enter the usernmae :) " ;
        cin >> username ;
        cout << "enter the password" ;
        cin >> pass ;

        Teacher teacher ( username , pass ) ;
        teacher.teacherAuthentication () ;
        bool loginstat = teacher.LoginStatus ;
        if ( loginstat) {



            int counter = 1 ;

            while ( counter ) {
                char action ;
                cout << "select you command fromn command list :) \n " ;
                cout <<  " 1 . add Student \n " ;
                cout <<  " 2 . Creat Exam \n" ;
                cout <<  " 3  . Add comment to the student Exam responses \n  " ;
                cout <<  " 4  . Check Grades for students and do grading :) \n " ;
                cout <<  " 0 .    Exit  \n " ;
                cin >> action ;



                if ( action == '1' ) {
                    string susername ;
                    string spass  ;
                    cout << "enter the student username : "  ;
                    cin  >> susername  ;
                    cout << "enter the student pass : "  ;
                    cin >> spass ;
                    teacher.addStudent(susername , spass ) ;
                }

                if ( action == '2') {
                    string examname ;
                    int  numq ;
                    int  numofStudents ;
//                    // how to define the the vector or array in the C++
//
//
                    cout << "enter the exam name " ;
                    cin >> examname ;
//
                    cout <<  " enter the qeuestions number ;) " ;
                    cin  >> numq  ;
                    std::vector<string> exam_st_list;
                    cout << "enter the total number of the students in the exam : ) " ;
                    cin  >> numofStudents ;
                    while (numofStudents != 0) {
                        string stid;
                        cout << "Enter the student ID: ";
                        cin >> stid;
                        // Add function to check the existing student ID

                        exam_st_list.push_back(stid);
                        numofStudents -- ;
                    }

                    teacher.addExam(examname , numq , exam_st_list ) ;




                }
                if ( action == '3') {
//                add the comment to the exam :)
                    string studientid ;
                    cout <<"enter the std id " ;
                    cin >> studientid ;
                    teacher.add_comment(studientid) ;

                } ;


                if ( action == '4') {
                    string stid ;
                    cout << "enter the stid :) " ;
                    cin >> stid ;
                    teacher.add_grade(stid ) ;



                } ;

                if ( action == '0') {
                    return  0 ;

                }



            }


        }

        return 0;
    }  }


//







//return 0;
//:x