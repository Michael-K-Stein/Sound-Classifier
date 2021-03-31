# C++ Research
  C++ (or c++, CPP, cpp) is one of the most widely used programming languages. C++ is essentially an extension to the classic C programming language, created by Bjarne Stroustrup. Bjarne Stroustrup developed C++ in 1979 at Bell Labs, as a way to enhance C with more features - most importantly: classes and object oriented programming.
	As C++ has classes, let’s dive into Object Oriented Programming (OOP):
## OOP - Object Oriented Programming
  Object Oriented Programming is a method of coding based on objects (classes), rather than Procedural Programming which is based on procedurally calling functions and thus creating routines and subroutines. Classes in OOP contain both code and data, and can interact with other classes, while Procedural Programming would have to pass data pointers between function calls.
	The most common examples of Object Oriented Languages are C++, C#, Java, and Python, while Procedural languages include: C, Fortran, COBOL, and Pascal.
	
  Three of the main highlights of OOP are: Encapsulation, Inheritance, and Polymorphism.
### Encapsulation
  Encapsulation is the concept of combining data with the methods (functions) which use it, and “hiding” that data from unauthorized parties. Data, for example an array of characters (a string), in a class will be accessible only to methods in that class. Therefore, if an external use of the data is made, it is done through a method in the class containing the data, usually through ‘get’ and ‘set’ methods. A simple use case of this could be a class “Student” which represents one student in a school, and in this class (disambiguation: programming class. we will refer to the physical room where the students learn as “school”) there is a string labeled “Name” which holds the name of the student. We can now use the concept of encapsulation to prevent a students name from being changed (as this should not generally happen) and guarantee that the class returns a properly capitalized name when asked. This class could look like:

```C++
class Student {
	private:
		char * Name;

  public:
		char error_no_name[9] = "No Name!";
		char * getName() {
			if (Name == NULL) { return error_no_name; } // Make sure Name isn't empty
			Name[0] = toupper(Name[0]); // Force capitalize first letter of name
			return Name;
		}

   void setName(int student_name_length, char * student_name) {
			if (Name != NULL) { 
				printf("Student already has a name!\n");
				return; 
			}
			else if (student_name_length < 2) {
				printf("This cannot be a legal name, it is too short!\n");
				return;
			}
			else if (student_name == NULL) {
				printf("No name was given!\n");
				return;
			}
			else {
				memcpy(Name, student_name, student_name_length);
				printf("Student's name was changed to: '%s'.\n", Name);
			}
			return;
		}
};
```

  In this example we already see how encapsulation protects data, and places functions in an intuitive location regarding the data they use. In the case we see here, by using encapsulation, we reduce and simplify the code, as we have these protective ‘if’ statements only once, rather than making sure we are not accidentally ruining the student’s name each time we use it.

### Inheritance
  Inheritance is the mechanism through which one class can be based on, and thus acquire the methods and data of, another class. Inherited classes will have the same attributes and functions as their parent (or base or super) class, while being able to build on those to include more. Inheritance can be used to create a hierarchy of classes, where there is a base class which includes common and standard functionality, and an inherited child class has some extra functionality. For example:

```C++
typedef struct shape {/* Data */};
typedef struct food {/* Data */ };
typedef struct place {/* Data */ };


class Animal {
	protected:
		int age;
		shape body;
		food diet;
		place habitat;

  public:
		void Walk() {};
		void Talk();
};
class Bird : public Animal {
	protected: 
		int wing_span;
	public:
		void Fly();
};
class Penguin : public Bird {
	public:
		void Swim();
		void Fly() = delete; // Penguins cannot fly
};
```

  We have here the hierarchy from a general animal to a penguin, where a penguin has all the attributes and information of a base animal, and also some of the features of a bird - without the flying. Using inheritance, we avoid writing definitions and implementations of standard functions like “Walk()” in each class of animal. Say we implemented a class for each of the ~400 dog breeds. It would be incredibly redundant to implement 400 walk functions, while if we inherited each ‘dog_breed’ class from a base ‘dog’ class, we would only implement this function - and many others - once.

