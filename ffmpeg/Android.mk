LIB_NAME_PREFIX := /Users/lujuezhou/ffmpegbuilddir/ffmpeg-install-dir/android-arm/lib
MY_ARM_MODE := arm
MY_ARM_NEON := true

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := $(MY_ARM_MODE)
LOCAL_MODULE := libavcodec
LOCAL_SRC_FILES := $(LIB_NAME_PREFIX)/libavcodec.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := $(MY_ARM_MODE)
LOCAL_MODULE := libavfilter
LOCAL_SRC_FILES := $(LIB_NAME_PREFIX)/libavfilter.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := $(MY_ARM_MODE)
LOCAL_MODULE := libavdevice
LOCAL_SRC_FILES := $(LIB_NAME_PREFIX)/libavdevice.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := $(MY_ARM_MODE)
LOCAL_MODULE := libavformat
LOCAL_SRC_FILES := $(LIB_NAME_PREFIX)/libavformat.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := $(MY_ARM_MODE)
LOCAL_MODULE := libavutil
LOCAL_SRC_FILES := $(LIB_NAME_PREFIX)/libavutil.so
LOCAL_EXPORT_C_INCLUDES := $(LIB_NAME_PREFIX)/../include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := $(MY_ARM_MODE)
LOCAL_MODULE := libswresample
LOCAL_SRC_FILES := $(LIB_NAME_PREFIX)/libswresample.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := $(MY_ARM_MODE)
LOCAL_MODULE := libswscale
LOCAL_SRC_FILES := $(LIB_NAME_PREFIX)/libswscale.so
include $(PREBUILT_SHARED_LIBRARY)

############################# ffmpegjni.so #############################
include $(CLEAR_VARS)
LOCAL_ARM_MODE := $(MY_ARM_MODE)
LOCAL_ARM_NEON := $(MY_ARM_NEON)
LOCAL_MODULE := ffmpegjni
LOCAL_SRC_FILES := ffmpegjni.c \
					cmdutils.c \
					ffmpeg.c \
					ffmpeg_opt.c \
					ffmpeg_hw.c \
					ffmpeg_filter.c

LOCAL_C_INCLUDES := /Users/lujuezhou/ffmpegbuilddir/ffmpeg-install-dir/android-arm/include
LOCAL_CFLAGS := -Wall -Werror -Wno-unused-parameter -Wno-switch -Wno-sign-compare \
				-Wno-implicit-function-declaration -Wno-incompatible-pointer-types-discards-qualifiers \
				-Wno-parentheses -Wno-sign-compare -Wno-implicit-const-int-float-conversion \
				-Wno-missing-field-initializers -Wno-asm-operand-widths -Wno-int-conversion

LOCAL_LDLIBS := -llog -lz -landroid
LOCAL_SHARED_LIBRARIES := libavfilter libavformat libavcodec libavutil libswresample libavdevice libswscale

ifeq ($(APP_STL), c++_shared)
    LOCAL_SHARED_LIBRARIES += c++_shared # otherwise NDK will not add the library for packaging
endif

include $(BUILD_SHARED_LIBRARY)
##########################################################################