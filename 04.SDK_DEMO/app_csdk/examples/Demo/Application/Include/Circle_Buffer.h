#ifndef _CIRCLE_BUFFER_H
#define _CIRCLE_BUFFER_H

#include <stdint.h>


typedef struct _CBuff_t     CBuff_t;


//brief: Creat cbuff
CBuff_t *CBuff_New(int size);
//brief: Free memory
void CBuff_Delete(CBuff_t *pcb);

//brief: Clear cbuff
void CBuff_Clean(CBuff_t *pcb);
//brief: Determines whether the cbuff is full
int CBuff_IsFull(CBuff_t *pcb);
//brief: Determines whether the cbuff is empty
int CBuff_IsEmpty(CBuff_t *pcb);
//brief: Return the number of bytes in cbuff
int CBuff_Length(CBuff_t *pcb);
//brief: Returns the remaining capacity
int CBuff_Capacity(CBuff_t *pcb);
//brief: cbuff tail add element
int CBuff_Write(CBuff_t *pcb, unsigned char *data, int len);
//breif: Read elements from the cbuff header
int CBuff_Read(CBuff_t *cq, unsigned char *pbuff, int len);
//brief: View elements in cbuff that are offset from the header
unsigned char CBuff_Pick(CBuff_t *pcb, int offset);
//brief: Move cbuff head to point
void CBuff_Move(CBuff_t *pcb, int index);


#endif

