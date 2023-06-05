// Support Code written by Michael D. Shah
// Last Updated: 6/15/21
// Please do not redistribute without asking permission.

// Functionality that we created
#include "SDLGraphicsProgram.hpp"


// The main application loop
void loop(){
}

// Code that should execute prior to the loop
void preloop(){

}

// The setup

int main(int argc, char** argv){

	// Create an instance of an object for a SDLGraphicsProgram
	SDLGraphicsProgram mySDLGraphicsProgram(1280,720);
	// Run our program forever
	mySDLGraphicsProgram.SetLoopCallback(loop);
	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	return 0;
}
