#include <iostream>

using namespace std;

int main(){
    int* p;
    cout << "p uninitialized : " << p << endl;

    p = new int[4];
    cout << "p after new : " << p << endl;

    delete[] p;
    cout << "p after delete : " << p << endl;
    
    return 0;
}