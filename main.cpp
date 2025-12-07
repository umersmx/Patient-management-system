#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Patient {
public:
    int id;
    string name;
    int age;
    string contact;
    string department;
    string diagnosis;
    string admissionDate;

    Patient() {
        id = 0;
        age = 0;
        diagnosis = "Pending Evaluation";
    }

    void inputDetails(int newId) {
        id = newId;
        if (cin.peek() == '\n') cin.ignore();
        
        cout << "Enter Name: "; getline(cin, name);
        cout << "Enter Age: "; cin >> age;
        cin.ignore(1000, '\n'); 
        cout << "Enter Contact: "; getline(cin, contact);
        cout << "Assign Department: "; getline(cin, department);
        cout << "Enter Date (DD/MM/YYYY): "; getline(cin, admissionDate);
    }

    void display() const {
        cout << "ID: " << id 
             << " | Name: " << name 
             << " | Dept: " << department 
             << " | Diag: " << diagnosis << endl;
    }
};

struct ListNode {
    Patient data;
    ListNode* next;
};

class ActivePatientList {
private:
    ListNode* head; 
    int idCounter;

public:
    ActivePatientList() {
        head = nullptr;
        idCounter = 100;
        loadFromFile();
    }

    ~ActivePatientList() {
    }

    void addPatient() {
        Patient p;
        p.inputDetails(idCounter++); 

        ListNode* newNode = new ListNode;
        newNode->data = p;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        } else {
            ListNode* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        
        cout << "\n[SUCCESS] Patient Added. Assigned ID: " << p.id << endl;
        saveToFile();
    }

    bool dischargePatient(int searchId, Patient &dischargedData) {
        if (head == nullptr) return false;

        ListNode* temp = head;
        ListNode* prev = nullptr;

        while (temp != nullptr && temp->data.id != searchId) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) return false;

        dischargedData = temp->data;

        if (prev == nullptr) {
            head = temp->next;
        } else {
            prev->next = temp->next;
        }
        
        delete temp;
        saveToFile();
        return true;
    }

    void restorePatient(Patient p) {
        ListNode* newNode = new ListNode;
        newNode->data = p;
        newNode->next = head; 
        head = newNode;
        saveToFile();
    }

    void searchAndDiagnose() {
        int searchId;
        cout << "Enter Patient ID: "; cin >> searchId;
        
        ListNode* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == searchId) {
                cout << "\n[PATIENT FOUND] " << temp->data.name << endl;
                cout << "Current Diagnosis: " << temp->data.diagnosis << endl;
                cout << "Enter New Diagnosis: ";
                if (cin.peek() == '\n') cin.ignore();
                getline(cin, temp->data.diagnosis);
                cout << "[SUCCESS] Record updated." << endl;
                saveToFile();
                return;
            }
            temp = temp->next;
        }
        cout << "[ERROR] Patient ID not found." << endl;
    }

    void searchOnly() {
        int searchId;
        cout << "Enter Patient ID: "; cin >> searchId;

        ListNode* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == searchId) {
                cout << "\n--- PATIENT DETAILS ---" << endl;
                temp->data.display();
                cout << "Age: " << temp->data.age << endl;
                cout << "Contact: " << temp->data.contact << endl;
                cout << "Admitted: " << temp->data.admissionDate << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "[ERROR] Patient ID not found." << endl;
    }

    void displayAll() {
        if (head == nullptr) {
            cout << "No active patients." << endl;
            return;
        }
        ListNode* temp = head;
        cout << "\n--- ACTIVE PATIENTS ---" << endl;
        while (temp != nullptr) {
            temp->data.display();
            cout << "-----------------------" << endl;
            temp = temp->next;
        }
    }

    void saveToFile() {
        ofstream outFile("patients.txt");
        if (!outFile) return;

        ListNode* temp = head;
        while (temp != nullptr) {
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
        if (!inFile) return;

        Patient p;
        while (inFile >> p.id) {
            inFile.ignore();
            getline(inFile, p.name);
            inFile >> p.age;
            inFile.ignore();
            getline(inFile, p.contact);
            getline(inFile, p.department);
            getline(inFile, p.diagnosis);
            getline(inFile, p.admissionDate);

            ListNode* newNode = new ListNode;
            newNode->data = p;
            newNode->next = nullptr;

            if (head == nullptr) {
                head = newNode;
            } else {
                ListNode* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newNode;
            }

            if (p.id >= idCounter) idCounter = p.id + 1;
        }
        inFile.close();
    }
};

