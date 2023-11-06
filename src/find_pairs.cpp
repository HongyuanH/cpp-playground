/*
Find pairs such that nums[i] + nums[j] == k, or nums[i] + k == nums[j], etc.

Conditions:
* unique: (nums[i],nums[j]) should be unique or not
* ordered: (i,j) or (j,i)
* self-pairing: i==j or i!=j

Approaches:
* Approach 1: use unordered_map or unordered_set, pay attention to the corner case where k==0
* Approach 2: two points -> this might be easier to implement:
vector<vector<int>> twoSum2Ptr(vector<int> nums, int target) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> ans;
    int left = 0;
    int right = nums.size() - 1;
    while ( left < right ) {
        const int left0 = left;
        const int right0 = right;
        const int sum = nums[left] + nums[right];
        if ( sum > target ) {
            while ( left < right && nums[right] == nums[right0] ) right --;
        } else if ( sum < target ) {
            while ( left < right && nums[left] == nums[left0] ) left ++;
        } else {
            ans.push_back( {nums[left], nums[right]} );
            while ( left < right && nums[left] == nums[left0] ) left ++;
            while ( left < right && nums[right] == nums[right0] ) right --;
        }
    }
    return ans;
}
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

void print2dVec(vector<vector<int>>& vecs) {
    for (const auto& vec : vecs) {
        cout << "(";
        for (const auto& num : vec) {
            cout << num << ",";
        } cout << ") ";
    } cout << endl;
}

vector<vector<int>> twoSum(vector<int> nums, int target) {
    unordered_set<int> numSeen;
    unordered_set<int> pairSeen;
    vector<vector<int>> ans;
    for (int num : nums) {
        int comp = target - num;
        if ( !pairSeen.count(comp) && numSeen.count(comp) ) {
            ans.push_back({comp, num});
            pairSeen.insert(comp);
            pairSeen.insert(num);
        }
        numSeen.insert(num);
    }
    return ans;
}

vector<vector<int>> twoSum2Ptr(vector<int> nums, int target) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> ans;
    int left = 0;
    int right = nums.size() - 1;
    while ( left < right ) {
        const int left0 = left;
        const int right0 = right;
        const int sum = nums[left] + nums[right];
        if ( sum > target ) {
            while ( left < right && nums[right] == nums[right0] ) right --;
        } else if ( sum < target ) {
            while ( left < right && nums[left] == nums[left0] ) left ++;
        } else {
            ans.push_back( {nums[left], nums[right]} );
            while ( left < right && nums[left] == nums[left0] ) left ++;
            while ( left < right && nums[right] == nums[right0] ) right --;
        }
    }
    return ans;
}

void test1() {
    std::vector<int> nums = {-1,0, 1,1, 2, 3,3, 4, 5,5, 6,6, 7, 1, 5, 2, 4};
    int target = 6;
    {
        auto ans = twoSum(nums, target);
        print2dVec(ans);
    }
    {
        auto ans = twoSum2Ptr(nums, target);
        print2dVec(ans);
    }
}


vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int> > res;
    std::sort(nums.begin(), nums.end());
    const int n = nums.size();
    for (int i = 0; i < n; i++) {
        int target = -nums[i];
        int front = i + 1;
        int back = n - 1;
        while (front < back) {
            int sum = nums[front] + nums[back];
            // Finding answer which start from number nums[i]
            if (sum < target)
                front++;
            else if (sum > target)
                back--;
            else {
                vector<int> triplet = {nums[i], nums[front], nums[back]};
                res.push_back(triplet);
                // Processing duplicates of Number 2
                // Rolling the front pointer to the next different number forwards
                while (front < back && nums[front] == triplet[1]) front++;
                // Processing duplicates of Number 3
                // Rolling the back pointer to the next different number backwards
                while (front < back && nums[back] == triplet[2]) back--;
            }
        }
        // Processing duplicates of Number 1
        while (i + 1 < n && nums[i + 1] == nums[i]) 
            i++;
    }
    return res;
}

vector<vector<int> > fourSum(vector<int> &nums, int target) {
    vector<vector<int> > res;
    std::sort(nums.begin(),nums.end());
    const int n = nums.size();
    for (int i = 0; i < n; i++) {
        long long target_3 = (long long)target - (long long)nums[i];
        for (int j = i + 1; j < n; j++) {
            long long target_2 = target_3 - (long long)nums[j];
            int front = j + 1;
            int back = n - 1;
            while(front < back) {
                int two_sum = nums[front] + nums[back];
                if (two_sum < target_2) 
                    front++;
                else if (two_sum > target_2) 
                    back--;
                else {
                    vector<int> quadruplet(4, 0);
                    quadruplet[0] = nums[i];
                    quadruplet[1] = nums[j];
                    quadruplet[2] = nums[front];
                    quadruplet[3] = nums[back];
                    res.push_back(quadruplet);
                    // Processing the duplicates of number 3
                    while (front < back && nums[front] == quadruplet[2]) ++front;
                    // Processing the duplicates of number 4
                    while (front < back && nums[back] == quadruplet[3]) --back;
                }
            }
            // Processing the duplicates of number 2
            while(j + 1 < n && nums[j + 1] == nums[j]) ++j;
        }
        // Processing the duplicates of number 1
        while (i + 1 < n && nums[i + 1] == nums[i]) ++i;
    }
    return res;
}

int main() {
    test1();
    return 0;
}