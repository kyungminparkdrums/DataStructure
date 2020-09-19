#include <iostream>
#include <time.h>
using namespace std;

void recursiveHanoi(int, char, char, char);

int main() {
    int nDisk; // # of disks
    //clock_t start, stop; 

    cout << "Solving the tower of Hanoi recursively!\nEnter the number of disks: ";
    cin >> nDisk;
    
    //start = clock();
    recursiveHanoi(nDisk,'A','B','C');
    //stop = clock();

    //cout << "Duration: " << (double)(stop-start) / CLOCKS_PER_SEC << "s" << endl;
 
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

