#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <limits>

struct Student {
    std::string id;
    std::string name;
    int age;
};

std::vector<Student> students;

//  DATA SANITATION 
bool isValidID(std::string id) {

    id.erase(remove(id.begin(), id.end(), ' '), id.end());

    for (char &c : id) {
        c = toupper(c);
    }

    if (id.length() != 8) return false;

    if (id.substr(0, 2) != "25") return false;
    if (id[2] != 'A') return false;
    if (id[3] != '-') return false;

    for (int i = 4; i < 8; i++) {
        if (!isdigit(id[i])) return false;
    }

    return true;
}

std::string formatName(std::string name) {
    std::stringstream ss(name);
    std::string surname, firstname, mi;

    ss >> surname >> firstname >> mi;

    if (surname.empty() || firstname.empty() || mi.empty()) return name;

    surname[0] = toupper(surname[0]);
    firstname[0] = toupper(firstname[0]);
    mi[0] = toupper(mi[0]);

    for (int i = 1; i < surname.length(); i++) surname[i] = tolower(surname[i]);
    for (int i = 1; i < firstname.length(); i++) firstname[i] = tolower(firstname[i]);

    if (!mi.empty() && mi.back() != '.') {
        mi += ".";
    } 

    return surname + " " + firstname + " " + mi;
}

bool isValidNameFormat(std::string name) {
    std::stringstream ss(name);
    std::string surname, firstname, mi, extra;

    ss >> surname >> firstname >> mi >> extra;

    // must have exactly 3 parts only
    if (surname.empty() || firstname.empty() || mi.empty() || !extra.empty())
        return false;

    if (mi.empty()) return false;

    if (mi.length() == 1) {
        if (!isalpha(mi[0])) return false;
    }
    else if (mi.length() == 2) {
        if (!isalpha(mi[0]) || mi[1] != '.') return false;
    }
    else {
        return false;
    }

    return true;
}

bool isDuplicateID(std::string id) {

    id.erase(remove(id.begin(), id.end(), ' '), id.end());
    for (char &c : id) c = toupper(c);

    for (int i = 0; i < students.size(); i++) {

        std::string existing = students[i].id;

        existing.erase(remove(existing.begin(), existing.end(), ' '), existing.end());
        for (char &c : existing) c = toupper(c);

        if (existing == id) return true;
    }

    return false;
}

//  LOAD STATE 
void loadFile() {
    students.clear(); // 

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

        ageStr.erase(remove(ageStr.begin(), ageStr.end(), '\r'), ageStr.end());
        
        if (id.empty() || name.empty() || ageStr.empty()) continue;

        Student s;
        s.id = id;
        // remove spaces
        s.id.erase(remove(s.id.begin(), s.id.end(), ' '), s.id.end());

        // force uppercase
        for (char &c : s.id) {
            c = toupper(c);
        }   

        s.name = name;

        // PARSING
        try {
            s.age = std::stoi(ageStr);
        } catch (...) {
            continue;
        }

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
    loadFile();
    Student s;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\n=== OLOPSC ENROLLMENT ===\n";
    std::cout << "Enter Student ID (Required Format: -25A-0000- (25A only and any 4 digit numbers)): ";
    getline(std::cin, s.id);

    for (char &c : s.id) {
    c = toupper(c);
    }

    if (!isValidID(s.id)) { 
        std::cout << "Invalid ID format!\n";
        return;
    }

    if (isDuplicateID(s.id)) {
        std::cout << "ID already exists!\n";
        return;
    }

    std::cout << "Enter Full Name Format: Surname Firstname MI (no Quotation mark strictly follow the format) ";
    getline(std::cin, s.name);

    if (!isValidNameFormat(s.name)) {
        std::cout << "Invalid name format! Use: Surname Firstname MI\n";
        return;
    }

    s.name = formatName(s.name);

    std::cout << "Enter Age (17-75 years old only): ";
        std::cin >> s.age;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (s.age < 17 || s.age > 75) {
        std::cout << "Invalid age! Must be 17 to 75 only.\n";
        return;
    }

    students.push_back(s);

    saveFile();
    
    std::cout << "Enrollment successful!\n";
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

    std::string id;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter ID to update: ";
    getline(std::cin, id);

    for (char &c : id) {
        c = toupper(c);
    }

    for (int i = 0; i < students.size(); i++) {
        if (students[i].id == id) {

            std::cout << "New Name Format: Surname Firstname MI (No Quotation mark strictly follow the format): ";
            getline(std::cin, students[i].name);

            if (!isValidNameFormat(students[i].name)) {
                std::cout << "Invalid format!\n";
                return;
            }

            students[i].name = formatName(students[i].name);

            std::cout << "New Age (17-75 years old): ";
            std::cin >> students[i].age;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (students[i].age < 17 || students[i].age > 75) {
                std::cout << "Invalid age! Must be 17 to 75 only.\n";
                return;
            }

            std::cout << "Record updated!\n";
            saveFile();
            return;
        }
    }

    std::cout << "Student not found.\n";
}

// Delete
void deleteStudent() {

    std::string id;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter ID to delete: ";
    getline(std::cin, id);

    for (char &c : id) {
        c = toupper(c);
    }

    for (int i = 0; i < students.size(); i++) {
        if (students[i].id == id) {

            students.erase(students.begin() + i);

            std::cout << "Record deleted.\n";
            saveFile();
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
        std::cout << "\n==== OLOPSC ROSTER FOR STUDENT LIST (2025) ====\n";
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