#include <iostream>

using namespace std;

class Test {
	int time{10};              // Data member
public:
	void countdown() {        // Member function which calls a lambda expression
		[this] () {        			// Capture class object by reference in order to have access and modify time
			if (time > 0)
				cout << time << endl;
			else if (time == 0)
				cout << "Liftoff!" << endl;
			--time;
		}();			 			   			// Call the lambda function
	}
};

/* 
Every member function is called with a pointer to the caller object as argument!
Test::countdown(&test) and &test is accessed by this inside the method
*/
int main() {
	Test test;
	for (int i = 0; i < 12; ++i)
		test.countdown();
}