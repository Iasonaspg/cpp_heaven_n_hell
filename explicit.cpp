#include <iostream>
#include <string>

using namespace std;

class Test {
	int i{42};
	string str;
public:
    explicit Test(int i) : i(i) {}
	explicit operator int() const { return i; }           // Explicit conversion operator to int
};

int main() {
	Test test{4};

	// cout << test << endl; // Error: no match for operator <<
	cout << static_cast<int>(test) << endl;

    Test a = 4; // would work if Test(int i) was not explicit. 
    Test b = Test(4);
}