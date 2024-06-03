* Instead of reference semantics, C++ uses values semantics by default. Arguments are passed by value.
* If we want reference semantics, we can have them

> C++ does not have garbage collection because it does not produce so much garbage

        int x{7}; // initialization
        vector<int> vec{0,1,2,5,6};

* The public members provide the interface of a class, "What it does". The private members provide the implementation of a class, "How it does it"

* Member functions are implemented as global functions. When a member function is called on an object, the object is passed **by address** in a hidden argument.

        Test test;
        test.func(1, 2.0);  // Called as Test::func(&test, 1, 2.0)
        // this would be equal to &test

  The pointer to the object is available with the keyword `this` 

* Copy constructor takes exactly one argument and this is a reference of an object of the class. If the argument was passed by value, the copy constructor would be called in an infinite loop!

        Test(Test& obj)

* Assignment operator return a reference of an object of the class to enable chained assignment statements like `a = b = c`

        Test& operator=(Test& other){
                i = other.i;
                return *this;
        }

* A pointer is a variable whose value is a memory address. This can be either in stack or in the heap.

* We allocate memory on the heap with `new`. Built-in types are left uninitialized. For classes, `new` calls the default constructor

* A pointer whose memory is not released with `delete` is called a dangling pointer. Accessing a dangling pointer results in undefined behavior

* A 1-d flattened array is more efficient than 2-d because we need 1 dereferencing instead of 2 to access an element.

        ar[i][j]
        ar[i*n + j]

* C++98 provided four new ways of casting an expression

        xyz_cast<Type>(expr)

  > An ugly operation should have an ugly syntactic form

* Four types of casting
  * `static_cast`: Converts an expression to a different type
  * `const_cast`:  Converts a const expression to the non-const equivalent
  * `reinterpret_cast`: Converts data in a buffer to untyped binary data
  * `dynamic_cast`: Converts pointer to base class object to pointer to derived

* `auto` keyword will only give the underlying type and will ignore const, reference, etc

* Iterators:
  * `iterator` : used to iterate forward
  * `const_iterator`: used to prevent modification
  * `reverse_iterator`: used to iterate backwards

* C++11 provides `begin()` and `end()` global functions that work even with built-in arrays

        std::string::iterator it = begin(str);
        std::string::const_iterator it = cbegin(str);

        int arr[] = {1, 2, 3, 4};
        auto it = begin(arr);

* In C++17 we can initialize local scope variable inside an if statement

        if (auto iter = begin(vec); iter != end(vec)){
                // iter is defined in this scope
        }

* C++17 introduced `[[fallthrough]]` attribute to denote intentional omitting of `break` statement

        switch (const int c = vec[0]; c){
                case '':
                  [[fallthrough]]
                case '\t':
                  [[fallthrough]]
                case '\n':
                  ++count;  // will be run on (c == '' || '\t' || '\n')
                  break;
        }

* With a regular function, the compiler only needs to be able to see its **declaration** when it is called. For a template function, the compiler must be able to see the full **definition** when it is called in order to generate the instantiated code.

* We can access a variable of the global namespace that has the same name as a local one using `::` in front of the variable's name. For example, `::counter`

* `using` directive will bring everything from a namespace into the global one.

        using xyz::Test

        Test          // xyz's Test
        abc::Test    // abc's Test
        ::Test       // Global class Test

* Data members of a class are default initialized if not set in the constructor. For built-in types the initial value is undefined.

* Copy constructor gets called when we pass and return objects by value in a function! Compiler's optimization sometimes can avoid this call.

* If no constructor, copy constructor, assign operator or destructor are defined, the compiler will synthesize "default" versions of them.

  * The default constructor takes no arguments and calls the constructor of each data member
  * Default copy constructor calls the copy constructor of each data member
  * Default assignment operator calls the assignment operator of each data member
  * Default destructor calls the destructor of each class-type data member

* User declared copy constructor (either user-provided, deleted or defaulted) prevents the implicit generation of a default constructor

* The implicitly-defined special member functions are typically incorrect if the class manages a resource whose handle is an object of non-class type (raw pointer, POSIX file descriptor, etc), whose destructor does nothing and copy constructor/assignment operator performs a "shallow copy" (copy the value of the handle, without duplicating the underlying resource).

* A RAII compatible class has to handle its resources on its own. This means that they should be created on the constructor and destroyed on the destructor. Furthermore, copy constructor and assignment operator must conduct a *deep copy* instead of a *shallow copy*

        class Test{
                int* ptr;
                int size;
            public:
                Test(int size){
                        ptr = new int[size];
                } 
                ~Test(){
                        delete[] ptr;
                }
                Test(const Test& arg){
                        size = arg.size;
                        ptr = new int[size];
                        std::copy(arg.ptr, arg.ptr + size, ptr);
                }
                Test& operator=(const Test& arg){
                        if (this != &arg){
                                delete[] ptr;
                                size = arg.size;
                                ptr = new int[size];
                                std::copy(arg.ptr, arg.ptr + size, ptr);
                        }
                        return *this;
                }
        
        };

> **Rule of Three**  If a class requires a user-defined destructor, a user-defined copy constructor, or a user-defined copy assignment operator, it almost certainly requires all three.

> **Rule of Five** Because the presence of a user-defined (or = default or = delete declared) destructor, copy-constructor, or copy-assignment operator prevents implicit definition of the move constructor and the move assignment operator, any class for which move semantics are desirable, has to declare all five special member functions.

> **Rule of Zero**  Classes that have custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership. Other classes should not have custom destructors, copy/move constructors or copy/move assignment operators.  -> If you can avoid defining default operations, do.

* Copy Elision means that the compiler is allowed to skip over a call to copy constructor in some cases

        Test temp{15};
        Test obj{temp};
        // temp not used again, can be omitted

> **Return Value Optimization**  A compiler optimization that involves eliminating the temporary object created to hold a function's return value.

        struct C {
                C() = default;
                C(const C&) { std::cout << "A copy was made.\n"; }
        };

        C f() {
                return C();
        }

        int main() {
                std::cout << "Hello World!\n";
                C obj = f();
        }