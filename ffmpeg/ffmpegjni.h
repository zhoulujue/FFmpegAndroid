/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include <android/log.h>
/* Header for class com_example_myffmpegandroid_FFmpegHelper */

#ifndef _Included_com_example_myffmpegandroid_FFmpegHelper
#define _Included_com_example_myffmpegandroid_FFmpegHelper

#define LOG_TAG "ffmpegjni"

/** Verbose Android logging macro. */
#define logv(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

/** Debug Android logging macro. */
#define logd(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

/** Info Android logging macro. */
#define logi(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

/** Warn Android logging macro. */
#define logw(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

/** Error Android logging macro. */
#define loge(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_myffmpegandroid_FFmpegHelper
 * Method:    executeCmd
 * Signature: ([Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_example_myffmpegandroid_FFmpegHelper_executeCmd
  (JNIEnv *, jobject, jobjectArray);

#ifdef __cplusplus
}
#endif
#endif