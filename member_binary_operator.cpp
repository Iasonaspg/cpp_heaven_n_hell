#include <iostream>
#include <string>

using namespace std;

class String {
	string s;
public:
	// Constructors
	String(const char* str): s(str) {}
	String(const string& s): s(s) {}

	String operator +(const String& arg) {
		return s + arg.s;
	}

	void print() { cout << s << endl; }

    // friend String operator +(const String& arg1, const String& arg2);
};

// String operator +(const String& arg1, const String& arg2){
// 	return arg1.s + arg2.s;
// }

int main() {
	String w{"world"}, bang{"!"};
	String wbang = w + bang;              // Calls w.operator + (bang); OK
	String hi = "hello " + w;             // Cannot call "hello".operator + (w); Compiler error! Will work with non member binary operator
	cout << "wbang: "; wbang.print();
	cout << "hi: "; hi.print();
}