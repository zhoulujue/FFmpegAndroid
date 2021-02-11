#!/bin/sh
export NDK_PROJECT_PATH=.
ANDROID_NDK_ROOT=/Users/lujuezhou/Library/Android/sdk/ndk/22.0.7026061

${ANDROID_NDK_ROOT}/ndk-build -B NDK_APPLICATION_MK=Application.mk APP_BUILD_SCRIPT=Android.mk