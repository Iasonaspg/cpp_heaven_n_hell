## C++ Philosophy

> **Zero Overhead Principle** If you have an abstraction it should not
cost anything compared to write the equivalent code at lower
level. For example, if I have say a matrix multiply it should be written in a
such a way that you could not drop to the C level of abstraction
and use arrays and pointers and such and run faster

* **Statically Typed Language**: Enforce safety at compile time whenever possible

  * Catch many bugs at compile time instead of run time
  * The type annotation makes the code more readable
  * Promote compiler optimizations and runtime efficiency
  * Allow users to define their own type system

> C makes it easy to shoot yourself in the foot; C++ makes it harder,
but when you do, it blows your whole leg off

> C++ does not have garbage collection because it does not produce so much garbage

## Details

* Instead of reference semantics, C++ uses values semantics by default. Arguments are passed by value.

* If we want reference semantics, we can have them


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

* A class can define a conversion operator that is used to convert an object of the class to another type. For example,

        class Test{
             int i;
           public:
             operator int() const { return i; }
        };

* When we call a function, the compiler has to find a function that resolves the call

        Test test;
        int x = test + 5; 
        
  First it tries to find a + operator that takes Test and int. If it is not found, it will try to convert the argmuments. If we want to avoid implicit conversions we can use the `explicit` keyword.

        explicit operator int() const { return i; }

        Test test;
        cout << test; // error
        cout << static_cast<int>(test);

* A constructor with one argument acts as an implicit conversion operator!

        class Test{
             int i;
           public:
             Test(int i) : i(i) {}
        };

        Test test = 4;

  Here we will have a call to constructor to create a temp object and a call to copy constructor to create *test* object. For this reason we can make a constructor `explicit`.

* We can force the compiler to synthesize special member functions by using the `default` keyword after their declaration.

        class Test{
          public:
            Test() = default;
            Test(const Test&) = default;
            Test& operator=(const Test&) = default;
        };

* We can create uncopyable class objects using the `delete` keyword. Before that, we could achieve the same result by defining the copy constructor and the assignment operator as private methods.

        class Test {
          public:
           Test(const Test&) = delete;        // Deleted copy constructor
           Test& operator =(const Test&) = delete;  // Deleted assignment operator
           Test() = default;          // Synthesized default constructor
        };

  With the introduction of move constructors and move assignment operators, the rules for when automatic versions of constructors, destructors and assignment operators are generated has become quite complex. Using `default` and `delete` makes things easier as you don't need to remember the rules: you just say what you want to happen.

* Operators take either one or two arguments and can be member functions or non-member functions. 
        
  * Unary operators are usually member functions
  * Binary operators are usually non-member functions

        c = a - b // int operator-(Type a, Type b);
        c = -b;   // int operator-(Type a);

* When we write our own class, C++ does not provide default operators except for operator=(). We can define our own operators by created overloaded versions of them. We must use the same symbols and syntax as built-in operators.

* In general programming, we usually overload the following operators

  * Assignment operator=
  * Equality operator==
  * Inequality operator!=
  * Less than operator<
  * Function call operator()

* Operators that should not be overloaded

  * Built-in logical && and || operators have a guaranteed order execution and provide "short-circuit" evaluation which cannot be done with user-defined functions
  
        true && false && true /* This is evaluated from left to right. As soon as the first comparison is false, it will stop evaluating the later expressions -> short circuit */
  * Address-of (&) and comma (,) are already defined for classes and redefinition will cause confusion
  * Scope (::), . operator, .* operator and ternary operator (?:) cannot be overloaded.

* We can give access to all the class's members to another non member function or class by using the `friend` keyword.

        class Test{
             {...}
           public:
             friend void print(const Test& test);
             friend class Example;
        };

        // Example and print can access even private members of Test

* Operators should be usually implemented as member functions

  * Provides direct access to private data
  * Keeps all class-related code together

* Binary **member** operators do not work if we need a type conversion on their arguments.

        class String {
	        string s;
          public:
	        String operator +(const String& arg) {
		        return s + arg.s;
	        }
        }

        String c = a + b; // transformed to a.operator+(b)
        String c = "hello" + b; // Compiler error. Cannot cast and detect the member operator. Searches for a non-member function.

  * \+ operator defined as non-member to allow type casting

        T operator +(const T& lhs, const T& rhs);
        a + b -> operator +(a,b)
  * += operator defined as member because it modifies the object

        T operator +=(const T& rhs);
        a += b -> a.operator +=(b)
  * == / != ty operator

        bool operator ==(const T& lhs, const T& rhs);
        a == b -> operator(a,b);

  *  < operator
        
         bool operator <(const T& lhs, const T& rhs);
         a < b -> operator <(a,b);

     The standard library using the < operator to sort elements. Container elements that will be sorted or elements that are inserted in sorted containers must implement < operator.

* The prefix and postfix ++ / -- operators increase / decrease their argument

        ++p -> p += 1; return p;
        p++ -> temp = p; p += 1; return temp;  Less efficient

