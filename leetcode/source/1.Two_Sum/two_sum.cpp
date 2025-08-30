/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

class solution
{
public:
    vector<int> tow_sum(vector<int>&arr,int n)
    {
        unordered_map <int,int>d;
        for(int i=0;;++i)
        {
            int x = arr[i];
            int y = n-x;
            if(d.contains(y))
            {
                return {d[y],i};
            }
            d[x]=i;
        }
    }
    
};

vector<int> abc={2,7,11,15};
vector<int> output;

int main()
{
    std::cout<<"Hello World\n";
    solution ojb;
    output=ojb.tow_sum(abc,9);
    
    for(int i=0;i<output.size();i++)
    {
        cout << output[i] << " ";
    }
    cout << endl;

    return 0;
}