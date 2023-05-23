## What is Glad?

Glad is a library for loading all of the OpenGL functions and extensions. That is, OpenGL is a graphics library (Open Graphics Library). Since it is a library, that means our programs that we create need to be able to load the functions from the OpenGL library that exists on our system.


If you look at glad.cpp you'll essentially see that this is done by 'loading' (search or grep the phrase 'load')  each individual OpenGL function. We obtain the function through it's address (using a pointer to a function (i.e., function pointer)). The glad library then provides the library name for each of these functions so that we can call them.

The key function is 'gladLoadGL' in order to create all of our function pointers. Scrolling through glad.cpp is somewhat interesting to see how this is done.

## What is the KHR platform:

https://stackoverflow.com/questions/49683210/what-is-the-khr-platform-header-doing/49700016


