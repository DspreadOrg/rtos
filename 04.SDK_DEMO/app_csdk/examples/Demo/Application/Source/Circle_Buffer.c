#include <string.h>
#include "Circle_Buffer.h"
#include "Common.h"
#include "Drv.h"

#define TRACE_CBUFF(fmt, ...)   TRACE("Line: %d, Func: %s," fmt "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__)

#if defined(MQTT_SUPPORT)

struct _CBuff_t
{
    unsigned char     *buff;
    int         size;
    int         front;
    int         tail;

};


//brief:Creat cbuff
CBuff_t *CBuff_New(int size)
{
    CBuff_t *pcb = NULL;
    unsigned char *pbuff = NULL;

    pcb = malloc(sizeof(CBuff_t));
    if (pcb == NULL)
    {
        TRACE_CBUFF("malloc failed");
        return NULL;
    }

    pbuff = malloc(size);
    if (pbuff == NULL)
    {
        TRACE_CBUFF("malloc failed");
        free(pcb);
        return NULL;
    }
    memset(pbuff, 0, size);

    pcb->buff = pbuff;
    pcb->size = size;
    pcb->front = 0;
    pcb->tail = 0;
    return pcb;

}


//brief: Free memory
void CBuff_Delete(CBuff_t *pcb)
{
    if (pcb == NULL)
    {
        return;
    }

    if (pcb->buff != NULL)
    {
        free(pcb->buff);
    }

    free(pcb);
    return;

}


//brief: Clear cbuff
void CBuff_Clean(CBuff_t *pcb)
{
    if (pcb == NULL)
    {
        TRACE_CBUFF("CBuff is null");
        return;
    }

    pcb->front = 0;
    pcb->tail = 0;
    memset(pcb->buff, 0, pcb->size);
    return;

}


//brief: Determines whether the cbuff is full
int CBuff_IsFull(CBuff_t *pcb)
{
    if (pcb == NULL)
    {
        TRACE_CBUFF("CBuff is null");
        return -1;
    }

    return (((pcb->tail + 1) % pcb->size) == pcb->front) ? 0 : 1;

}


//brief: Determines whether the cbuff is empty
int CBuff_IsEmpty(CBuff_t *pcb)
{
    if (pcb == NULL)
    {
        TRACE_CBUFF("CBuff is null");
        return -1;
    }

    return (((pcb->front + 1) % pcb->size) == pcb->tail) ? 0 : 1;

}


//brief: Return the number of bytes in cbuff
int CBuff_Length(CBuff_t *pcb)
{
    if (pcb == NULL)
    {
        TRACE_CBUFF("CBuff is null");
        return -1;
    }

    return ((pcb->tail + pcb->size - pcb->front) % pcb->size);

}

//brief: Returns the remaining capacity
int CBuff_Capacity(CBuff_t *pcb)
{
    if (pcb == NULL)
    {
        TRACE_CBUFF("CBuff is null");
        return -1;
    }

    return pcb->size - CBuff_Length(pcb) - 1;
}


//brief: cbuff tail add element
int CBuff_Write(CBuff_t *pcb, unsigned char *data, int len)
{
    int length;
    if (pcb == NULL)
    {
        TRACE_CBUFF("CBuff is null");
        return -1;
    }
    if (CBuff_IsFull(pcb) == 0){
        TRACE_CBUFF("cbuff is full");
        return -2;
    }

    length = CBuff_Capacity(pcb);
    if (len > length){
        len = length;
    }

    if (pcb->size - pcb->tail >= len)
    {
        memcpy(&(pcb->buff[pcb->tail]), data, len);
        pcb->tail += len;
    }
    else
    {
        memcpy(&(pcb->buff[pcb->tail]), data, pcb->size - pcb->tail);
        memcpy(pcb->buff, &data[pcb->size - pcb->tail], len - (pcb->size - pcb->tail));
        pcb->tail = len - (pcb->size - pcb->tail);
    }
    pcb->tail %= pcb->size;

    return len;

}


//breif: Read elements from the cbuff header
int CBuff_Read(CBuff_t *cq, unsigned char *pbuff, int len)
{
    int length = CBuff_Length(cq);

    if (cq == NULL)
    {
        TRACE_CBUFF("CBuff is null");
        return -1;
    }

    if (length <= 0)
    {
        return -2;
    }

    if (len > length)
    {
        len = length;
    }

    if (cq->size - cq->front >= len)
    {
        memcpy(pbuff, &(cq->buff[cq->front]), len);
        cq->front += len;
    }
    else
    {
        memcpy(pbuff, &(cq->buff[cq->front]), cq->size - cq->front);
        memcpy(&pbuff[cq->size - cq->front], cq->buff, len - (cq->size - cq->front));
        cq->front = len - (cq->size - cq->front);
    }
    cq->front %= cq->size;

    return len;

}


//brief: View elements in cbuff that are offset from the header
unsigned char CBuff_Pick(CBuff_t *pcb, int offset)
{
    if (pcb == NULL)
    {
        TRACE_CBUFF("CBuff is null");
        return 0;
    }

    return pcb->buff[(pcb->front + offset) % pcb->size];

}


//brief: Move cbuff head to point
void CBuff_Move(CBuff_t *pcb, int index)
{
    if (pcb == NULL)
    {
        TRACE_CBUFF("CBuff is null");
        return;
    }

    pcb->front = (pcb->front + index) % pcb->size;

}
#endif

