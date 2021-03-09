# 移植FFmpeg来做音视频处理
A demo for cross compile ffmpeg to Android platforms.

交叉编译ffmpeg，以shared lib方式移植到Android的一个Demo.

## 关键产物与类

- app/libs/arm64-v8a/libffmpegjni.so：以 shared lib 方式提供 ffmpeg.c 里 `ffmpeg_execute` 方法（原本是ffmpeg.c的`main`方法）
- app/src/main/java/com/example/myffmpegandroid/FFmpegHelper.java：Android侧jni方法的入口
- ffmpegjni.h、ffmpegjni.c：Android侧jni与ffmpeg.c的中间层，主要工作是方法对应关系的注册、两边参数的适配、Android log的支持

## 代码结构

### ffmpeg目录
- 编译libffmpegjni.so 所需的源文件，cmdutils、ffmpeg.h、ffmpeg.c、ffmpeg_filter、ffmpeg_opt、ffmpeg_hw 都是从官方 ffmpeg 源代码的 fftools 目录下拷贝而来
- makefile：Android.mk、Application.mk，主要工作是描述以 ffmpeg 交叉编译的七个 shared lib 为依赖，编译产物 libffmpegjni.so 
- libs：编译产物输出目录

#### fftools目录下几个文件的改动点
- ffmpeg.c的main 方法重命名为 ffmpeg_execute
- cmdutils.c 删除 #include "libavresample/avresample.h" 和 #include "libpostproc/postprocess.h"
- cmdutils.c 删除 PRINT_LIB_INFO(avresample, AVRESAMPLE, flags, level); 和 PRINT_LIB_INFO(postproc, POSTPROC, flags, level);
- cmdutils.h 添加 #define AV_LOG_STDERR -16
- cmdutils.c 和 ffmpeg.c 去掉 include "config.h"
- ffmpeg_filter.c 去掉 #include "libavresample/avresample.h"

### ffmpeg/libs 目录
此目录下都是 ffmpeg 以 shared lib 方式交叉编译的 so 文件，会作为 Android.mk 文件里定义的 lib 来 include，用于最后编译 libffmpegjni.so 


### app 目录
此目录主要用于演示如何在Android应用层使用交叉编译好的 libffmpegjni.so 来做音视频处理
