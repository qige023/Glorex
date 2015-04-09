LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := ../tools/include/libSOIL
LOCAL_SRC_FILES := ../tools/library/libSOIL.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)

include $(call all-subdir-makefiles)
