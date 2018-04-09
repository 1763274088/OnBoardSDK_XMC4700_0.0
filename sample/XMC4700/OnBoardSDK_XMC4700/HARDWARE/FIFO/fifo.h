
#ifndef _FIFO_H_
#define _FIFO_H_

//#include "main.h" 
//#include "cppforXMC4700.h"
#include "XMC4700.h"


#define true 1
#define false 0

typedef struct _fifo {
	uint8_t* buf;
	uint16_t length;
	uint16_t head;
	uint16_t tail;
} fifo_t;


#ifdef __cplusplus 
extern "C" {
#endif
uint8_t fifo_read_ch(fifo_t* fifo, uint8_t* ch);//fifo数据区读函数，未超过length读取数据tail加1，超过归零，tail已经追上head即未成功读取数据报错
uint8_t fifo_write_ch(fifo_t* fifo, uint8_t ch);//fifo数据区写函数，未超过length写入数据head加1，超过归零，head已经追上tail即未成功写入数据报错
uint16_t fifo_free(fifo_t* fifo);//head还有多少位要追上tail
uint16_t fifo_used(fifo_t* fifo);//tail还有多少位要追上head
void fifo_init(fifo_t* fifo, uint8_t* buf, uint16_t length);//初始化fifo数据类型函数

#ifdef __cplusplus 
}
#endif

#endif  /*_FIFO_H_*/



