import std.stdio;

void main(){

		const(float[]) vertexPositions = [
				-0.8f, -0.8f, 0.0f, // Left vertex position
				 0.8f, -0.8f, 0.0f, // right vertex position
				 0.0f,  0.8f, 0.0f, // Top vertex position
		];

		writeln("vert 1: ",vertexPositions[0 .. 3]);
		writeln("vert 2: ",vertexPositions[3 .. 6]);
		writeln("vert 3: ",vertexPositions[6 .. 9]);
}