* The prefix operator returns a reference to the incremented object

        Test& operator ++(){
           ++member;
           return *this;
        }

* The postfix operator takes a dummy argument in order to have a different signature than prefix operator. We cant overload based only on the return type!

        Test operator ++(int t){
           Test temp(*this);
           ++member;
           return temp;
        }

* Callable objects are used to  to implement functional programming in C++

* A C++ class that implements the function call operator() is called a *functor*.

        class Test{
           {...}
           T operator()(...){
                ...
           }
        };
        
        Test test;
        test(...) -> test.operator()(...);

* Functors can have data members to store values which need to be kept between function calls. This is known as *state*.

* We can overload the print operator `<<` with any class using the following prototype.

        std::ostream& operator <<(std::ostream& os, const Test& test){
           test.print(os);
           return os;
        }

        void Test::print(std::ostream& os) const{
           os << "i = " << i << ", str = " << str << endl;
        }

  std::ostream objects are not copyable so we have to return a reference.

* Many algorithms like `std::find()` call a function on each element that returns a bool. This function is called a *predicate* and usually we can provide our own.

* When the compiler encounters a lambda function, it will generate code that defines a functor. Its call operator will have the same body and return type as the lambda.

* In C++11, the return type of a lambda function is deduced only when there is a single statement that returns a value. For more complex expressions we need to define the return type

        [](int n) -> bool{

        }

* In later versions the return type is always deduced automatically.
  * In C++14 the return type must be the same in all return paths
  * In C++17 this is no longer a requirement

* A lambda with capture is implemented as a functor with state.
  * The functor has a private data member that stores the the variable
  * It is initialized in the functor's constructor
  * The data member is available in the operator() body
 
* By default the captured value is passed to the functor's constructor by value. The functor will have its own copy of the variable

* We can capture variables by reference and those references are stored inside the functor instead of actual copies

        int n{5}, idx{-1};
        [n, &idx](const string& x){
                ++idx;
                return (x.size() > n);
        }

* We can capture all the the local variables in a scope using *implicit capture*
  * `[=]`  Captures all variables by value
  * `[&]` Captures all variables by reference

* Usually it is safer to capture by reference only those variables that have to be modified or have expensive copy semantics.
        
        [=, &x] // capture x by reference, anything else by value
        [&, =a, =b] // make sure a and b are not altered

* We can call a lambda function inside a member function as below. In order to access object's members we have to capture the object itself. This is done by capturing `[this]`. The capture is done by reference. In order to capture the object by value we use `[*this]` in C++17

        lass Test {
        int time{10};
        public:
        void countdown() {       
        	[this]() {    
		      if (time > 0) cout << time << endl;
			else if (time == 0)
				cout << "Liftoff!" << endl;
	      --time;
	     }();
        }
       };

* When we capture by reference, we must make sure the captured variable is still valid when the lambda is called

* We can use the `auto` keyword for the type of the arguments to the lambda expression. These are known as *generic lambdas* or *polymorphic lambdas*

* We can define variables in the local scope of a lambda function. Those must be initialized as their type is auto

        [y=2](int x){
                return x + y;
        }

* An insert iterator adds new elements in a container. There are three types of insert iterators
  * `std::back_inserter_iterator` adds an element at the back
  * `std::front_inserter_iterator` adds an element at the front
  * `std::insert_iterator` adds an element at any given position
  
* To add a new element in any container, we assign a value to the corresponding insert iterator

        std::vector<int> vec;
        auto it = std::back_inserter(vec);
        *it = 99; // calls vec.push_back(99)

* Comparison between pointers is only well-defined if both are part of the same array. Otherwise, the result is unspecified. Note that this isn't the same as undefined behavior. 

   `<` on pointers doesn't necessarily implement a strict-weak ordering, whereas `std::less` (via specialisation) is required to:

  > For templates less, greater, less_equal, and greater_equal, the specializations for any pointer type yield a result consistent with the implementation-defined strict total order over pointers.

  `std::less` and friends let us make the code generic and flexible without having to write a lot of boiler plate.

        template<typename Iterator, typename Cmp>
        void sort(Iterator begin, Iterator end, 
          Cmp c = std::less<typename std::iterator_traits<Iterator>::value_type>) 
          { /* sort here using c /* } 
  
   This allows you to sort all built in types, any type that has an `operator<()`, and lets you specify any other way you want to compare elements in the data to sort them.

   Instead of a functor like `std::less`, we could use a pointer to a comparison function but this would not work for built-in types as there is no function you can point to! 
   
   Operators aren't available as functions. Invoking built-in operators as functions will not work simply because the language specification never says that such functions exist. Built-in operators are just operators. **Function-based implementations are specific to overloaded operators only.**

        int a, b, c; 

        c = a + b;          //works 
        c = operator+(a,b); //fails to compile, 'operator+' not defined. 

* A temporary cannot be bound to a non-const reference, but it can be bound to const reference. This is allowed because the const reference extends the lifetime of a temporary object until its deallocation.

        const Foo& a = Foo(15);
        a.print(); // works, the temporary is now bound to the reference a

* 