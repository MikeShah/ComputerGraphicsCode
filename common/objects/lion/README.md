# How to use the lion 3D model

This model was originally downloaded from: https://sketchfab.com/3d-models/lion-statue-optimized-62662d27c94d4994b2479b8de3a66ca7

I modified it in blender3D to center it and export it to OpenGL coordinate system default.

The lion_2_percent_of_triangels model is decimated to ~2% of the original mesh. (From 28,488 triangles to 568)

If you want to use the .ppm images for this class, note that the ppm format is the P6 binary format (i.e. you'll need to update your PPM loader to read in P6 format).

You'll also need to update the .mtl file to take in a .ppm instead of a .png file.
