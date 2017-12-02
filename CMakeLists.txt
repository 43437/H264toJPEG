cmake_minimum_required(VERSION 2.6)
project(H264toJPEG)

include_directories("/home/acmax/app/ffmpeg-dev/include/")

link_directories("/home/acmax/app/ffmpeg-dev/lib/")

#link_libraries("/home/acmax/app/ffmpeg-dev/lib/libavformat.so" "/home/acmax/app/ffmpeg-dev/lib/libavcodec.so" "/home/acmax/app/ffmpeg-dev/lib/libavutil.so" "/home/acmax/app/ffmpeg-dev/lib/libswresample.so" "/home/acmax/app/ffmpeg-dev/lib/libswscale.so")

add_executable(H264toJPEG main.cpp)

target_link_libraries(H264toJPEG "avformat" "avcodec" "avutil" "swresample" "swscale")