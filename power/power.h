#include <nrf.h>

#define RAM7BaseAddress	((uint32_t *)0x2000e000)
#define RAM6BaseAddress	((uint32_t *)0x2000c000)
#define RAM5BaseAddress	((uint32_t *)0x2000a000)
#define RAM4BaseAddress	((uint32_t *)0x20008000)
#define RAM3BaseAddress	((uint32_t *)0x20006000)
#define RAM2BaseAddress	((uint32_t *)0x20004000)
#define RAM1BaseAddress	((uint32_t *)0x20002000)
#define RAM0BaseAddress	((uint32_t *)0x20000000)

#define RAMBaseSize			0x2000UL

#define Power_FBMsk			0x1U

typedef enum{
	Reset_RESETPIN=0,		//Reset from pin-reset detected
	Reset_DOG,					//Reset from watchdog detected
	Reset_SREQ,					//Reset from soft reset detected
	Reset_LOCKUP,				//Reset from CPU lock-up detected
	Reset_OFF,					//Reset due to wake up from System OFF mode when wakeup is triggered from DETECT signal from GPIO
	Reset_LPCOMP,				//Reset due to wake up from System OFF mode when wakeup is triggered from ANADETECT signal from LPCOMP
	Reset_DIF,					//Wake up from System OFF (Debug interface mode)
	Reset_NFC,					//Reset due to wake up from System OFF mode by NFC field
	Reset_PowerON				//Normal PowerON
} ResetReason_e;

ResetReason_e CheckResetReason()
{
	uint32_t ResetReason;
	ResetReason_e out;
	ResetReason=NRF_POWER->RESETREAS;
	
	if(ResetReason>>POWER_RESETREAS_RESETPIN_Pos&Power_FBMsk)
		out= Reset_RESETPIN;
	else if(ResetReason>>POWER_RESETREAS_DOG_Pos&Power_FBMsk)
		out= Reset_DOG;
	else if(ResetReason>>POWER_RESETREAS_SREQ_Pos&Power_FBMsk)
		out= Reset_SREQ;
	else if(ResetReason>>POWER_RESETREAS_LOCKUP_Pos&Power_FBMsk)
		out= Reset_LOCKUP;
	else if(ResetReason>>POWER_RESETREAS_OFF_Pos&Power_FBMsk)
		out= Reset_OFF;
	else if(ResetReason>>POWER_RESETREAS_LPCOMP_Pos&Power_FBMsk)
		out= Reset_LPCOMP;
	else if(ResetReason>>POWER_RESETREAS_DIF_Pos&Power_FBMsk)
		out= Reset_DIF;
	else if(ResetReason>>POWER_RESETREAS_NFC_Pos&Power_FBMsk)
		out= Reset_NFC;
	else
		out= Reset_PowerON;
	NRF_POWER->RESETREAS=NRF_POWER->RESETREAS;
	return out;
}
void EnableDCDC()
{
	NRF_POWER->DCDCEN=POWER_DCDCEN_DCDCEN_Enabled;
}
void SystemOFF()
{
	NRF_POWER->SYSTEMOFF=POWER_SYSTEMOFF_SYSTEMOFF_Enter;
}
void Power_failure_comparator_configuration(bool Enable,uint8_t Threshold)
{
	NRF_POWER->POFCON=Threshold<<POWER_POFCON_THRESHOLD_Pos|Enable<<POWER_POFCON_POF_Pos;
}

void EnableRamRetention(uint8_t RamX)
{
	switch (RamX) //8KB * 8 = 64KB
	{
		case 7://0x2000E000-0x20010000
		NRF_POWER->RAM[7].POWERSET=
		POWER_RAM_POWER_S0RETENTION_On<<POWER_RAM_POWER_S0RETENTION_Pos|
		POWER_RAM_POWER_S1RETENTION_On<<POWER_RAM_POWER_S1RETENTION_Pos;
		break;
		case 6://0x2000C000-0x2000E000
		NRF_POWER->RAM[6].POWERSET=
		POWER_RAM_POWER_S0RETENTION_On<<POWER_RAM_POWER_S0RETENTION_Pos|
		POWER_RAM_POWER_S1RETENTION_On<<POWER_RAM_POWER_S1RETENTION_Pos;
		break;
		case 5://0x2000A000-0x2000C000
		NRF_POWER->RAM[5].POWERSET=
		POWER_RAM_POWER_S0RETENTION_On<<POWER_RAM_POWER_S0RETENTION_Pos|
		POWER_RAM_POWER_S1RETENTION_On<<POWER_RAM_POWER_S1RETENTION_Pos;
		break;
		case 4://0x20008000-0x2000A000
		NRF_POWER->RAM[4].POWERSET=
		POWER_RAM_POWER_S0RETENTION_On<<POWER_RAM_POWER_S0RETENTION_Pos|
		POWER_RAM_POWER_S1RETENTION_On<<POWER_RAM_POWER_S1RETENTION_Pos;
		break;
		case 3://0x20006000-0x20008000
		NRF_POWER->RAM[3].POWERSET=
		POWER_RAM_POWER_S0RETENTION_On<<POWER_RAM_POWER_S0RETENTION_Pos|
		POWER_RAM_POWER_S1RETENTION_On<<POWER_RAM_POWER_S1RETENTION_Pos;
		break;
		case 2://0x20004000-0x20006000
		NRF_POWER->RAM[2].POWERSET=
		POWER_RAM_POWER_S0RETENTION_On<<POWER_RAM_POWER_S0RETENTION_Pos|
		POWER_RAM_POWER_S1RETENTION_On<<POWER_RAM_POWER_S1RETENTION_Pos;
		break;
		case 1://0x20002000-0x20004000
		NRF_POWER->RAM[1].POWERSET=
		POWER_RAM_POWER_S0RETENTION_On<<POWER_RAM_POWER_S0RETENTION_Pos|
		POWER_RAM_POWER_S1RETENTION_On<<POWER_RAM_POWER_S1RETENTION_Pos;
		break;
		case 0://0x20000000-0x20002000
		NRF_POWER->RAM[0].POWERSET=
		POWER_RAM_POWER_S0RETENTION_On<<POWER_RAM_POWER_S0RETENTION_Pos|
		POWER_RAM_POWER_S1RETENTION_On<<POWER_RAM_POWER_S1RETENTION_Pos;	
		break;
	}
}
void EraseRAM(uint8_t RamX)
{
	uint32_t * p;
	uint32_t * Start;
	uint32_t * End;
	
	switch (RamX)
	{
		case 7:
			Start=(uint32_t *)0x2000E000;
			End=(uint32_t *)0x20010000;
		break;
		case 6:
			Start=(uint32_t *)0x2000C000;
			End=(uint32_t *)0x2000E000;
		break;
		case 5:
			Start=(uint32_t *)0x2000A000;
			End=(uint32_t *)0x2000C000;
		break;
		case 4:
			Start=(uint32_t *)0x20008000;
			End=(uint32_t *)0x2000A000;
		break;
		case 3:
			Start=(uint32_t *)0x20006000;
			End=(uint32_t *)0x20008000;
		break;
		case 2:
			Start=(uint32_t *)0x20004000;
			End=(uint32_t *)0x20006000;
		break;
		case 1:
			Start=(uint32_t *)0x20002000;
			End=(uint32_t *)0x20004000;
		break;
		case 0:
			Start=(uint32_t *)0x20000000;
			End=(uint32_t *)0x20002000;
		break;
	}
	
	p=Start;
	do{
		*p=0;
		p++;
	}while(p<End);
}