### Polymorphism
  Polymorphism allows two interesting and useful features which can be classified as static and dynamic polymorphisms. Static polymorphism (or ad hoc polymorphism) allows multiple functions two have the same name, and differentiates between them by their formal parameters - function overloading. This allows us to define similar functions which use different types of parameters with the same name:

```C++
double Sum(int num1, int num2) {
	return num1 + num2;
}
double Sum(double num1, double num2) {
	return num1 + num2;
}
double Sum(float num1, float num2) {
	return num1 + num2;
}
double Sum(int length_of_array, double * num_array) {
	double sum = 0;
	for (int i = 0; i < length_of_array; i++) {
		sum += num_array[i];
	}
	return sum;
}
```

  Now when we need to use some implementation of the ‘Sum’ function, we do not need to worry about which one to choose, as the compiler will select the proper one for each call according to that call's parameters.
	The second type, dynamic polymorphism, allows us to refer to objects of different derived classes the same way, given the derive from a common function. We can use this feature to iterate over an array of animals (see Encapsulation) and call their common “Walk()” function.

```C++
Animal * animals[4];

Animal * generic_animal = new Animal();
Bird * bird = new Bird();
Penguin * penguin = new Penguin();
Dog * dog = new Dog(); // assume we implement a class Dog similarly to Penguin.

animals[0] = generic_animal;
animals[1] = bird;
animals[2] = penguin;
animals[3] = dog;

for (int i = 0; i < 4; i++) {
	animals[i]->Walk();
}
```



  Here we see that, although each of the four objects in the array are of a different type, we can store them alongside each other, and call their common functions in such a way.
	This is granted by how inheritance is implemented in memory, which is that the base class’ attributes and methods are at the beginning and are then followed by the child class’ information.

