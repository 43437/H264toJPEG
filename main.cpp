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

int writeJPEG(AVFrame *pFrame, int width, int height, int iIndex);

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;
    
    int videoStream=-1;
    AVCodecContext *pCodecCtx;
    AVFormatContext *pFormatCtx;
    AVCodec *pCodec;
    AVFrame *pFrame/*, *pFrameRGB*/;
//     struct SwsContext *pSwsCtx;
    const char *filename="movie.h264";
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
    
    if ( avformat_find_stream_info(pFormatCtx, NULL ) < 0 ){
      std::cout<<"av find stream failed! "<<std::endl;
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
    
    std::printf("video stream %d\n", videoStream);
    std::cout<<"video stream "+videoStream<<std::endl;

    pCodecCtx=pFormatCtx->streams[videoStream]->codec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL){
      std::printf("avcodec find decorder failed!\n");
      exit(1);
    }
    
    if ( avcodec_open2(pCodecCtx, pCodec, NULL ) < 0){
      std::cout<<"avcode open failed! "<<std::endl;
      exit(1);
    }

    pFrame=av_frame_alloc();
//     pFrameRGB=av_frame_alloc();

    if(pFrame==NULL/*||pFrameRGB==NULL*/){
      std::printf("avcodec alloc frame failed!\n");
      exit(1);
    }
   /* 
    PictureSize=avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
    std::printf("picture size: %d\n", PictureSize);

    outBuff = (uint8_t*)av_malloc(PictureSize);
    if ( outBuff == NULL ){
      std::printf("av malloc failed!\n");
      exit(1);
    }
    avpicture_fill((AVPicture *)pFrameRGB, outBuff, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
    
    pSwsCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
			     pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    */
    int ret_av_read_frame = -2;
    while((ret_av_read_frame=av_read_frame(pFormatCtx, &packet)) >=0){
      
      if (i>50){
	break;
      }
      if(packet.stream_index == videoStream ){
	std::cout<<"video stream"<<std::endl;
	avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
	
	std::cout<<"frameFinished is finished"<<frameFinished<<std::endl;
	if(frameFinished){
	    std::cout<<"before call writeJPEG "<<std::endl;
	    std::cout<<"i is "<<i<<std::endl;
	    writeJPEG(pFrame, pCodecCtx->width, pCodecCtx->height, i++);
	}
      } else {
	std::cout<<packet.stream_index<<"is not video stream. "<<std::endl;
      }
      
      av_free_packet(&packet);
    } 
     
    std::cout<<"ret_av_read_frame"<<ret_av_read_frame<<std::endl;

    
//     sws_freeContext(pSwsCtx);
    av_free(pFrame);
//     av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}

int writeJPEG(AVFrame *pFrame, int width, int height, int iIndex){
  
  char out_file[20] = {0};
  std::cout<<"index is "<<iIndex<<std::endl;
  std::sprintf(out_file,  "out%d.jpg", iIndex);
  
  AVFormatContext *pFormatCtx = avformat_alloc_context();
  pFormatCtx->oformat = av_guess_format("mjpeg", NULL, NULL);
  
  if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0){
    std::cout<<"Couldn't open output file."<<std::endl;
    return -1;
  }
  
  AVStream *pAVStream = avformat_new_stream(pFormatCtx, 0);
  if ( pAVStream == NULL ){
    return -1;
  }
  
  AVCodec* pCodec = avcodec_find_encoder(pFormatCtx->oformat->video_codec);
  AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
  if (pCodecCtx == NULL ){
    std::cout<<"AVCodecContext alloc failed. "<<std::endl;
    exit(1);
  }
  
  pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
  pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
  pCodecCtx->width = width;
  pCodecCtx->height = height;
  pCodecCtx->time_base.num = 1;
  pCodecCtx->time_base.den = 25;
  
  av_dump_format(pFormatCtx, 0, out_file, 1);
  
  if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
    std::cout<<"Could not open codec. "<<std::endl;
    return -1;
  }
  
  avformat_write_header(pFormatCtx, NULL);
  
  int y_size = pCodecCtx->width * pCodecCtx->height;
  
  AVPacket pkt;
  av_new_packet(&pkt, y_size * 3);
  
  int got_picture = 0;
  int ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_picture);
  if ( ret < 0 ){
    std::cout<<"Encode Error. "<<std::endl;
    return -1;
  }
  
  if (got_picture == 1){
    ret = av_write_frame(pFormatCtx, &pkt);
  }
  
  av_free_packet(&pkt);
  av_write_trailer(pFormatCtx);
  
  std::cout<<"Encode Successful. "<<std::endl;
  
  if (pAVStream){
    avcodec_close(pAVStream->codec);
  }
  
  avio_close(pFormatCtx->pb);
  avformat_free_context(pFormatCtx);
  
  return 0;
}
