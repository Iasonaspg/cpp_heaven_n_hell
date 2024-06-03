#include <iostream>

using namespace std;

class A{
    private:
        int i;

    public:
        A(const int j) : i(j) {}
        A& operator=(const A& other){
            std::cout << "Assignment operator called\n";
            i = other.i;
            return *this;
        }

        void print(){
            cout<<"i: " << i << endl;
        }
};

class B{
    private:
        int i;

    public:
        B(const int j) : i(j) {}
        B operator=(const B& other){ // creates a copy during return
            std::cout << "Assignment operator called\n";
            i = other.i;
            return *this;
        }

        void print(){
            cout<<"i: " << i << endl;
        }
};

int main(){
    {
        A a(0), b(10), c(20);
        B d(30), e(40), f(50);

        a = b = c; // correct
        d = e = f; // correct but inefficient

        a.print();
        d.print();
    }

    A a(0), b(10), c(20);
    B d(30), e(40), f(50);

    (a = b) = c; // correct
    (d = e) = f; // wrong, f is assigned to an object that is temporary

    a.print();
    d.print();

    return 0;
}