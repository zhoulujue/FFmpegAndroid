#include "ffmpegjni.h"

#include "jni.h"
#include "ffmpeg.h"
#include "libavutil/bprint.h"
#include "libavutil/log.h"
#include <pthread.h>
#include <sys/types.h>

// ffmpeg.c 中的入口方法，由 main方法改名而来
int ffmpeg_execute(int argc, char **argv);

static JavaVM *m_vm;

// 绑定 Java methods 和 native methods
static JNINativeMethod configMethods[] = {
    {"executeCmd", "([Ljava/lang/String;)I", (void*) Java_com_example_myffmpegandroid_FFmpegHelper_executeCmd}
};

// 注册configMethods，这个方法会在 JNI_OnLoad 中调用
int register_ffmpeg_helper(JNIEnv *env)
{
    int numMethods = (sizeof(configMethods) / sizeof( (configMethods)[0]));
    jclass clazz = (*env)->FindClass(env, "com/example/myffmpegandroid/FFmpegHelper");
    jint ret = (*env)->RegisterNatives(env, clazz, configMethods, numMethods);
    (*env)->DeleteLocalRef(env, clazz);
    return ret;
}

/////////////////////////////////// log 方法 ///////////////////////////////////
static const char *avutil_log_get_level_str(int level) {
    switch (level) {
    case AV_LOG_STDERR:
        return "stderr";
    case AV_LOG_QUIET:
        return "quiet";
    case AV_LOG_DEBUG:
        return "debug";
    case AV_LOG_VERBOSE:
        return "verbose";
    case AV_LOG_INFO:
        return "info";
    case AV_LOG_WARNING:
        return "warning";
    case AV_LOG_ERROR:
        return "error";
    case AV_LOG_FATAL:
        return "fatal";
    case AV_LOG_PANIC:
        return "panic";
    default:
        return "";
    }
}

static void avutil_log_format_line(void *avcl, int level, const char *fmt, va_list vl, AVBPrint part[4], int *print_prefix) {
    int flags = av_log_get_flags();
    AVClass* avc = avcl ? *(AVClass **) avcl : NULL;
    av_bprint_init(part+0, 0, 1);
    av_bprint_init(part+1, 0, 1);
    av_bprint_init(part+2, 0, 1);
    av_bprint_init(part+3, 0, 65536);

    if (*print_prefix && avc) {
        if (avc->parent_log_context_offset) {
            AVClass** parent = *(AVClass ***) (((uint8_t *) avcl) +
                                   avc->parent_log_context_offset);
            if (parent && *parent) {
                av_bprintf(part+0, "[%s @ %p] ",
                         (*parent)->item_name(parent), parent);
            }
        }
        av_bprintf(part+1, "[%s @ %p] ",
                 avc->item_name(avcl), avcl);
    }

    if (*print_prefix && (level > AV_LOG_QUIET) && (flags & AV_LOG_PRINT_LEVEL))
        av_bprintf(part+2, "[%s] ", avutil_log_get_level_str(level));

    av_vbprintf(part+3, fmt, vl);

    if(*part[0].str || *part[1].str || *part[2].str || *part[3].str) {
        char lastc = part[3].len && part[3].len <= part[3].size ? part[3].str[part[3].len - 1] : 0;
        *print_prefix = lastc == '\n' || lastc == '\r';
    }
}

static void avutil_log_sanitize(uint8_t *line) {
    while(*line){
        if(*line < 0x08 || (*line > 0x0D && *line < 0x20))
            *line='?';
        line++;
    }
}

