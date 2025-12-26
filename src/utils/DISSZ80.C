/*
	Z80 Disassembler Module v1.31, Copyright 1996-1998 Mark Incley.

    * Now requires an external function "dz80ProgressUpdate" which is
      called regularly during the disassembly with two DWORD parameters,
      the first one is the number of bytes actually processed, the second is
      total number of bytes to be processed.

      This allows the Windows version to "PeekMessage" and keep dZ80
      responsive, and draw a progress bar. Other platforms may simply
      return, or display a % done indicator. The return type is void.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <sys/types.h>
#include "dissz80.h"

#define	AddToDis(str)	strcat(DisBuf, str);

const char	*VersionString 	= "v1.31";		/* Disassembler version */
const char	*IMModes[4]		= {"0",	"0", "1", "2"};

static	char	HexDisBuf[32];		/* Holds the hex. disassembly (easy one!) */
static	char	DisBuf[128];		/* Holds the disassembly */
static	WORD	PC,	LastPC,	LastRefAddr, NumInstructions;
static	WORD	DisFlags;
static 	int		op,	realop, LineCmd;
static	BYTE	*Z80MemBase;
static	signed char	IXIYDisp;			/* The IXIY displacement (-128 to 127) */
static	int		Z80Flags, Pass,	NumRecursions, SkippedLastOpcode;
static	char	Buffer[128], CommentBuffer[128];
static	DWORD	BytesToProcess,	BytesProcessed;
static	int		DissingToScreen, HaveTabbed;
static	WORD	DisStart, DisEnd;
static	BYTE	z80pageloaded = 0;
static	BYTE	z80pagenum = 0;
static	BYTE	z80page[256];
  
#define DDEVIC		0x300
#define DUNIT		0x301
#define DCOMND		0x302
#define DSTATS		0x303
#define DBUFLO		0x304
#define DBUFHI		0x305
#define DTIMLO		0x306
#define DBYTLO		0x308
#define DBYTHI		0x309
#define DAUX1		0x30A
#define DAUX2		0x30B
#define FlagFn(x)	(x)

char 	*Conditions[8]=
{
	"nz",
	"z",
	"nc",
	"c",
	"po",
	"pe",
	"p",
	"m"
};

char	*AccRotType[8]=
{
	"rlca",
	"rrca",
	"rla",
	"rra",
	"daa",
	"cpl",
	"scf",
	"ccf"
};

char	*CBRotType[8] =
{
	"rlc",
	"rrc",
	"rl",
	"rr",
	"sla",
	"sra",
	"sll",
	"srl"
};

char	*Reg8Idx[8] =
{
	"b",
	"c",
	"d",
	"e",
	"h",
	"l",
	"(hl)",
	"a"	   
};

char	*Reg8AFIdx[8] =
{
	"b",
	"c",
	"d",
	"e",
	"h",
	"l",
	"f",
	"a"	   
};


char	*Reg16Idx[5] =
{
	"bc",
	"de",
	"hl",
	"sp",
	"af"
};

char	*BasicOps[8] =
{
	"add",
	"adc",
	"sub",
	"sbc",
	"and",
	"xor",
	"or",
	"cp"
};

char	*RepeatOps[16] =
{
	"ldi",
	"cpi",
	"ini",
	"outi",

	"ldd",
	"cpd",
	"ind",
	"outd",

	"ldir",
	"cpir",
	"inir",
	"otir",

	"lddr",
	"cpdr",
	"indr",
	"otdr"
};


char *Z180RepeatOps[4] =
{
	"otim",
    "otdm",
    "otimr",
    "otdmr"
};

void callSIO(BYTE device, BYTE unit, BYTE cmd, BYTE status, WORD buffer, WORD bytes, BYTE aux1, BYTE aux2) {

	*(BYTE *)DDEVIC = device;

	*(BYTE *)DUNIT	= unit;
	*(BYTE *)DCOMND	= cmd;
	*(BYTE *)DSTATS	= status;
	*(WORD *)DBUFLO = buffer;
	//*(char *)DTIMLO;
	*(WORD *)DBYTLO	= bytes;
	*(BYTE *)DAUX1 = aux1;
	*(BYTE *)DAUX2 = aux2;

	asm ("jsr $E459");
}

