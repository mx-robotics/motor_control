### Memory Layout - Where does your code reside ?

This is write-up summarizing the highly confused topic of where does the code, variables (automatic, static, global, initialized, uninitialized) gets stored. It should be noted that this layout is a bit different for PC's and embedded devices. Especially for embedded devices, it changes from architecture to architecture and also vendor to vendor. This guide tries to shed light on ARM-Cortex architecture.    

####  General Layout

The memory is organized in 5 sections: .heap, .stack, .bss, .text and .data

##### .text

This is where you actual code resides. On micro-controllers, this is almost always located in Flash and not RAM. 

##### .bss and data

Any global variable as well as anything denoted with static keyword finds itself here. .bss stores uninitialized variables (aslo zero initialized variables) and data holds the rest. They are usually adjacent in the memory. This section usually gets copied to RAM before the start of execution. This means they will both take space in RAM and in Flash.

* .rodata: Read-only data. All consts go in this section. The caveat is: On embedded devices, read-only things(consts, constexper) usually get stored on Flash rather then RAM since it they wont change and it might be a good idea to save RAM at the cost of access speed. The Flash access is way slower than RAM access which makes .rodata always the slowest to use.
* Cost of statics: If you are using C++ rather than C, statics have to be thread-safe. This brings on the overhead of checking if it is initialized (more on that later) or not, which means branching . This is negligible on PC's since a simple branch predictor will take the right branch after the first 2 tries but on MCU's, a branch prediction is usually hard to find. (Important: this is only relevant for **local** statics and not member static variables.)
* Cost of globals: Globals are at the same address space with statics (copied to RAM) but dont have the branching overhead of statics. They are still slower than auto variables (stack).

##### heap and stack

Heap depicts dynamically allocated memory and usually grows in the opposite direction of stack. Stack holds any variable that is local (even if its const but why would you want a local const ?). Function parameters are also held in stack.

*  Benefits of Stack: Stack is usually the fastest since every memory access is done with a simple stack pointer increment and the cost of accessing a .data/.bss variable (address calculation) doesn't incur. Variables on stack can also be placed in CPU registers which further speeds things up.



```none
Segment     Memory   Contents

.data       RAM      Explicitly initialized variables with static storage duration
.bss        RAM      Zero-initialized variables with static storage duration
.stack      RAM      Local variables and function call parameters
.heap       RAM      Dynamically allocated variables (usually not used in embedded systems)
.rodata     ROM      const variables with static storage duration. String literals.
.text       ROM      The program. Integer constants. Initializer lists.
```

