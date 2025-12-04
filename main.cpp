/**
 * HOSPITAL PATIENT MANAGEMENT SYSTEM
 * ----------------------------------
 * Semester Project: Data Structures and Algorithms
 * Prepared by: Muhammad Umer Farooq (Leader) & Abdur Rehman
 *
 * ADJUSTMENTS:
 * - ADDED: File Handling (patients.txt) to save/load data.
 * - KEPT REMOVED: No "buffer spaces" (screen clearing gaps).
 * - Fixed input buffering for smooth user experience.
 */

#include <iostream>
#include <string>
#include <fstream> // Added for File Handling
#include <limits>  // Required for buffer clearing

using namespace std;

// ==========================================
// DATA STRUCTURES
// ==========================================

struct Patient {
    int id;
    string name;
    int age;
    string contact;
    string department;
    string diagnosis;
    string admissionDate;
};

struct Node {
    Patient data;
    Node* next;
};

struct StackNode {
    Patient data;
    StackNode* next;
};

Node* head = NULL;            
StackNode* top = NULL;        
int idCounter = 100;          

// ==========================================
// HELPER FUNCTIONS (UI)
// ==========================================

void clearScreen() {
    // Intentionally empty to prevent "buffer spaces" (vertical gaps)
}

void waitForKey() {
    cout << "\nPress Enter to continue...";
    // Robust buffer clearing check
    if (cin.peek() == '\n') cin.ignore();
    cin.get(); 
}

void setColor(int color) {
    if (color == 11) cout << "\033[96m"; 
    else if (color == 12) cout << "\033[91m";
    else if (color == 10) cout << "\033[92m";
    else cout << "\033[0m";
}

void printHeader() {
    cout << "\n==============================================================" << endl;
    cout << "           HOSPITAL PATIENT MANAGEMENT SYSTEM                 " << endl;
    cout << "==============================================================" << endl;
    setColor(7); // White
}

void loadingBar() {
    cout << "\n\n\tLoading System: ";
    cout << "####################\n"; 
}

// ==========================================
// FILE HANDLING (NEWLY ADDED)
// ==========================================

void saveToFile() {
    ofstream outFile("patients.txt");
    if (!outFile) {
        cout << "Error accessing file system!" << endl;
        return;
    }

    Node* temp = head;
    while (temp != NULL) {
        outFile << temp->data.id << endl;
        outFile << temp->data.name << endl;
        outFile << temp->data.age << endl;
        outFile << temp->data.contact << endl;
        outFile << temp->data.department << endl;
        outFile << temp->data.diagnosis << endl;
        outFile << temp->data.admissionDate << endl;
        temp = temp->next;
    }
    outFile.close();
}

void loadFromFile() {
    ifstream inFile("patients.txt");
    if (!inFile) return; // File might not exist on first run, which is fine

    Patient p;
    while (inFile >> p.id) {
        inFile.ignore(); // consume newline
        getline(inFile, p.name);
        inFile >> p.age;
        inFile.ignore();
        getline(inFile, p.contact);
        getline(inFile, p.department);
        getline(inFile, p.diagnosis);
        getline(inFile, p.admissionDate);

        // Add to Linked List
        Node* newNode = new Node;
        newNode->data = p;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }

        // Update ID counter so new patients get unique IDs
        if (p.id >= idCounter) {
            idCounter = p.id + 1;
        }
    }
    inFile.close();
}

// ==========================================
// AUTHENTICATION
// ==========================================

