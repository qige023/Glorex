BUILD_ALL := TRUE

ifeq ($(BUILD_ALL),TRUE)
	include $(call all-subdir-makefiles)
else
	JNI_ROOT:= $(call my-dir)
	include $(CLEAR_VARS)
	
	include $(JNI_ROOT)/BlendingWindow/Android.mk
	#include $(JNI_ROOT)/Instancing/Android.mk
	#include $(JNI_ROOT)/SimpleVertexShader/Android.mk
	#include $(JNI_ROOT)/SimpleTexture/Android.mk
	#include $(JNI_ROOT)/TorusDiffuse/Android.mk
endif