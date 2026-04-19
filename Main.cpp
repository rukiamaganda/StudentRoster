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

//  DATA SANITATION 
bool isValidID(std::string id) {
    if (id.length() != 8) return false;

    std::string part1 = id.substr(0, 2);
    std::string letter = id.substr(2, 1);
    std::string dash = id.substr(3, 1);
    std::string part2 = id.substr(4, 4);

    for (char c : part1 + part2) {
        if (!isdigit(c)) return false;
    }

    if (!isalpha(letter[0])) return false;
    if (dash != "-") return false;

    return true;
}

//  LOAD STATE 
void loadFile() {
    students.clear(); // 🔥 prevents duplication

    std::ifstream file("students.txt");
    if (!file.is_open()) return;

    std::string line;

    while (getline(file, line)) {
        std::stringstream ss(line);

        std::string id, name, ageStr;

        // TOKENIZATION
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, ageStr, ',');

        if (id.empty() || name.empty() || ageStr.empty()) continue;

        Student s;
        s.id = id;
        s.name = name;

        // PARSING
        s.age = std::stoi(ageStr);

        students.push_back(s);
    }

    file.close();
}

//  SAVE STATE 
void saveFile() {
    std::ofstream file("students.txt");

    for (int i = 0; i < students.size(); i++) {
        file << students[i].id << ","
             << students[i].name << ","
             << students[i].age << "\n";
    }

    file.close();
}

//CREATE
void createStudent() {
    Student s;

    std::cin.ignore();

    std::cout << "\n=== OLOPSC ENROLLMENT ===\n";
    std::cout << "Enter Student ID (25A-0030): ";
    getline(std::cin, s.id);

    if (!isValidID(s.id)) {
        std::cout << "Invalid ID format!\n";
        return;
    }

    std::cout << "Enter Full Name: ";
    getline(std::cin, s.name);

    std::cout << "Enter Age: ";
    std::cin >> s.age;

    if (s.age < 0) {
        std::cout << "Invalid age!\n";
        return;
    }

    students.push_back(s);

    std::cout << "Enrollment successful! Welcome to OLOPSC!\n";
}

//  READ 
void readStudents() {
    loadFile(); 

    if (students.empty()) {
        std::cout << "No records found.\n";
        return;
    }

    std::cout << "\n--- OLOPSC STUDENT LIST ---\n";
    for (int i = 0; i < students.size(); i++) {
        std::cout << students[i].id << " | "
                  << students[i].name << " | "
                  << students[i].age << "\n";
    }
}

//UPDATE 
void updateStudent() {
    loadFile();

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

            std::cout << "Record updated!\n";
            return;
        }
    }

    std::cout << "Student not found.\n";
}

// Delete
void deleteStudent() {
    loadFile(); // 🔥

    std::string id;

    std::cin.ignore();
    std::cout << "Enter ID to delete: ";
    getline(std::cin, id);

    for (int i = 0; i < students.size(); i++) {
        if (students[i].id == id) {

            students.erase(students.begin() + i);

            std::cout << "Record deleted.\n";
            return;
        }
    }

    std::cout << "Student not found.\n";
}

// Main
int main() {

    loadFile();

    char choice;

    do {
        std::cout << "\n===== OLOPSC STUDENT ROSTER =====\n";
        std::cout << "[1] Enroll Student\n";
        std::cout << "[2] View Students\n";
        std::cout << "[3] Update Record\n";
        std::cout << "[4] Delete Record\n";
        std::cout << "[5] Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case '1': createStudent(); break;
            case '2': readStudents(); break;
            case '3': updateStudent(); break;
            case '4': deleteStudent(); break;

            case '5':
                saveFile();
                std::cout << "Data saved. Thank you!\n";
                break;

            default:
                std::cout << "Invalid choice!\n";
        }

    } while (choice != '5');

    return 0;
}