void getZ80page(BYTE page) {
	callSIO('Z', 1, 'S', 0, 0, 0, 0, page);
	if (*(BYTE *)DSTATS > 127) 
		printf("Z S rc: %02X\n", *(BYTE *)DSTATS);
 
	callSIO('Z', 1, 'R', 0x40, (WORD)z80page, 256, 0, 0);
	if (*(BYTE *)DSTATS > 127) 
		printf("Z R rc: %02X\n", *(BYTE *)DSTATS);
}

BYTE getZ80Mem(WORD ptr) {
	BYTE page = ptr >> 8;

	if (!z80pageloaded || z80pagenum != page) {
		getZ80page(page);
		z80pageloaded = TRUE;
		z80pagenum = page;
	}

	return z80page[ptr & 0xff];
}

int	DisassembleZ80(DISZ80 *Dis)
{
char	DisLine[128];
int		NumPasses;

	Dis->CreatedREFOK = FALSE;

	Z80MemBase = Dis->Mem0Start;
	DisFlags = Dis->Flags;
	NumPasses =	1;
	SkippedLastOpcode =	FALSE;	/* Used to blank lines */
	Dis->NumInstructions = 0;

    if ( (DisFlags & DISFLAG_CONSOLE) || (Dis->OutFileName == NULL) )
        DissingToScreen = TRUE;

/* If creating assembler output, switch off the opcode and address dump */

//	if (LabelledOutput == TRUE)
//		DisFlags &=	~(DISFLAG_ADDRDUMP | DISFLAG_OPCODEDUMP);

	DisStart = Dis->Start;
	DisEnd = Dis->End;

	if (DisEnd < DisStart)
		{
		LastPC = DisStart;
		DisStart = DisEnd;
		DisEnd = LastPC;
		}

	BytesToProcess = ((DWORD)DisEnd	- (DWORD)DisStart + 1)	* NumPasses;
	BytesProcessed = 0;

/* Ok, at last, here's the disassembly loop! */

	for	(Pass =	NumPasses; Pass	> 0; Pass--)
		{

        NumInstructions = 0;				/* Number of instructions disassembled */
    	PC = DisStart;
		LastPC = PC;
		while (PC <= DisEnd && NumInstructions < Dis->MaxReferences)
			{
			if (LastPC > PC)		/* Check for PC wraparound */
				break;

			LastPC = PC;

			HexDisBuf[0] = DisBuf[0] = CommentBuffer[0] = 0;
			Z80Flags = NumRecursions = 0;	/* Clear and IX/IY prefixes, etc. */
            HaveTabbed = FALSE;

            //DoProgress();

            LineCmd = 0;				/* Clear out line commands */
			DisassembleInstruction();
			NumInstructions++;			/* Another line disassembled */

/* If we're building up the FnMap, don't disassemble */

			if (Pass !=	1)
				continue;

/* Clear out the disline (where the disassembly is built up) */

			DisLine[0] = 0;

			if (SkippedLastOpcode)
				sprintf(DisLine, ";\n;\n");

			SkippedLastOpcode =	FALSE;

/* Add the instruction's address ? */

			if (DisFlags & DISFLAG_ADDRDUMP)
				{
				sprintf(Buffer,	"%04X:", LastPC);
				strcat(DisLine,	Buffer);
				}

/* Add the hex dump? */

			if (DisFlags & DISFLAG_OPCODEDUMP)
				{
				sprintf(Buffer,	"%-8s  ", HexDisBuf);
				strcat(DisLine,	Buffer);
				
				}

/* Add the disassembly. */

			if ((DisFlags &	(DISFLAG_OPCODEDUMP	| DISFLAG_ADDRDUMP)) ==	0)
					strcat(DisLine,	"        ");

			strcat(DisLine,	DisBuf);

/* Make uppercase if necessary */

			if (DisFlags & DISFLAG_UPPER)
				StringToUpper(DisLine);
/* Finally, add the new-line */

			strcat(DisLine,"\n");

/* Really finally, process any line commands */


			if ( DisFlags &	DISFLAG_CONSOLE )
				printf("%s", DisLine);

			}					/* Next instruction */

		}						/* Next pass */


	Dis->NumInstructions = NumInstructions;

	//DisZ80CleanUp();
	return TRUE;			/* Disassembled OK */
}

