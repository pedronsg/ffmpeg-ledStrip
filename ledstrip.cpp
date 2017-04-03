/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "ws2811/ws2811.h"
	//#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include  "libswscale/swscale.h"
#ifdef __cplusplus
}
#endif

#include <sys/time.h>
#include <stdint.h>
#include "ledstrip.h"
#include <iostream>
#include <unistd.h>

using namespace std;

#define TARGET_FREQ                             WS2811_TARGET_FREQ
//#define GPIO_PIN                                18
#define DMA                                     5
//#define STRIP_TYPE                              WS2811_STRIP_RGB        // WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE                            WS2811_STRIP_GBR        // WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE                            SK6812_STRIP_RGBW       // SK6812RGBW (NOT SK6812RGB)
#define STRIPE_TYPE        WS2811_STRIP_GRB 
#define LED_COUNT                               144


ws2811_t ledArray;
 AVFormatContext *fmt_ctx = NULL;
AVCodecContext *video_dec_ctx = NULL;
int width, height;
enum AVPixelFormat pix_fmt;
AVStream *video_stream = NULL;
const char *src_filename = NULL;
uint8_t *video_dst_data[4] = {NULL};
int video_stream_idx = -1;
AVFrame *frame = NULL;
AVPacket pkt;
//int video_frame_count = 0;
int refcount = 0;

struct SwsContext *dstContext;
uint8_t *src_data[4], *dst_data[4];
int src_linesize[4], dst_linesize[4];
//bool media_initialized = false;


uint64_t RGBToHEX(int r, int g, int b) {
	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

	


int decode_packet(int *got_frame) {
	int ret = 0;
	int decoded = pkt.size;

	*got_frame = 0;

	if (pkt.stream_index == video_stream_idx) {
		/* decode video frame */
		ret = avcodec_decode_video2(video_dec_ctx, frame, got_frame, &pkt);
		if (ret < 0) {
			//  fprintf(stderr, "Error decoding video frame (%s)\n", av_err2str(ret));
			return ret;
		}

		if (*got_frame) {

			if (frame->width != width || frame->height != height ||
					frame->format != pix_fmt) {
				/* To handle this change, one could call av_image_alloc again and
				 * decode the following frames into another rawvideo file. */
				/*        fprintf(stderr, "Error: Width, height and pixel format have to be "
								"constant in a rawvideo file, but the width, height or "
								"pixel format of the input video changed:\n"
								"old: width = %d, height = %d, format = %s\n"
								"new: width = %d, height = %d, format = %s\n",
								width, height, av_get_pix_fmt_name(pix_fmt),
								frame->width, frame->height,
								av_get_pix_fmt_name(frame->format));
				 * */
				cout << "error" << endl;
				return -1;
			}

		//	printf("video_frame%s n:%d coded_n:%d\n(cached)", video_frame_count++, frame->coded_picture_number);

			/* copy decoded frame to destination buffer:
			 * this is required since rawvideo expects non aligned data */
			av_image_copy(src_data, src_linesize, (const uint8_t **) (frame->data), frame->linesize, pix_fmt, width, height);
			av_image_copy(dst_data, dst_linesize, (const uint8_t **) (frame->data), frame->linesize, AV_PIX_FMT_RGB24, width, height);

			sws_scale(dstContext, (uint8_t const * const *) frame->data, frame->linesize, 0, frame->height, dst_data, dst_linesize);
			
			
			for (int x=0;x<LED_COUNT;x++){
				int p = x * 3 + 0 * dst_linesize[0];
				int r = dst_data[0][p];
				int g = dst_data[0][p + 1];
				int b = dst_data[0][p + 2];
				ledArray.channel[0].leds[x] = RGBToHEX(r,g,b);
				//cout<<r<<" "<<g<<" "<<b<<endl;
			}
			ws2811_render(&ledArray);
			usleep(10000);
		}
	}

	/* If we use frame reference counting, we own the data and need
	 * to de-reference it when we don't use it anymore */
	if (*got_frame && refcount) {
		av_frame_unref(frame);

	}


	return decoded;
}

int open_codec_context(int *stream_idx, AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type) {
	int ret, stream_index;
	AVStream *st;
	AVCodec *dec = NULL;
	AVDictionary *opts = NULL;

	ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
	if (ret < 0) {
		//  fprintf(stderr, "Could not find %s stream in input file '%s'\n",
		//     av_get_media_type_string(type), src_filename);
		return ret;
	} else {
		stream_index = ret;
		st = fmt_ctx->streams[stream_index];

		/* find decoder for the stream */
		dec = avcodec_find_decoder(st->codecpar->codec_id);
		if (!dec) {
			cout << "error Failed to find codec" << endl;
			//   fprintf(stderr, "Failed to find %s codec\n",
			//      av_get_media_type_string(type));
			return AVERROR(EINVAL);
		}

		/* Allocate a codec context for the decoder */
		*dec_ctx = avcodec_alloc_context3(dec);
		if (!*dec_ctx) {
			cout << "error Failed to find context" << endl;
			//  fprintf(stderr, "Failed to allocate the %s codec context\n",
			//   av_get_media_type_string(type));
			return AVERROR(ENOMEM);
		}

		/* Copy codec parameters from input stream to output codec context */
		if ((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar)) < 0) {
			cout << "error Failed to copy %s codec parameters to decoder context" << endl;
			//   fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
			//    av_get_media_type_string(type));
			return ret;
		}

		/* Init the decoders, with or without reference counting */
		av_dict_set(&opts, "refcounted_frames", refcount ? "1" : "0", 0);
		if ((ret = avcodec_open2(*dec_ctx, dec, &opts)) < 0) {
			cout << "error Failed to open codec" << endl;
			//  fprintf(stderr, "Failed to open %s codec\n",
			//   av_get_media_type_string(type));
			return ret;
		}
		*stream_idx = stream_index;
	}

	return 0;
}

