/// @file: abstractions/app.d
import graphics_app;

/// Program entry point 
/// NOTE: When debugging, this is '_Dmain'
void main(string[] args)
{
    GraphicsApp app = GraphicsApp(640,480);
    app.Loop();
}