struct StackNode {
    Patient data;
    StackNode* next;
};

class RecycleBin {
private:
    StackNode* top;

public:
    RecycleBin() {
        top = nullptr;
    }

    void push(Patient p) {
        StackNode* newNode = new StackNode;
        newNode->data = p;
        newNode->next = top;
        top = newNode;
    }

    bool pop(Patient &restoredData) {
        if (top == nullptr) return false;

        StackNode* temp = top;
        restoredData = temp->data;
        top = top->next;
        
        delete temp;
        return true;
    }

    void viewBin() {
        if (top == nullptr) {
            cout << "Recycle Bin is empty." << endl;
            return;
        }
        StackNode* temp = top;
        cout << "\n--- RECYCLE BIN (Recently Discharged) ---" << endl;
        while (temp != nullptr) {
            cout << "ID: " << temp->data.id << " | Name: " << temp->data.name << endl;
            temp = temp->next;
        }
    }

    bool isEmpty() {
        return top == nullptr;
    }
};

class HospitalSystem {
private:
    ActivePatientList patientList;
    RecycleBin trashBin;

    void printHeader() {
        cout << "\n==============================================" << endl;
        cout << "   HOSPITAL PATIENT MANAGEMENT SYSTEM (OOP)   " << endl;
        cout << "==============================================" << endl;
    }

    void pause() {
        cout << "\nPress Enter to continue...";
        if (cin.peek() == '\n') cin.ignore();
        cin.get();
    }

public:
    void login() {
        string user, pass;
        bool authenticated = false;
        while (!authenticated) {
            printHeader();
            cout << "\n[LOGIN REQUIRED]" << endl;
            if (cin.peek() == '\n') cin.ignore();
            cout << "Username: "; cin >> user;
            cout << "Password: "; cin >> pass;

            if (user == "admin" && pass == "admin123") {
                cout << "\n[ACCESS GRANTED]" << endl;
                authenticated = true;
                pause();
            } else {
                cout << "\n[ACCESS DENIED] Try again." << endl;
                pause();
            }
        }
    }

    void mainMenu() {
        int choice;
        while (true) {
            printHeader();
            cout << "1. Add New Patient" << endl;
            cout << "2. Search Patient" << endl;
            cout << "3. Doctor Diagnosis" << endl;
            cout << "4. Discharge Patient (Delete)" << endl;
            cout << "5. Undo Last Discharge" << endl;
            cout << "6. View Active Patients" << endl;
            cout << "7. View Recycle Bin" << endl;
            cout << "8. Save & Exit" << endl;
            cout << "----------------------------------------------" << endl;
            cout << "Select Option: ";
            
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(1000, '\n');
                choice = 0;
            }

            switch(choice) {
                case 1: 
                    patientList.addPatient(); 
                    break;
                case 2:
                    patientList.searchOnly();
                    break;
                case 3:
                    patientList.searchAndDiagnose();
                    break;
                case 4: {
                    int id;
                    cout << "Enter Patient ID to discharge: "; cin >> id;
                    Patient p;
                    if (patientList.dischargePatient(id, p)) {
                        trashBin.push(p);
                        cout << "[SUCCESS] Patient moved to Recycle Bin." << endl;
                    } else {
                        cout << "[ERROR] Patient not found." << endl;
                    }
                    break;
                }
                case 5: {
                    Patient p;
                    if (trashBin.pop(p)) {
                        patientList.restorePatient(p);
                        cout << "[SUCCESS] Restored Patient: " << p.name << endl;
                    } else {
                        cout << "[ERROR] Recycle Bin is empty." << endl;
                    }
                    break;
                }
                case 6:
                    patientList.displayAll();
                    break;
                case 7:
                    trashBin.viewBin();
                    break;
                case 8:
                    patientList.saveToFile();
                    cout << "Data Saved. Exiting..." << endl;
                    exit(0);
                default:
                    cout << "Invalid Option." << endl;
            }
            if (choice != 8) pause();
        }
    }
};

int main() {
    HospitalSystem app;
    app.login();
    app.mainMenu();
    return 0;
}