/* Release allocated memory, close open files, etc.. */


/* Time to disassemble an instruction */

void DisassembleInstruction(void)
{
	if (++NumRecursions	> 3)
		{
		AddToDisUnknown(NULL);
		return;
		}

	GetNextOpCode();	// Get initial opcode in op
	switch (op)
		{

		case 0xcb:		// Disassemble the Rotates, SETs and RES's
			Z80Flags |=	Z80CB;
			DisCB();
			return;

		case 0xed:
			Z80Flags |=	Z80ED;
			DisED();
			return;

		case 0xdd:
			Z80Flags |=	Z80IX;		/* Signal IX prefix */
			Z80Flags &=	~Z80IY;		/* Clear IY prefix */
			DisassembleInstruction();
			return;

		case 0xfd:
			Z80Flags |=	Z80IY;		/* Signal IY prefix */
			Z80Flags &=	~Z80IX;		/* Clear IX prefix */
			DisassembleInstruction();
			return;
		}

	realop = op;

	if (op < 0x40)
		{
		Dis00to3F();
		return;
		}

	if (op >= 0x40 && op <=	0x7f)
		{
		Dis40to7F();			// All the LD's
		return;
		}

	if (op >= 0x80 && op <=	0xbf)
		{
		Dis80toBF();
		return;
		}

	if (op >= 0xc0)
		{
		DisC0toFF();
		return;
		}

	AddToDisUnknown(NULL);
}

/* The main man! */

int	GetNextOpCode(void)
{
	//op = Z80MemBase[PC++];
	op = getZ80Mem(PC++);
	sprintf(Buffer,	"%02x",	(BYTE)op);
	strcat(HexDisBuf, Buffer);

	BytesProcessed++;

	return op;
}
/*
void FlagFn(unsigned int Addr)
{
	return;
}*/

void DisCB(void)
{
char	Num[3];

/*
	If there's an IX IY prefix, then the displacement comes *BEFORE* the
	final opcode. LD (IX+dd),nn is a similar case.
*/

	if (Z80Flags & Z80IXIY)
		GetIXIYDisplacement();

	GetNextOpCode();

/* Test for undocumented DDCB stuff */

	if (Z80Flags & Z80IXIY)
		if ((op	& 7) !=	REG_HL)
			{
			DisDDCB();
			return;
			}

	if (op < 0x40)
		{
/* Do the rotates */
		AddToDisTab(CBRotType[(op >> 3)	& 7]);
		AddToDisReg8(op, FALSE);
		}
	else
		{
		switch (op >> 6)
			{
			case 1:
				AddToDisTab("bit");
				break;

			case 2:
				AddToDisTab("res");
				break;

			case 3:
				AddToDisTab("set");
				break;
			}

		Num[0] = '0'+((op >> 3)& 7);
		Num[1] = ',';
		Num[2] = 0;
		AddToDis(Num);

/* Finally, add the register component. */
		AddToDisReg8(op, FALSE);
		}
}


/* Decode the DDCB instruction */

void DisDDCB(void)
{
char	Num[4],	BitResSet;


	if (op < 0x40)
		{
/* Do the undocumented rotates */
		AddToDisTabLD("");
		AddToDisReg8(op, REG_HL);		/* REG_HL stops L->IXl, etc.. */
		AddToDis(",");
		AddToDis(CBRotType[(op >> 3) & 7]);
		AddToDis(" ");
		AddToDisReg8(REG_HL, FALSE);
		return;
		}

	BitResSet =	op >> 6;

	if (BitResSet >= 2)
		{
		AddToDisTabLD("");
		AddToDisReg8(op, REG_HL);		/* REG_HL stops L->IXl, etc.. */
		AddToDis(",");
		}

	switch (BitResSet)
		{
		case 1:
			AddToDisTab("bit");
			break;
		case 2:
			AddToDisTab("res");
			break;
		case 3:
			AddToDisTab("set");
			break;
		}

	Num[0] = ' ';
	Num[1] = '0'+((op >> 3)& 7);
	Num[2] = ',';
	Num[3] = 0;
	
	AddToDis(Num + (BitResSet <	2) );

	// Finally, add the register component.
	AddToDisReg8(REG_HL, FALSE);

	if (BitResSet == 1)
		AddToDisUndoc();
	return;
}


