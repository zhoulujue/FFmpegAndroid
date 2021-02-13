
fftools 目录下改动点：

ffmpeg.c:
- main 方法重命名为 ffmpeg_execute
- cmdutils.c 删除 `#include "libavresample/avresample.h"` 和 `#include "libpostproc/postprocess.h"`
- cmdutils.c 删除 `PRINT_LIB_INFO(avresample, AVRESAMPLE, flags, level);` 和
`PRINT_LIB_INFO(postproc,   POSTPROC,   flags, level);`
- cmdutils.h 添加 `#define AV_LOG_STDERR    -16`
- cmdutils.c 和 ffmpeg.c 去掉 include "config.h"
- ffmpeg_filter.c 去掉 `#include "libavresample/avresample.h"`
-

