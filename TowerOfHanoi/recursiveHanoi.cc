#include <iostream>
using namespace std;

void recursiveHanoi(int, char, char, char);

int main() {
    int nDisk; // # of disks

    cout << "Solving the tower of Hanoi recursively!\nEnter the number of disks: ";
    cin >> nDisk;
    
    recursiveHanoi(nDisk,'A','B','C');
 
    return 0;
}

void recursiveHanoi(int nDisk, char source, char tmp, char target) {
    if ( nDisk == 1 ) { 
        cout << "Moving disk " << nDisk << " from " << source << " to " << target <<  endl;
        return;
    }

    // recursion, with changing source/target/tmp towers
    recursiveHanoi(nDisk-1, source, target, tmp); 
    cout << "Moving disk " << nDisk << " from " << source << " to " << target << endl;
    recursiveHanoi(nDisk-1, tmp, source, target);
}

