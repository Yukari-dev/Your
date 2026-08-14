# KWALA.kwl
`Kwala` is a mini-compiler for `kwala` language written in C.

Designed to be very __simple__, *easy* to use.

This compiler was built as a challenge between me and my friend [Github Profile](https://github.com/ily4sss).

## USAGE:

#### HELLO WORLD
> "linus trivold: hello world."
<img width="341" height="205" alt="helloworld" src="https://github.com/user-attachments/assets/ec04673a-f901-4032-b21d-8b8b57eafa32" />

`reveal` is a function being called, a system level function to be exact. 
We will discuss functions later on.

#### Variables
<img width="425" height="298" alt="variables" src="https://github.com/user-attachments/assets/18ea4932-3516-4ea3-85c8-4e7d43eb7811" />

Declaring a variable with `var` followed by the `name`, continued by `>` which specify the variable type, in this case `x` is an int, meanwhile `y` is a string
> "NOTE: specifying a type is ___manditory___"

#### Comments
<img width="425" height="205" alt="comment" src="https://github.com/user-attachments/assets/6a1d7d48-fbdc-4285-8d95-b1683a0d349a" />

Comments in `Kwala` is rather simple, starting with `~` indicate a comment in this line, which will be ignored in the compiler.

#### When
<img width="425" height="354" alt="when" src="https://github.com/user-attachments/assets/0425c4ef-257a-4085-b851-abfaecb65dec" />

In `Kwala` it only have __one__ check statement, `when` used instead of `if` which is more popular and widely used, however we are not following that path, every keyword is unique.

#### Functions-1
<img width="425" height="316" alt="functions" src="https://github.com/user-attachments/assets/5d94fab8-d0f5-405d-89ce-86c0d7d5acd3" />

A function is declared first by `fn` keyword, which means `function`, after that you must specify the function name, after that YOU MUST specify the return type, (there is no void type for the moment) and then use `~>` and `<~` to let the compiler know the boundaries of the function.

#### Funtions-2
<img width="441" height="335" alt="FunctionParam" src="https://github.com/user-attachments/assets/54ccf440-0c61-423b-b086-dae2e0289309" />

Function parameters are simple as we can see here, declaring only the variable name and it type, multiple parameters are separated by `,`, the `give` keyword works same as `return` in C and other languages, it gives the value it contains to the caller, to call the function you write the desired function name, followed by `:`, then specify the arguments.
> "NOTE: for a 'void' function, you must at least give one argument when calling the function"

# LICENSE:
MIT - do whatever you want with it (with a star and a fork ofc ^W^).