void DisED(void)
{
	GetNextOpCode();

/* Catch the solitary SLT pseudo-opcode */

   switch (op)
          {
          case 0xFB:
            AddToDisUnknown("Super Loader Trap");
		    return;
		  }

    if (op <= 0x3f)
       if (DisED00to3F())
          return;

	if (op >= 0x40 && op <=	0x7f)
       if (DisED40to7F())
          return;

	if (op >= 0x80 && op <=	0xbf)
		if (DisED80toBF())
           return;

/* Unknown EDxx opcode */

	AddToDisUnknown("Undocumented 8 T-State NOP");
	return;
}

/* ED00 - ED3F are currently only Z180 instructions */

int DisED00to3F(void)
{
	int		r, bb;

    if (DisFlags & DISFLAG_EXCLUDEZ180)
    	return FALSE;

    bb = op & 7;
    r = (op >> 3) & 7;

    switch (bb)
    	{
        case 0:		// ED 0x00 - 0x38
        	AddToDisTab("in0");
            AddToDis(Reg8Idx[r]);
            AddToDis(",(");
            AddToDis8BitAbs(FALSE);
            AddToDis(")");
            AddToDisCommentZ180();
            return TRUE;

        case 1:		// ED 0x01 - 0x39
        	AddToDisTab("out0");
            AddToDis("(");
            AddToDis8BitAbs(FALSE);
            AddToDis("),");
            AddToDis(Reg8Idx[r]);
            AddToDisCommentZ180();
            return TRUE;

        case 4:		// ED 0x04 - 0x3c
        	AddToDisTab("tst");
            AddToDis(Reg8Idx[r]);
            AddToDisCommentZ180();
            return TRUE;
        }

	return FALSE;
}


int	DisED40to7F(void)
{
char	Address[16];
BYTE	EDop;

/* First, let's get those nasty special case opcodes. */

	EDop = op;


	switch (op)
		{
        case 0x76:		// This is SLP which clashes with the undocumented Z80's IM 1
        	if (!(DisFlags & DISFLAG_EXCLUDEZ180))
            	{
                AddToDisTab("slp");
                AddToDisCommentZ180();
	            return TRUE;
                }
            break;


        case 0x4c:		// The Z180's MLT instructions
        case 0x5c:
        case 0x6c:
        case 0x7c:
        	if (DisFlags & DISFLAG_EXCLUDEZ180)
            	return FALSE;
            AddToDisTab("mlt");
            AddToDis(Reg16Idx[(op>>4) & 3]);
            AddToDisCommentZ180();
            return TRUE;

        case 0x64:		// Z180's TST nn
        	if (DisFlags & DISFLAG_EXCLUDEZ180)
            	return FALSE;
            AddToDisTab("tst");
            AddToDis8BitAbs(FALSE);
            AddToDisCommentZ180();
            return TRUE;

        case 0x74:
        	if (DisFlags & DISFLAG_EXCLUDEZ180)
            	return FALSE;
            AddToDisTab("tstio");
            AddToDis("(");
            AddToDis8BitAbs(FALSE);
            AddToDis(")");
            AddToDisCommentZ180();
            return TRUE;


/* Back to the regular Z80's stuff */

		case 0x45:
			AddToDisTab("retn");
            LineCmd |= LC_BLANKLINE;
			return TRUE;

		case 0x47:
			AddToDisTabLD("i,a");
			return TRUE;

		case 0x4d:
			AddToDisTab("reti");
            LineCmd |= LC_BLANKLINE;
			return TRUE;

		case 0x4f:
			AddToDisTabLD("r,a");
			return TRUE;

		case 0x57:
			AddToDisTabLD("a,i");
			return TRUE;

		case 0x5f:
			AddToDisTabLD("a,r");
			return TRUE;

		case 0x67:
			AddToDisTab("rrd");
			return TRUE;

		case 0x6f:
			AddToDisTab("rld");
			return TRUE;
		}

	switch (op & 7)
		{
		case 0:
			AddToDisTab("in");
			AddToDis(Reg8AFIdx[(op >> 3) & 7] );
			AddToDis(",(c)");
			return TRUE;

		case 1:
			AddToDisTab("out");
			AddToDis("(c),");
			AddToDis(Reg8AFIdx[(op >> 3) & 7] );
			return TRUE;

		case 2:
			AddToDisTab( (op & 0x8)	? "adc"	: "sbc");
			AddToDisHLIXIY();
			AddToDis(",");
			AddToDisReg16(op>>4);
			return TRUE;

		case 3:
			sprintf(Address, "(#%04x)",	Get16BitParam()	);
			//AddRefEntry(LastRefAddr, LastPC, DISREF_ADDR);
			AddToDisTabLD("");
			if (EDop & 0x8)
				{
				AddToDisReg16(EDop >> 4);
				AddToDis(",");
				AddToDis(Address);
				return TRUE;
				}
			else
				{
				AddToDis(Address);
				AddToDis(",");
				AddToDisReg16(EDop >> 4);
				return TRUE;
				}

		case 4:
			AddToDisTab("neg");		/* It's a NEG */
			if (EDop !=	0x44)
				AddToDisUndoc();	/* But undocumented? */
			return TRUE;

		case 5:
			AddToDisTab("ret");
			AddToDisUndoc();
            LineCmd |= LC_BLANKLINE;
			return TRUE;

		case 6:
			AddToDisTab("im");		/* Interrupt mode... */
			AddToDis(IMModes[(EDop & 0x18) >> 3] );
			if ((EDop == 0x4e) || (EDop	>= 0x60))
				AddToDisUndoc();
			return TRUE;
		}

	return FALSE;
}


