# Shadow - Reflection - Refraction
This is a project created by Alejandro Sans (sansalejandro@gmail.com) and Víctor Felipe for a Computer Graphic course (Universidad Central de Venezuela). 

This project shows the use of reflection, refraction and shadows in a scene using Glut.

# Compiling

This project requires compiling with visual studio 2015 and the following external libraries:

* [FreeGLUT] 3.0.0-2
* [AntTweakBar] 1.16
* [FreeImage] 3.17.0
* [GLEW] 2.0.0

To compile in visual studio, the header files must be on a "tools" folder that must be created in the forlder "Project". The headers should be included as follow:

    - Project/tools/GL/ (FreeGlut and GLEW)
    - Project/tools/AntTweakBar/ (AntTweakBar)
    - Project/tools/FreeImage (FreeImage)

The libraries must be on a folder "lib" that must be created in the root as follow:
    - lib

And the dll should be include on the root folder,

# Images

Here are some image of the application:

this image show the use of volume and planar shadows

![alt tag] (./Screenshot/Shadow.jpg)

this image show the use of reflection (all floor has the reflection active)

![alt tag] (./Screenshot/Reflection.jpg)

thios image show the use of refraction (only refract skybox in the ball in the air)

![alt tag] (./Screenshot/RefractionSkybox.jpg)



   [video]: <https://vimeo.com/37664294>
   [SDL]: <https://www.libsdl.org/>
   [SDL_mixer]: <https://www.libsdl.org/projects/SDL_mixer/>
   [FreeGLUT]: <http://freeglut.sourceforge.net/>
   [FreeImage]: <http://freeimage.sourceforge.net/>
   [AntTweakBar]: <http://anttweakbar.sourceforge.net/doc/>
   [glew]: <http://glew.sourceforge.net/>
