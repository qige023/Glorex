LOCAL_PATH          := $(call my-dir)
COMMON_PATH			:= ../../common
COMMON_INC_PATH		:= src/main/common/include
COMMON_SRC_PATH		:= $(COMMON_PATH)/source

include $(CLEAR_VARS)

LIBRARY_NAME = HelloTriangle

LOCAL_MODULE    := $(LIBRARY_NAME)

LOCAL_CFLAGS    += -DANDROID

LOCAL_SRC_FILES := $(COMMON_SRC_PATH)/esShader.c \
				   $(COMMON_SRC_PATH)/esShapes.c \
				   $(COMMON_SRC_PATH)/esTransform.c \
				   $(COMMON_SRC_PATH)/esUtil.c \
				   $(COMMON_SRC_PATH)/Android/esUtil_Android.c \
				   $(LIBRARY_NAME).c

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(COMMON_INC_PATH)

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3

LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