int	DisED80toBF(void)
{

	if (!(DisFlags & DISFLAG_EXCLUDEZ180))
    	{
		switch (op)
    		{
        	case 0x83:		// otim
        	case 0x8b:		// otdm
        	case 0x93:		// otimr
        	case 0x9b:		// otdmr
            	AddToDisTab(Z180RepeatOps[(op >> 3) & 3]);
            	AddToDisCommentZ180();
            	return TRUE;
        	}
        }

	if (op >= 0xA0)
    	{
		if ((op	& 7) > 3)
			return FALSE;		/* Don't know this! */

		AddToDisTab(RepeatOps[(op &	3)+((op	& 0x18)	>> 1)] );
        return TRUE;
        }

	return FALSE;
}


void Dis00to3F(void)
{
	switch (op & 0x0f)
		{
		case 0:
		case 8:
			switch ((op	>> 3) &	7)
				{
				case 0:		//0x00
					AddToDisTab("nop");
					return;

				case 1:		//0x08
					AddToDisTab("ex");
					AddToDis("af,af'");
					return;

				case 2:		//0x10
					AddToDisTab("djnz");
					FlagFn(AddToDisRel8(FALSE));
					return;

				case 3:		//0x18
					AddToDisTab("jr");
					FlagFn(AddToDisRel8(FALSE));
					return;

				default:
					AddToDisTab("jr");
					AddToDis(Conditions[(op	>> 3) &	3] );
					FlagFn(AddToDisRel8(TRUE));
					return;
				}
		case 1:
			AddToDisTabLD("");
			AddToDisReg16(op >>	4);
			AddToDis16BitAbs(TRUE);
			//AddRefEntry(LastRefAddr, LastPC, DISREF_ADDR);
			return;

		case 2:
			switch ((op	>> 4) &	3)
				{
				case 0:	   	/* ld (bc),a */
				case 1:		/* ld (de),a */
					AddToDisTabLD("(");
					AddToDis(Reg16Idx[(op >> 4)	& 1] );
					AddToDis("),a");
					return;

				case 2:	   	// 0x22 = ld (nn),hl
				case 3:		// 0x32 = ld (nn),a
					AddToDisTabLD("(");
					AddToDis16BitAbs(FALSE);
					AddToDis("),");
					if (realop & 0x10)
						{
						AddToDis("a");
						}
					else
						{
						AddToDisHLIXIY();
						}
					//AddRefEntry(LastRefAddr, LastPC, DISREF_INDIRECT);
					return;
				}

		case 3:
			AddToDisTab("inc");
			AddToDisReg16(op>>4);
			return;

		case 4:
			AddToDisTab("inc");
			AddToDisReg8(op>>3,	op>>3);
			return;

		case 5:
			AddToDisTab("dec");
			AddToDisReg8(op>>3,	op>>3);
			return;

		case 6:
		case 0x0e:
			AddToDisTabLD("");
			AddToDisReg8(op>>3,	op>>3);
			AddToDis8BitAbs(TRUE);
			return;

		case 7:
		case 0x0f:
			AddToDisTab(AccRotType[(op>>3) & 7]	);
			return;

		case 9:
			AddToDisTab("add");
			AddToDisHLIXIY();
			AddToDis(",");
			AddToDisReg16((op>>4) &	3);
			return;

		case 0x0a:
			switch ((op	>> 4) &	3)
				{
				case 0:
				case 1:
					AddToDisTabLD("a,(");
					AddToDis(Reg16Idx[(op >> 4)	& 1] );
					AddToDis(")");
					return;

				case 2:
				case 3:
					if (op & 0x10)
						AddToDisTabLD("a");
					else
						{
						AddToDisTabLD("");
						AddToDisHLIXIY();
						}
					AddToDis(",(");
					AddToDis16BitAbs(FALSE);
            		AddToDis(")");
			        //AddRefEntry(LastRefAddr, LastPC, DISREF_INDIRECT);
					return;
				}

		case 0xb:
			AddToDisTab("dec");
			AddToDisReg16((op>>4) &	3);
			return;


		case 0xc:
			AddToDisTab("inc");
			AddToDisReg8(op>>3,	op>>3);
			return;

		case 0xd:
			AddToDisTab("dec");
			AddToDisReg8(op>>3,	op>>3);
			return;
		}

	AddToDisUnknown(NULL);
}

