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
};```

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
};```

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
}```

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
}```



  Here we see that, although each of the four objects in the array are of a different type, we can store them alongside each other, and call their common functions in such a way.
	This is granted by how inheritance is implemented in memory, which is that the base class’ attributes and methods are at the beginning and are then followed by the child class’ information.

![image22](https://user-images.githubusercontent.com/36902556/113138464-7a1ffe00-922e-11eb-9bc8-61c9a7023f8f.png)
