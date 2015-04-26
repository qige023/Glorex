Reference:
===============================

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

* when see apinames file error
```
copy path-to-freetype-local/objs/apinames to path-to-freetype/objs/apinames
cd path-to-freetype
make install DESTDIR=your-path-to-build-freetype
```

## Others ##
[stb_truetype解析ttf字体获取顶点信息](http://blog.csdn.net/wind_hzx/article/details/11830645)

[Opengl分格化（libtess）移植与使用](http://blog.csdn.net/wind_hzx/article/details/11830425)

[import-module的注意事项与NDK_MODULE_PATH的配置](http://blog.sina.com.cn/s/blog_4057ab62010197z8.html)