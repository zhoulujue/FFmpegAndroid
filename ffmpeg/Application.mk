# https://developer.android.com/ndk/guides/application_mk
APP_OPTIM := release

APP_ABI := arm64-v8a
#APP_ABI := armeabi-v7a

APP_STL := none

APP_PLATFORM := android-21

APP_CFLAGS := -O3 -DANDROID -DMOBILE_FFMPEG_BUILD_DATE=20210210 \
				-Wall -Wno-deprecated-declarations -Wno-pointer-sign -Wno-switch -Wno-unused-result -Wno-unused-variable \

APP_LDFLAGS := -Wl,--hash-style=both
