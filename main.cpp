#include <iostream>
#include <cstdio>

#ifdef __cplusplus
extern "C" {
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libswscale/swscale.h>
#endif
#ifdef __cplusplus
} // endof extern "C"
#endif


int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;
    
    int videoStream=-1;
    AVCodecContext *pCodecCtx;
    AVFormatContext *pFormatCtx;
    AVCodec *pCodec;
    AVFrame *pFrame, *pFrameRGB;
    struct SwsContext *pSwsCtx;
    const char *filename="movie.mp4";
    AVPacket packet;
    int frameFinished;
    int PictureSize;
    uint8_t *outBuff;

    av_register_all();
    avformat_network_init();
    pFormatCtx=avformat_alloc_context();

    if(avformat_open_input(&pFormatCtx, filename, NULL, NULL)!=0){
      std::printf("av open input file failed!\n");
      exit(1);
    }

    int i=0;
    for(i=0;i<pFormatCtx->nb_streams;i++){
      if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
	videoStream=i;
	break;
      }
    }
    
    if(videoStream == -1){
      std::printf("find video stream failed!\n");
      exit(1);
    }

    pCodecCtx=pFormatCtx->streams[videoStream]->codec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL){
      std::printf("avcodec find decorder failed!\n");
      exit(1);
    }

    pFrame=av_frame_alloc();
    pFrameRGB=av_frame_alloc();

    if(pFrame==NULL||pFrameRGB==NULL){
      std::printf("avcodec alloc frame failed!\n");
      exit(1);
    }

    PictureSize=avpicture_get_size(AV_PIX_FMT_YUVJ420P, pCodecCtx->width, pCodecCtx->height);
    std::printf("picture size: %d\n", PictureSize);


    return 0;
}
