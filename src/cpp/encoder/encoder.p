#define PRU0_PRU1_INTERRUPT     17
#define PRU1_PRU0_INTERRUPT     18
#define PRU0_ARM_INTERRUPT      19
#define PRU1_ARM_INTERRUPT      20
#define ARM_PRU0_INTERRUPT      21
#define ARM_PRU1_INTERRUPT      22

#define CONST_PRUDRAM   C24
#define CONST_L3RAM     C30
#define CONST_DDR       C31
#define CONST_IEP 		C26
#define CONST_PRUCFG 	C4

// Address for the Constant table Programmable Pointer Register 0(CTPPR_0)
#define CTBIR_0         0x22020
// Address for the Constant table Programmable Pointer Register 0(CTPPR_0)
#define CTBIR_1         0x22024

// Address for the Constant table Programmable Pointer Register 0(CTPPR_0)
#define CTPPR_0         0x22028
// Address for the Constant table Programmable Pointer Register 1(CTPPR_1)
#define CTPPR_1         0x2202C

#define COUNTER_SIZE 20

#define ENABLED_BIT 0
#define RISING_BIT 1
#define FALLING_BIT 2
#define DIRECTION_BIT 3
#define BOTH_CHANNELS_BIT 4

#define STALLED_BIT 0

.struct counter
	.u32 count
	.u32 period
	.u8 pinA
	.u8 pinB
	.u8 cmd
	.u8 status
	.u32 lastEdgeTime
	.u32 maxPeriod
.ends

.assign counter, r3, r7, Current

.origin 0
.entrypoint MEMACCESSPRUDATARAM

MEMACCESSPRUDATARAM:
	MOV       r0, 0x00000000
	MOV       r1, CTBIR_0
	SBBO      r0, r1, #0x08, 4
	
INIT_IEP:
	LBCO    r2, CONST_PRUCFG, 4, 4
    CLR     r2, r2, 4
    SBCO    r2, CONST_PRUCFG, 4, 4
	
	LBCO r2, CONST_PRUCFG, 0x10, 4
	SET r2, 17
	SBCO r2, CONST_PRUCFG, 0x10, 4
	// Enable CMP0
	MOV r2, (1 << 1)// | (1 << 0)
	SBCO r2, CONST_IEP, 0x40, 4
	MOV r2, 0
	SBCO r2, CONST_IEP, 0x08, 4
	MOV r2, 1
	SBCO r2, CONST_IEP, 0x0c, 4
	// Enable counter, configured to count nS (increments by 5 each clock)
	MOV r2, 0x00000550
	SBCO r2, CONST_IEP, 0x00, 4
	// Timer comparison value. 1000 is 1 uS.
	MOV r2, 1000
	SBCO r2, CONST_IEP, 0x48, 4
	
INIT_REGISTER:
	//Register map:
	//r0: Memory loc of shared memory (where position will be written)
	//r1: Number of registered counters
	//r2: Last number of registered counters
	//r8: Last GPIO A value
	//r9: Current GPIO A value
	//r10: Last GPIO B value
	//r11: Current GPIO B value
	//r12: Current time
	//r13: Period
	//r29: GPIO input register last state
	//r31: GPIO input register (readonly)

    //Load address of PRU data memory in r0
	MOV r0, 0x0000
	
	// Initialize previous pins states
	MOV r29, r31
	
	// Start timer
	LBCO r2, CONST_IEP, 0x00, 4
	SET r2, 0
	SBCO r2, CONST_IEP, 0x00, 4
	
	// Initialize previous counter number
	MOV r2, 0
	
START_LOOP:
	MOV r0, 0x0000
	LBBO r1, r0, #0x00, 4
	//QBGT SETUP_COUNTER, r2, r1
	ADD r0, r0, 0x04
	LBCO r21, CONST_IEP, 0x0c, 4
	SUB r22, r21, r20
	MOV r20, r21
	SBBO r22, r0, #0x00, 4
	ADD r0, r0, 0x04
	
PROCESS_COUNTER:
	QBEQ START_LOOP, r1, 0x00
	
	LBBO Current, r0, #0x00, COUNTER_SIZE
	// Skip if not enabled
	QBBC NEXT_COUNTER, Current.cmd, ENABLED_BIT
	
	LSR r8, r29, Current.pinA
	AND r8, r8, 1
	LSR r10, r29, Current.pinB
	AND r10, r10, 1
	
	//Store pinA current value
	LSR r9, r31, Current.pinA
	AND r9, r9, 1

	//Store pinB current value
	LSR r11, r31, Current.pinB
	AND r11, r11, 1

	NOT r14, r8
	AND r14, r14, 1
	NOT r15, r10
	AND r15, r15, 1

	//Update previous value of pinA
	CLR r29, Current.pinA
	LSL r8, r9, Current.pinA
	OR r29, r8, r29
	//Update previous value of pinB
	CLR r29, Current.pinB
	LSL r10, r11, Current.pinB
	OR r29, r10, r29

A_EDGE:
	//Jump to edge detection steps if edge detected in pinA
	MOV r16, r9
	MOV r17, r11
	QBEQ EDGEDETECTED, r14, r9
	QBBC NO_EDGE, Current.cmd, BOTH_CHANNELS_BIT
	
B_EDGE:
	//Jump to edge detection steps if edge detected in pinB
	NOT r17, r16
	AND r17, r17, 1
	MOV r16, r11
	QBEQ EDGEDETECTED, r15, r11

NO_EDGE:
	LBCO r12, CONST_IEP, 0x0c, 4
	SUB r13, r12, Current.lastEdgeTime
	QBGT NEXT_COUNTER, r13, Current.maxPeriod
	MOV r13, 0
	SBBO r13, r0, #0x04, 4
	SET Current.status, 0
	SBBO Current.status, r0, #0x0b, 1
	
	QBA NEXT_COUNTER
	
EDGEDETECTED:
	QBEQ FALLING, r16, 0
RISING:
	QBBC NO_EDGE, Current.cmd, RISING_BIT
	QBA PERIOD_CALC
	
FALLING:
	QBBC NO_EDGE, Current.cmd, FALLING_BIT
	
PERIOD_CALC:
	// Load current time
	LBCO r12, CONST_IEP, 0x0c, 4
	// Store current time into last edge time from shared memory
	SBBO r12, r0, #0x0c, 4
	
	QBBS STALLED, Current.status, STALLED_BIT
	
	// Calculate and store period
	SUB r13, r12, Current.lastEdgeTime
	SBBO r13, r0, #0x04, 4
	
	QBA CCW
	
STALLED:
	MOV r13, 0
	SBBO r13, r0, #0x04, 4
	CLR Current.status, 0
	SBBO Current.status, r0, #0x0b, 1
	
CCW:
	QBBC CW, Current.cmd, DIRECTION_BIT
	QBNE CW, r17, r16
	SUB Current.count, Current.count, 1
	// Move value from register to the PRU local data memory using registers
	SBBO Current.count, r0, #0x00, 4
	QBA NEXT_COUNTER

CW:
	ADD Current.count, Current.count, 1
	// Move value from register to the PRU local data memory using registers
	SBBO Current.count, r0, #0x00, 4
	QBA NEXT_COUNTER
	
NEXT_COUNTER:
	ADD r0, r0, COUNTER_SIZE
	SUB r1, r1, 1
	QBA PROCESS_COUNTER
	
SETUP_COUNTER:
	// Setup here
	ADD r2, r2, 1
	QBGT SETUP_COUNTER, r2, r1
	QBA START_LOOP
	
END:
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
    HALT
	