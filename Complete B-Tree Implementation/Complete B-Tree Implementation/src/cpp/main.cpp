#include <iostream>
#include "../header/B-Tree.h" 
using namespace std;

int main(int argc, char* argv[]) {

    BTree b(6);
    int arr[] = {25, 10, 22, 63, 70, 101, 
        1, 9, 11, 12, 14, 23, 
        24, 60, 61, 62, 66, 67, 
        68, 80, 100, 105, 106, 200};
    //int arr[] = { 10, 22, 63, 70, 
    //    1, 9, 11, 12, 14, 23,
    //    66, 67,
    //    68, 80, 100, 200 };
    for (int num : arr) { b.Insert(num); } 

    b.printLevel(0);
    cout << endl;
    b.printLevel(1);
    cout << endl;
    b.printLevel(2);
    cout << endl;
    cout << endl;

    cin.get();
}