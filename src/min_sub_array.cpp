/* 
https://leetcode.com/problems/minimum-size-subarray-sum/

Generic template for finding min sub array using sliding window:

int findMinSubArray(vector<int> nums) {
    int i = 0;
    int j = 0;
    int ans = INT_MAX;
    while (j < nums.size()) {
        // move j and process nums[j], e.g. add
        while (condition met) { // e.g. sum of sub array >= target
            ans = min(ans, j-i+1);
            // move i and process nums[i], e.g. delete
            i++;
        }
        j++;
    }
    return ans;
}
*/

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

namespace test1 {

//https://leetcode.com/problems/minimum-size-subarray-sum
int minSubArrayLen(int target, vector<int>& nums) {
    int i = 0;
    int j = 0;
    int ans = INT_MAX;
    int sum = 0;
    while ( j < static_cast<int>(nums.size()) ) {
        sum += nums[j];
        while ( sum >= target ) {
            ans = min( ans, j - i + 1 );
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return ans == INT_MAX ? 0 : ans;
}

//
int findMinLengthGoodArray(vector<int> nums, int k) {
    // An array is GOOD if it has at least k distinct integers
    // Find the minimum length of sub array from `nums` that is GOOD
    unordered_map<int, int> m;
    int i = 0;
    int j = 0;
    int ans = INT_MAX;
    while (j < static_cast<int>(nums.size())) {
        m[nums[j]] ++; // keep count of distinct integers
        while (static_cast<int>(m.size()) >= k) {
            ans = min(ans, j-i+1);
            if (m[nums[i]] == 1) {
                m.erase(nums[i]);
            } else {
                m[nums[i]]--;
            }
            i++;
        }
        j++;
    }
    if (ans == INT_MAX) {
        return -1;
    } else {
        return ans;
    }
}

void test1() {
    cout << findMinLengthGoodArray({1, 2, 2, 2, 3}, 3) << endl;
    cout << findMinLengthGoodArray({1, 2, 2, 2, 3, 2, 2, 1}, 3) << endl;
    cout << findMinLengthGoodArray({1, 2, 2, 2, 3, 2, 2, 1, 2, 3}, 3) << endl;
}
}

int main() {
    test1::test1();
    return 0;
}