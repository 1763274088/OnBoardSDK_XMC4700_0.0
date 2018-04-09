/**
 * @file fifo.c
 * @brief ����FIFO���ݵĶ�д�Լ�ʣ�����ݵ��ж�
 * @author DavidWen
 * @version v1.0
 * @date 2017-08-02
 */

#include "fifo.h"

/* --------------------------------------------------------------------------*/
/**
 * @brief ��FIFO����
 *
 * @param fifo ��ȡFIFOָ��
 * @param ch ��ȡ��������ָ��
 *
 * @returns  ��ȡ��״̬���ɹ���ȡ���true�����ȡ���0 
 *
 * @warning �������һ�����serial_read_ch()����ʹ�á�
 */
/* ----------------------------------------------------------------------------*/
uint8_t fifo_read_ch(fifo_t* fifo, uint8_t* ch)
{
	if(fifo->tail == fifo->head) return false;
	*ch = fifo->buf[fifo->tail];  
	
	if(++fifo->tail >= fifo->length) fifo->tail = 0;
  return true;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief дһ���ֽ����ݵ�FIFO
 *
 * @param fifo ��д��FIFOָ��
 * @param ch Ҫд�������
 *
 * @returns  д��ɹ�����1��д��ʧ�ܷ���0 
 */
/* ----------------------------------------------------------------------------*/
uint8_t fifo_write_ch(fifo_t* fifo, uint8_t ch)
{
	uint16_t h = fifo->head;
	
	if(++h >= fifo->length) h = 0;
	if(h == fifo->tail) return false;
	
	fifo->buf[fifo->head] = ch;
	fifo->head = h;
  return true;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief ����FIFO�ж���λ�ÿ���д��������дָ�뻹�ж���λ����׷�϶�ָ��
 *
 * @param fifo Ҫ����FIFOָ��
 *
 * @returns  ���ؿ�д���� 
 *
 * @note  ʣ���ֽڳ��ȴ��ڵ���2ʱ���ſ�д������
 */
/* ----------------------------------------------------------------------------*/
uint16_t fifo_free(fifo_t* fifo)  
{
	uint16_t free;
	
	if(fifo->head >= fifo->tail) free = fifo->tail + (fifo->length - fifo->head);
	else free = fifo->tail - fifo->head;
	
  return free;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief ����FIFO�������ж���λ�ÿɶ�����������ָ�뻹�ж���λ����׷��дָ��
 *
 * @param fifo Ҫ����FIFOָ��
 *
 * @returns  ���Զ��ĳ��� 
 */
/* ----------------------------------------------------------------------------*/
uint16_t fifo_used(fifo_t* fifo)
{
	uint16_t used;
	
	if(fifo->head >= fifo->tail) used = fifo->head - fifo->tail;
	else used = fifo->head + (fifo->length - fifo->tail);
	
	return used;	
}

/* --------------------------------------------------------------------------*/
/**
 * @brief ��ʼFIFO���ݺ���
 *
 * @param fifo Ҫ��ʼ����FIFOָ��
 * @param buf Ҫָ����FIFOָ��������ַ
 * @param length FIFO����ĳ���
 */
/* ----------------------------------------------------------------------------*/
void fifo_init(fifo_t* fifo, uint8_t* buf, uint16_t length)  
{
	uint16_t i;
	
	fifo->buf = buf;
	fifo->length = length;
	fifo->head = 0;
	fifo->tail = 0;
	
	for(i=0; i<length; i++) fifo->buf[i] = 0;	
}

