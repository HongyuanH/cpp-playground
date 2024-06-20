// https://www.geeksforgeeks.org/find-the-interval-which-contains-maximum-number-of-concurrent-meetings/

// C++14 implementation of the
// above approach
#include <bits/stdc++.h>
using namespace std;
 
bool cmp(vector<int> a,vector<int> b)
{
 
    if(a[0] != b[0])
        return a[0] < b[0];
         
    return a[1] - b[1];
}
 
// Function to find time slot of
// maximum concurrent meeting
void maxConcurrentMeetingSlot(
    vector<vector<int>> meetings)
{
     
    // Sort array by
    // start time of meeting
    sort(meetings.begin(), meetings.end(), cmp);
 
    // Declare Minheap
    priority_queue<int, vector<int>,
                       greater<int>> pq;
     
    // Insert first meeting end time
    // !!! This is wrong !!!
    // pq.push(meetings[0][1]);
 
    // Initialize max_len,
    // max_start and max_end
    int max_len = 0, max_start = 0;
    int max_end = 0;
 
    // Traverse over sorted array
    // to find required slot
    for(auto k : meetings)
    {
         
        // Pop all meetings that end
        // before current meeting
        while (pq.size() > 0 &&
                    k[0] >= pq.top()) {
            pq.pop();
        }
 
        // Push current meeting end time
        pq.push(k[1]);
 
        // Update max_len, max_start
        // and max_end if size of
        // queue is greater than max_len
        if (pq.size() > static_cast<long unsigned>(max_len))
        {
            max_len = pq.size();
            max_start = k[0];
            max_end = pq.top();
        }
    }
 
    // Print slot of maximum
    // concurrent meeting
    cout << max_len << " " << max_start << " " << max_end << endl;
}

void maxConcurrentMeetingSlotMyVer(
    vector<vector<int>> meetings);

// Driver Code
int main()
{
     
    // Given array of meetings
    vector<vector<int>> meetings = {{100, 300}, {145, 215}, {200, 230}, {215, 300}, {215, 400}, {500, 600}, {600, 700}} ;
                                      
    // Function call
    maxConcurrentMeetingSlot(meetings);
    maxConcurrentMeetingSlotMyVer(meetings);
}
 
// This code is contributed by mohit kumar 29

void maxConcurrentMeetingSlotMyVer(
    vector<vector<int>> meetings)
{
    vector<pair<int, int>> times;
    for (const auto& m : meetings) {
        times.push_back({m[0], 1});
        times.push_back({m[1], -1});
    }

    // Need to consider edge overlapping, sort based on ending time first and then starting time
    sort(times.begin(), times.end(), [](const pair<int, int>& p1, const pair<int, int>& p2){
        return (p1.first < p2.first) || ( (p1.first == p2.first) && (p1.second < p2.second) );
    });
    
    int maxMeetings = 0;
    int maxStart = 0;
    int maxEnd = 0;
    int nMeetings = 0;
    bool newMaxStart = false; // Can't use if (nMeetings == maxMeetings - 1) because of potential edge overlapping
    for (const auto& t : times) {
        nMeetings += t.second;
        if (nMeetings > maxMeetings) {
            maxMeetings = nMeetings;
            maxStart = t.first;
            newMaxStart = true;
        } else if (newMaxStart && t.first != maxStart) {
            maxEnd = t.first;
            newMaxStart = false;
        }
    }

    cout << maxMeetings << "," << maxStart << "," << maxEnd << endl;
}