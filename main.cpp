/**
 * HOSPITAL PATIENT MANAGEMENT SYSTEM
 * ----------------------------------
 * Semester Project: Data Structures and Algorithms
 * Prepared by: Muhammad Umer Farooq (Leader) & Abdur Rehman
 *
 * EXPLANATION FOR BEGINNERS:
 * This program uses three main "tools":
 * 1. Structs: These are like "forms" to hold patient info.
 * 2. Linked List: A chain of patients currently in the hospital.
 * 3. Stack: A pile of "deleted" patients (Recycle Bin) to undo mistakes.
 */

#include <iostream>  // Used for input (cin) and output (cout)
#include <string>    // Used to handle text (names, diagnosis)
#include <fstream>   // Used for File Handling (reading/writing .txt files)
#include <limits>    // Used to clear the input buffer (fix skipping issues)

using namespace std;

// ==========================================
// 1. DEFINING OUR "FORMS" (DATA STRUCTURES)
// ==========================================

// This is the "Form" that holds all info for ONE patient.
struct Patient {
    int id;                 // Unique ID (e.g., 101)
    string name;            // Patient Name
    int age;                // Patient Age
    string contact;         // Phone Number
    string department;      // Assigned Ward/Dept
    string diagnosis;       // What the doctor says
    string admissionDate;   // Date they arrived
};

// This is a "Link" in our chain (Linked List).
// It holds the Patient data AND a pointer (arrow) to the next patient.
struct Node {
    Patient data;   // The actual info
    Node* next;     // The arrow pointing to the next node
};

// This is a "Plate" in our stack (Recycle Bin).
// It works exactly like Node, but we name it differently to avoid confusion.
struct StackNode {
    Patient data;
    StackNode* next;
};

// ==========================================
// 2. GLOBAL VARIABLES (THE STORAGE)
// ==========================================

Node* head = NULL;       // "head" is the START of the patient list. NULL means it's empty.
StackNode* top = NULL;   // "top" is the TOP of the recycle bin stack.
int idCounter = 100;     // We start IDs at 100. We will increase this by 1 every time.

// ==========================================
// 3. HELPER FUNCTIONS (MAKING IT LOOK NICE)
// ==========================================

// This function clears the screen so the menu looks fresh.
void clearScreen() {
    // We left this empty on purpose to avoid the "buffer space" gaps.
    // If you want to force clear on Linux/Mac, uncomment the next line:
    // cout << "\033[2J\033[1;1H";
}

// This pauses the program so the user can read the text before moving on.
void waitForKey() {
    cout << "\nPress Enter to continue...";
    // The next line checks if there is a leftover "Enter" key press and ignores it.
    if (cin.peek() == '\n') cin.ignore();
    cin.get(); // Waits for the user to hit Enter
}

// This adds color to the text (Cyan, Red, Green) using special codes.
void setColor(int color) {
    if (color == 11) cout << "\033[96m";      // Cyan (Blue-ish)
    else if (color == 12) cout << "\033[91m"; // Red (Error)
    else if (color == 10) cout << "\033[92m"; // Green (Success)
    else cout << "\033[0m";                   // Reset to White
}

// Prints the title banner.
void printHeader() {
    cout << "\n==============================================================" << endl;
    cout << "           HOSPITAL PATIENT MANAGEMENT SYSTEM                 " << endl;
    cout << "==============================================================" << endl;
    setColor(7); // Reset to white
}

// Simulates a loading bar for visual effect.
void loadingBar() {
    cout << "\n\n\tLoading System: ";
    cout << "####################\n"; 
}

// ==========================================
// 4. FILE HANDLING (THE NOTEBOOK)
// ==========================================

// Function to SAVE data from RAM (Linked List) to Hard Drive (patients.txt)
void saveToFile() {
    ofstream outFile("patients.txt"); // Open file for writing
    if (!outFile) {
        cout << "Error accessing file system!" << endl;
        return;
    }

    Node* temp = head; // Start at the beginning of the list
    
    // Loop through every person in the list
    while (temp != NULL) {
        // Write their details one by one
        outFile << temp->data.id << endl;
        outFile << temp->data.name << endl;
        outFile << temp->data.age << endl;
        outFile << temp->data.contact << endl;
        outFile << temp->data.department << endl;
        outFile << temp->data.diagnosis << endl;
        outFile << temp->data.admissionDate << endl;
        
        temp = temp->next; // Move to the next person
    }
    outFile.close(); // Close the notebook
}

