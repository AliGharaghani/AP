#include <iostream>
#include <vector>
#include <string>

// Assuming Teacher class with addExam method is defined somewhere
class Teacher {
public:
    void addExam(std::string examName, int numq, std::vector<int> students);
};

int main() {
    Teacher teacher;
    int action;
    std::cout << "Enter action number: ";
    std::cin >> action;

    if (action == 2) {
        int counter, numq;
        std::string examname;
        std::vector<int> exam_st_list;

        std::cout << "Enter the total number of students: ";
        std::cin >> counter;

        std::cout << "Enter the number of questions: ";
        std::cin >> numq;

        std::cout << "Enter the exam name: ";
        std::cin.ignore();
        std::getline(std::cin, examname);

        while (counter != 0) {
            int stid;
            std::cout << "Enter the student ID: ";
            std::cin >> stid;
            // Add function to check the existing student ID

            exam_st_list.push_back(stid);
            counter--;
        }

        teacher.addExam(examname, numq, exam_st_list);
    }

    return 0;
}