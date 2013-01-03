.globl _start
_start:
	@set sp in IRAM
	ldr sp,=0xD0037000

	@1.close WDT
	ldr r0,=0XE2700000
	mov r1,#0
	str r1,[r0]

	@2.close FIQ,IRQ
	mrs r0,CPSR
	bic r0,r0,#0XC0
	msr CPSR,r0

	@3.init CLOCK
	bl clock_init

	@4.init MEMORY
	bl mem_ctrl_asm_init
	
	@set sp in DRAM
	ldr sp,=0x38000000

	@copy bootloader from nand to dram 0x20000010
	bl copy_myself

	ldr pc, =loader



	
