/*
 * can_proc.c
 *
 *  Created on: 2018. 10. 11.
 *      Author: swpar
 */

#include "can_proc.h"

#define	CAN_RING_BUF_SIZE		20

u32 _u32CAN1RXRingBufIndex=0;
u32 _u32CAN1RXRingBufCount=0;
u32 _u32CAN1ReadIndex=0;

CanRxMsg _pU8CAN1RXRingBuf[CAN_RING_BUF_SIZE];

u32 _u32CAN2RXRingBufIndex=0;
u32 _u32CAN2RXRingBufCount=0;
u32 _u32CAN2ReadIndex=0;
CanRxMsg _pU8CAN2RXRingBuf[CAN_RING_BUF_SIZE];


extern void ParseCAN1(CanRxMsg canData);
extern void ParseCAN2(CanRxMsg canData);
//
//void CAN1_IRQHandler_Loop(void)
//{
//	u32 u32ReadIndex;
//	CanRxMsg canData;
//
//	if(_u32CAN1RXRingBufCount>0)
//	{
//		// ring buf
//		_u32CAN1ReadIndex = _u32CAN1ReadIndex%CAN_RING_BUF_SIZE;
//		u32ReadIndex=_u32CAN1ReadIndex ;
//		canData=_pU8CAN1RXRingBuf[u32ReadIndex];
//		_u32CAN1RXRingBufCount--;
//		_u32CAN1ReadIndex++;
//		// A/D같음 ???
//		ParseCAN1(canData);
//	}
//}
//
void CAN2_IRQHandler_Loop(void)
{
	u32 u32ReadIndex;
	CanRxMsg canData;

	if(_u32CAN2RXRingBufCount>0)
	{
		// ring buf
		_u32CAN2ReadIndex = _u32CAN2ReadIndex%CAN_RING_BUF_SIZE;
		u32ReadIndex=_u32CAN2ReadIndex ;
		canData=_pU8CAN2RXRingBuf[u32ReadIndex];
		_u32CAN2RXRingBufCount--;
		_u32CAN2ReadIndex++;
		//main<->통신보드
		ParseCAN2(canData);
	}
}
//
//void CAN1_RX0_IRQHandler(void)
//{
//	CanRxMsg rxMessage;
//	CAN_Receive(CAN1,CAN_FIFO0, &rxMessage);
//
//	_u32CAN1RXRingBufIndex = _u32CAN1RXRingBufIndex%CAN_RING_BUF_SIZE;
//	_pU8CAN1RXRingBuf[_u32CAN1RXRingBufIndex] = rxMessage;
//
//	_u32CAN1RXRingBufIndex++;
//	_u32CAN1RXRingBufCount++;
//}
//
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg rxMessage;
	CAN_Receive(CAN2,CAN_FIFO0, &rxMessage);

	_u32CAN2RXRingBufIndex = _u32CAN2RXRingBufIndex%CAN_RING_BUF_SIZE;
	_pU8CAN2RXRingBuf[_u32CAN2RXRingBufIndex] = rxMessage;

	_u32CAN2RXRingBufIndex++;
	_u32CAN2RXRingBufCount++;
}

//	.word CAN1_RX0_IRQHandler
// 	.word	CAN1_RX1_IRQHandler

//// CAN1_RX1_IRQn
//void CAN1_RX0_IRQHandler(void)
//{
//	CanRxMsg rxMessage;
//	CAN_Receive(CAN1,CAN_FIFO0, &rxMessage);
//	//ParseCAN1(rxMessage);
//}
//
//void CAN2_RX0_IRQHandler(void)
//{
//  CanRxMsg rxMessage2;
//  CAN_Receive(CAN2, CAN_FIFO1, &rxMessage2);
// // g_u8CanFlag2 = 1;
//  ParseCAN2(rxMessage2);
//}

void SendCAN1(u32 u32ID,u8 u8Std,u8 u8DLC, u8* pData)
{
  CanTxMsg TxMsg;

  u8 TxMailBox=0;
  int i = 0;

  if(u8Std == CAN_ID_STD)
  {
	  TxMsg.StdId=u32ID;
	  TxMsg.IDE=u8Std;
  }
  else if(u8Std == CAN_ID_EXT)
  {
	  TxMsg.ExtId=u32ID;
	  TxMsg.IDE=u8Std;
  }
  else
  {

  }

  if(u8DLC==0)
  {
	TxMsg.RTR=CAN_RTR_REMOTE;
  }
  else
  {
	TxMsg.RTR=CAN_RTR_DATA;

	for(i=0;i<u8DLC;i++)
	{
	  TxMsg.Data[i]=pData[i];
	}
  }

  TxMsg.DLC=u8DLC;
//  TxMsg.IDE=u8Std;
  TxMailBox=CAN_Transmit(CAN1,&TxMsg);

  while(CAN_TransmitStatus(CAN1,TxMailBox)!=CANTXOK && i!=0xff)
  {
	i++;
  }
}
// 추가 수정 되엇다.
//메인과 통신보드 사용
void SendCAN2(u32 u32ID,u8 u8Std,u8 u8DLC, u8* pData)
{
  CanTxMsg TxMsg;

  u8 TxMailBox=0;
  int i = 0;

  if(u8Std == CAN_ID_STD)
  {
	  TxMsg.StdId=u32ID;
	  TxMsg.IDE=u8Std;
  }
  else if(u8Std == CAN_ID_EXT)
  {
	  TxMsg.ExtId=u32ID;
	  TxMsg.IDE=u8Std;
  }
  else
  {

  }

  if(u8DLC==0)
  {
	TxMsg.RTR=CAN_RTR_REMOTE;
  }
  else
  {
	TxMsg.RTR=CAN_RTR_DATA;

	for(i=0;i<u8DLC;i++)
	{
	  TxMsg.Data[i]=pData[i];
	}
  }

  TxMsg.DLC=u8DLC;
  TxMailBox=CAN_Transmit(CAN2,&TxMsg);

  while(CAN_TransmitStatus(CAN2,TxMailBox)!=CANTXOK && i!=0xff)
  {
	i++;
  }
}

