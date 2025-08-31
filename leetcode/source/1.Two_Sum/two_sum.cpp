
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class solution
{
public:
    vector<int> two_sum(vector<int>&nums,int target)
    {
        unordered_map <int,int>nums_map;
        for(int i=0;;++i)
        {
            int complement  = target - nums[i];
            if(nums_map.count(complement))
            {
                return {nums_map[complement],i};
            }
            nums_map.insert({nums[i], i});
        }
    }
    
};

vector<int> my_vector={2,7,11,15};
vector<int> output_vector;

int main()
{
    std::cout<<"Hello World\n";
    solution ojb;
    output_vector=ojb.two_sum(my_vector,9);
    
    for(int i=0;i<output_vector.size();i++)
    {
        cout << output_vector[i] << " ";
    }
    cout << endl;

    return 0;
}