// Function to LOAD data from Hard Drive to RAM when program starts
void loadFromFile() {
    ifstream inFile("patients.txt"); // Open file for reading
    if (!inFile) return; // If file doesn't exist (first run), just stop.

    Patient p;
    // Keep reading as long as we find an ID
    while (inFile >> p.id) {
        inFile.ignore(); // Skip the newline character after the number
        getline(inFile, p.name);
        inFile >> p.age;
        inFile.ignore();
        getline(inFile, p.contact);
        getline(inFile, p.department);
        getline(inFile, p.diagnosis);
        getline(inFile, p.admissionDate);

        // CREATE A NEW NODE (Restoring the patient)
        Node* newNode = new Node;
        newNode->data = p;
        newNode->next = NULL;

        // Add this node to the Linked List
        if (head == NULL) {
            head = newNode; // If list is empty, this is the first one
        } else {
            Node* temp = head;
            while (temp->next != NULL) {
                temp = temp->next; // Find the last person
            }
            temp->next = newNode; // Link the new person to the end
        }

        // IMPORTANT: Update our ID counter so we don't give a new patient
        // an ID that already exists.
        if (p.id >= idCounter) {
            idCounter = p.id + 1;
        }
    }
    inFile.close();
}

// ==========================================
// 5. AUTHENTICATION (LOGIN)
// ==========================================

