// @file:  applicationstage.d
import std.stdio;

void input(){
}

void update(){
}

void render(){
}

void main(){

    float vertexDataOnCPU = [-1.0f, 0.0f, 0.0f,
          1.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f];

		while(true){
				input();
				update();
				render();
		}
}



