#include <stdio.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

static void *pruDataMem;
static unsigned int *pruDataMem_int;

int main (void)
{
	system("echo pruin > /sys/devices/ocp.3/P9_27_pinmux.*/state");
	system("echo pruin > /sys/devices/ocp.3/P9_41_pinmux.*/state");

    unsigned int ret;
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
    
    printf("\nINFO: Starting %s example.\r\n", "encoder");
    /* Initialize the PRU */
    prussdrv_init ();		
    
    /* Open PRU Interrupt */
    ret = prussdrv_open(PRU_EVTOUT_0);
    if (ret)
    {
        printf("prussdrv_open open failed\n");
        return (ret);
    }
    
    /* Get the interrupt initialized */
    prussdrv_pruintc_init(&pruss_intc_initdata);
	
    //Initialize pointer to PRU data memory
    prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, &pruDataMem);
    pruDataMem_int = (unsigned int*) pruDataMem;
    
    // Flush the values in the PRU data memory locations
    pruDataMem_int[0] = 0x00;

    /* Execute example on PRU */
    prussdrv_exec_program (0, "./encoder.bin");
    
	union
	{
		unsigned int data;
		char values[4];
	} pinsCmdStatus;
	pruDataMem_int[2] = 0;
	pruDataMem_int[3] = 0;
	pinsCmdStatus.values[0] = 15;
	pinsCmdStatus.values[1] = 14;
	pinsCmdStatus.values[2] = 0x0F;
	pinsCmdStatus.values[3] = 0x01;
	pruDataMem_int[4] = pinsCmdStatus.data;
	pruDataMem_int[6] = 500000000;
	pruDataMem_int[0]++;
	pruDataMem_int[7] = 0;
	pruDataMem_int[8] = 0;
	pinsCmdStatus.values[0] = 16;
	pinsCmdStatus.values[1] = 5;
	pinsCmdStatus.values[2] = 0x1F;
	pinsCmdStatus.values[3] = 0x01;
	pruDataMem_int[9] = pinsCmdStatus.data;
	pruDataMem_int[11] = 500000000;
	pruDataMem_int[0]++;

	//Read out memory forever
    for(;;){
		printf("T: %u\n", pruDataMem_int[1]);
        printf("1: %d %f %d\n", pruDataMem_int[2], 1.0 / ((float)pruDataMem_int[3] / 1000000000.0), (pruDataMem_int[4] >> 24) & 0x01);
        printf("2: %d %f %d\n", pruDataMem_int[7], 1.0 / ((float)pruDataMem_int[8] / 1000000000.0), (pruDataMem_int[9] >> 24) & 0x01);
		usleep(10000);
	}
    
    /* Wait until PRU0 has finished execution */
    printf("\tINFO: Waiting for HALT command.\r\n");
    prussdrv_pru_wait_event (PRU_EVTOUT_0);
    printf("\tINFO: PRU completed transfer.\r\n");
    prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable (0);
    prussdrv_exit ();

    return(0);
}
