#include <iostream>
#include <string>
#include <cmath>      // math library
using namespace std;
//C++ Final Project
//SUBMITTED BY: Rida Batool,Rubab Yasin Khan Baloch
//Submitted to: Sir Abdul Rafay Kalim

// Constants
const int MAX_USERS = 10;
const int NUM_QUESTIONS = 5;

// Structures
struct Question {
    string text;
    string options[4];
    char correct;        // 'A','B','C','D'
};

struct User {
    string name;
    int score;
};

// Enums
enum MenuOption { REGISTER = 1, TAKE_QUIZ, VIEW_LEADERBOARD, EXIT };

// Function prototypes
void displayMenu();
void addUser(User* users, int& count, const string& name);          // reference parameter
void updateScore(User& u, int points);                              // reference parameter
void takeQuiz(User* users, int userIndex, const Question questions[]);
void displayLeaderboard(const User* users, int count);
void sortLeaderboard(User* users, int count);                       // uses lambda
int factorial(int n);                                               // recursion
void printMessage(const string& msg);                               // overload 1
void printMessage(int num);                                         // overload 2

int main() {
    // Memory management: dynamic allocation
    User* users = new User[MAX_USERS];
    int userCount = 0;

    // Questions array
    Question questions[NUM_QUESTIONS] = {
        {"What is the capital of France?", {"Paris", "London", "Berlin", "Madrid"}, 'A'},
        {"Which language is used for C++?", {"Python", "Java", "C++", "JavaScript"}, 'C'},
        {"What is 5 + 7?", {"10", "11", "12", "13"}, 'C'},
        {"Who developed C++?", {"Dennis Ritchie", "Bjarne Stroustrup", "James Gosling", "Guido van Rossum"}, 'B'},
        {"Which symbol denotes a pointer?", {"&", "*", "#", "!"}, 'B'}
    };

    int choice;
    do {    // do-while loop
        displayMenu();
        cin >> choice;
        cin.ignore();   // ignore newline

        switch (choice) {
            case REGISTER: {
                if (userCount >= MAX_USERS) {
                    cout << "Leaderboard full!\n";
                    break;
                }
                string name;
                cout << "Enter your name: ";
                getline(cin, name);
                addUser(users, userCount, name);   // count passed by reference
                cout << "User registered.\n";
                break;
            }
            case TAKE_QUIZ: {
                if (userCount == 0) {
                    cout << "No users registered. Please register first.\n";
                    break;
                }
                cout << "Select user by index (0 to " << userCount-1 << "): ";
                int idx;
                cin >> idx;
                if (idx >= 0 && idx < userCount) {
                    takeQuiz(users, idx, questions);
                } else {
                    cout << "Invalid index.\n";
                }
                break;
            }
            case VIEW_LEADERBOARD:
                displayLeaderboard(users, userCount);
                break;
            case EXIT:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid option. Try again.\n";
        }
    } while (choice != EXIT);

    delete[] users;   // free dynamically allocated memory
    return 0;
}

void displayMenu() {
    cout << "\n==== Quiz System Menu ====\n";
    cout << REGISTER << ". Register User\n";
    cout << TAKE_QUIZ << ". Take Quiz\n";
    cout << VIEW_LEADERBOARD << ". View Leaderboard\n";
    cout << EXIT << ". Exit\n";
    cout << "Enter choice: ";
}

void addUser(User* users, int& count, const string& name) {
    // Using pointer arithmetic (equivalent to users[count].name = name)
    (*(users + count)).name = name;
    (*(users + count)).score = 0;
    count++;
}

void updateScore(User& u, int points) {   // reference parameter
    u.score += points;
}

void takeQuiz(User* users, int userIndex, const Question questions[]) {
    int score = 0;
    char answer;
    bool correct;

    for (int i = 0; i < NUM_QUESTIONS; i++) {   // for loop
        cout << "\nQ" << i+1 << ": " << questions[i].text << "\n";
        for (int j = 0; j < 4; j++) {            // nested for loop
            cout << char('A' + j) << ". " << questions[i].options[j] << "\n";
        }

        // Input validation with while loop
        bool valid;
        do {
            cout << "Your answer (A/B/C/D, S=skip, Q=quit): ";
            cin >> answer;
            answer = toupper(answer);
            valid = (answer >= 'A' && answer <= 'D') || answer == 'S' || answer == 'Q';
            if (!valid) cout << "Invalid. ";
        } while (!valid);

        if (answer == 'Q') {    // break example
            cout << "Quitting quiz.\n";
            break;
        }
        if (answer == 'S') {    // continue example
            cout << "Question skipped.\n";
            continue;
        }

        // Check answer
        if (answer == questions[i].correct) {
            cout << "Correct!\n";
            score++;
        } else {
            cout << "Wrong! Correct answer: " << questions[i].correct << "\n";
        }
    }

    // else-if example (trivial)
    if (score == NUM_QUESTIONS) {
        cout << "Perfect score!\n";
    } else if (score >= NUM_QUESTIONS / 2) {
        cout << "Good effort!\n";
    } else {
        cout << "Better luck next time.\n";
    }

    // Recursion for bonus (factorial)
    int bonus = factorial(score) / 2;   // just for demonstration
    score += bonus;

    // Update user's score using reference function
    updateScore(users[userIndex], score);

    cout << "\nQuiz finished! Your score: " << score << " (bonus: " << bonus << ")\n";
}

void displayLeaderboard(const User* users, int count) {
    if (count == 0) {
        cout << "No users to display.\n";
        return;
    }

    // Copy users to a temporary array for sorting (preserve original)
    User sortedUsers[MAX_USERS];
    for (int i = 0; i < count; i++) {
        sortedUsers[i] = *(users + i);   // pointer dereference
    }

    // Lambda expression for comparison
    auto compare = [](const User& a, const User& b) -> bool {
        return a.score > b.score;
    };

    // Bubble sort using lambda (manual sorting, no STL)
    for (int i = 0; i < count-1; i++) {
        for (int j = 0; j < count-i-1; j++) {
            if (!compare(sortedUsers[j], sortedUsers[j+1])) {
                // Swap
                User temp = sortedUsers[j];
                sortedUsers[j] = sortedUsers[j+1];
                sortedUsers[j+1] = temp;
            }
        }
    }

    cout << "\n==== Leaderboard ====\n";
    for (int i = 0; i < count; i++) {
        User* u = &sortedUsers[i];   // pointer to user
        cout << i+1 << ". " << u->name << " - Score: " << u->score;
        // Using math library (sqrt)
        double perf = sqrt(u->score);
        cout << " (Perf: " << perf << ")\n";
    }
}

int factorial(int n) {   // recursion
    if (n <= 1) return 1;
    return n * factorial(n-1);
}

// Overloaded functions
void printMessage(const string& msg) {
    cout << msg << endl;
}

void printMessage(int num) {
    cout << "Number: " << num << endl;
}