APP_PLATFORM := android-18

APP_STL := stlport_static

APP_OPTIM := debug

# use this to select gcc instead of clang
#NDK_TOOLCHAIN_VERSION := 4.9
# OR use this to select the latest clang version:
#NDK_TOOLCHAIN_VERSION := clang

# then enable c++11 extentions in source code
#APP_CPPFLAGS += -std=c++11
# or use APP_CPPFLAGS := -std=gnu++11