LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := assimp
LOCAL_SRC_FILES := ../../../../libs/$(TARGET_ARCH_ABI)/libassimp.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include \
			$(LOCAL_PATH)/include/assimp
include $(PREBUILT_STATIC_LIBRARY)
