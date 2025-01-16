
import std.stdio;
import std.array;
// Import the dependent package
import bindbc.opengl;

/*
 Import the sharedlib module for error handling. Assigning an alias ensures the function names do not conflict with
 other public APIs. This isn't strictly necessary, but the API names are common enough that they could appear in other
 packages.
*/
import loader = bindbc.loader.sharedlib;

// Create the OpenGL context before calling this function.
bool LoadOpenGLLib(){
    writeln("-".replicate(45));
    writeln("OpenGL Setup  ".replicate(3));
    writeln("-".replicate(45));
	/*
	Compare the return value of loadGL with the global `glSupport` constant to determine if the version of GLFW
	configured at compile time is the version that was loaded.
	*/
	auto ret = loadOpenGL();

	if(ret != glSupport){
		// Log the error info
		foreach(info; loader.errors){
			/*
			A hypothetical logging function. Note that `info.error` and `info.message` are `const(char)*`, not
			`string`.
			*/
			writeln(info.error, info.message);
		}
		
		// Optionally construct a user-friendly error message for the user
		string msg;
		if(ret == GLSupport.noLibrary){
			msg = "This application requires OpenGL.";
		}else if(ret == GLSupport.badLibrary){
			msg = "The version of OpenGL on your system is too low. Please upgrade.";
		}else{
			// GLSupport.noContext
			msg = "This program has encountered a graphics configuration error. Please report it to the developers.";
		}
		// A hypothetical message box function
		writeln(msg);
		return false;
	}

    // Provided some output
	writeln("OpenGL Supported Version:",ret);

	return true;
}

/// Return the OpenGL veresion that was loaded
/// Note: You'll need to setup an OpenGL context with for example SDL prior
///       to checking the version number
void GetOpenGLVersionInfo(){
    import std.conv;
    import std.array;
    writeln("-".replicate(45));
    writeln("Vendor:            ", glGetString(GL_VENDOR).to!string);
    writeln("Renderer:          ", glGetString(GL_RENDERER).to!string);
    writeln("Version:           ", glGetString(GL_VERSION).to!string);
    writeln("Shading language:  ", glGetString(GL_SHADING_LANGUAGE_VERSION).to!string);
    writeln("-".replicate(45));
}
