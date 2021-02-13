#LIB_FILE_PATH=%CURRENT_PATH%\obj\local\armeabi\%1%
LIB_FILE_PATH=./obj/local/arm64-v8a/$1
CRASH_ADDRESS=$2

TOOL_PATH="/Users/lujuezhou/Library/Android/sdk/ndk-bundle/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-android-addr2line"

$TOOL_PATH -C -f -e "${LIB_FILE_PATH}" "${CRASH_ADDRESS}"