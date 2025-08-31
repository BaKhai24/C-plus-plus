/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>

using namespace std;

struct Node
{
    int data;
    Node* next;
};

class Linkedlist
{
    Node* head;
    Node* carry=nullptr;
    int a =0;
public:
    Linkedlist() : head(NULL) {}
    
    void addendlist(int value)
    {
        Node* newnode = new Node();
        
        newnode->data=value;
        newnode->next=NULL;
        
        if(!head)
        {
            head=newnode;
            return;
        }
        
        Node* temp=head;
        while(temp->next)
        {
            temp=temp->next;
        }
        temp->next=newnode;
    }
    
    void display()
    {
        Node* temp=head;
        while(temp)
        {
            cout<<temp->data<<"->";
            temp=temp->next;
        }
        cout<<"NULL"<<endl;
    }
    
    int getnode()
    {
        int value=0;
        if(a==0)
        {
            if(!carry)
            {
                carry=head;
            }
        }
        else
        {
            if(!carry)
            {
                return -1;
            }
        }
        
        a++;
         
        value=carry->data;
        carry=carry->next;
         
        return value;
        
    }
};

class solution
{
public:
    void add2number(Linkedlist l1, Linkedlist l2)
    {
        int plus=0;
        int carry=0;
        Linkedlist s;
        int data1=l1.getnode();
        int data2=l2.getnode();
        while((data1 != -1) || (data2 !=-1) || carry)
        {
            if(data1==-1)
            {
                data1=0;
            }
            if(data2==-1)
            {
                data2=0;
            }
            plus = data1 + data2 + carry;
            carry = plus/10;
            s.addendlist(plus%10);
            data1=l1.getnode();
            data2=l2.getnode();
        }
        
        s.display();
    }
};
int main()
{
    Linkedlist list1;
    Linkedlist list2;
    solution ojb;
    list1.addendlist(9);
    list1.addendlist(9);
    list1.addendlist(9);
    list1.addendlist(9);
    list1.addendlist(9);
    list1.addendlist(9);
    list1.addendlist(9);
    list1.display();
    list2.addendlist(9);
    list2.addendlist(9);
    list2.addendlist(9);
    list2.addendlist(9);
    list2.display();

    ojb.add2number(list1,list2);
    std::cout<<"Hello World";

    return 0;
}