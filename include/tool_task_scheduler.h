//*****************************************************************************
//*			File Name		:TaskScheduler.h
//*			Version			:V1.1
//*			Create Date	:2018-09-09
//*			Modufy Date	:2019-06-10
//*			Information :
//*****************************************************************************
#ifndef H_TASK_SCHEDULER
#define H_TASK_SCHEDULER
#include "stdint.h"
#include "stdbool.h" 
#include "FIFO.h"

#ifdef __cplusplus
extern "C" {
#endif



//-------------------------------------
//	Typedef 
//------------------------------------- 
typedef void (*TS_Callback)(void);
typedef void (*TS_Task)(void* args);

//-------------------------------------
//	Struct TS_Event_T 
//------------------------------------- 
typedef struct{
	TS_Task task;
	void*	args;
}TS_Event_T;

//-------------------------------------
//	Struct TS_Handler_T
//------------------------------------- 
typedef struct{
	uint32_t mem[0x10];
}TS_Handler_T;

//-------------------------------------
//	Struct TS_Handler_T
//------------------------------------- 
typedef enum{
	TS_PrtorityLow,
	TS_PrtorityNormal,
	TS_PrtorityHigh
}TS_Task_Prtority;

//-------------------------------------
//	Struct TS_Config_T
//------------------------------------- 
typedef struct{
	struct{
		TS_Event_T *pEventBuffer;
		uint16_t bufferQuantity;
	}PrtorityLow;
	struct{
		TS_Event_T *pEventBuffer;
		uint16_t bufferQuantity;
	}PrtorityNormal;
	struct{
		TS_Event_T *pEventBuffer;
		uint16_t bufferQuantity;
	}PrtorityHigh;
	TS_Callback idleCallback;

}TS_Config_T;

//-------------------------------------
//	Struct TS_API_T
//------------------------------------- 
typedef struct {
	bool(*initialze)(TS_Handler_T *pHandler, const TS_Config_T *config);
	bool(*start)(TS_Handler_T *pHandler);
	bool(*stop)(TS_Handler_T *pHandler);
	bool(*addTask)(TS_Handler_T *pHandler, TS_Event_T task, TS_Task_Prtority prtority);
	bool(*addTaskSuper)(TS_Handler_T *pHandler, TS_Event_T task, TS_Task_Prtority prtority);	
}TS_API_T;

extern const TS_API_T TS_API;

#ifdef __cplusplus
}
#endif
#endif
//*****************************************************************************
//*	End of file
//*****************************************************************************

