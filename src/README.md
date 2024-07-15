### Tree
* BFS - queue & iterative
  BFS with level: for loop pop
* DFS - stack & iterative OR recursive
* Left first or right first 
* Traversal: 
  inorder - BST -> sorted array
* Impl: std::map (Red Black Tree), std::set (Red Black Tree)
* Validate BST: `bool isValidBST(Node* root, int mn, int mx)`

### List
* 2 pointers (slow/fast pointers) can be used to:
  * Check if cycle exists
  * Find where the cycle begins `x+y=N*length of cycle`
  * Find middle node
  * Find nth node from the end
* Sort: merge sort (find middle + sort + merge)
* Intersection? jump to the other list once x + z + y = y + z + x
* Copy? hashmap if random jump

### Graph
* Copy: hashmap 
* Topological Sort: stack + visited + recursion over all nodes, `O(V+E)`, V is number of vertices, E is number of edges

### HashMap
* Implementation: HashFunction (uniformly distributed) for Keys + array for List (collision) of Values
* Impl: std::unordered_map
* !There's no standard way of calculating hash from a pair, so in order to use pair as key we need to provide a customer hash function: https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/

### Queue/Stack

### Binary Search
* Inclusive: `left = 0`, `right = n -1`
* Termination condition: `while (left <= right)`
* `return left` or `return right`: 
  - when looking for `number <= x`, `return right` because `left > right` when the loop ends
  - when looking for `number >= x`, `return left` because `left > right` when the loop ends
  - when looking for insertion position, it's essentially the same as looking for `number >= x`, so `return left`
```cpp
int search(vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;
    int mid = (left + right) / 2;
    while (left <= right) { // when left == right, there's one element to check
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        } else {
            return mid;
        }
        mid = (left + right) / 2;
    }
    return -1;
}
```

```python
def findFirstElement_rightInclusive(nums, target):
    if len(nums) == 0:
        return -1
    if target > nums[-1]:
        return -1
    left = 0
    right = len(nums) - 1
    while left <= right:
        mid = (left + right) // 2
        if nums[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    if nums[left] != target:
        return -1
    else:
        return left

def findLastElement_rightInclusive(nums, target):
    if len(nums) == 0:
        return -1
    if nums[0] > target:
        return -1
    left = 0
    right = len(nums) - 1 
    while left <= right:
        mid = (left + right) // 2
        if nums[mid] > target:
            right = mid - 1
        else:
            left = mid + 1
    if nums[right] != target:
        return -1
    else:
        return right
```

### Two Pointers
```cpp
int left = 0;
int right = 1;
int ans = 0;
while ( right < N ) {
    while ( left < right && condition ) {
        logic1();
        left ++;
    }
    logic2();
    right ++;
}
```

### Heap
* heap is not fully sorted, but only maintain a property that parent.val <= child.val, so it takes log(n) to do insert, delete
* when building a heap, `heapify()` needs to be called on each subtree, intuitively complexity is O(n*logn) but it can be proved that it's O(n) because most subtrees have a small height, and there're less nodes at bigger heights
* usages: 
  - Heap Sort (heapify and extract top for N times)
  - Priority Queue
  - Graph Algorithms: Dijkstra’s Shortest Path? Prim’s Minimum Spanning Tree?
  - K’th Largest Element in an array -> Can also use Quick Select which is O(n) on average
  - Sort an almost sorted array
  - Merge K Sorted Arrays

### Sorting 

| Algorithm      | Average   | Best     | Worst    | Space  |
|----------------|-----------|----------|----------|--------|
| Selection Sort | O(n^2)    | O(n^2)   | O(n^2)   | O(1)   |
| Insertion Sort | O(n^2)    | O(n)     | O(n^2)   | O(1)   |
| Bubble Sort    | O(n^2)    | O(n)     | O(n^2)   | O(1)   |
| Merge Sort     | O(nlogn)  | O(nlogn) | O(nlogn) | O(n)   |
| Heap Sort      | O(nlogn)  | O(nlogn) | O(nlogn) | O(1)   |
| Quick Sort     | O(nlogn)  | O(nlogn) | O(n^2)   | O(1)*  |
| Count Sort     | O(n+k)*   | O(n+k)   | O(n+k)   | O(k)   |
| Bucket Sort    | O(n+k)*   | O(n+k)   | O(n^2)   | O(n)   |
| Radix Sort     | O(d(n+b))*| O(d(n+b))| O(d(n+b))| O(n+b) |

* Quick Sort can take O(n) space due to recursion
* k is the number of target numbers in Count Sort, or the number of buckets in Bucket Sort
* b is the base for representing numbers Radix Sort, for decimals, this is 10
* d is the number of digits in Radix Sort

**Quick Sort**
- Worst case is when picking the last element as pivot: `T(n) = T(0) + T(n-1) + O(n)` -> `T(n) = O(n^2)`
- Best case is when picking the middle as pivot: ` T(n) = 2T(n/2) + O(n)` -> `T(n) = O(nlogn)`
- Average case: ` T(n) = 1/n*SUM[T(i)+T(n-i-1)] + O(n)` -> `T(n) = O(nlogn)`

**Bucket Sort**
- Bucket Sort time complexity: 
  * Create buckets: `O(k)`
  * Place items into buckets: `O(n)`
  * Sort each bucket (assuming insertion sort): `k*O(m^2)=O(n^2/k)`, assuming `m=n/k`, then this is `O(n)`
  * Normally k ~ n is chosen so that if inputs are uniformly distributed, on average complexity is `O(k)+O(n)+O(n^2/k)=O(n)+O(n)+O(n)=O(n)`

**Radix Sort**
- For decimal numbers, b=10, if k is the maximum number possible, then complexity is `O(d*(n+10))=O(logk*(n+10))=O(nlogk)`, if logk is small, it becomes `O(n)`