void login() {
    string user, pass;
    bool isAuthenticated = false; // Flag to check if login is successful

    // Keep asking UNTIL the user gets it right
    while (!isAuthenticated) {
        printHeader();
        cout << "\n\t[ SECURE LOGIN REQUIRED ]" << endl;
        
        // Clear buffer if there's a leftover newline
        if (cin.peek() == '\n') cin.ignore();

        cout << "\n\tUsername: "; 
        cin >> user;

        cout << "\tPassword: "; 
        cin >> pass;

        // CHECK CREDENTIALS
        if (user == "admin" && pass == "admin123") {
            cout << "\n\tAccess Granted!" << endl;
            isAuthenticated = true; // Stop the loop
            cout << "\nPress Enter to access Main Menu...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cin.get();
        } else {
            setColor(12); // Red color
            cout << "\n\tAccess Denied! Invalid Credentials." << endl;
            setColor(7); // Reset color
            cout << "\n\tTry Again? (Press Enter)";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
}

// ==========================================
// 6. PATIENT OPERATIONS (LINKED LIST)
// ==========================================

void addPatient() {
    printHeader();
    cout << "--- REGISTER NEW PATIENT ---\n" << endl;

    // 1. Create a new blank form (Node)
    Node* newNode = new Node;
    newNode->data.id = idCounter++; // Assign ID and then increase counter
    
    // 2. Clear input buffer so we can take text names
    if (cin.peek() == '\n') cin.ignore();

    // 3. Get input from user
    cout << "Enter Name: "; 
    getline(cin, newNode->data.name);
    
    cout << "Enter Age: "; cin >> newNode->data.age;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer after number
    
    cout << "Enter Contact: "; getline(cin, newNode->data.contact);
    cout << "Assign Department: "; getline(cin, newNode->data.department);
    
    newNode->data.diagnosis = "Pending Evaluation"; // Default value
    cout << "Enter Date (DD/MM/YYYY): "; getline(cin, newNode->data.admissionDate);

    newNode->next = NULL; // This will be the last person, so they point to NULL

    // 4. Link it to the chain
    if (head == NULL) {
        // If list is empty, this new person is the Head
        head = newNode;
    } else {
        // If list is not empty, travel to the end
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        // Link the last person to this new person
        temp->next = newNode;
    }

    saveToFile(); // Save immediately
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
        // Loop: Keep going as long as 'temp' is not NULL
        while (temp != NULL) {
            cout << "ID: " << temp->data.id 
                 << " | Name: " << temp->data.name 
                 << " | Dept: " << temp->data.department 
                 << " | Diag: " << temp->data.diagnosis << endl;
            cout << "--------------------------------------------------------------" << endl;
            
            // Move to the next person in the chain
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
    
    // Traverse the list to find the ID
    while (temp != NULL) {
        if (temp->data.id == searchId) {
            // WE FOUND THEM! Print details.
            cout << "\n--- PATIENT FOUND ---" << endl;
            cout << "Name: " << temp->data.name << endl;
            cout << "Age: " << temp->data.age << endl;
            cout << "Contact: " << temp->data.contact << endl;
            cout << "Department: " << temp->data.department << endl;
            cout << "Diagnosis: " << temp->data.diagnosis << endl;
            cout << "Admitted: " << temp->data.admissionDate << endl;
            found = true;
            break; // Stop looking
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
// 7. DOCTOR DIAGNOSIS
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
            getline(cin, temp->data.diagnosis); // Overwrite the diagnosis
            
            saveToFile(); // Save the update
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
// 8. STACK OPERATIONS (RECYCLE BIN)
// ==========================================

// Pushes a patient onto the "Trash Pile" (Stack)
void pushToRecycleBin(Patient p) {
    StackNode* newNode = new StackNode;
    newNode->data = p;
    newNode->next = top; // New node points to the current top
    top = newNode;       // New node BECOMES the top
}

// Pops (Removes) a patient from the "Trash Pile" to restore them
Patient popFromRecycleBin() {
    if (top == NULL) {
        Patient empty; // Return empty placeholder if bin is empty
        empty.id = -1; 
        return empty;
    }
    StackNode* temp = top;
    Patient p = temp->data; // Get the data
    top = top->next;        // Move top down one level
    delete temp;            // Delete the node from memory
    return p;               // Return the data
}

// "Discharges" a patient = Moves them from List to Stack
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
    Node* prev = NULL; // We need to know who is BEHIND the person we delete

    // Search for the node
    while (temp != NULL && temp->data.id != searchId) {
        prev = temp;       // Save current as previous
        temp = temp->next; // Move to next
    }

    if (temp == NULL) {
        cout << "\nPatient not found." << endl;
    } else {
        // 1. Save to Recycle Bin BEFORE deleting
        pushToRecycleBin(temp->data);

        // 2. Unlink from Linked List
        if (prev == NULL) {
            // If deleting the HEAD (first person)
            head = temp->next;
        } else {
            // If deleting someone in the middle/end
            prev->next = temp->next;
        }
        delete temp; // Delete from memory

        saveToFile(); // Save changes
        setColor(10);
        cout << "\nPatient Discharged and moved to Recycle Bin." << endl;
        setColor(7);
    }
    waitForKey();
}

// Undoes the last discharge
void undoDischarge() {
    printHeader();
    cout << "--- UNDO LAST DISCHARGE ---\n" << endl;

    if (top == NULL) {
        cout << "Recycle Bin is empty. No operations to undo." << endl;
    } else {
        // 1. Get data from Stack
        Patient p = popFromRecycleBin();
        
        // 2. Add back to Linked List (We add to HEAD for simplicity)
        Node* newNode = new Node;
        newNode->data = p;
        newNode->next = head; 
        head = newNode;

        saveToFile(); // Save changes
        setColor(10);
        cout << "Restored Patient: " << p.name << " (ID: " << p.id << ")" << endl;
        setColor(7);
    }
    waitForKey();
}

// Just looks at who is in the trash
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
// 9. MAIN MENU & DRIVER
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
        cout << " 8. Save & Exit" << endl;
        cout << "==============================================================" << endl;
        cout << " Select Option: ";

        int choice;
        // Check if user entered a number or garbage text
        if (!(cin >> choice)) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear bad input
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
                saveToFile(); 
                cout << "\nExiting System... Data Saved." << endl;
                running = false; // Stop the loop
                break;
            default: cout << "Invalid Option!" << endl;
        }
    }
}

int main() {
    // 1. Load any data from previous sessions
    loadFromFile();

    // 2. Show loading visual
    loadingBar();
    
    // 3. Force Login
    login(); 

    // 4. Show Menu (Only happens if Login success)
    mainMenu();

    return 0;
}
