#include <iostream>
#include <vector>
using namespace std;

// Vector for storing disks for each tower; 0 indicates no disk on the tower
vector<int> towerA = {0};
vector<int> towerB = {0};
vector<int> towerC = {0};

void iterativeHanoi(int);

int main() {
    int nDisk; // # of disks

    cout << "Solving the tower of Hanoi iteratively!\nEnter the number of disks: ";
    cin >> nDisk;
    
    iterativeHanoi(nDisk);
 
    return 0;
}

void iterativeHanoi(int nDisk) {
    // initialize tower A
    for (int i=0; i<nDisk; i++) towerA.push_back(i+1);

    // if # of disks is even, the first move should be between A and B
    if ( nDisk % 2 == 0 ) {
        while ( towerC.size() != nDisk + 1 ) { 
            // allowed move between A and B
            if (towerA.back() > towerB.back()) {
                towerB.push_back(towerA.back());
                towerA.pop_back();
                cout << "Moving disk " << (nDisk-towerB.back()+1) << " from A to B" <<  endl;
            }
            else if (towerB.back() > towerA.back()) {
                towerA.push_back(towerB.back());
                towerB.pop_back();
                cout << "Moving disk " << (nDisk-towerA.back()+1) << " from B to A" <<  endl;
            }

           // allowed move between A and C
            if (towerA.back() > towerC.back()) {
                towerC.push_back(towerA.back());
                towerA.pop_back();
                cout << "Moving disk " << (nDisk-towerC.back()+1) << " from A to C" <<  endl;
            }
            else if (towerC.back() > towerA.back()) {
                towerA.push_back(towerC.back());
                towerC.pop_back();
                cout << "Moving disk " << (nDisk-towerA.back()+1) << " from C to A" <<  endl;
            }

           // allowed move between B and C
            if (towerB.back() > towerC.back()) {
                towerC.push_back(towerB.back());
                towerB.pop_back();
                cout << "Moving disk " << (nDisk-towerC.back()+1) << " from B to C" <<  endl;
            }
            else if (towerC.back() > towerB.back()) {
                towerB.push_back(towerC.back());
                towerC.pop_back();
                cout << "Moving disk " << (nDisk-towerB.back()+1) << " from C to B" <<  endl;
            }
        }
    }
    // if # of disks is odd, the first move should be between A and C
    else{
        while ( towerC.size() != nDisk ) { 
            // allowed move between A and C
            if (towerA.back() > towerC.back()) {
                towerC.push_back(towerA.back());
                towerA.pop_back();
                cout << "Moving disk " << (nDisk-towerC.back()+1) << " from A to C" <<  endl;
            }
            else if (towerC.back() > towerA.back()) {
                towerA.push_back(towerC.back());
                towerC.pop_back();
                cout << "Moving disk " << (nDisk-towerA.back()+1) << " from C to A" <<  endl;
            }

            // allowed move between A and B
            if (towerA.back() > towerB.back()) {
                towerB.push_back(towerA.back());
                towerA.pop_back();
                cout << "Moving disk " << (nDisk-towerB.back()+1) << " from A to B" <<  endl;
            }
            else if (towerB.back() > towerA.back()) {
                towerA.push_back(towerB.back());
                towerB.pop_back();
                cout << "Moving disk " << (nDisk-towerA.back()+1) << " from B to A" <<  endl;
            }

           // allowed move between B and C
            if (towerB.back() > towerC.back()) {
                towerC.push_back(towerB.back());
                towerB.pop_back();
                cout << "Moving disk " << (nDisk-towerC.back()+1) << " from B to C" <<  endl;
            }
            else if (towerC.back() > towerB.back()) {
                towerB.push_back(towerC.back());
                towerC.pop_back();
                cout << "Moving disk " << (nDisk-towerB.back()+1) << " from C to B" <<  endl;
            }
        }
    }
}
