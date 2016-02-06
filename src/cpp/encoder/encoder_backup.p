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

#define ENC_A 16
#define ENC_B 5

.origin 0
.entrypoint MEMACCESSPRUDATARAM

.struct Counter
	.u32 count
	.u8 pinA
	.u8 pinB
	.u8 mode
	.u8 stalled
	.u32 lastEdgeTime
	.u32 maxPeriod
.ends

MEMACCESSPRUDATARAM:
	MOV       r0, 0x00000000
	MOV       r1, CTBIR_0
	SBBO      r0, r1, #0x08, 4
	
INIT_IEP:
	LBCO    r7, CONST_PRUCFG, 4, 4
    CLR     r7, r7, 4
    SBCO    r7, CONST_PRUCFG, 4, 4
	
	LBCO r7, CONST_PRUCFG, 0x10, 4
	SET r7, 17
	SBCO r7, CONST_PRUCFG, 0x10, 4
	// Enable CMP0
	MOV r7, (1 << 1)// | (1 << 0)
	SBCO r7, CONST_IEP, 0x40, 4
	MOV r7, 0
	SBCO r7, CONST_IEP, 0x08, 4
	MOV r7, 1
	SBCO r7, CONST_IEP, 0x0c, 4
	// Enable counter, configured to count nS (increments by 5 each clock)
	MOV r7, 0x00000550
    SBCO r7, CONST_IEP, 0x00, 4
	// Timer comparison value. 1000 is 1 uS.
	MOV r7, 1000
	SBCO r7, CONST_IEP, 0x48, 4
	
INIT_COUNTER:
	//Register map:
	//r0: Memory loc of shared memory (where position will be written)
	//r1: pin1 current
	//r2: pin1 prev
	//r3: pin2 current
	//r4: Is edge?
	//r5: Position
	//r6: pin1 inverted
	//r8: current timer
	//r9: last timer
	//r31: GPIO input register (readonly)
	//r29: GPIO input register last state

    //Load address of PRU data memory in r0
	MOV r0, 0x0004
    
    //Initialize the "previous" value for pinA of encoder (edge pin)
	MOV r2, 0
    
    //Initialize position
	MOV r5, 0
	SBBO r5, r0, #0x00, 4
	
	MOV r29, r31
	
	LBCO r7, CONST_IEP, 0x00, 4
	SET r7, 0
	SBCO r7, CONST_IEP, 0x00, 4
	
	LBCO r8, CONST_IEP, 0x0c, 4
	SBBO r8, r9, #0x00, 4
	SBBO r5, r0, #0x04, 4
	
READPINS:
	//LBCO r8, CONST_IEP, 0x44, 4
	//QBBC READPINS, r8, 0
	//SET r8, 0
	//SBCO r8, CONST_IEP, 0x44, 4
	
	LSR r2, r29, ENC_A
	//Store pin1 current value
	LSR r1, r31, ENC_A
	AND r1, r1, 1

	//Store pin2 current value
	LSR r3, r31, ENC_B
	AND r3, r3, 1

	//Invert pin1 value and store in r6 to test logic later on...
	NOT r6, r2
	AND r6, r6, 1

	//Update previous value of PIN1
	CLR r29, ENC_A
	LSL r2, r1, ENC_A
	OR r29, r2, r29

	//Jump to edge detection steps if edge detected
	QBEQ EDGEDETECTED, r6, r1

	LBCO r8, CONST_IEP, 0x0c, 4
	SUB r10, r8, r9
	LBBO r11, r0, #0x08, 4
	QBGT READPINS, r10, r11
	MOV r10, 0
	SBBO r10, r0, #0x04, 4
	
	//Loop forever
	QBA READPINS
	
EDGEDETECTED:
	LBCO r8, CONST_IEP, 0x0c, 4
	SUB r10, r8, r9
	
	SBBO r10, r0, #0x04, 4
	MOV r9, r8
	
	QBNE CW, r3, r1
	SUB r5, r5, 1
	// Move value from register to the PRU local data memory using registers
	SBBO r5, r0, #0x00, 4
	QBA READPINS

CW:
	ADD r5, r5, 1
	// Move value from register to the PRU local data memory using registers
	SBBO r5, r0, #0x00, 4
	QBA READPINS
	
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
    HALT