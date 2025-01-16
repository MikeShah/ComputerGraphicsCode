// @file: code.d
// Compile & run: rdmd code.d
import std.stdio;
import std.format;

void main(){

	// Static-sized array
	float[9] vertexData =  [1.0f, 1.0f, 1.0f,
													0.0f, 0.0f, 0.0f,
													2.0f, 0.0f, 1.0f];
	// Dynamic array
	float[] vertexData2 =		[1.0f, 1.0f, 1.0f,
													0.0f, 0.0f, 0.0f,
													2.0f, 0.0f, 1.0f];
	foreach(f ; vertexData2){
		write(format!"%f "(f)); // Format string checked at
														// compile-time.
	}
	writeln;

	uint[string] map_example;
	map_example["test"] = 7U;
	writeln(map_example);
	
}
	
