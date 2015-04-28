LOCAL_PATH          := $(call my-dir)
ENGINE_PATH			:= ../../engine
ENGINE_INC_PATH		:= src/main/engine

include $(CLEAR_VARS)

LIBRARY_NAME = TorusDiffuse

LOCAL_MODULE    := $(LIBRARY_NAME)

LOCAL_CFLAGS    += -DANDROID

LOCAL_SRC_FILES := $(ENGINE_PATH)/esprogram.cpp \
				   $(ENGINE_PATH)/esutil.cpp \
				   $(ENGINE_PATH)/esfile.cpp \
				   $(ENGINE_PATH)/android/esutil_android.cpp \
				   $(ENGINE_PATH)/vbos/vbotorus.cpp \
				   $(LIBRARY_NAME).cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(ENGINE_INC_PATH) \
                    $(ENGINE_INC_PATH)/android \
                    $(ENGINE_INC_PATH)/vbos \
                    $(ENGINE_INC_PATH)/loader

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3

LOCAL_STATIC_LIBRARIES := android_native_app_glue

#The NDK toolchain supports C++ exceptions, since NDK r5, 
#however all C++ sources are compiled with -fno-exceptions support by default, 
#for compatibility reasons with previous releases.
#To enable it, use the new LOCAL_CPP_FEATURES variable in your Android.mk, as in:
LOCAL_CPP_FEATURES += exceptions

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
