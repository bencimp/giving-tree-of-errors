#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include <regex>
#include <unordered_set>

//By Ben Cimperman

using namespace std;

string recurThroughTree(unordered_map<char, vector<char>> map, char key);

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    //Since the definition of this is recursive, I imagine that the solution will likely be recursive, at least when it comes to printing.
    //This problem has n parts, as far as I see from an initial glance.
    //The first part is parsing the input into a binary tree and storing it. Preliminarily, I'm thinking about using a Vector or other linearly-arranged array-like container where children of node n can
    //  be stored in slots 2n and 2n+1. This makes it very quick to check for the presence of any children, and overall is pretty quick and not space-inefficient.
    //The second part is error checking.
    //  E1 (Invalid Input) is pretty trivial to check for using a regular expression.
    //  E2 (Duplicate Pairs) could be checked for in a number of ways, and won't be too great of a challenge. Doing it time efficiently, however, may be a different story. Naive solution would be a
    //      nested for loop, I'm gonna go with a hashmap for lower time complexity.
    //  E3 (Too Many Children) is pretty simple to solve: you could either use a map, or you could check for an open slot in the implementation I discussed in the first part when you try to assign a new
    //      member to the slot.
    //  E4 (Multiple Roots)
    //  E5 (Looping Structure) might be a tough one to run time-efficiently. It may also make sense to implement this in a
    //The third part is printing it in lexicographically smallest order. This could be done by checking to ensure that they are inserted in this order and recursively moving the children to new slots, or
    //  by sorting the input first, although that may be a bigger headache than it's worth. Another way of accomplishing this would be through checking at print time which is lexicographically smaller
    //  and simply calling the recursive print function on the smaller first. This solution is naive, though, and may add additional time complexity.


    //Take input
    string input;
    getline(cin, input);

    //This regex checks for error 1 and prints an error and returns 0 if there is not a match.
    regex error1 ("(\\([A-Z],[A-Z]\\)\\s?)+\\n?");
    if(!(regex_match(input, error1))){
        cout << "E1";
        return 0;
    }

    //This splits the input (which we have just sanitized) into strings and pushes them into a "staging" vector.
    vector<string> stageVec;
    string hold;
    stringstream myStream(input);
    while(!myStream.eof()){
        getline(myStream, hold, ' ');
        stageVec.push_back(hold);
    }


    //This unordered map checks for error 2, printing an error if there are any duplicate entries.
    unordered_map<string, int> error2;
    for(unsigned int x = 0; x < stageVec.size(); x ++){
        error2[stageVec[x]] += 1;
        if (error2[stageVec[x]] == 2){
            cout << "E2";
            return 0;
        }
    }

    //This unordered map checks for error 3 by counting the number of children for each node and rejecting if it has more than three. This test abuses the fact that I've sanitized my inputs already by
    //  just looking at the second character in the element of stageVec we grab. Cheeky!
    unordered_map<char, int> error3;
    for(unsigned int x = 0; x < stageVec.size(); x ++){
        error3[stageVec[x][1]] += 1;
        if (error3[stageVec[x][1]] > 2){
            cout << "E3";
            return 0;
        }
    }

    //This unordered map checks for error 4 using an unordered set to collect all of the names of nodes, and then checking to see if that node has any parents using yet another unordered map.
    unordered_set<char> nodeNames;
    for (unsigned int x = 0; x < stageVec.size(); x ++){
        nodeNames.insert(stageVec[x][1]);
        nodeNames.insert(stageVec[x][3]);
    }
    unordered_map<char, vector<char>> error4;
    for(unsigned int x = 0; x < stageVec.size(); x ++){
        error4[stageVec[x][3]].push_back(stageVec[x][1]);
    }
    bool foundRoot = false;
    char root;
    for (auto x : nodeNames){
        if (error4.find(x) == error4.end()){
            if (foundRoot == false){
                foundRoot = true;
                root = x;
            }
            else{
                cout << "E4";
                return 0;
            }
        }
    }

    //This test is for error 5. An interesting property of cycles in input is that they will all come about when a node has more than one parent. So, if we check each node to ensure it only has one
    //  parent using a test very similar to test three, we will avoid input loops entirely.
    unordered_map<char, int> error5;
    for(unsigned int x = 0; x < stageVec.size(); x ++){
        error5[stageVec[x][3]] += 1;
        if (error5[stageVec[x][1]] > 1){
            cout << "E5";
            return 0;
        }
    }

    //This function parses and prints the tree.
    unordered_map<char, vector<char>> map;
    for(unsigned int x = 0; x < stageVec.size(); x ++){
        map[stageVec[x][1]].push_back(stageVec[x][3]);
    }
    cout << "(" + recurThroughTree(map, root) + ")";

    return 0;
}

string recurThroughTree(unordered_map<char, vector<char>> map, char key){
    //find() the key. This initially caused much consternation, because I was getting a rather confusing segfault. Turns out, since nodes without children don't have entries in the map.
    string pass(1,key);
    auto holder = map.find(key);
    if(holder == map.end()){
        return pass;
    }
    auto valueVec = holder->second;
    //if it has one child, call recurThroughTree, passing in the child
    if(valueVec.size() == 1){
        return pass + "(" + recurThroughTree(map, valueVec[0]) + ")";
    }
    //if it has two children (which it must to have reached this point), sort them lexicographically, then call recurseThroughTree on them in order
    else{
        if(valueVec[0] < valueVec[1]) return pass + "(" + recurThroughTree(map, valueVec[0]) + ")("+ recurThroughTree(map, valueVec[1]) + ")";
        if(valueVec[0] > valueVec[1]) return pass + "(" + recurThroughTree(map, valueVec[1]) + ")("+ recurThroughTree(map, valueVec[0]) + ")";
    }
    return "";
}
