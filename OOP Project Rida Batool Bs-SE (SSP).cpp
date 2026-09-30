/* Project Title: "Ultimate University Management and Registration Dev-C++ Multi Module Architecture"
Submitted to: Dr. Mohsin Nazir
Submitted by: Rida Batool
Roll no.: 25212522056
Semester: Second
Program: Bs-SE (SSP) 2025-2029
Subject: OOP (Object Oriented Programming)*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>    // ostringstream, stringstream
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <iomanip>
#include <cstdlib>    // for system(), atoi()
using namespace std;

// ==========================================
//  TEMPLATE CONCEPT (Utility Function)
// ==========================================
template <typename T>
void printCentered(T text, int width = 60) {
    ostringstream oss;
    oss << text;             // works for any type with operator<<
    string s = oss.str();
    int len = s.length();
    if (len >= width) {
        cout << s << endl;
        return;
    }
    int padding = (width - len) / 2;
    cout << string(padding, ' ') << s << string(width - padding - len, ' ') << endl;
}

void printCentered(string s, int width = 60) {
    int len = s.length();
    if (len >= width) {
        cout << s << endl;
        return;
    }
    int padding = (width - len) / 2;
    cout << string(padding, ' ') << s << string(width - padding - len, ' ') << endl;
}

// UI Borders Utility
void printBorder(char ch = '=', int width = 60) {
    cout << string(width, ch) << endl;
}

// ==========================================
//  ABSTRACTION & BASE CLASS
// ==========================================
class Person {
protected:
    string name;
    int id;
public:
    Person(string n = "Unknown", int i = 0) : name(n), id(i) {}
    virtual ~Person() {}

    string getName() const { return name; }
    int getId() const { return id; }

    virtual void displayDetails() const = 0; 
};

// ==========================================
//  COURSE ClASS (Aggregation/Association)
// ==========================================
class Course {
private:
    string courseCode;
    string courseTitle;
    int maxSeats;
    int availableSeats;
    Course* prerequisite;   // pointer to prerequisite course

public:
    Course() : courseCode(""), courseTitle(""), maxSeats(50), availableSeats(50), prerequisite(NULL) {}
    
    Course(string code, string title, int seats, Course* prereq = NULL) {
        courseCode = code;
        courseTitle = title;
        maxSeats = seats;
        availableSeats = seats;
        prerequisite = prereq;
    }

    Course(const Course& other) {
        courseCode = other.courseCode;
        courseTitle = other.courseTitle;
        maxSeats = other.maxSeats;
        availableSeats = other.availableSeats;
        prerequisite = other.prerequisite;
    }

    string getCourseCode() const { return courseCode; }
    string getCourseTitle() const { return courseTitle; }
    Course* getPrerequisite() const { return prerequisite; }
    int getAvailableSeats() const { return availableSeats; }

    bool reserveSeat() {
        if (availableSeats > 0) {
            availableSeats--;
            return true;
        }
        return false;
    }

    bool operator==(const Course& other) const {
        return this->courseCode == other.courseCode;
    }
};

// ==========================================
//  INHERITANCE & RUNTIME POLYMORPHISM
// ==========================================
class Student : public Person {
private:
    vector<string> passedCourses;
    vector<Course> registeredCourses;
    static int totalStudents;

public:
    Student(string n = "Unknown", int i = 0) : Person(n, i) {
        totalStudents++;
    }

    // Copy constructor that correctly increments static counter
    Student(const Student& other)
        : Person(other), passedCourses(other.passedCourses), registeredCourses(other.registeredCourses) {
        totalStudents++;
    }

    ~Student() {
        totalStudents--;
    }

    static int getTotalStudents() { return totalStudents; }

    void addPassedCourse(string code) {
        passedCourses.push_back(code);
    }

    bool hasPassed(string code) const {
        for (size_t i = 0; i < passedCourses.size(); ++i)
            if (passedCourses[i] == code) return true;
        return false;
    }

    bool isAlreadyRegistered(string code) const {
        for (size_t i = 0; i < registeredCourses.size(); ++i)
            if (registeredCourses[i].getCourseCode() == code) return true;
        return false;
    }

    void registerInCourse(Course& course) {
        if (isAlreadyRegistered(course.getCourseCode()))
            throw runtime_error("Error: You are already registered in this course!");

        if (course.getAvailableSeats() <= 0)
            throw runtime_error("Error: No seats available in this course!");

        if (course.getPrerequisite() != NULL) {
            string prereqCode = course.getPrerequisite()->getCourseCode();
            if (!hasPassed(prereqCode))
                throw runtime_error("Prerequisite Violation! You must pass '" + prereqCode + "' first.");
        }

        if (course.reserveSeat()) {
            registeredCourses.push_back(course);
            cout << "\n\t[SUCCESS] Registration in " << course.getCourseTitle() << " was successful!\n";
        }
    }

    void generateReportCard() const {
        printBorder('-', 50);
        cout << "  ACADEMIC SUMMARY FOR: " << name << " (ID: " << id << ")\n";
        printBorder('-', 50);
        cout << "  Completed Courses:\n";
        if (passedCourses.empty()) cout << "    - None\n";
        for (size_t i = 0; i < passedCourses.size(); ++i)
            cout << "    [CR] " << passedCourses[i] << " - Passed\n";
        
        cout << "\n  Current Semester Enrollments:\n";
        if (registeredCourses.empty()) cout << "    - No active registrations\n";
        for (size_t i = 0; i < registeredCourses.size(); ++i)
            cout << "    [REG] " << registeredCourses[i].getCourseCode() << " - " << registeredCourses[i].getCourseTitle() << "\n";
        printBorder('-', 50);
    }

    void displayDetails() const override {
        cout << left << setw(10) << id << setw(20) << name << " | Registered in: ";
        if (registeredCourses.empty()) {
            cout << "None";
        } else {
            for (size_t i = 0; i < registeredCourses.size(); ++i)
                cout << registeredCourses[i].getCourseCode() << " ";
        }
        cout << endl;
    }

    void saveToFile(ofstream& out) const {
        out << id << "," << name << ",";
        out << passedCourses.size() << ",";
        for (size_t i = 0; i < passedCourses.size(); ++i)
            out << passedCourses[i] << ",";
        out << registeredCourses.size() << ",";
        for (size_t i = 0; i < registeredCourses.size(); ++i)
            out << registeredCourses[i].getCourseCode() << ",";
        out << "\n";
    }
};

int Student::totalStudents = 0;

// ==========================================
// INHERITANCE (Teacher)
// ==========================================
class Teacher : public Person {
private:
    string designation;
    string department;
public:
    Teacher(string n = "Unknown", int i = 0, string des = "Lecturer", string dept = "CS") 
        : Person(n, i), designation(des), department(dept) {}

    void displayDetails() const override {
        cout << left << setw(10) << id << setw(20) << name 
             << " | Dept: " << setw(6) << department 
             << " | Desig: " << designation << endl;
    }
};

// ==========================================
// SYSTEM CONTROLLER
// ==========================================
class RegistrationSystem {
private:
    vector<Course> universityCourses; 
    vector<Student> studentDirectory;
    vector<Teacher> facultyDirectory;

public:
    void addCourse(const Course& c) { universityCourses.push_back(c); }
    void addStudent(const Student& s) { studentDirectory.push_back(s); }
    void addTeacher(const Teacher& t) { facultyDirectory.push_back(t); }

    Course* findCourse(string code) {
        for (size_t i = 0; i < universityCourses.size(); ++i)
            if (universityCourses[i].getCourseCode() == code) return &universityCourses[i];
        return NULL;
    }

    Student* findStudent(int id) {
        for (size_t i = 0; i < studentDirectory.size(); ++i)
            if (studentDirectory[i].getId() == id) return &studentDirectory[i];
        return NULL;
    }

    void showAvailableCourses() const {
        printBorder('-', 65);
        cout << left << setw(12) << "Code" << setw(30) << "Course Title" << setw(10) << "Seats" << setw(15) << "Prerequisite" << endl;
        printBorder('-', 65);
        for (size_t i = 0; i < universityCourses.size(); ++i) {
            const Course& c = universityCourses[i];
            cout << left << setw(12) << c.getCourseCode()
                 << setw(30) << c.getCourseTitle()
                 << setw(10) << c.getAvailableSeats()
                 << setw(15) << (c.getPrerequisite() ? c.getPrerequisite()->getCourseCode() : "None")
                 << endl;
        }
        printBorder('-', 65);
    }

    void showAllStudents() const {
        printBorder('-', 65);
        cout << left << setw(10) << "ID" << setw(20) << "Student Name" << "Registered Courses" << endl;
        printBorder('-', 65);
        for (size_t i = 0; i < studentDirectory.size(); ++i)
            studentDirectory[i].displayDetails();
        printBorder('-', 65);
        cout << "Total Active Students in Memory: " << Student::getTotalStudents() << endl;
    }

    void showFaculty() const {
        printBorder('-', 65);
        cout << left << setw(10) << "Faculty ID" << setw(20) << "Teacher Name" << "Department & Designation" << endl;
        printBorder('-', 65);
        if (facultyDirectory.empty()) cout << "\tNo faculty records initialized.\n";
        for (size_t i = 0; i < facultyDirectory.size(); ++i)
            facultyDirectory[i].displayDetails();
        printBorder('-', 65);
    }

    void saveSystemState() {
        ofstream studentFile("students_db.txt", ios::out);
        if (!studentFile) {
            cout << "Error: Could not save file!\n";
            return;
        }
        for (size_t i = 0; i < studentDirectory.size(); ++i)
            studentDirectory[i].saveToFile(studentFile);
        studentFile.close();
        cout << "\n\t[SYSTEM] Data successfully saved to 'students_db.txt'.\n";
    }

    void loadSystemState() {
        ifstream studentFile("students_db.txt", ios::in);
        if (!studentFile) return;   // no file → keep default data

        studentDirectory.clear();
        string line;
        while (getline(studentFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, nameStr, passedCountStr, regCountStr;
            
            getline(ss, idStr, ',');
            getline(ss, nameStr, ',');
            getline(ss, passedCountStr, ',');
            
            int id = atoi(idStr.c_str());                // fixed: no stoi()
            int passedCount = atoi(passedCountStr.c_str()); // fixed
            
            Student loadedStudent(nameStr, id);
            
            for (int i = 0; i < passedCount; ++i) {
                string pCourse;
                getline(ss, pCourse, ',');
                loadedStudent.addPassedCourse(pCourse);
            }
            
            getline(ss, regCountStr, ',');
            int regCount = atoi(regCountStr.c_str());   // fixed
            for (int i = 0; i < regCount; ++i) {
                string rCourseCode;
                getline(ss, rCourseCode, ',');
                Course* targetC = findCourse(rCourseCode);
                if (targetC != NULL) {
                    try {
                        loadedStudent.registerInCourse(*targetC);
                    } catch (...) {
                        // silently ignore load‑time registration errors
                    }
                }
            }
            studentDirectory.push_back(loadedStudent);
        }
        studentFile.close();
        cout << "\n\t[SYSTEM] Existing database file 'students_db.txt' loaded successfully into cache!\n";
    }
};

// ==========================================
// MAIN INTERFACE
// ==========================================
int main() {
    RegistrationSystem sys;

    // 1. Setup courses (prerequisite pointers point to stack objects – safe as long as they remain in scope)
    Course pf("CS101", "Programming Fundamentals", 50, NULL);
    Course oop("CS201", "Object-Oriented Programming", 45, &pf);
    Course ds("CS202", "Data Structures", 40, &oop);
    sys.addCourse(pf);
    sys.addCourse(oop);
    sys.addCourse(ds);

    // 2. Setup faculty
    sys.addTeacher(Teacher("Dr. Kamran", 501, "Professor", "CS"));
    sys.addTeacher(Teacher("Engr. Sana", 502, "Assistant Prof", "SE"));

    // 3. Add default students (temporary objects keep the static counter correct)
    sys.addStudent(Student("Ali Ahmed", 2401));
    if (Student* s = sys.findStudent(2401))
        s->addPassedCourse("CS101");
    sys.addStudent(Student("Ayesha Khan", 2402));

    // 4. Try to load previously saved data (if any)
    sys.loadSystemState();

    int choice;
    do {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printBorder();
        printCentered("ULTIMATE UNIVERSITY MANAGEMENT & REGISTRATION");
        printCentered("DEV-C++ MULTI-MODULE ARCHITECTURE");
        printBorder();
        cout << "\n\t1. View Available Courses\n";
        cout << "\t2. View Registered Students List\n";
        cout << "\t3. View Faculty Directory\n";
        cout << "\t4. New Student Course Registration\n";
        cout << "\t5. Generate Student Academic Report Card\n";
        cout << "\t6. Save Current State to Database File\n";
        cout << "\t7. Exit Program\n\n";
        printBorder('-');
        cout << "Enter your choice (1-7): ";
        
        if (!(cin >> choice)) {
            cin.clear(); 
            cin.ignore(1000, '\n');
            choice = 0;
        }

        switch (choice) {
            case 1:
                #ifdef _WIN32
                    system("cls");
                #endif
                printBorder();
                printCentered("AVAILABLE UNIVERSITY COURSES");
                printBorder();
                sys.showAvailableCourses();
                cout << "\nPress Enter to return to Main Menu...";
                cin.ignore(1000, '\n'); cin.get();
                break;

            case 2:
                #ifdef _WIN32
                    system("cls");
                #endif
                printBorder();
                printCentered("STUDENT ENROLLMENT DIRECTORY");
                printBorder();
                sys.showAllStudents();
                cout << "\nPress Enter to return to Main Menu...";
                cin.ignore(1000, '\n'); cin.get();
                break;

            case 3:
                #ifdef _WIN32
                    system("cls");
                #endif
                printBorder();
                printCentered("UNIVERSITY FACULTY MEMBERS");
                printBorder();
                sys.showFaculty();
                cout << "\nPress Enter to return to Main Menu...";
                cin.ignore(1000, '\n'); cin.get();
                break;

            case 4: {
                #ifdef _WIN32
                    system("cls");
                #endif
                printBorder();
                printCentered("NEW COURSE REGISTRATION PORTAL");
                printBorder();
                
                int sId;
                string cCode;
                cout << "\nEnter Student ID (e.g., 2401, 2402): ";
                cin >> sId;
                
                Student* stud = sys.findStudent(sId);
                if (stud == NULL) {
                    cout << "\n\t[ERROR] Student ID not found!\n";
                    cin.ignore(1000, '\n'); cin.get();
                    break;
                }

                cout << "Enter Course Code to register (CS101, CS201, CS202): ";
                cin >> cCode;

                Course* crs = sys.findCourse(cCode);
                if (crs == NULL) {
                    cout << "\n\t[ERROR] Invalid Course Code!\n";
                    cin.ignore(1000, '\n'); cin.get();
                    break;
                }

                try {
                    stud->registerInCourse(*crs);
                } 
                catch (const runtime_error& e) {
                    cout << "\n\t[REGISTRATION FAILED] " << e.what() << "\n";
                }

                cout << "\nPress Enter to continue...";
                cin.ignore(1000, '\n'); cin.get();
                break;
            }

            case 5: {
                #ifdef _WIN32
                    system("cls");
                #endif
                printBorder();
                printCentered("STUDENT REPORT CARD GENERATOR");
                printBorder();
                
                int sId;
                cout << "\nEnter Student ID to generate summary: ";
                cin >> sId;
                
                Student* stud = sys.findStudent(sId);
                if (stud != NULL) {
                    cout << endl;
                    stud->generateReportCard();
                } else {
                    cout << "\n\t[ERROR] Student record not found.\n";
                }
                
                cout << "\nPress Enter to continue...";
                cin.ignore(1000, '\n'); cin.get();
                break;
            }

            case 6:
                sys.saveSystemState();
                cout << "\nPress Enter to continue...";
                cin.ignore(1000, '\n'); cin.get();
                break;

            case 7:
                printBorder();
                printCentered("Thank you for using the Project! System shutting down cleanly.");
                printBorder();
                break;

            default:
                cout << "\n\t[WARNING] Invalid choice! Please try again.\n";
                cout << "\nPress Enter to continue...";
                cin.ignore(1000, '\n'); cin.get();
                break;
        }
    } while (choice != 7);

    return 0;
}