void Dis40to7F(void)
{
	if (op == 0x76)
		{
		AddToDisTab("halt");
		return;
		}

	AddToDisTabLD("");
	AddToDisReg8(realop	>> 3, realop);
	AddToDis(",");
	AddToDisReg8(realop, realop	>> 3);
	return;
}


void Dis80toBF(void)
{
int	GenOp;

	GenOp =	(op>>3)	& 7;
	AddToDisTab(BasicOps[GenOp]);

	if (GenOp <	2 || GenOp == 3)
		AddToDis("a,");

	AddToDisReg8(op, op);
}


void	DisC0toFF(void)
{
int	GenOp;

	GenOp =	(op	>> 3) &	7;

	switch (op & 0xf)		/* Break it down into eight basics */
		{
		case 0:
		case 8:
			AddToDisTab("ret");
			AddToDis(Conditions[GenOp]);
            LineCmd |= LC_BLANKLINE;
			return;

		case 1:			/* POP rr */
		case 5:			/* PUSH rr */
			AddToDisTab((op	& 4) ? "push" :	"pop");
			AddToDisReg16NoAnd(((op	>> 4) &	3) + (op >=	0xf0));
			return;

		case 2:
		case 0x0a:
			AddToDisTab("jp");
			AddToDis(Conditions[GenOp]);
			FlagFn(AddToDis16BitAbs(TRUE));
			return;

		case 4:
		case 0x0c:
			AddToDisTab("call");
			AddToDis(Conditions[GenOp]);
			FlagFn(AddToDis16BitAbs(TRUE));
			return;

		case 3:
			switch ((op	>> 4) &	3)
				{
				case 0:		// 0xc3
					AddToDisTab("jp");
					FlagFn(AddToDis16BitAbs(FALSE));
					return;

				case 1:		// 0xd3
					AddToDisTab("out");
					sprintf(Buffer,"(#%02x),a",	GetNextOpCode()	);
					AddToDis(Buffer);
					//AddRefEntry(op, LastPC, DISREF_OUTPORT);
					return;

				case 2:		// 0xe3
					AddToDisTab("ex");
					AddToDis("(sp),");
					AddToDisHLIXIY();
					return;

				case 3:		// 0xf3
					AddToDisTab("di");
					return;
				}

		case 6:
		case 0x0e:
			AddToDisTab(BasicOps[GenOp]	);
			if (GenOp <	2 || GenOp == 3)
				AddToDis("a,");
			AddToDis8BitAbs(FALSE);
			return;

		case 7:
		case 0x0f:
			AddToDisTab("rst");
						sprintf(Buffer,	"#%x", (op & (7	<< 3)) );
			AddToDis(Buffer);
			return;

		case 9:
			switch ((op	>> 4) &	3)
				{
				case 0:		// 0xc9
					AddToDisTab("ret");
                    LineCmd |= LC_BLANKLINE;
					return;

				case 1:		// 0xd9
					AddToDisTab("exx");
					return;

				case 2:		// 0xe9
					AddToDisTab("jp");
					AddToDis("(")
					AddToDisHLIXIY();
					AddToDis(")");
					return;

				case 3:		// 0xf9
					AddToDisTabLD("sp,");
					AddToDisHLIXIY();
					return;
				}


		case 0x0b:
			switch ((op	>> 4) &	3)
				{
				case 1:		// 0xdb
					AddToDisTab("in");
					sprintf(Buffer,"a,(#%02x)",	GetNextOpCode()	);
					AddToDis(Buffer);
					//AddRefEntry(op, LastPC, DISREF_INPORT);
					return;

				case 2:		//0xeb
					AddToDisTab("ex");
					AddToDis("de,hl");
					return;

				case 3:		//0xfb
					AddToDisTab("ei");
					return;
				}



		case 0x0d:	   		/* N.B. this can only get here with #cd */
			AddToDisTab("call");
			FlagFn(AddToDis16BitAbs(FALSE));
			return;
		}

	AddToDisUnknown(NULL);
}


