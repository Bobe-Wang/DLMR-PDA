#ifndef __SD_H__
#define __SD_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include "includes.h"
 								    	 
// SD卡类型定义  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   

// SD卡指令表  	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ，SEND_IF_COND
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define CMD23   23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define CMD41   41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00

//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

//SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

#define SD_SECTOR_SIZE             512u

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 
 							   						 	 
#define	SD_CS      PAout(4) //SD卡片选引脚		

#define FDISK      0
#define SFD        1

#define SD_DRV     0
#define SD_FORMAT  FDISK

extern u8 SD_Type; //SD卡的类型
extern char SD_FileName[50][16];
extern unsigned long SD_FileSize[50];

//函数申明区 
void SD_SPI_SpeedLow(void);
void SD_SPI_SpeedHigh(void);
u8 SD_WaitReady(void);							//等待SD卡准备
u8 SD_GetResponse(u8 Response);					//获得相应
u8 SD_Init(void);							    //初始化
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);		//读块
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt);		//写块
u32 SD_GetSectorCount(void);   					//读扇区数
u8 SD_GetCID(u8 *cid_data);                     //读SD卡CID
u8 SD_GetCSD(u8 *csd_data);                     //读SD卡CSD
uint8_t SD_ReadWriteByte(uint8_t Data);
u8 SD_ReadBlock(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumberOfBlocks);
u8 SD_WriteBlock(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumberOfBlocks);
void Scan_Files(char *path);
void FatFs_Test(void);


#ifdef __cplusplus
}
#endif

#endif
