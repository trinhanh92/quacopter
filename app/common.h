#ifndef __COMMON_H__
#define __COMMON_H__

#define I2C_SLAVE_ADDR  0x08
/** For controling the wheels */
#define GO_FORWARD 		0x11
#define TURN_LEFT       0x22
#define TURN_RIGHT      0x33
#define GO_BACKWARD     0x44
/** For controling the servo */
#define ROTATE_LEFT     0x69
#define ROTATE_RIGHT    0x96
#define ROTATE_STOP     0x66         

/** mode defination */
#define MODE_AUTO       0x01
#define MODE_MANUAL     0x02

/** Define data types*/
typedef unsigned char u8_t;
typedef signed char i8_t;
typedef unsigned short u16_t;
typedef signed short i16_t;
typedef unsigned int u32_t;
typedef signed int i32_t;

#define UV4L_STREAMING "sudo uv4l -nopreview --auto-video_nr --driver raspicam --encoding h264 \
                        --width 640 --height 480 --framerate 30 --server-option '--port=9696' \
                        --server-option '--max-queued-connections=30' --server-option '--max-streams=25'\
                        --server-option '--max-threads=29'"
#endif
