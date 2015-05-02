GLOREX - LIGHT WEIGHT 3D Framework on OpenGL ES 3.0 
===============================

This repository contains GLOREX engine source and the sample code for about 10+ 3D demo using GLOREX native c/c++.

*Currently this project's core C++ engine is under active development, not guarantee build successfully on 
master. But you still can get a previously builded well app from [here](http://www.github.com)*

## Features ##
* Focus on mobile(Android/IOS) gles3.0+ only
* Use native C/C++ and modern OpenGL ES3.0 API
* Use recommended shaders for rendering, no more fixed pipeline on OpenGL ES1.0
* Multi-touch Support
* Use [GLM](http://glm.g-truc.net/0.9.6/index.html) for graphic math
* Use [assimp](https://github.com/assimp/assimp) for asset import
* Use [stb_image](https://github.com/nothings/stb) for image import
* Use [freetype](http://www.freetype.org/) for TTF import
* Nice framework and examples based on a couple of projects (see below)
* Build once, get all! Plenty examples just in a single demo app
* Use Gradle for Building On Android, easy to build

## Content ##
to be add..

## Sample ##

*below is two running demo from the branch "branch-c", witch is sample of simple vertex shader and basic light modal*

 ![colorcube](https://raw.githubusercontent.com/qige023/OpenGL-ES3-Programming-On-Android/master/docs/colorcube.gif)
 
 ![diffusetorse](https://raw.githubusercontent.com/qige023/OpenGL-ES3-Programming-On-Android/master/docs/diffusetorse.gif)

## Support Platforms ##
This project currently target on Android only, IOS support will be in consider later

builds and test on the following platforms:

* Android 4.3+ NDK (C/C++)
* Android 4.3+ SDK (Java)

API Level 18+ on Android 4.3+ (It's the minimal version of Android that support GLES 3.0 :D )

## Builds ##
need Android sdk & ndk installed

need Android Studio or Eclipse(with CDT and Gradle plugins) 

import gradle project, build and enjoy...

to be add..

## Special Thanks ##
*Some parts of framework code inspired by:*

* [OpenGL ES 3.0 Programming Guide](http://www.opengles-book.com)
* [OpenGL Shader Language Cookbook](https://github.com/daw42/glslcookbook)

*Some examples and image assets borrow from:*

* [OpenGL ES 3.0 Programming Guide](http://www.opengles-book.com)
* [Nehe OpenGL's Lesson](http://nehe.gamedev.net/)
* [OpenGL Shader Language Cookbook](https://github.com/daw42/glslcookbook)
* [Learn Opengl](http://www.learnopengl.com/)

*Please contact me know if some demo assets need more license granted or has free use limitation*

## License ##
The MIT License

## Authors ##
vicky yuqi.fan@foxmail.com