#define LINE_SZ 1024
void log_anyway(void *ptr, int level, const char *fmt, va_list vl)
{
    static int print_prefix = 1;
    static char line[LINE_SZ];
    unsigned tint = 0;

    if (level >= 0) {
        tint = level & 0xff00;
        level &= 0xff;
    }

    memset(line, '\0', LINE_SZ);
    av_log_format_line(ptr, level, fmt, vl, line, LINE_SZ, &print_prefix);

    switch (level) {
        case AV_LOG_QUIET:
            break;
        case AV_LOG_PANIC:
        case AV_LOG_FATAL:
            loge("%s", line);
            break;
        case AV_LOG_ERROR:
            loge("%s", line);
            break;
        case AV_LOG_WARNING:
            logw("%s", line);
            break;
        case AV_LOG_INFO:
            logi("%s", line);
            break;
        case AV_LOG_VERBOSE:
            logv("%s", line);
            break;
        case AV_LOG_DEBUG:
        case AV_LOG_TRACE:
            logd("%s", line);
            break;
    }
}
//////////////////////////////////////////////////////////////////////

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    m_vm = vm;
    JNIEnv* env = NULL;
    jint result = -1;

    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        logv("%s", "ERROR: GetEnv failed\n");
        goto bail;
    }
    //assert(env != NULL);
    if (env == NULL) {
        goto bail;
    }

    if (register_ffmpeg_helper(env) < 0) {
        loge("%s", "ERROR: FFmpegNativeHelper native registration failed\n");
        goto bail;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_6;

    bail:
    return result;
}

// Log 的 callback 方法，avutil 的 example 里有
/**
 * Callback function for FFmpeg logs.
 *
 * @param ptr pointer to AVClass struct
 * @param level log level
 * @param format format string
 * @param vargs arguments
 */
void log_callback_function(void *ptr, int level, const char* format, va_list vargs) {
    AVBPrint fullLine;
    AVBPrint part[4];
    int print_prefix = 1;

    if (level >= 0) {
        level &= 0xff;
    }
    int activeLogLevel = av_log_get_level();

    if ((activeLogLevel == AV_LOG_QUIET) || (level > activeLogLevel)) {
        return;
    }
    log_anyway(ptr, level, format, vargs);

    av_bprint_init(&fullLine, 0, AV_BPRINT_SIZE_UNLIMITED);

    avutil_log_format_line(ptr, level, format, vargs, part, &print_prefix);
    avutil_log_sanitize(part[0].str);
    avutil_log_sanitize(part[1].str);
    avutil_log_sanitize(part[2].str);
    avutil_log_sanitize(part[3].str);

    // COMBINE ALL 4 LOG PARTS
    av_bprintf(&fullLine, "%s%s%s%s", part[0].str, part[1].str, part[2].str, part[3].str);

    //if (fullLine.len > 0) {
    //    logCallbackDataAdd(level, &fullLine);
    //    appendLastCommandOutput(&fullLine);
    //}

    av_bprint_finalize(part, NULL);
    av_bprint_finalize(part+1, NULL);
    av_bprint_finalize(part+2, NULL);
    av_bprint_finalize(part+3, NULL);
    av_bprint_finalize(&fullLine, NULL);
}

// 响应 Java 层的入口方法
JNIEXPORT jint Java_com_example_myffmpegandroid_FFmpegHelper_executeCmd(JNIEnv *env, jobject thiz, jobjectArray strArray)
{
    loge("%s", "native executeCmd called!\n");
    // avutil 提供的设置log的方法
    av_log_set_callback(log_callback_function);

    // 构造要传给 ffmpeg_execute 的两个参数：argc、argv
    int argc = (*env)->GetArrayLength(env, strArray);
    char **argv = (char **)malloc(sizeof(char*) * argc);

    for (int i = 0; i < argc; i++) {
        argv[i] = (*env)->GetStringUTFChars(env, (jstring)(*env)->GetObjectArrayElement(env, strArray, i), NULL);
    }

    // execute!
    int ret = ffmpeg_execute(argc, argv);

    // cleanup
    // 回收 argv
    for (int i = 0; i < argc; i++) {
        (*env)->ReleaseStringUTFChars(env, (jstring)(*env)->GetObjectArrayElement(env, strArray, i), argv[i]);
    }
    free(argv);

    return ret;
}