void login() {
    string user, pass;
    bool isAuthenticated = false;

    while (!isAuthenticated) {
        printHeader();
        cout << "\n\t[ SECURE LOGIN REQUIRED ]" << endl;
        
        if (cin.peek() == '\n') cin.ignore();

        cout << "\n\tUsername: "; 
        if (!(cin >> user)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "\tPassword: "; 
        if (!(cin >> pass)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (user == "admin" && pass == "admin123") {
            cout << "\n\tAccess Granted!" << endl;
            isAuthenticated = true;
            cout << "\nPress Enter to access Main Menu...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cin.get();
        } else {
            setColor(12); // Red
            cout << "\n\tAccess Denied! Invalid Credentials." << endl;
            setColor(7);
            cout << "\n\tTry Again? (Press Enter)";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
}

// ==========================================
// LINKED LIST OPERATIONS
// ==========================================

void addPatient() {
    printHeader();
    cout << "--- REGISTER NEW PATIENT ---\n" << endl;

    Node* newNode = new Node;
    newNode->data.id = idCounter++;
    
    // Clear buffer if needed
    if (cin.peek() == '\n') cin.ignore();

    cout << "Enter Name: "; 
    getline(cin, newNode->data.name);
    
    cout << "Enter Age: "; cin >> newNode->data.age;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    
    cout << "Enter Contact: "; getline(cin, newNode->data.contact);
    cout << "Assign Department: "; getline(cin, newNode->data.department);
    
    newNode->data.diagnosis = "Pending Evaluation";
    cout << "Enter Date (DD/MM/YYYY): "; getline(cin, newNode->data.admissionDate);

    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    saveToFile(); // SAVE DATA
    setColor(10); // Green
    cout << "\nPatient Added Successfully! Assigned ID: " << newNode->data.id << endl;
    setColor(7);
    
    waitForKey();
}

void displayAllPatients() {
    printHeader();
    cout << "--- ACTIVE PATIENT LIST ---\n" << endl;
    if (head == NULL) {
        cout << "No patients currently admitted." << endl;
    } else {
        Node* temp = head;
        while (temp != NULL) {
            cout << "ID: " << temp->data.id 
                 << " | Name: " << temp->data.name 
                 << " | Dept: " << temp->data.department 
                 << " | Diag: " << temp->data.diagnosis << endl;
            cout << "--------------------------------------------------------------" << endl;
            temp = temp->next;
        }
    }
    cout << "\n";
    waitForKey();
}

void searchPatient() {
    printHeader();
    int searchId;
    cout << "Enter Patient ID to Search: "; cin >> searchId;

    Node* temp = head;
    bool found = false;
    while (temp != NULL) {
        if (temp->data.id == searchId) {
            cout << "\n--- PATIENT FOUND ---" << endl;
            cout << "Name: " << temp->data.name << endl;
            cout << "Age: " << temp->data.age << endl;
            cout << "Contact: " << temp->data.contact << endl;
            cout << "Department: " << temp->data.department << endl;
            cout << "Diagnosis: " << temp->data.diagnosis << endl;
            cout << "Admitted: " << temp->data.admissionDate << endl;
            found = true;
            break;
        }
        temp = temp->next;
    }

    if (!found) {
        setColor(12);
        cout << "\nPatient with ID " << searchId << " not found." << endl;
        setColor(7);
    }
    waitForKey();
}

// ==========================================
// DOCTOR DIAGNOSIS
// ==========================================

void doctorDiagnosis() {
    printHeader();
    cout << "--- DOCTOR DIAGNOSIS TERMINAL ---\n" << endl;
    int searchId;
    cout << "Enter Patient ID to Diagnose: "; cin >> searchId;

    Node* temp = head;
    bool found = false;
    while (temp != NULL) {
        if (temp->data.id == searchId) {
            found = true;
            cout << "\nCurrent Diagnosis: " << temp->data.diagnosis << endl;
            cout << "Enter New Clinical Diagnosis: ";
            
            if (cin.peek() == '\n') cin.ignore();
            getline(cin, temp->data.diagnosis);
            
            saveToFile(); // SAVE DATA
            setColor(10);
            cout << "\nDiagnosis Updated Successfully." << endl;
            setColor(7);
            break;
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "\nPatient ID not found." << endl;
    }
    waitForKey();
}

// ==========================================
// STACK OPERATIONS (Recycle Bin / Undo)
// ==========================================

void pushToRecycleBin(Patient p) {
    StackNode* newNode = new StackNode;
    newNode->data = p;
    newNode->next = top;
    top = newNode;
}

Patient popFromRecycleBin() {
    if (top == NULL) {
        Patient empty;
        empty.id = -1; // Indicator for empty
        return empty;
    }
    StackNode* temp = top;
    Patient p = temp->data;
    top = top->next;
    delete temp;
    return p;
}

void dischargePatient() {
    printHeader();
    cout << "--- DISCHARGE PATIENT (Recycle Bin) ---\n" << endl;
    int searchId;
    cout << "Enter Patient ID to Discharge: "; cin >> searchId;

    if (head == NULL) {
        cout << "List is empty." << endl;
        waitForKey();
        return;
    }

    Node* temp = head;
    Node* prev = NULL;

    while (temp != NULL && temp->data.id != searchId) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "\nPatient not found." << endl;
    } else {
        pushToRecycleBin(temp->data);

        if (prev == NULL) {
            head = temp->next;
        } else {
            prev->next = temp->next;
        }
        delete temp;

        saveToFile(); // SAVE DATA (Update file after removal)
        setColor(10);
        cout << "\nPatient Discharged and moved to Recycle Bin." << endl;
        setColor(7);
    }
    waitForKey();
}

void undoDischarge() {
    printHeader();
    cout << "--- UNDO LAST DISCHARGE ---\n" << endl;

    if (top == NULL) {
        cout << "Recycle Bin is empty. No operations to undo." << endl;
    } else {
        Patient p = popFromRecycleBin();
        
        Node* newNode = new Node;
        newNode->data = p;
        newNode->next = head; 
        head = newNode;

        saveToFile(); // SAVE DATA (Update file after restoration)
        setColor(10);
        cout << "Restored Patient: " << p.name << " (ID: " << p.id << ")" << endl;
        setColor(7);
    }
    waitForKey();
}

void viewRecycleBin() {
    printHeader();
    cout << "--- RECYCLE BIN (Recently Discharged) ---\n" << endl;
    if (top == NULL) {
        cout << "Bin is empty." << endl;
    } else {
        StackNode* temp = top;
        while (temp != NULL) {
            cout << "ID: " << temp->data.id << " | Name: " << temp->data.name << " (Waiting Restore)" << endl;
            temp = temp->next;
        }
    }
    cout << "\n";
    waitForKey();
}

// ==========================================
// MAIN MENU
// ==========================================

void mainMenu() {
    bool running = true;
    while (running) {
        printHeader();
        cout << " 1. Register New Patient" << endl;
        cout << " 2. Doctor Diagnosis" << endl;
        cout << " 3. Discharge Patient (Move to Recycle Bin)" << endl;
        cout << " 4. Search Patient" << endl;
        cout << " 5. Display All Active Patients" << endl;
        cout << " 6. View Recycle Bin" << endl;
        cout << " 7. Undo Last Discharge" << endl;
        cout << " 8. Save & Exit" << endl; // Updated text
        cout << "==============================================================" << endl;
        cout << " Select Option: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }

        switch (choice) {
            case 1: addPatient(); break;
            case 2: doctorDiagnosis(); break;
            case 3: dischargePatient(); break;
            case 4: searchPatient(); break;
            case 5: displayAllPatients(); break;
            case 6: viewRecycleBin(); break;
            case 7: undoDischarge(); break;
            case 8: 
                saveToFile(); // Explicit save on exit
                cout << "\nExiting System... Data Saved." << endl;
                running = false; 
                break;
            default: cout << "Invalid Option!" << endl;
        }
    }
}

// ==========================================
// DRIVER
// ==========================================

int main() {
    // Load data from file at start
    loadFromFile();

    loadingBar();
    
    login(); 

    mainMenu();

    return 0;
}
