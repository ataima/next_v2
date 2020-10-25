
#ifndef MERLINO_VCPU_HEADER
#define MERLINO_VCPU_HEADER

typedef unsigned int reg, *p_reg;
typedef unsigned short wreg, *p_wreg;
typedef unsigned char breg, *p_breg;


union tag_mem_base {
	reg *memBASE; // Start off CPU REGISTER  ( as user DATA MEM INT)
	wreg *wmemBASE; // Start off CPU REGISTER  ( as user DATA MEM SHORT)
	breg *bmemBASE; // Start off CPU REGISTER  ( as user DATA MEM BYTE)
};

typedef union tag_mem_base bwl;

struct cpu_tag {
	void * h_timer; // thread timer at 50 ms
	bwl u;
	reg memSIZE; // TOTAL Size of REGISTER 
	wreg *programBASE; // Where is the current execution program
	reg programSIZE; // size of program as 0..n (check bound alwais)
	reg programMAXSIZE; // program area + konst area : konst area RD only
	reg *stackBASE; // CPU stack as 32bit uint
	reg stackSIZE; // Stack size : at START:CS=0 at END CS=0
				   // add link to dll system function
	reg *sysBASE; // function table of dynamic link system function void * for 64 bit not reg
	reg sysSIZE; // size of function table for dll func  
	reg syscurIndex; // current index of first free position for  dll func  
	reg *handleBASE; // function table of dynamic link system function  void * for 64 bit not reg
	reg handleSIZE; // size of function table for dll func  
	reg handlecurIndex; // current index of first free position for  dll func 
	reg signature;      // sign of specific cpu ex 31303030
};


typedef struct cpu_tag merlinoVCPU, *pMerlinoVCPU;

#endif
