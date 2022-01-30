/*------------------------------------------------ 
Nano Debugger Data inerface between NDE and NDI
-------------------------------------------------*/
#pragma once

//#include <stdint.h>

//#include "dlist.h"

#pragma pack(1)
typedef struct _tag_dbgrdata_head64_t{
    struct  list_head list;
    unsigned int tag;
    unsigned int size;
}dbgrdata_head64_t,*pdbgrdata_head64_t;
#pragma pack()

#define DBGR_DATA_SIZE 0x340
#define NANO_DBGR_DATA_TAG 'NANO' 

#define ND_DBGRDATA_VERSION 1

typedef struct _tag_dbgrdata64_t
{
   /*0x000*/ dbgrdata_head64_t head;// _DBGKD_DEBUG_DATA_HEADER64
   /*0x018*/ uint16_t Revision; // version of this struct
             uint16_t Reserved;
             uint32_t ReservedMore;
   /*0x020*/ uint64_t KernelBase;// Uint8B
   /*0x028*/ uint64_t KaslrOffset;// offset by the Kernel Address Space Layout Randomization 
   /*0x030*/ uint64_t BuildLabString;// linux_banner for Linux 
   char reserved[DBGR_DATA_SIZE-0x30];
   // 0x340
}dbgrdata64_t,*pdbgrdata64_t;