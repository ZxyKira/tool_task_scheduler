//*****************************************************************************
//*			File Name		:TaskScheduler.c
//*			Version			:V1.1
//*			Create Date	:2018-09-09
//*			Modufy Date	:2019-06-10
//*			Information :
//*****************************************************************************
#include "TaskScheduler.h"
#include "FIFO.h"

//*****************************************************************************
//*	Typedef Struct
//*****************************************************************************

//-------------------------------------
//	Struct TS_Handler_Struct_T
//------------------------------------- 
typedef struct{
	struct{
		FIFO_T high;
		FIFO_T normal;
		FIFO_T low;
	}Fifo;
	struct{
		TS_Callback Idle;
		TS_Callback Reserved;
	}Callback;
	struct{
		uint16_t reserved2 	:16;
		uint8_t	 start 			:8;
		uint8_t	 reserved 	:8;
	}info;
}TS_Handler_Entity_T;

#define TS_FIFO_T FIFO_T

//-------------------------------------
//	TS_Idle_Callback
//------------------------------------- 
static void TS_Idle_Callback(void) {
	
}

//*****************************************************************************
//*	Public Function 
//*****************************************************************************
static bool TS_Initialze(TS_Handler_T *pHandler, const TS_Config_T *config) {
	TS_Handler_Entity_T *entity = (TS_Handler_Entity_T*)pHandler;
	FIFO_Config_T fifo_cfg;
		
	if (config->idleCallback == 0x00000000)
		entity->Callback.Idle = TS_Idle_Callback;
	else
		entity->Callback.Idle = config->idleCallback;
	
  fifo_cfg.buffer = &config->PrtorityHigh.pEventBuffer[0];
	fifo_cfg.count = config->PrtorityHigh.bufferQuantity;
	fifo_cfg.itemSize = sizeof(TS_Event_T);
	FIFO_API.init(&entity->Fifo.high, &fifo_cfg);
	
  fifo_cfg.buffer = &config->PrtorityNormal.pEventBuffer[0];
	fifo_cfg.count = config->PrtorityNormal.bufferQuantity;
	fifo_cfg.itemSize = sizeof(TS_Event_T);
	FIFO_API.init(&entity->Fifo.normal, &fifo_cfg);
	
  fifo_cfg.buffer = &config->PrtorityLow.pEventBuffer[0];
	fifo_cfg.count = config->PrtorityLow.bufferQuantity;
	fifo_cfg.itemSize = sizeof(TS_Event_T);
	FIFO_API.init(&entity->Fifo.low, &fifo_cfg);

	entity->info.start = 0;
	return true;
}

static bool TS_Start(TS_Handler_T *pHandler) {
	TS_Handler_Entity_T *entity = (TS_Handler_Entity_T*)pHandler;
	TS_Event_T task = {0x00000000, 0x00000000};

	if (entity->info.start != 0)
		return false;

	entity->info.start = 1;

	while (entity->info.start != 0) {
		if (FIFO_API.pop(&entity->Fifo.high, &task)) 
			task.task(task.args);
		else if (FIFO_API.pop(&entity->Fifo.normal, &task))
			task.task(task.args);
		else if (FIFO_API.pop(&entity->Fifo.low, &task))
			task.task(task.args);
		else
			entity->Callback.Idle();
	}
	return true;
}

static bool TS_Stop(TS_Handler_T *pHandler) {
	TS_Handler_Entity_T *entity = (TS_Handler_Entity_T*)pHandler;
	if (entity->info.start == 0)
		return false;

	entity->info.start = 0;
	return true;
}


static bool TS_AddTask(TS_Handler_T *pHandler, TS_Event_T task, TS_Task_Prtority prtority) {
	TS_Handler_Entity_T *entity = (TS_Handler_Entity_T*)pHandler;

	switch (prtority) {
		case TS_PrtorityLow:
			return FIFO_API.insert(&entity->Fifo.low, &task);
		case TS_PrtorityNormal:
			return FIFO_API.insert(&entity->Fifo.normal, &task);
		case TS_PrtorityHigh:
			return FIFO_API.insert(&entity->Fifo.high, &task);
		default:
			return false;
	}
}

static bool TS_AddTaskSuper(TS_Handler_T *pHandler, TS_Event_T task, TS_Task_Prtority prtority) {
	TS_Handler_Entity_T *entity = (TS_Handler_Entity_T*)pHandler;

	switch (prtority) {
		case TS_PrtorityLow:
			return FIFO_API.insertTail(&entity->Fifo.low, &task);
		case TS_PrtorityNormal:
			return FIFO_API.insertTail(&entity->Fifo.normal, &task);
		case TS_PrtorityHigh:
			return FIFO_API.insertTail(&entity->Fifo.high, &task);
		default:
			return false;
	}
}



const TS_API_T TS_API = {
	.initialze = TS_Initialze,
	.start = TS_Start,
	.stop = TS_Stop,
	.addTask = TS_AddTask,
	.addTaskSuper = TS_AddTaskSuper
};

//*****************************************************************************
//*	End of file
//*****************************************************************************