void AddToDisTab(char *str)
{
	int Len;

	AddToDis(str);

    if (HaveTabbed)
    	return;

    Len = strlen(DisBuf);
	memset(Buffer, ' ',	TABSIZE	- Len );
	Buffer[TABSIZE - Len] = 0;
	AddToDis(Buffer);

    HaveTabbed = TRUE;
	return;
}


void AddToDisTabLD(char	*str)
{
	AddToDisTab("ld");
	AddToDis(str);
	return;
}


void AddToDisCommentZ180(void)
{
	AddToDisComment("Z180 instruction");
    return;
}

void AddToDisComment(char *str)
{
	if (!(Z80Flags & Z80COMMENT))
			{
			sprintf(CommentBuffer, "; ");
            Z80Flags |= Z80COMMENT;
            }

    strcat(CommentBuffer, str);
	return;
}

void AddToDisHLIXIY(void)
{
	if (!(Z80Flags & Z80IXIY) )
		{
		AddToDis("hl");
		return;
		}

	AddToDis( (Z80Flags	& Z80IX) ? "ix"	: "iy");
	return;
}


/*  AddToDisReg8(opcode)

	Adds b,c,d,e,h,l,(hl) or a to disassembly, taking into consideration
	the IX and IY prefixes. "op2" is used to determine whether a "IXl"
	reference is valid or not.
*/

void AddToDisReg8(int op, int op2)
{
	op &= 7;

	if (Z80Flags & Z80IXIY)
		{
		op2	&= 7;

		if (!(op & (Z80CB |	Z80ED))	&& (op2	!= REG_HL) )
			{
			if (op == REG_L)
				{
				AddToDis((Z80Flags & Z80IX)	? "ixl"	: "iyl");
				return;
				}
		
			if (op == REG_H)
				{
				AddToDis((Z80Flags & Z80IX)	? "ixh"	: "iyh");
				return;
				}
			}

		   	if (op == REG_HL)
			{
			if (Z80IXIY)
				GetIXIYDisplacement();
			if (IXIYDisp >=	0)
				sprintf(Buffer,"(%s+#%02x)", ((Z80Flags	& Z80IX) ? "ix"	: "iy"), IXIYDisp );
			else
				sprintf(Buffer,"(%s-#%02x)", ((Z80Flags	& Z80IX) ? "ix"	: "iy"), -IXIYDisp );
			AddToDis(Buffer);
			return;
			}
		}

	AddToDis(Reg8Idx[op]);
	return;
}