* [arm-gcc-toolchains for pio, teensy](https://bintray.com/platformio/dl-packages/toolchain-gccarmnoneeabi) 

### Actual code placement on MCU's (ARM)

The memory layout explained above usually differs in PC's and MCU's in the following way: On PC's, you write your code to a file and this file gets compiled, linked and the executable is produced. This executable and everything that comes with it gets placed in to ram, for faster execution. 

On the other hand, MCU's usually execute the code from FLASH (non volatile, slower memory) and also don't bother copying read-only .data to RAM. **Variables** placed on .data or bss. gets copied into RAM.

Some MCU's implement some sort of code cache, that stores some part of the program on RAM (but not all of it) to allow faster execution. 

##### Change the default layout : Linker scripts and scatter files

The default layout above can be changed via editing/writing a linker script. After the compilation, the Linker determines where the aforementioned memory sections will be copied in the target device. The linker uses a scatter file to know where in the target certain memory areas exist.  The scatter file will inform the linker of these areas and direct it to place certain sections of the output in certain areas. So by giving different addresses in the scatter file, it is possible to change what ends up in FLASH and in RAM.

Related links:

* [SE: Purpose of arm Linker files](https://electronics.stackexchange.com/questions/108515/purpose-of-arm-linker-files)
* [Actual example of FLASH vs RAM Code Placement](https://www.ele.uva.es/~jesus/hardware_empotrado/Compiler.pdf)
* [Actual ARM scatter file example](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.100748_0607_00_en/pge1362066000009.html)

### Specific Example: Teensy

Teensy 3.2 by default follows the default memory layout. Teensy 4.0 on the other hand, due to its gigantic RAM, places the code on RAM and executes it from there. It is still not very clear if .rodata gets placed on RAM as well.

##### FASTRUN

Teensy offers a feature thats called FASTRUN (just put it before the function declaration) that **copies** the labeled function into RAM alongside FLASH (where .text is stored). This is done via the scatter file example given before. 



Functions calls by default are jumps to other memory locations. The concept of **inline** tries to overcome the function call overhead by copying the function body to the call location. This is related to FASTRUN since FASTRUN only puts the jump adress for the labeled function to RAM. Any other/further call  from this function will be executed from .text if not specified otherwise.



### Related Links

##### StackExchange

* https://electronics.stackexchange.com/questions/237740/what-resides-in-the-different-memory-types-of-a-microcontroller
* https://stackoverflow.com/questions/3180619/basic-question-regarding-rom-based-executable
* https://electronics.stackexchange.com/questions/102764/flash-and-ram-code-execution
* https://stackoverflow.com/questions/93039/where-are-static-variables-stored-in-c-and-c
* 

##### Teensy

* https://www.pjrc.com/teensy/gcc.html

* [Teensy 4.0 - Very good info that relates to 3.2 as well](https://www.pjrc.com/store/teensy40_pins.html)
* https://www.digikey.com/en/articles/memory-considerations-for-faster-mcus
* [AttachInterruptVector - Very good](https://forum.pjrc.com/threads/30017-Are-interrupts-slow)
* [Flash Cache and FASTRUN](https://forum.pjrc.com/threads/43811-Speed-test-and-optimizations-some-surprises!)
* Dogusun bug i https://www.sciencedirect.com/topics/engineering/memory-layout
* FASTRUN in nasil oldugu __attribute seysi -> http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.100748_0607_00_en/pge1362066000009.html 
* Short summary:

  * Both constexper and const seems to put the data in to .text segment. The advantage of constexpr should be inquired further. Helps with the compiler optimization maybe.
  * Declaring something not const or constexpr seems to have the effect that its in .data which is ram
  * M4 executes the code from flash by default but puts it to ram chunk by chunk -> Flash Cache
  * Check the cache size of teensy 3.2
  * Everything is put in flash by default, but some of them gets copied to Ram at the very start. So in any case, everything is in Flash although it gets copied to RAM because you want it to be copied again if the power is off.
  * FASTRUN basically (check the concept of scatter files) instructs the linker to copy a function to the RAM - mind you, copies so it is also in flash - so it can be executed faster.
  * Any local variable or anything on stack, is created when the function call happens. (check the concept of main stack) so CHECK everything static related in the light of this. 
  * Inline init not possible -> what does this mean, multiple inclusion problem or ?  yes, see -> https://stackoverflow.com/questions/22869788/why-should-i-not-initialize-static-variable-in-header
  * is static somewhat slower ? Yes

### Inline variables, static member initialization, ODR and multiple inclusion errors

#### ODR (One Definition Rule)

ODR states that everything can have one and only one **definition**. You can declare everything as may times as you want but define only once. This is due to the linking stage, where a declaration only makes the linker note that a definition will be provided along the way. A definition gets itself exported as a symbol to the symbol table which can not contain more than one definition of the same thing - or two identical symbols.

* Decleration Examples:
  * extern int a;
  * void foo(int a);
  * class Foo{ int bar(); };

* Definition Examples
  * int a;
  * int b= 10;
  * void foo(int a){/***/};
  * int Foo::bar(){/***/};

* Classes can only be declared **but** their functions and static members must be defined. They only exist once in a program's address space where as instance variables exist for every object of this class.

#### Multiple Inclusion Errors

Every .c or .cpp file gets compiled on its own with every header they own. Header contents are basically copied in the source file. This combination of one source file (.c or .cpp) and various headers are called a **translation unit**. After the compilation is done, all translation units are brought together by the Linker. 

When you try to initialize (essentially define a static member variable) inside the class or inside the header - it doesn't matter which - this is not allowed since it is seen as a possible violation of ODR:

1. All source files that include the header with the static member variable initialization basically copy the initialization line and after the compilation, every and each one of them try to export it as a symbol which the linker doesn't allow.

2. `Class A {`

   ​	`static int index; };`  is allowed since this is just a declaration and is allowed to into any translation unit. **But** the initialization `int A::index=12;` is only allowed in a source file (and can be done only once, it will throw a multiple definition error again if you do this in different source files).

3. const integral types can be initialized inside the header or class declaration. 

   `Class A {`

   ​	`static const int x = 12;`

   `};` is allowed. This is possible since consts like this can be interpreted as compile time constants and optimized by the compiler in such a way that they dont even make it the symbol table. Same is also true for constexpr variables. A caveat is, if the address is taken in any way, a definition must be made, compile time const interpretation stated above doesn't hold anymore.

4. C++17 introduces a new keyword for variables : **inline**. This basically tells the compiler to ignore ODR for this one variable and take only one of the definitions into account. 

   `Class A {`

   ​	`static inline int x = 12; };` is allowed. 

### Member static variables

The static key word describes 3 different concepts in c++ and it is a bit difficult to distinguish between them. Local statics are lazy-initialized which makes them non-thread-safe by default. Some measure of thread safety came with c++11, alongside the overhead of checking if the variable has been initialized before. 

Global statics are about linkage: They are translation-unit level private to block external linkage. Defining a global variable/function static basically tell the compiler not to export this as symbol.

Member static variables are basically globals with limitable scope: If they are declared public, they are pretty much like globals with a namespace. The namespace is the class in this case. If they are declared private, they are invisible by default to the outside word and are only visible to the other member functions.

The use of member statics is discouraged if they are **not** consts due to the following reason: They are a shared mutable global state and come with the common perils of this notion. If they are private, this state is shared and open to modification only by the member functions and if they are left public, to everyone. There are of course cases that one has to use statics.

Member statics, local statics and globals share the same address space but unlike local statics, member statics and globals are initialized before any thread starts.



### Memory concepts: Where do member functions and variable reside ?

Member functions are not bound to an object of the class. They reside in a specific location in memory, just like all other functions. Global or member static functions are just the same. They are an address that hold a piece of code to be executed. 

All objects shared the same set of non-static member functions. Since different objects can have different internal states, there is the question of "How does a function distinguish between different objects? ". In the background any member function has an implicit parameter: **this**. This depicts the address of the calling object, this way the internal state of this particular object is accessed.

Non-static member variables are created on the memory when the object is created. They are created with the order that they have been declared in. Their addresses are consecutive on the memory. If they all have the same size, they can be accessed with pointer arithmetics like an array. If their sizes dont match, there is the possibility of adding padding which can be adjusted to certain sizes, turned on and off. 

The advantage of padding is ease of access, resembling an homogeneous array. This might cause very subtle bugs if a code is written with assumption of padding since it depends on the compiler.

Static member functions are just like their non-static counterparts, with exception of not having an implicit **this** pointer. This is expected since they are non bound to an object. Static member functions can only access static member variables which are also not bound to an object hence no need of **this **pointer.

Static member variables are also created before any thread starts (no lazy initialization), they are by default thread safe (at least in gcc implementation). They have a distinct memory location (at .data or .bss) and access is basically like a global. There might a small overhand due increased level of indirection.

