cmake_minimum_required(VERSION 2.6)
project(ffmpeguse)

include_directories("/home/acmax/app/ffmpeg-dev/include/")

link_directories("/home/acmax/app/ffmpeg-dev/lib/")

#link_libraries("/home/acmax/app/ffmpeg-dev/lib/libavformat.so" "/home/acmax/app/ffmpeg-dev/lib/libavcodec.so" "/home/acmax/app/ffmpeg-dev/lib/libavutil.so" "/home/acmax/app/ffmpeg-dev/lib/libswresample.so" "/home/acmax/app/ffmpeg-dev/lib/libswscale.so")
link_libraries("avformat" "avcodec" "avutil" "swresample" "swscale")

add_executable(ffmpeguse main.cpp)
