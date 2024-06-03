#include <iostream>
#include <string>

using namespace std;
using namespace std::literals;

int main() {
	const char *cca = "Hello, world!";
	cout << "cca: " << cca << endl;
	
	string str = "Hello, world!"s;
	cout << "str: " << cca << endl;
	
	cout << "std::string literal: " << "Hello"s  + ", world!"s << endl;

    // String literal with escaped characters
	string url = "<a href=\"file\">C:\\\"Program Files\"\\</a>\\n";
	cout << "URL is " << url << endl << endl;
	
	// Raw string literal with unescaped characters
	string raw_url = R"(<a href="file">C:\"Program Files"\</a>\n)";
	cout << "Raw URL is " << raw_url << endl << endl;
	
	// Raw string literal with delimiter x to define opening and closing parenthesis
	string delimited_url = R"x(<a href="file">C:\"Program Files (x86)"\</a>\n)x";
	cout << "Delimited URL is " << delimited_url << endl << endl;
}