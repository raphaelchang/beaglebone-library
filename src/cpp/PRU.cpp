#include "PRU.h"
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

void PRU::Init()
{
	if (pruNum != 0 && pruNum != 1)
		return;
		
	unsigned int ret;
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

	/* Initialize the PRU */
	prussdrv_init ();		

	/* Open PRU Interrupt */
	ret = prussdrv_open(PRU_EVTOUT_0);
	if (ret)
	{
		printf("prussdrv_open open failed\n");
		return;
	}
	ret = prussdrv_open(PRU_EVTOUT_1);
	if (ret)
	{
		printf("prussdrv_open open failed\n");
		return;
	}

	/* Get the interrupt initialized */
	prussdrv_pruintc_init(&pruss_intc_initdata);

	//Initialize pointer to PRU data memory
	void* pruMem;
	prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, &pruMem);
	pruDataMem[0] = (unsigned int*) pruMem;
	prussdrv_map_prumem (PRUSS0_PRU1_DATARAM, &pruMem);
	pruDataMem[1] = (unsigned int*) pruMem;

	// Flush the values in the PRU data memory locations
	pruDataMem[0][0] = 0x00;
	pruDataMem[1][0] = 0x00;
}

void PRU::Execute(const char* file, int pruNum)
{
    prussdrv_exec_program (pruNum, file);
}

void PRU::Exit()
{
    /* Wait until PRU0 has finished execution */
    prussdrv_pru_wait_event(PRU_EVTOUT_0);
    prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
    prussdrv_pru_wait_event(PRU_EVTOUT_1);
    prussdrv_pru_clear_event(PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);

    /* Disable PRU and close memory mapping */
    prussdrv_pru_disable(0);
    prussdrv_pru_disable(1);
    prussdrv_exit();
}

void PRU::Write(int index, unsigned int value, int pruNum)
{
	pruDataMem[pruNum][index] = value;
}

unsigned int PRU::Read(int index, int pruNum)
{
	return pruDataMem[pruNum][index];
}