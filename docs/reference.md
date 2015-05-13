Reference
===============================
some useful link and steps

## FreeType Cross Compile ##

[Wiki](http://en.wikibooks.org/wiki/OpenGL_Programming/Installation/Android_NDK#FreeType)

* first build freetype in your machine
```
cd path-to-freetype
cp path-to-freetype path-to-freetype-local
./configure
make
```

* establish android standalone compile toolchain
```
$NDK/build/tools/make-standalone-toolchain.sh --platform=android-8 --install-dir=/tmp/my-android-toolchain --arch=arm

$NDK is your path to ndk root  directory
```

* configure make freetype.so
```
cd path-to-freetype
export PATH=/home/vicky/mytoolchain/arm-linux-androideabi/bin:$PATH
./configure --host=arm-linux-androideabi --prefix=/freetype --without-zlib --with-png=no
make -j4
```

* when see apinames file error (because the it try to exec the arm version apinames file, we just replace it to PC version apinamesfile)
```
copy path-to-freetype-local/objs/apinames to path-to-freetype/objs/apinames
cd path-to-freetype
make install DESTDIR=your-path-to-build-freetype
```

## Assimp Cross Compile ##

[使用ANDROID NDK编译ASSIMP](http://airtheva.net/wordpress/?p=62)

* check out [assimp](https://github.com/assimp/assimp)  and switch to branch *opengex_support*
```
git checkout opengex_support
```

* check out [android-cmake-toolchains](https://github.com/taka-no-me/android-cmake)

* install cmake-gui (cmake version should 2.8+)
```
sudo apt-get install cmake-gui
```

* export ANDROID_NDK to path
```
 export ANDROID_NDK=you-path-to-ANDROID-NDK
```

* configure cmake-gui
```
specify Source as Assimp source directory。
specify Build as working directory(better different from souce)。
press Configure,
choose Specify toolchain file for cross-compiling
specify your path to android-cmake-toolchains

mark BUILD_SHARED_LIBS
mark ASSIMP_NO_EXPORT
mark ASSIMP_ENABLE_BOOST_WORKAROUND
unmark ASSIMP_ANDROID_JNIIOSYSTEM (useful, but it has some problems for us to build, so forget it currently)

press Configure
press Generate
```

* modify build.make file manually to prevent the version number behind the .so file extension
```
 cd you-path-to-build-assimp/code/CMakeFiles
 1) open build.make
 replace code/libassimp.so.3.1.1 to code/libassimp.so
 
 remove these lines:
    cd /home/vicky/Projects/assimp-build/code && $(CMAKE_COMMAND) -E cmake_symlink_library libassimp.so.3.1.1 libassimp.so.3 libassimp.so

code/libassimp.so.3: code/libassimp.so.3.1.1

code/libassimp.so: code/libassimp.so.3.1.1

  2) open link.txt 
  
  repace -soname,libassimp.so.3 -o libassimp.so.3.1.1 
  to -soname,libassimp.so -o libassimp.so
```

* make and make install
```
    make
    make install
    get libassimp.so in ./code
    get include files in your install directory or just paste from assimp source include folders is OK
```

## Skeletal Animation ##

[Skeletal Animation ussing ASSIMP and GLSL: bone uniform array size](http://stackoverflow.com/questions/20803441/skeletal-animation-ussing-assimp-and-glsl-bone-uniform-array-size)

## C++ ##
[C++ callback for non-static function between different classes](http://stackoverflow.com/questions/11522422/c-callback-for-non-static-function-between-different-classes?rq=1)

[Callback functions in c++](http://stackoverflow.com/questions/2298242/callback-functions-in-c)

[Google C++ Style Guide](https://google-styleguide.googlecode.com/svn/trunk/cppguide.html)

## Others ##
[Flexible and Economical UTF-8 Decoder](http://bjoern.hoehrmann.de/utf-8/decoder/dfa/)

[std::wstring VS std::string](http://stackoverflow.com/questions/402283/stdwstring-vs-stdstring)

[Flexible and Economical UTF-8 Decoder](http://bjoern.hoehrmann.de/utf-8/decoder/dfa/)

[stb_truetype解析ttf字体获取顶点信息](http://blog.csdn.net/wind_hzx/article/details/11830645)

[Opengl分格化（libtess）移植与使用](http://blog.csdn.net/wind_hzx/article/details/11830425)

[import-module的注意事项与NDK_MODULE_PATH的配置](http://blog.sina.com.cn/s/blog_4057ab62010197z8.html)

[Android Assets IOSystem(I have tried but it can't work)](https://github.com/assimp/assimp/tree/master/port/AndroidJNI)