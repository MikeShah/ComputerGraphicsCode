OpenGL 4.5/4.6 is unfortunately not as documented in terms of 'tutorials' as I would like. So this document and repository is here to fix some of that, and otherwise provide a guide.

# What is Modern OpenGL?

Modern OpenGL is any version from 3.3/4.0 or later. 

# Why OpenGL 4.5/4.6?

OpenGL has been around since at least 1992. Depending on when you read this, that means OpenGL has been around for nearly 35 years! It's a testament to the importance of needing to make good APIs that can withstand change, but even for all that planning, we still may run into issues. Since OpenGL's inception, the GPU and the programmability of the GPU has changed significantly to give us more power. Even a 'semi-modern' OpenGL through version 4.1 suffers from having lots of global state that both: 1.) slows down the driver 2.) Makes it harder to write correct OpenGL code.

> Read more about the history of OpenGL here: https://wikis.khronos.org/opengl/History_of_OpenGL

So, my goal here is to help you learn the actual *modern* OpenGL that starts around OpenGL 4.5 (2014) and continues to the latest release of OpenGL 4.6 (2017). OpenGL continues to get extensions as well (even including things like [Mesh Shaders](https://www.supergoodcode.com/mesh-shaders-in-the-current-year/) which modern graphics APIs have)

## OpenGL 4.6 Resources

- https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions
  - This is probably the **key resources** if you already have some experience with OpenGL.
- https://juandiegomontoya.github.io/modern_opengl.html
  - Nice blog on some of the Modern OpenGL 4.6 features and how to apply them. One important thing is the idea of mimicking the newer APIs 'pipeline' passes (i.e. bundling the state and geometry needed to render in one object). There's also some nice advice on tying state to each glDraw* command, and then making sure to use something like RAII to ensure there are no data leaks.
  - The author also implemented this functionality in their engine here: https://github.com/JuanDiegoMontoya/Fwog

## Compute Shaders

Since OpenGL 4.3 compute shaders have been available. A compute shader operates seperate of the 'graphics pipeline', and allows us to perform more general purpose 'compute operations' on data (Often coined as GPGPU programming). OpenGL's glsl shading language with compute shaders, allows us to work with things like 'image buffers', 'shader storage buffers', etc. to add to our pipeline.

- Some resources on learning about compute shaders
  - [Compute Shaders || Lou Kramer || AMD](https://www.youtube.com/watch?v=eDLilzy2mq0)
