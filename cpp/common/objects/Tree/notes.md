Note that there are actually '3' models loaded in this single .obj file. 

So your parser would have to properly look for the 'o ' that indicates a new object is being started.

Alternatively, you could split the .obj file into three separate files, with three separate .mtl files as well.
