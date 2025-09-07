#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

/*IPrintable is an interface class, containing purely virtual functions printInfo()*/
class IPrintable {
public:
    /*The IPrintable class is interface and cannot be instantiated directly.
    Any class that inherits from IPrintable must implement printinfo().*/
    virtual void printinfo() const = 0; /*pure virtual function*/

    /*virtual destructor, When deleting an object through a base class pointer, the derived class's destructor is called.*/
    virtual ~IPrintable() {};
};

/*The public keyword ensures that public members of IPrintable retain the same access in Employee*/
class Employee : public IPrintable {
protected:
    string name;
    int id;
public:
    /*contructor Employee class : name(name), id(id) are initializer list*/
    Employee(string name, int id) : name(name), id(id) {}

    /*The Employee class is abstract and cannot be instantiated directly.
    Any class that inherits from Employee must implement calculatesalary().*/
    virtual double calculatesalary() const = 0; /*pure virtual function*/
    virtual string gettype () const = 0; /*pure virtual function*/

    /*virtual destructor, When deleting an object through a base class pointer, the derived class's destructor is called.*/
    virtual ~Employee() {}

    /*printInfo() is inherited in the Employee class, but is not defined there. 
    It is still a pure virtual function in Employee,
    Since it is not redefined, Employee is still an abstract class,
    and therefore, any class that inherits from Employee is required to override this function 
    if it does not want to be considered abstract.*/
};

class officestaff : public Employee {
private:
    double basesalary;
    int baseindex;
public:
    /*OfficeStaff(...): constructor of OfficeStaff class.
    Employee(name, id): calls constructor of parent class Employee to initialize name and id.
    baseSalary(baseSalary), baseindex(baseindex): initializes private properties of OfficeStaff class*/
    officestaff(string name, int id, double basesalary, int baseindex) 
        : Employee(name,id),basesalary(basesalary),baseindex(baseindex) {}

    /*override: confirms that you are overriding a pure virtual function from the parent class (Employee).
    const: ensures that the function does not change the state of the object.*/
    double calculatesalary() const override {
        return basesalary * baseindex;
    }
    string gettype() const override {
        return "officestaff";
    }
    void printinfo() const override {
        cout << "Office Staff - Name: " << name << ", ID: " << id << ", Salary: " << calculatesalary() << endl;
    }
};

class Engineer : public Employee {
private:
    double basesalary;
    int baseindex;
    int projectbonus;
public:
    /*Engineer(...): constructor of Engineer class.
    Employee(name, id): calls constructor of parent class Employee to initialize name and id.
    baseSalary(baseSalary), baseindex(baseindex): initializes private properties of Engineer class*/
    Engineer(string name, int id, double basesalary, int baseindex, int projectbonus) 
        : Employee(name,id),basesalary(basesalary),baseindex(baseindex),projectbonus(projectbonus) {}

    /*override: confirms that you are overriding a pure virtual function from the parent class (Employee).
    const: ensures that the function does not change the state of the object.*/
    double calculatesalary() const override {
        return (basesalary * baseindex) + projectbonus;
    }
    string gettype() const override {
        return "Engineer";
    }
    void printinfo() const override {
        cout << "Engineer - Name: " << name << ", ID: " << id << ", Salary: " << calculatesalary() << endl;
    }
};

void fillterEmployeeBySalary(const std::vector<std::unique_ptr<Employee>>& employees, double minsalary) {
    cout << "Employees with salary >= " << minsalary << ":\n";
    for (const auto& emp : employees) {
        if (emp->calculatesalary() >= minsalary) {
            emp->printinfo();
        }
    }
}

void fillterEmployeeBytype(const std::vector<std::unique_ptr<Employee>>& employees, const string mytype) {
    cout << "Employees with type: " << mytype << ":\n";
    for (const auto& emp : employees) {
        if (emp->gettype() == mytype) {
            emp->printinfo();
        }
    }
}

void sortEmployeesBySalary(std::vector<std::unique_ptr<Employee>>& employees) {
    /*std::sort is a sorting function in C++ STL.
    employee is a std::vector<std::unique_ptr<Employee>>.
    Lambda function compares two Unique_ptr<Employee> pointers by calling CalculateSalary() to sort by ascending salary.*/
    std::sort(employees.begin(), employees.end(), [](const std::unique_ptr<Employee>& a, const std::unique_ptr<Employee>& b) {
        return a->calculatesalary() < b->calculatesalary();
    });
    cout << "Employees sorted by salary:\n";
    for (const auto& emp : employees) {
        emp->printinfo();
    }
}

int main() {
    /*Employee* is an abstract class pointer.
    OfficeStaff(...) is a child class object that inherits from Employee.
    Instantiating a child class object but assigning it to a parent class pointer → this is polymorphism.
    When call employee1->printInfo() or employee1->calculateSalary(),
    the program will automatically call the corresponding version in OfficeStaff thanks to the virtual function.
    Allows you to handle different types of employees using the same cursor type (Employee*)*/
    Employee* employee1 = new officestaff("Nguyen Van A", 101, 500, 2);
    Employee* employee2 = new Engineer("Tran Thi B", 102, 700, 3, 200);

    /*unique_ptr is a smart pointer in C++11 and above, which has the ability to:
    Automatically free memory when leaving scope
    Helps avoid memory leaks
    Does not allow copying, only moving → ensures unique ownership*/
    std::unique_ptr<Employee> employee3 = std::make_unique<officestaff>("Nguyen Van C", 103, 500, 3);
    std::unique_ptr<Employee> employee4 = std::make_unique<Engineer>("Tran Thi D", 103, 700, 4, 300);

    employee1->printinfo();
    employee2->printinfo();

    employee3->printinfo();
    employee4->printinfo();

    delete employee1; /*free memory*/
    delete employee2; /*free memory*/
    /*employee3 and employee4 are unique_ptr, so no need to manually free memory. 
    When they go out of scope, their destructors will be automatically called, freeing the memory.*/
    cout << "----------------------------------\n";
    std::vector<std::unique_ptr<Employee>> employees;
    employees.push_back(std::make_unique<officestaff>("Nguyen Van A", 101, 500, 5));
    employees.push_back(std::make_unique<Engineer>("Tran Thi B", 102, 700, 3, 200));
    employees.push_back(std::make_unique<officestaff>("Nguyen Van C", 103, 500, 3));
    employees.push_back(std::make_unique<Engineer>("Tran Thi D", 104, 700, 4, 300));
    employees.push_back(std::make_unique<officestaff>("Le Van E", 105, 600, 2));

    for(const auto& emp : employees) {
        emp->printinfo();
    }

    fillterEmployeeBySalary(employees, 2000);
    fillterEmployeeBytype(employees, "officestaff");
    sortEmployeesBySalary(employees);

    return 0;
}