![image22](https://user-images.githubusercontent.com/36902556/113138464-7a1ffe00-922e-11eb-9bc8-61c9a7023f8f.png)


## Evolvement - Practical Research
One of the most significant challenges which rose during this project, was that the platform I was using to run and execute the machine learning algorithms is a standard home computer - with the processing power of one. This means that each iteration over, for example, a euclidean distance calculation function took a massive amount of time, on the order of 1,500 milliseconds. This function being called 4,000 times for each of the 1,200 data points in the prototype-phase dataset, meant training the neural network would take 2,000 hours.
	To increase efficiency to a more feasible training time, I tried the following:
### Calling Conventions
Calling conventions are the standard methods by which functions are called - each convention defines how arguments are passed into the function and how where the output is placed. These conventions are all implemented in low-level languages, in our case we will use Assembly 8086 (x86) - although the actual project is in 64bit assembly.
The standard calling convention is ‘cdecl’ (C declaration), which passes arguments to the function through the stack. This means that code in C like: 
```C++
int func(int a,int b,int c,int d, int e, int f) {
    int out = a + b + c + d + e + f;
    return out;
}
 
int main(int argc, char ** argv) {
    int output = func(0,1,2,3,4,5);
}
```

Will look like: 
```ASM
_func   PROC
        push    ebp
        mov     ebp, esp
        push    ecx
        mov     eax, DWORD PTR [ebp + 8]
        add     eax, DWORD PTR [ebp + 12]
        add     eax, DWORD PTR [ebp + 16]
        add     eax, DWORD PTR [ebp + 20]
        add     eax, DWORD PTR [ebp + 24]
        add     eax, DWORD PTR [ebp + 28]
        mov     DWORD PTR [ebp - 4], eax
        mov     eax, DWORD PTR [ebp - 4]
        mov     esp, ebp
        pop     ebp
        ret     0
_func   ENDP
 
_main   PROC
        push    ebp
        mov     ebp, esp
        push    ecx
        push    5
        push    4
        push    3
        push    2
        push    1
        push    0
        call    _func
        add     esp, 24                             ; 00000018H
        mov     DWORD PTR [ebp - 4], eax
        xor     eax, eax
        mov     esp, ebp
        pop     ebp
        ret     0
_main   ENDP
```

While this is a good convention, and for general use is sufficient, in our case we need to optimize the code further, and remove as many redundant instructions as possible. The first thing to do, is change the calling convention to ‘fastcall’ which will pass the arguments through registers instead of the stack. Luckily, we only actually need two arguments, so the function could look like this:
```ASM
func:
        push    rbp
        mov     rbp, rsp
        mov     edx, edi
        mov     eax, esi
        add     eax, edx
        mov     DWORD PTR [rbp-4], eax
        mov     eax, DWORD PTR [rbp-4]
        pop     rbp
        ret
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        mov     DWORD PTR [rbp-20], edi
        mov     QWORD PTR [rbp-32], rsi
        mov     esi, 1
        mov     edi, 0
        call    func
        mov     DWORD PTR [rbp-4], eax
        mov     eax, 0
        leave
        ret
```

Notice how we reduced the amount of instructions in func which will be expressed as a significant speed increase when we repeatedly call the function.
Parameter Types
In C/C++ parameters can be passed to function either by-value, meaning you pass the literal bits of the value to the function, or by-reference, meaning you pass an address pointer to the value in memory.
By-Value is clearly faster, as we avoid having to dereference the address when using the value. However, there is a caveat to by-value, and an exception:
The caveat: An “object” passed by-value cannot be changed. Any changes done to the object are temporary within the scope of the called function, as the function actually received a bitwise copy of the object, and this copy is in a different memory location than the original.
The exception: “Objects” or structures cannot be properly passed by-value. While some compilers (not MSVC, which was used to compile most of this project) allow passing a structure by-value, this is quite odd and wildly inefficient.
By-Reference, while being slower, solves the caveat of being unable to change the object outside the scope of the function.
Notice the redundancy, however, with passing a standard object - like an int32_t or char - by-reference in order to change it:
```C++
int square(int num) {
    return num * num; // Multiplies num 
}
void square(int * num) {
    *num = (*num) * (*num); // Changes num at its original location as allocated in main.
}
 
int main() {
    int number = 5;
    number = square(number);
 
    int number2 = 7;
    square(&number);
}
```
```ASM
square(int):
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-4], edi
        mov     eax, DWORD PTR [rbp-4]
        imul    eax, eax
        pop     rbp
        ret
square(int*):
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR [rbp-8], rdi
        mov     rax, QWORD PTR [rbp-8]
        mov     edx, DWORD PTR [rax]
        mov     rax, QWORD PTR [rbp-8]
        mov     eax, DWORD PTR [rax]
        imul    edx, eax
        mov     rax, QWORD PTR [rbp-8]
        mov     DWORD PTR [rax], edx
        nop
        pop     rbp
        ret
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     DWORD PTR [rbp-8], 5
        mov     eax, DWORD PTR [rbp-8]
        mov     edi, eax
        call    square(int)
        mov     DWORD PTR [rbp-8], eax
        mov     DWORD PTR [rbp-4], 7
        lea     rax, [rbp-8]
        mov     rdi, rax
        call    square(int*)
        mov     eax, 0
        leave
        ret
```

A very simple 7 line function mutates into a complex 13 line function. This is an easily avoidable redundancy.
		Therefore, for performance critical functions, we will pass only simple types by-value.
### Arrays vs Vectors
However, even with all these optimizations, most functions, specifically uint32_t knn::calculate_distance(data * query_point, data * input), were still severely hindering the process.
The original version of knn:calculate_distance(data*, data*) looked a little like this: 
```C++
double knn::calculate_distance(data * query_point, data * input) {
    double distance = 0.0;
    if (query_point->get_feature_vector_size() != input->get_feature_vector_size()) {
        printf("Vector size mismatch");
        exit(1);
    }
 
    for (uint16_t i = 0; i < query_point->get_feature_vector_size(); i++) {
        distance += pow(query_point->get_feature_vector()->at(i) - input->get_feature_vector()->at(i),2);
    }
    distance = sqrt(distance);
    return distance;
}
```

The first optimization I made, was realizing that the feature vectors were of type std::vector<uint8_t>, meaning that the difference between two values on two vectors was also of type uint8_t (assuming we take the absolute value, which we do). This automatically means we are working with 8 bit integers instead of 64 bit doubles. Using these 8 bit integers, we gain the capability to use 8 bit operations, those using AL, AH, BL, …, DH, which are significantly faster than 64 bit operations. (Note: during the research, I initially tried using floating points (float) instead of doubles, in the hopes that using Floating-Point arithmetic - which can be done on the Floating Point Unit (FPU) - would be faster. Though this was successful and did in-fact increase performance, ultimately it was inferior to simply using unsigned byte-sized integers). One of my reasons for using 8 bit integers was the idea that twice as many could fit in the computer’s registers (see below) and thus reduce time spent fetching values from RAM (Random Access Memory) or the Stack. 


![image9](https://user-images.githubusercontent.com/36902556/113139151-67f28f80-922f-11eb-973a-4ad428481fe1.png)


Now to the crux of the issue; Vectors. Although vectors massively simplify dynamic size list usage, they are exponentially slower than normal c-style arrays. The main problem in our case with vectors is the ‘at(int)’ function, which through trial-and-error, I found to be the bottleneck of our distance function. To solve this, I added another step in the initialization of the entire algorithm: after setting everything up, copy each and every vector in every single object found into a corresponding array inside that object. Obviously, this additional step nearly doubles the initialization time. However, we only need to do this step once, and by doing so we reduce the processing time of each data point by a factor of 10. The actual implementation of this was quite simple, as I merely duplicated all the performance critical functions and wrote a “fast” version which used arrays instead of vectors. The final distance function looks like this: 
uint32_t knn::calculate_distance_fast(data * query_point, data * input) {
    uint32_t distance = 0;
 
    uint8_t * arr1 = query_point->get_feature_array();
    uint8_t * arr2 = input->get_feature_array();
 
    uint16_t size = query_point->get_feature_vector_size();
 
    for (uint16_t i = 0; i < size; i++) {
        distance += pow(arr1[i] - arr2[i], 2);
    }
    //distance = sqrt(distance); //Not necessary for comparisons
 
    return distance;
}

We use integer types of the smallest possible sizes; unsigned 32 bit for the total distance - as it can be quite large, unsigned 8 bit for the inputs - as they are simply those (see our FFT class), and unsigned 16 bit for the size of the arrays (which is obviously the same as the vector’s size) - as this is around 4,000 in our case.
We moved the array length validation out of this function, as to not check each and every time we call the distance function, rather only before beginning the distance comparisons.
We do not perform the square root calculation on the total distance. We use euclidean distance, which, informally, looks like this:
![image13](https://user-images.githubusercontent.com/36902556/113139180-72148e00-922f-11eb-8115-2d62a3fcf3d8.png)
And formally:

Assume points a and b such that their coordinates in an N dimensions space are {ai | i ∈ ℝ ∧ 1 ≤ i ≤ N} = {a1,a2,...,an} and {b1,b2,...,bn}.
The euclidean distance between points a and b is defined as: ![image16](https://user-images.githubusercontent.com/36902556/113139218-8193d700-922f-11eb-9c00-1b56ccaafcf1.png)
However, if all we care about is the relation between distances, we do not need to square root the sum since:
∀a,b ∈ ℝ+. a > b ⇐⇒ ![image17](https://user-images.githubusercontent.com/36902556/113139268-8f495c80-922f-11eb-87a0-7778e2a4faad.png) > ![image24](https://user-images.githubusercontent.com/36902556/113139280-93757a00-922f-11eb-816e-0544814eb03b.png)



We do not need to ask the data points’ class for the array or vector for each value, as we store our own pointers with ‘arr1’ and ‘arr2’. This trivial fix reduces the extra calls to the ‘get_feature_array()’ function, which while being very compact, still is redundant.
All these improvements reduced the processing time of the algorithm from a couple thousand hours, to a few minutes. Specifically the bottleneck of knn::find_knearest(data*), which loops over the calculate_distance function many times, improved from around 1,850 milliseconds per loop, to under 330 milliseconds => a 82.16% increase in speed.
