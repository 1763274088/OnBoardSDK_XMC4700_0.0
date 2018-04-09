
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
uint8_t fifo_read_ch(fifo_t* fifo, uint8_t* ch);//fifo��������������δ����length��ȡ����tail��1���������㣬tail�Ѿ�׷��head��δ�ɹ���ȡ���ݱ���
uint8_t fifo_write_ch(fifo_t* fifo, uint8_t ch);//fifo������д������δ����lengthд������head��1���������㣬head�Ѿ�׷��tail��δ�ɹ�д�����ݱ���
uint16_t fifo_free(fifo_t* fifo);//head���ж���λҪ׷��tail
uint16_t fifo_used(fifo_t* fifo);//tail���ж���λҪ׷��head
void fifo_init(fifo_t* fifo, uint8_t* buf, uint16_t length);//��ʼ��fifo�������ͺ���

#ifdef __cplusplus 
}
#endif

#endif  /*_FIFO_H_*/