Ledstrip::Ledstrip(int gpioPin){
	videoLoop = false;
	isPlaying=false;
	ledArray.freq = TARGET_FREQ;
	ledArray.dmanum = DMA;
	ledArray.channel[0].strip_type = STRIPE_TYPE;
	ledArray.channel[0].gpionum = gpioPin;
	ledArray.channel[0].count = LED_COUNT;
	ledArray.channel[0].invert = 0;
	brightness = 50;
	ledArray.channel[0].brightness = brightness;
	ledArray.channel[1].gpionum = 0;
	ledArray.channel[1].count = 0;
	ledArray.channel[1].invert = 0;
	ledArray.channel[1].brightness = 0;
	//	cout << ledArray.freq << endl;
	ws2811_init(&ledArray);
}


int initMedia(const char *filename){
	int ret = 0;//, got_frame;

	/* register all formats and codecs */
	av_register_all();

	/* open input file, and allocate format context */
	if (avformat_open_input(&fmt_ctx, filename, NULL, NULL) < 0) {
		fprintf(stderr, "Could not open source file %s\n", src_filename);
		return -1;
	}

	/* retrieve stream information */
	if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
		fprintf(stderr, "Could not find stream information\n");
		return -1;
	}


	if (open_codec_context(&video_stream_idx, &video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
		video_stream = fmt_ctx->streams[video_stream_idx];

		// allocate image where the decoded image will be put 
		width = video_dec_ctx->width;
		height = video_dec_ctx->height;
		pix_fmt = video_dec_ctx->pix_fmt;
	  	}
	 

	// allocate source and destination image buffers 
	if ((ret = av_image_alloc(src_data, src_linesize, width, height, pix_fmt, 1)) < 0) {
		fprintf(stderr, "Could not allocate source image\n");
		return -1;
	}

	// buffer is going to be written to rawvideo file, no alignment 
	if ((ret = av_image_alloc(dst_data, dst_linesize, width, height, AV_PIX_FMT_RGB24, 1)) < 0) {
		fprintf(stderr, "Could not allocate destination image\n");
		return -1;
	}

	/* dump input information to stderr */
	av_dump_format(fmt_ctx, 0, src_filename, 0);



	//	fps=av_q2d(fmt_ctx->time_base);;
	//	 if(fps > 0.0001f) {
	//  frame_delay = (1.0f/fps) * 1000ull * 1000ull * 1000ull;
	//  frame_timeout = rx_hrtime() + frame_delay;
	//}

	
	if (!video_stream) {
		fprintf(stderr, "Could not find video stream in the input, aborting\n");
	//	ret = 1;
		return -1;
	}



	frame = av_frame_alloc();
	

	if (!frame) {
		fprintf(stderr, "Could not allocate frame\n");
		ret = AVERROR(ENOMEM);
		return -1;
	}

	
	dstContext = NULL;
	dstContext = sws_getContext(width, height, pix_fmt, width, height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
	return ret;
}





//initialize packet, set data to NULL, let the demuxer fill it //
void playMedia(int ret){
	int got_frame;
	av_init_packet(&pkt);
	pkt.data = NULL;
	pkt.size = 0;
	
	
	// read frames from the file //
	while (av_read_frame(fmt_ctx, &pkt) >= 0) {
		AVPacket orig_pkt = pkt;
		do {
			ret = decode_packet(&got_frame);
			if (ret < 0)
				break;
			pkt.data += ret;
			pkt.size -= ret;
		} while (pkt.size > 0);
		av_packet_unref(&orig_pkt);
	}
}




//0-255 Brightness for all leds
void Ledstrip::setBrightness(int value) {
	brightness = value;
	ledArray.channel[0].brightness = brightness;
}

void Ledstrip::play(const char* filename) {
	
	int ret=initMedia(filename);
	
	if (ret<0) return;
	
	//while (videoLoop==true){
		playMedia(ret);
	//}
	
/*
	//initialize packet, set data to NULL, let the demuxer fill it //
	av_init_packet(&pkt);
	pkt.data = NULL;
	pkt.size = 0;

	
	// read frames from the file //
	while (av_read_frame(fmt_ctx, &pkt) >= 0) {
		AVPacket orig_pkt = pkt;
		do {
			ret = decode_packet(&got_frame);
			if (ret < 0)
				break;
			pkt.data += ret;
			pkt.size -= ret;
		} while (pkt.size > 0);
		av_packet_unref(&orig_pkt);
	}
	*/
/*	
	// flush cached frames 
	pkt.data = NULL;
	pkt.size = 0;

	do {
		decode_packet(&got_frame);
	} while (got_frame);
	///	k++;
	//	}
*/
	avcodec_free_context(&video_dec_ctx);
	// avcodec_free_context(&audio_dec_ctx);
	avformat_close_input(&fmt_ctx);
	av_frame_free(&frame);
	av_free(video_dst_data[0]);
/*	for (int x = 0; x < LED_COUNT; x++) {
		ledArray.channel[0].leds[x] = 0;
	}
	//cout<<"Frame "<<endl;
	ws2811_render(&ledArray);
*/
 }

int Ledstrip::stop() {
	avcodec_free_context(&video_dec_ctx);
	// avcodec_free_context(&audio_dec_ctx);
	avformat_close_input(&fmt_ctx);
	av_frame_free(&frame);
	av_free(video_dst_data[0]);
	for (int x = 0; x < 144; x++) {
		ledArray.channel[0].leds[x] = 0;
	}
	//cout<<"Frame "<<endl;
	ws2811_render(&ledArray);
}

void Ledstrip::loop(bool l) {
	this->videoLoop = l;
}

uint64_t Ledstrip::nano() {
	static clock_t fast_clock_id = -1;
	struct timespec t;
	clock_t clock_id;

	if (fast_clock_id == -1) {
		if (clock_getres(CLOCK_MONOTONIC_COARSE, &t) == 0 && t.tv_nsec <= 1 * 1000 * 1000LLU) {
			fast_clock_id = CLOCK_MONOTONIC_COARSE;
		} else {
			fast_clock_id = CLOCK_MONOTONIC;
		}
	}

	clock_id = CLOCK_MONOTONIC;
	if (clock_gettime(clock_id, &t)) {
		return 0;
	}

	return t.tv_sec * (uint64_t) 1e9 + t.tv_nsec;
}