/*	AddToDisReg16(opcode)

	Adds bc,de,hl or sp to disassembly, taking into consideration
	the IX and IY prefixes
*/

void AddToDisReg16(int op)
{
	op &= 3;
	if ( (op ==	2) && (Z80Flags	& Z80IXIY) )
		AddToDisHLIXIY();
	else
		AddToDis(Reg16Idx[op]);
	return;
}

void AddToDisReg16NoAnd(int	op)
{
	if ( (op ==	2) && (Z80Flags	& Z80IXIY) )
		AddToDisHLIXIY();
	else
		AddToDis(Reg16Idx[op]);
	return;
}


WORD AddToDisRel8(int CommaFlag)
{
	if (CommaFlag)
		AddToDis(",");

	GetNextOpCode();

	LastRefAddr	= (PC +	(char)op);
	Add16BitAddress(LastRefAddr);

	sprintf(Buffer,	"(%d)",	(char)op);
	AddToDisComment(Buffer);
	return LastRefAddr;
}

void AddToDis8BitAbs(int CommaFlag)
{
	if (CommaFlag)
		AddToDis(",");

	GetNextOpCode();

	sprintf(Buffer,	"#%02x",op );
	AddToDis(Buffer);
	return;
}

WORD AddToDis16BitAbs(int CommaFlag)
{
	if (CommaFlag)
		AddToDis(",");

	Get16BitParam();
	Add16BitAddress(LastRefAddr);
	return LastRefAddr;
}

void	AddToDisUndoc(void)
{
	AddToDisComment("Undocumented");
	return;
}

void	AddToDisUnknown(char *Comment)
{
int	i, NumOpCodes;

	AddToDisTab("db");

	NumOpCodes = abs(PC	- LastPC);
	for	(i = 0;	i <	NumOpCodes;	i++)
		{
//		sprintf(Buffer,	"#%02x", Z80MemBase[(LastPC	+ i)&0xffff] );
		sprintf(Buffer,	"#%02x", getZ80Mem((LastPC + i) & 0xffff));
		if (i <	(NumOpCodes-1))
			strcat(Buffer, ", ");
		AddToDis(Buffer);
		}

	if (Comment	== NULL)
		AddToDisComment("Unknown opcode");
	else
		AddToDisComment(Comment);

	return;
}

char	GetIXIYDisplacement(void)
{
	if (Z80Flags & Z80GOTIXIYDISP)	// Already got IXIY dispacement?
		return IXIYDisp;

	IXIYDisp = (char)GetNextOpCode();
	Z80Flags |=	Z80GOTIXIYDISP;
	return IXIYDisp;
}


WORD Get16BitParam(void)
{
	LastRefAddr	= GetNextOpCode();
	LastRefAddr	|= (GetNextOpCode()	<< 8);
	return LastRefAddr;
}


void Add16BitAddress(WORD Addr)
{
	sprintf(Buffer,	"#%04x", Addr );
	AddToDis(Buffer);
	return;
}


void StringToLower(char *s)
{
	while (*s) {
    	*s = tolower(*s);
		s++;
	}
}

void StringToUpper(char *s)
{
	while (*s) {
    	*s = toupper(*s);
		s++;
	}
}

int main(int argc, char **argv) {

	DISZ80 dis;
	WORD w = 0x2000;

	printf("DISSZ80\n");

	dis.Mem0Start = 0;
	dis.Start = 0xf000;
	dis.End = 0xf130;
	dis.Flags = DISFLAG_OPCODEDUMP | DISFLAG_ADDRDUMP | DISFLAG_UPPER | DISFLAG_CONSOLE | DISFLAG_VERBOSE | DISFLAG_QUIET;
	dis.MaxReferences = 16;
	dis.NumInstructions = 0; 
	dis.CreatedREFOK = 0;	 
	dis.SrcFileName = NULL;			 
 	dis.OutFileName = NULL;  	 
 	dis.OpMapFileName = NULL;	
	dis.RefFileName = NULL;

	DisassembleZ80(&dis);

	for(;;);
	
	return TRUE;
}