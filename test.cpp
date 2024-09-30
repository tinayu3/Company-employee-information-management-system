#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <map>
using namespace std;

class Employee {
private:
    int id; // Employee number
    string name;
    string gender;
    string phone;
    string department;
    double salary;

public:
    // Constructors
    Employee() : id(0), name(""), gender(""), phone(""), department(""), salary(0.0) {}
    Employee(int id, string name, string gender, string phone, string department, double salary)
        : id(id), name(name), gender(gender), phone(phone), department(department), salary(salary) {}

    // Getters and setters
    void setID(int id) { this->id = id; }
    int getID() const { return id; }

    void setName(string name) { this->name = name; }
    string getName() const { return name; }

    void setGender(string gender) { this->gender = gender; }
    string getGender() const { return gender; }

    void setPhone(string phone) { this->phone = phone; }
    string getPhone() const { return phone; }

    void setDepartment(string department) { this->department = department; }
    string getDepartment() const { return department; }

    void setSalary(double salary) { this->salary = salary; }
    double getSalary() const { return salary; }

    // Input employee data
    void input() {
        cout << "Enter employee number (ID): ";
        cin >> id;
        cout << "Enter name: ";
        cin.ignore(); // Clear the newline from the buffer
        getline(cin, name);
        cout << "Enter gender: ";
        getline(cin, gender);
        cout << "Enter phone number: ";
        getline(cin, phone);
        cout << "Enter department: ";
        getline(cin, department);
        cout << "Enter salary: ";
        cin >> salary;
    }

    // Display employee data
    void display() const {
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Gender: " << gender << endl;
        cout << "Phone: " << phone << endl;
        cout << "Department: " << department << endl;
        cout << "Salary: " << salary << endl;
    }
};

// Function to load employees from file
void loadEmployees(vector<Employee>& employees) {
    ifstream infile("employees.txt");
    if (!infile) {
        cout << "No existing data file. Starting with empty employee list." << endl;
        return;
    }

    int numEmployees;
    infile >> numEmployees;
    infile.ignore(); // Ignore the newline after the number

    for (int i = 0; i < numEmployees; ++i) {
        int id;
        string name;
        string gender;
        string phone;
        string department;
        double salary;

        infile >> id;
        infile.ignore(); // Ignore newline
        getline(infile, name);
        getline(infile, gender);
        getline(infile, phone);
        getline(infile, department);
        infile >> salary;
        infile.ignore(); // Ignore newline

        Employee emp(id, name, gender, phone, department, salary);
        employees.push_back(emp);
    }
}

// Function to save employees to file
void saveEmployees(const vector<Employee>& employees) {
    ofstream outfile("employees.txt");
    if (!outfile) {
        cout << "Error opening file for writing." << endl;
        return;
    }

    int numEmployees = employees.size();
    outfile << numEmployees << endl;

    for (const Employee& emp : employees) {
        outfile << emp.getID() << endl;
        outfile << emp.getName() << endl;
        outfile << emp.getGender() << endl;
        outfile << emp.getPhone() << endl;
        outfile << emp.getDepartment() << endl;
        outfile << emp.getSalary() << endl;
    }
}

// Function to add an employee
void addEmployee(vector<Employee>& employees) {
    Employee emp;
    emp.input();

    // Check for duplicate ID
    for (const Employee& e : employees) {
        if (e.getID() == emp.getID()) {
            cout << "Employee with ID " << emp.getID() << " already exists. Cannot add duplicate ID." << endl;
            return;
        }
    }

    employees.push_back(emp);
    saveEmployees(employees);
    cout << "Employee added successfully." << endl;
}

// Function to delete an employee by ID
void deleteEmployee(vector<Employee>& employees) {
    int id;
    cout << "Enter the employee ID to delete: ";
    cin >> id;

    bool found = false;
    for (auto it = employees.begin(); it != employees.end(); ++it) {
        if (it->getID() == id) {
            employees.erase(it);
            found = true;
            cout << "Employee with ID " << id << " deleted successfully." << endl;
            saveEmployees(employees);
            break;
        }
    }
    if (!found) {
        cout << "Employee with ID " << id << " not found." << endl;
    }
}

// Function to modify an employee's information
void modifyEmployee(vector<Employee>& employees) {
    int id;
    cout << "Enter the employee ID to modify: ";
    cin >> id;

    bool found = false;
    for (Employee& emp : employees) {
        if (emp.getID() == id) {
            found = true;
            cout << "Current information of employee with ID " << id << ":" << endl;
            emp.display();

            cout << "\nEnter new information:" << endl;
            emp.input();

            saveEmployees(employees);
            cout << "Employee information updated successfully." << endl;
            break;
        }
    }
    if (!found) {
        cout << "Employee with ID " << id << " not found." << endl;
    }
}

// Function to query employees by name
void queryEmployee(const vector<Employee>& employees) {
    string name;
    cout << "Enter the name to query: ";
    cin.ignore(); // Clear the newline in the buffer
    getline(cin, name);

    bool found = false;
    for (const Employee& emp : employees) {
        if (emp.getName() == name) {
            found = true;
            cout << "\nEmployee information:" << endl;
            emp.display();
            cout << endl;
        }
    }
    if (!found) {
        cout << "No employee found with the name \"" << name << "\"." << endl;
    }
}

// Function to calculate and display average salary per department
void statistics(const vector<Employee>& employees) {
    // Map to store department to total salary and count
    map<string, pair<double, int>> deptSalary;

    for (const Employee& emp : employees) {
        string dept = emp.getDepartment();
        double salary = emp.getSalary();
        if (deptSalary.find(dept) != deptSalary.end()) {
            deptSalary[dept].first += salary; // total salary
            deptSalary[dept].second += 1;     // count
        } else {
            deptSalary[dept] = make_pair(salary, 1);
        }
    }

    cout << "\nAverage Salary per Department:" << endl;
    cout << left << setw(20) << "Department" << setw(15) << "Average Salary" << endl;
    cout << "----------------------------------------" << endl;
    for (const auto& entry : deptSalary) {
        string dept = entry.first;
        double totalSalary = entry.second.first;
        int count = entry.second.second;
        double avgSalary = totalSalary / count;
        cout << left << setw(20) << dept << setw(15) << avgSalary << endl;
    }
}

// Main function with menu for function selection
int main() {
    vector<Employee> employees;
    loadEmployees(employees);

    int choice;
    do {
        cout << "\nEmployee Management System Menu:" << endl;
        cout << "1. Add Employee" << endl;
        cout << "2. Delete Employee" << endl;
        cout << "3. Modify Employee" << endl;
        cout << "4. Query Employee by Name" << endl;
        cout << "5. Statistics (Average Salary per Department)" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // Add Employee
                addEmployee(employees);
                break;
            case 2:
                // Delete Employee
                deleteEmployee(employees);
                break;
            case 3:
                // Modify Employee
                modifyEmployee(employees);
                break;
            case 4:
                // Query Employee by Name
                queryEmployee(employees);
                break;
            case 5:
                // Statistics
                statistics(employees);
                break;
            case 0:
                // Exit
                saveEmployees(employees);
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please select again." << endl;
                break;
        }
    } while (choice != 0);

    return 0;
}
