#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
    Node* head;
public:
    LinkedList() : head(nullptr) {}

    void append(int value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = newNode;
    }
    
    void appendArray(const int arr[], int size) {
    for (int i = 0; i < size; ++i) {
        append(arr[i]);
    }
    }

    void display() const {
        Node* temp = head;
        while (temp) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }

    Node* getHead() const {
        return head;
    }
};

class Solution {
public:
    LinkedList addTwoNumbers(LinkedList* l1, LinkedList* l2) {
        if (!l1 || !l2) {
            /*If either list is nullptr, return an empty list*/
            return LinkedList();
        }

        Node* p1 = l1->getHead();
        Node* p2 = l2->getHead();
        int carry = 0;

        LinkedList result;

        while (p1 || p2 || carry) {
            int val1 = p1 ? p1->data : 0;
            int val2 = p2 ? p2->data : 0;

            int sum = val1 + val2 + carry;
            carry = sum / 10;
            result.append(sum % 10);

            if (p1) p1 = p1->next;
            if (p2) p2 = p2->next;
        }

        return result;
    }
};


int main() {
    LinkedList list1;
    LinkedList list2;

    int arr1[] = {9, 9, 9, 9, 9, 9, 9};
    int arr2[] = {9, 9, 9, 9};

    list1.appendArray(arr1, sizeof(arr1) / sizeof(arr1[0]));
    list2.appendArray(arr2, sizeof(arr2) / sizeof(arr2[0]));

    cout << "List 1: ";
    list1.display();
    cout << "List 2: ";
    list2.display();

    Solution solver;
    LinkedList result = solver.addTwoNumbers(&list1, &list2);

    cout << "Sum: ";
    result.display();

    return 0;
}