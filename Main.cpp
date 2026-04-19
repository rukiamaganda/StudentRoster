#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>

struct Student {
    std::string id;
    std::string name;
    int age;
};

std::vector<Student> students;


bool isValidID(std::string id) {
    if (id.length() != 8) return false;

    if (!(isdigit(id[0]) && isdigit(id[1]))) return false;
    if (!isalpha(id[2])) return false;
    if (id[3] != '-') return false;
    if (!(isdigit(id[4]) && isdigit(id[5]) && isdigit(id[6]) && isdigit(id[7]))) return false;

    return true;
}


void loadFile() {
    std::ifstream file("students.txt");
    std::string line;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, name, ageStr;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, ageStr, ',');

        Student s;
        s.id = id;
        s.name = name;
        s.age = std::stoi(ageStr); // parsing string → int

        students.push_back(s);
    }

    file.close();
}


void saveFile() {
    std::ofstream file("students.txt");

    for (int i = 0; i < students.size(); i++) {
        file << students[i].id << ","
             << students[i].name << ","
             << students[i].age << "\n";
    }

    file.close();
}

// ================= CREATE =================
void createStudent() {
    Student s;

    std::cin.ignore();

    std::cout << "Enter a new ID (Format Example: 25A-0030): ";
    getline(std::cin, s.id);

    if (!isValidID(s.id)) {
        std::cout << "Invalid ID format!\n";
        return;
    }

    std::cout << "Enter your Name (Surname, First Name, M.I): ";
    getline(std::cin, s.name);

    std::cout << "Enter your Age: ";
    std::cin >> s.age;

    students.push_back(s);
    saveFile();

    std::cout << "Student added!\n";
}

// ================= READ =================
void readStudents() {
    if (students.empty()) {
        std::cout << "No records found.\n";
        return;
    }

    std::cout << "\n--- STUDENT LIST ---\n";
    for (int i = 0; i < students.size(); i++) {
        std::cout << students[i].id << " | "
                  << students[i].name << " | "
                  << students[i].age << "\n";
    }
}

// ================= UPDATE =================
void updateStudent() {
    std::string id;

    std::cin.ignore();
    std::cout << "Enter ID to update: ";
    getline(std::cin, id);

    for (int i = 0; i < students.size(); i++) {
        if (students[i].id == id) {

            std::cout << "New Name: ";
            getline(std::cin, students[i].name);

            std::cout << "New Age: ";
            std::cin >> students[i].age;

            saveFile();
            std::cout << "Updated!\n";
            return;
        }
    }

    std::cout << "Not found.\n";
}

// ================= DELETE =================
void deleteStudent() {
    std::string id;

    std::cin.ignore();
    std::cout << "Enter ID to delete: ";
    getline(std::cin, id);

    for (int i = 0; i < students.size(); i++) {
        if (students[i].id == id) {
            students.erase(students.begin() + i);
            saveFile();

            std::cout << "Deleted!\n";
            return;
        }
    }

    std::cout << "Not found.\n";
}


int main() {
    loadFile(); 

    char choice;

    do {
        std::cout << "\n===== STUDENT ROSTER =====\n";
        std::cout << "[1] Create\n";
        std::cout << "[2] Read\n";
        std::cout << "[3] Update\n";
        std::cout << "[4] Delete\n";
        std::cout << "[5] Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case '1': createStudent(); break;
            case '2': readStudents(); break;
            case '3': updateStudent(); break;
            case '4': deleteStudent(); break;
            case '5': std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice!\n";
        }

    } while (choice != '5');

    return 0;
}