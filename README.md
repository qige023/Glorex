Glorex
===================================================

A light-weight cross-platform 3D framework and oriented tools collection for OpenGL ES 3.0 on mobile. Glorex dedicated to build real 3D scene on mobile. 

This repository contains Glorex source and the sample code for 10+ 3D demos using Glorex and native C/C++.

*Currently this project's core code is under active development, not guarantee build successfully on master. But you still can download a previously builded well app from [here](https://github.com/qige023/Glorex/releases/tag/v0.0.1).* (Notice: support Android 4.3+ Only and please read Run Demos Section)

## Features ##
* Focus on mobile(Android/IOS) OpenGL ES3.0+ only
* Use recommended GLSL for rendering, no more fixed pipeline on OpenGL ES1.0
* Text Rendering support (muti-language using UTF-8) 
* Model import support
* Multi-touch control support
* Physics/ Collision Detection/ Animation /Scene Management (coming soon)
* Nice framework and examples based on a couple of projects (see below)
* Plenty demos (currently 10+ 3D demos) in a single demo app (see below)
* Use Gradle for Building On Android, easy to build

## Technology Stack ##
* Native C/C++
* Opengl ES 3.0
* [GLM](http://glm.g-truc.net/0.9.6/index.html) for graphic math
* [assimp](https://github.com/assimp/assimp) for asset import
* [stb_image](https://github.com/nothings/stb) for image import
* [freetype](http://www.freetype.org/) for TTF import
* [bullet](https://github.com/bulletphysics/bullet3) 3D Real-Time Multiphysics Library (coming soon)

## Demos ##

*below are demo pics from the branch-master release tag v0.0.1*

 ![nanosuit](https://raw.githubusercontent.com/qige023/Glorex/master/docs/demopics/nanosuit.jpeg)
 
Nanosuit [Video On Youku.com](http://v.youku.com/v_show/id_XOTUwNTc1OTc2.html)


 ![blendingwindow](https://raw.githubusercontent.com/qige023/Glorex/master/docs/demopics/blendingwindow.jpeg)
 
Blending Window [Video On Youku.com](http://v.youku.com/v_show/id_XOTUwNTgxMzg4.html)
 
 
 ![cubemap](https://raw.githubusercontent.com/qige023/Glorex/master/docs/demopics/cubemap.jpeg)
 
 Cubemap [Video On Youku.com](http://v.youku.com/v_show/id_XOTUwNTc4MjM2.html)
 
 
![floatingflag](https://raw.githubusercontent.com/qige023/Glorex/master/docs/demopics/floatingflag.jpeg)

Floating Flag [Video On Youku.com](http://v.youku.com/v_show/id_XOTUwNTc1MTY4.html)


![textrender](https://raw.githubusercontent.com/qige023/Glorex/master/docs/demopics/textrender.jpeg)

Text Rending Multi-languare with "幼圆.ttf" fonttype 


![simplevertexshader](https://raw.githubusercontent.com/qige023/Glorex/master/docs/demopics/simplevertexshader.jpeg)

Simple Vertexshader 


![simpletexture](https://raw.githubusercontent.com/qige023/Glorex/master/docs/demopics/simpletexture.jpeg)

Simple Texture 


![instancing](https://raw.githubusercontent.com/qige023/Glorex/master/docs/demopics/instancing.jpeg)

Instancing. Muti mesh using one vbo


![torusdiffuse](https://raw.githubusercontent.com/qige023/Glorex/master/docs/demopics/torusdiffuse.jpeg)

Torus ambient diffuse Light Model


![teacoptwoside](https://raw.githubusercontent.com/qige023/Glorex/master/docs/demopics/teacoptwoside.jpeg)

Teacop Two side Phong Light Model


to be add..

## Code Samples ##
Shader/Program Operating
```  
  ESProgram prog;
  // compile/link/use vertex shader and fragement shaders
  prog.compileShader("shader/nanosuit.vert");
  prog.compileShader("shader/nanosuit.frag");
  prog.link();
  prog.validate();
  prog.use();
  // set matrix in a uniform for shaders' use
  model = mat4(1.0f);
  prog.setUniform("model", model);
```

Texture loading
```
    //load textures
    cubeTexture = STBLoader::loadTex("media/texture/marble.jpg");
    floorTexture = STBLoader::loadTex("media/texture/metal.png");
    //load textures with alpha channel
    windowTexture = STBLoader::loadTex("media/texture/blending_transparent_window.png", STBLoader::CHANNEL_RGBA);
    //load cubemaps
    vector<const GLchar*> skyboxFaces;
    skyboxFaces.push_back("media/skybox/valley/right.jpg");
    skyboxFaces.push_back("media/skybox/valley/left.jpg");
    skyboxFaces.push_back("media/skybox/valley/top.jpg");
    skyboxFaces.push_back("media/skybox/valley/bottom.jpg");
    skyboxFaces.push_back("media/skybox/valley/back.jpg");
    skyboxFaces.push_back("media/skybox/valley/front.jpg");
    skyboxTexture = STBLoader::loadCubemap(skyboxFaces);
```

Model loading
```
    //Import a obj file
    nanosuit = new ESModel(esContext, "sdcard/Glorex/model/nanosuit/nanosuit.obj");
    //render it
    nanosuit->draw(prog);
```

Text rendering.
```
  charactersHolder = new ESCharactersHolder();
  // load font
  charactersHolder->loadFont("font/youyuan.ttf");
  // render text in specify position and scale
  charactersHolder->displayText("(C) Glorex 3D Framework", 300.0f, 570.0f, 1.5f);
  charactersHolder->displayText("格里斯三维影像渲染框架", 100.0f, 250.0f, 2.0f);
```

MultiTouch Support
```
    controlLayer = new ControlLayer();
    controlLayer->initLayer(esContext);
    // find out which direction we press and process the camera's position
    float angle, factor, degree;
    ESboolean isLeftPanelActive = controlLayer->getPanelState(ControlLayer::LEFT_PANEL, angle, factor);
    if(isLeftPanelActive == TRUE) {
        degree = ES_TO_DEGREES(angle);
        if(degree > 45.0f && degree <= 135.0f) {
            camera->moveByDirection(ESCamera::FORWARD, deltaTime);
        } else if(degree > 135.0f || degree <= -135.0f) {
            camera->moveByDirection(ESCamera::RIGHT, deltaTime);
        } else if(degree > -135.0f && degree <= -45.0f) {
            camera->moveByDirection(ESCamera::BACKWARD, deltaTime);
        } else if(degree > -45.0f && degree <= 45.0f) {
            camera->moveByDirection(ESCamera::LEFT, deltaTime);
        }
    }
    ESboolean isRightPanelActive = controlLayer->getPanelState(ControlLayer::RIGHT_PANEL, angle, factor);
    if(isRightPanelActive == TRUE) {
        degree = ES_TO_DEGREES(angle);
        if(degree > 45.0f && degree <= 135.0f) {
            camera->rotate(0, factor);
        } else if(degree > 135.0f || degree <= -135.0f) {
            camera->rotate(factor, 0);
        } else if(degree > -135.0f && degree <= -45.0f) {
            camera->rotate(0, -factor);
        } else if(degree > -45.0f && degree <= 45.0f) {
            camera->rotate(-factor, 0);
        }
    }
    //render control layer
    controlLayer->render(esContext);
```

to be add..

## Support Platforms ##
This project currently target on Android only, IOS support will be in consider later

builds and test on the following platforms:

* Android 4.3+ NDK (C/C++) & SDK (Java)

API Level 18+ on Android 4.3+ (It's the minimal version of Android that support GLES 3.0 :D )

## Builds ##
Need Android sdk & ndk installed

Need Android Studio or Eclipse(with CDT and Gradle plugins) 

libfreetype.a and libassimp.so is needed for build for some demos. Here I provide my own complied version, you can checkout in app/libs folders (tested on Android 4.3). But you can find some useful link and hints about cross-compile skills from docs/reference.md.

Import gradle project, build and enjoy...

to be add..

## Run Demos ##
For Nanosuit Demo, you must place project's sdcard/model folder to your phone's sdcard/Glorex/model .

## Special Thanks ##
*Some parts of framework code inspired by:*

* [OpenGL Shader Language Cookbook](https://github.com/daw42/glslcookbook)
* [OpenGL ES 3.0 Programming Guide](http://www.opengles-book.com)
* [Learn Opengl](http://www.learnopengl.com/)

*Some examples and image assets borrow from:*

* [OpenGL Shader Language Cookbook](https://github.com/daw42/glslcookbook)
* [Learn Opengl](http://www.learnopengl.com/)
* [Nehe OpenGL's Lesson](http://nehe.gamedev.net/)
* [OGLdev](http://ogldev.atspace.co.uk/)

*Please contact me if some demo assets need more license granted or has free use limitation*

## License ##
The MIT License

## Authors ##
vicky yuqi.fan@foxmail.com

