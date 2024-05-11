#include <iostream>

using namespace std;

void func(int i) {
	cout << "func(int) called\n";
}

void func(int *i) {
	cout << "func(int *) called\n";
}

int main() {
	func(NULL); // does not compile with gcc, calls func(int) with VC++, func(int*) with clang
	func(nullptr);
}

// nullptr is a literal representing a null pointer and is compatible with any pointer type but cannot be converted to int
// The traditional value of NULL is 0 and its type is implementation defined