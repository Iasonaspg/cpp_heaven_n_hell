#include <iostream>

using namespace std;

class Foo{
    private:
        int i;

    public:
        Foo(const int j) : i(j) {} // constructor
        Foo(const Foo& other) : i(other.i) { // copy constructor
            // other must be passed by ref. Otherwise, the copy constructor would call itself
            // recursively forever as the argument should be copied to a local function object first!
            std::cout << "Copy constructor operator called\n";
        }
        Foo& operator=(const Foo& other){ // assignment operator
            // returns a reference to avoid calling of copy constructor
            // In a = b = c, the assignment operator is called twice, b = c and a = b
            // If a non reference was returned, each assignment would call also the copy constructor to return a new object 
            // Theoretically, we can also create a void operator=() but will not be identical to primitive types
            // The returned value of the operator is only stored in chained assignment
            std::cout << "Assignment operator called\n";
            i = other.i;
            return *this;
        }
        ~Foo(){
        }

        void print(){
            cout<<"i: " << i << endl;
        }
};

int main(){
    Foo a(5);
    Foo b = a; // copy constructor
    Foo c(0), d(10);
    c = d; // assignment operator
    // c = a is equivalent to c.operator=(a) which indicates that it could be void
    
    (a = b) = c; // If operator=() returns a ref, it works.
    // Otherwise, wrong result. c is assigned to a temp object that is not stored.

    c.print();
    a.print();

    return 0;
}