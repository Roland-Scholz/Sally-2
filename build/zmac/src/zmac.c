/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "zmac.y"

// GWP - keep track of version via hand-maintained date stamp.
#define VERSION "18oct2022"

/*
 *  zmac -- macro cross-assembler for the Zilog Z80 microprocessor
 *
 *  Bruce Norskog	4/78
 *
 *  Last modification  1-18-87 by cdk
 *  This assembler is modeled after the Intel 8080 macro cross-assembler
 *  for the Intel 8080 by Ken Borgendale.  The major features are:
 *	1.  Full macro capabilities
 *	2.  Conditional assembly
 *	3.  A very flexible set of listing options and pseudo-ops
 *	4.  Symbol table output
 *	5.  Error report
 *	6.  Elimination of sequential searching
 *	7.  Commenting of source
 *	8.  Facilities for system definition files
 *
 * Revision History:
 *
 * jrp	3-8-82	Converted to run on Vax, updated syntax to conform better
 *		to the Zilog standard.
 *
 * jrp	3-15-82	Added underscore as a character type in the lex table
 *		'numpart' (0x5F).
 *
 *		Changed maximum number of characters in a label to 15
 *		from 7. Note that 'putsymtab' uses this value inside
 *		of a quoted string, so we use 15.
 *
 * jrp	2-15-83	Fixed 'getlocal' to return better local labels. It used
 *		to crash after 6 invocations.
 *
 * jrp	6-7-83	Fixed bug in the ADD IX,... instruction.
 *
 * jrp	5-11-84	Added code to print unused labels out with the symbol table
 *		Also sped up the macro processor by using stdio.
 *
 * jrp 5-22-84	Added include files ala ormac
 *
 * jrp 8-27-84	Added PHASE/DEPHASE commands
 *
 * cdk 9-20-86	Converted to run on a Pyramid.  This meant changing yylval
 *		to be a %union, and then putting in the appropriate
 *		typecasts where ints are pointers are used interchangeably.
 *		The current version still probably won't run on machines where
 *		sizeof(int) != sizeof(char *).
 *		Also changed emit() to use varargs, and got rid of the
 *		old style = in front of yacc action code.
 *			-Colin Kelley  vu-vlsi!colin
 *
 * cdk 10-2-86	Added some more typecasts to keep lint a little happier.
 *		Removed several unused variables.  Changed most vars
 *		declared as char to int, since many of them were being
 *		compared with -1!  I still don't know what's going on with
 *		est[][] being malloc'd and free'd everywhere...it looks pretty
 *		fishy...
 *
 * cdk 1-18-87  Added MIO code to emulate 'mfile' using malloc()'d memory.
 *		This was needed to get the code to work when compiled under
 *		MSC 4.0 on a PC, and it's probably faster anyway.
 *
 * cdk 2-5-87	Added 'cmp' as a synonym for 'cp', 'jmp' as a synonym for
 *		'jp', and added tolerance of accumulator specification for arithmetic
 *		and logical instructions.  (For example, 'or a,12' is now accepted,
 *		same as 'or 12'.)
 *
 * gwp 12-29-08	Changes to allow compilation with modern C compiler and using bison
 *		as the .y to .c converter.  assert, tstate pseudo-ops.
 *		t(), tilo(), tihi() functions.  ==, <=, >=, !=, !, <, > operators.
 *		-c to turn cycle counts off in listing.  Usage, -h and version.
 *
 * gwp 9-26-10	Add ocf() and setocf to track and set op code fetch counts.
 *		Add sett as an alias for tstate
 *
 * gwp 12-30-11	Add undocumented instructions sl1, pfix, pfiy, in (c), out (c),0
 *		bit/set/res (ixy+d),reg and ld/inc/dec ixylh.
 *
 * gwp 2-8-12   Increase MAXIFS massively due to massive lowt macro
 *
 * gwp 2-11-12  Support 32 bit constants.  '%' alias for MOD.  Add defd, dword.
 *		lo(x) and hi(x) for easy low and high byte extraction.  Allow
 *		filenames longer than 15 characters.  All output to "zout" subdirectory
 *		of source file.
 *
 * gwp 2-15-12	Perform multiple passes while equates are changing.  Support
 *		.label for temporary label definitions and _label for file
 *		scoped labels.  Allow '.' within labels.  Assert listing bugfix.
 *
 * gwp 4-27-12	Implement $ prefixed hex constants and double-quoted strings.
 *
 * gwp 6-30-12	Minor changes to allow compilation with gcc.
 *
 * gwp 9-05-12	incbin added.
 *
 * gwp 11-24-12	Fix macro expansion bug when symbol larger than MAXSYMBOLSIZE
 *		due to file name prepending when symbol starts with '_'.
 *
 * gwp 12-04-12	Optional JR promotion and JP demotion errors.  Output a warning
 *		if no execute address given.  Output ".bds" file to enable easy
 *		simple source level debugging.
 *
 * gwp 4-14-13	Parens in expressions, else, .pseudo, full set of C operators
 *		with conventional precedence and various aliases and code
 *		changes to make source similar to zmac 1.3 on internet.
 *
 * gwp 5-5-13	.cmd,.cas,.lcas,.bin output.  dc (both MACRO-80 and EDAS types).
 *		lo, hi renamed to low, high and make unary operators.  Allow
 *		label::, placeholder public, extern declarations.  Bug fixes
 *		in defs, t, ocf, tihi, tilo in phase mode.  Initial support
 *		for -I include directories. 0x hex constants. --mras flag for
 *		limited MRAS compatibility (allows $ in labels, $? to start
 *		labels).
 *
 * gwp 4-6-13	--rel for .rel (Microsoft linker) output and extern, public,
 *		aseg, cseg, dseg in support (big emit + expression revamp).
 *		-I follows C preprocessor convention, output relative to
 *		current directory.  8080 mnemonics, .z80, .8080, -z, -8.
 *		Change .bin to .cim.  Warn on labels not in first column.
 *
 * gwp 8-11-13	Allow $ to start identifiers and do '$' dropping when macro
 *              parsed so we no longer need to drop '$' in identifiers. 
 *              Even $FCB allowed, with warning.  Add --zmac for backwards
 *		compatibility with zmac.  ` now used for joining in macros.
 *		Most reserved words can be used as labels or variables.
 *		Free-form title, name, comment, subttl parsing.  Allow #arg
 *		for macro arguments (in --mras mode).  Support <CR> delimited
 *		files.  Add .ams output.  Integrate documentation (--doc).
 *
 * gwp 3-12-14	Emit jr even if out of range.  z80.lib support.
 *		Warning and bug fixes from Mark Galanger.
 *		Macros can override built-ins and are no longer listed
 *		in symbol table.  A, B, C, D, E, H, L, M, PSW, SP are
 *		pre-defined values which can be used in data statements
 *		(db, dw, dd).  Reserved words can be equated but are only
 *		accessbile in data.  SET can be used in place of DEFL
 *		(MAC and MACRO-80 agree on this).  '=' can be used in place
 *		of EQU. 'maclib file' includes 'file.lib'.  Bug fix in "dw 0,$".
 *		Removed error flagging in expressions which could cause parse
 *		to fail from that point onwards.
 *		expression(ix) equivalent to (ix + expression).
 *		Macro expanded lines didn't go through the line analyzer.
 *		Empty macro arguments (e.g., mac 1,,2)
 *		Implemented rept, irp, irpc, exitm.  Add more detail on phase
 *		errors. '' is an empty string in db/ascii/etc, 0 otherwise.
 *		Almost any name can be used as a macro parameter name.
 *		Allow 'if' in first column.
 *		Fix .rel mode bug in dc, incbin.
 *		Emit .bds output for dc, incbin.
 *		Allow assembly to wrap past end of memory.
 *		"pragma bds" for raw output to .bds file.  Also output equates
 *		to .bds file.
 *		Macro bug fix from Sergey Erokhin.
 *
 * gwp 9-5-16	Allow ' on identifiers for manual prime register tracking.
 *		Improve MRAS support with *LIST, *INCLUDE and Pk=n parameters.
 *
 * gwp 20-9-16	Big MRAS compatibility fixes.  Can do MRAS operator precedence,
 *		proper .operator. tokenization and agressive macro parameter
 *		substituion.  Change Pk=n to -Pk=n.  Add ++, += and variants
 *		for more compact variable adjustment than defl.  First crack
 *		at .tap output for ZX Spectrum support.
 *
 * gwp 13-8-17	Add "import" for simple once-only inclusion of files.
 *		Track full path so relative includes work properly.
 *		Allow push af', pop af' for notational convenience.
 *		Add "bytes" as alias for "dc".  Fix --rel output bugs in
 *		low(), high(), div and mod.
 *
 * gwp 12-3-18	250 baud .cas output and .wav format.  Common blocks.
 *		--oo, --xo, --od to control output.  Delete output on fail.
 *
 * gwp 2-6-18	1000 baud .cas ouput and .mds (MAME/MESS debug script) output.
 *
 * gwp 28-7-18	Double free of output files bug fix from Pedro Gimeno.  Don't
 *		output SYSTEM files if no entry point thanks to Tim Halloran.
 *
 * gwp 6-10-18	Stop quoted inclued file crash on for OSX (and linux?). Thanks
 *		to Paulo Silva. Added "dm" as "ascii" alias for Blair Robins.
 *
 * gwp 5-12-18	Peter Wilson ran into quoted include crash.  Also suggested
 *		import be allowed in column 0 and noted that keywords like
 *		IF and LIST could not be labels even if given colons.
 *
 * gwp 6-12-18	hex output does proper EOF record if no entry address given.
 *		include and some other pseudo-opts can have a label.
 *
 * gwp 3-1-19	Improve jrpromote to handle some edges cases better where
 *		promotion isn't needed as long as shortest code is tried first.
 *
 * gwp 5-1-19	Add cycle count and output bytes to macro invocations.
 *		[ with listing bug fixes added slightly later ]
 *
 * gwp 19-1-19	DRI compatibility enhancements and other bug fixes from
 *		Douglas Miller. TWOCHAR constants ('XY') usable everywhere.
 *		dolopt moved into function to stop LEX stack crash.  Support
 *		labels on cseg/dseg.  Relax paren parsing for LXI instruction.
 *		Recognize TITLE in column 0.  Stop "Not relocatable error" on
 *		"ORG 0".  -i option stops all macro line listing.
 *		--dri mode:
 *		silences warnings about builtin redefinition (for Z80.LIB).
 *		Strips '$' from constants and symbols.
 *		Accepts '*' in column 0 as comment.
 *		All 8080 opcodes can be use in DB and expressions.
 *		Add recognition of DRI $-MACRO directives
 *
 * gwp 20-1-19	Z-80 mnemonics usable as values.  --nmnv turns that off.  def3
 *		Register aliases.  Multiple statements per line roughed in.
 *
 * gwp 20-4-20	Preserve case of symbols in symbol table.  Output hexadecimal
 *		in upper case and show decimal value of symbols.
 *
 * tjh 9-5-20	Add -Dsym to allow definition of symbols on the command line.
 *		ZMAC_ARGS environment variable added to command line.
 *
 * gwp 25-8-20	Fix crash in "out (c),0".  Make "in f,(c)" work.
 *
 * gwp 17-3-21	Stop line buffer overflow when too many "dc" or "incbin" lines
 *		appear contiguously.  --fcal option and Z-180 instructions.
 *
 * gwp 10-4-21	Put code and data indications in .bds output.
 *
 * gwp 9-2-22	Fix --z180 and improve usage message on unknown -- flags.
 *
 * gwp 10-4-22	Much better symbol table hash function from Al Petrofsky as
 *		used in gcc and gas.
 *
 * gwp 10-18-22	Added *GET, IFEQ, IFEQ, IFLT, IFGT to improved MRAS support.
 *		Change FILE/EXT to FILE.EXT for includes in --mras mode.
 *		xh, xl, yh, yl, hx, lx, hy, ly alternates for ixh, ixy, iyh, iyl.
 */

#if defined(__GNUC__)
#pragma GCC diagnostic error "-Wreturn-type"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>

#ifdef WIN32
#include <windows.h>	// just for colouring the output
#include <direct.h>		// _mkdir
#ifdef _MSC_VER
#define strdup _strdup
#define unlink _unlink
#define strncasecmp _strnicmp
#endif
#endif

#if defined(__APPLE__) || defined(__linux__) || defined(__FreeBSD__)
#include <unistd.h>	// just for unlink
#endif

#include "zi80dis.h"

#ifdef vax11c
#define unlink(filename) delete(filename)
#endif

#include "mio.h"

/*
 * DEBUG turns on pass reporting.
 * DBUG enables -d to allow yacc/bison yydebug increment (but must be passed
 *	on the command line)
 * Macro debug and Token debug enables.
#define	DEBUG
#define	M_DEBUG	
#define	T_DEBUG
 */

#ifdef DBUG
#define YYDEBUG 1
#endif

#define ITEMTABLESIZE	100000
#define TEMPBUFSIZE	(1000+MAXSYMBOLSIZE)
#define LINEBUFFERSIZE	1000
#define EMITBUFFERSIZE	200
#define MAXSYMBOLSIZE	40
#define IFSTACKSIZE	20
// GWP - I use lots of if's with my lowt macro
#define MAXIFS		65536
#define TITLELEN	50
#define BINPERLINE	16
#define	PARMMAX		25
#define MAXEXP		25
#define SYMMAJIC	07203
#define	NEST_IN		32
#define MAXPASS		32
#define MAXINCPATH	32

int iflist();
int yylex();
int phaseaddr(int addr);
int nextchar();
int getcol();
int skipline(int ac);
int tokenofitem(int deftoken, int keyexclude, int keyinclude);
int getm();
int counterr();
int countwarn();
int convert(char *buf, char *bufend, int *overflow);

void yyerror(char *err)
{}		/* we will do our own error printing */

struct argparse {
	char *arg;	// output buffer for argument
	int argsize;	// size of output buffer
	int (*getch)(struct argparse *); // get next character
	int *peek;	// pointer single item pushback buffer
	int macarg;	// working on macro arguments

	char *user_ptr;	// state for getch
	int user_int;	// state for getch
	int user_peek;	// state for getch

	int didarg;	// internal parsing state
	int numarg;	// internal parsing state
};

int getarg(struct argparse *ap);

struct	item	{
	char	*i_string;
	int	i_value;
	int	i_token;
	int	i_uses;
	int	i_scope;
	int	i_chain;
	int	i_pass;
};

void itemcpy(struct item *dst, struct item *src);
int item_is_verb(struct item *i);
int item_value(struct item *i);
struct item *keyword(char *name);
int keyword_index(struct item *k);

#define SCOPE_NONE	(0)
#define SCOPE_PROGRAM	(1)
#define SCOPE_DATA	(2)
#define SCOPE_COMMON	(3)
#define SCOPE_PUBLIC	(4)
#define SCOPE_EXTERNAL	(8)
#define SCOPE_NORELOC	(16)
#define SCOPE_BUILTIN	(32)	/* abuse */
#define SCOPE_COMBLOCK	(64)
#define SCOPE_TWOCHAR	(128)
#define SCOPE_ALIAS	(256)
#define SCOPE_CMD_P	(512)
#define SCOPE_CMD_D	(1024)

#define SCOPE_SEGMASK	(3)
#define SCOPE_SEG(s)	((s) & SCOPE_SEGMASK)

struct expr {
	int e_value;
	int e_scope;
	int e_token;
	int e_parenthesized;
	struct item *e_item;
	struct expr *e_left;
	struct expr *e_right;
};

#define EXPR_SEG(e)	SCOPE_SEG(e->e_scope)

FILE	*fout,
	*fbuf,
	*fbds,
	*fcmd,
	*fcas,
	*flcas,
	*flnwcas,
	*ftcas,
	*fcim,
	*fams,
	*frel,
	*ftap,
	*fmds,
	*f1500wav,
	*f1000wav,
	*f500wav,
	*f250wav,
	*fin[NEST_IN],
	*now_file ;

char *output_dir = "zout";

struct OutputFile {
	char *suffix;
	char *mode;
	FILE **fpp;
	int system; // A cassette SYSTEM file
	int no_open;
	int wanted; // output file has been explicitly selected
	char *filename;
	int temp;
}
outf[] = {
	{ "lst",	"w",	&fout		},
	{ "hex", 	"w",	&fbuf		},
	{ "bds",	"w",	&fbds		},
	{ "cmd",	"wb",	&fcmd		},
	{ "1500.wav",	"wb",	&f1500wav, 1	}, // must be 1 before 1500.cas
	{ "1500.cas",	"w+b",	&fcas,	   1	},
	{ "1000.wav",	"wb",	&f1000wav, 1	}, // must be 1 before 1000.cas
	{ "1000.cas",	"w+b",	&flnwcas,  1	},
	{ "500.wav",	"wb",	&f500wav,  1	}, // must be 1 before 500.cas
	{ "500.cas",	"w+b",	&flcas,    1	},
	{ "250.wav",	"wb",	&f250wav	}, // must be 1 before 250.cas
	{ "250.cas",	"w+b",	&ftcas		},
	{ "cim",	"wb",	&fcim		},
	{ "ams",	"wb",	&fams		},
	{ "rel",	"wb",	&frel,	0, 1	},
	{ "tap",	"wb",	&ftap		},
	{ "mds",	"w",	&fmds		},
};
#define CNT_OUTF (sizeof outf / sizeof outf[0])

int getoptarg(int argc, char *argv[], int i);
void stop_all_outf();
void clean_outf();
void clean_outf_temp();
void suffix_list(char *sfx_lst, int no_open);

int	pass2;	/*set when pass one completed*/
int	outpass; 	// set when we decide to stop doing passes */
int	passfail;	// set when an error means passes should not continue
int	passretry;	// set when an inconsistency will require another pass
int	dollarsign ;	/* location counter */
int	olddollar ;	/* kept to put out binary */
int	oldothdollar;	// output address of next .cmd/.cas/.lcas block
int	emit_addr;	// where code and data are being placed in memory
int	tstates;	// cumulative T-states
int	ocf;		// cumulative op code fetches
int	llseq;		// local label sequence number
int	mras;		// MRAS semi-compatibility mode
int	trueval = 1;	// Value returned for boolean true
int	zcompat;	// Original zmac compatibility mode
char	modstr[8];	// Replacement string for '?' in labels when MRAS compatible
int	relopt;		// Only output .rel files and length of external symbols
int	driopt;		// DRI assemblers compatibility
int	macopt;		// DRI assemblers $-MACRO et al.
int	comnt;		// DRI assemblers '*' comment line
int	nmnvopt;	// Mnemonics are not treated as predefined values.
char	progname[8];	// Program name for .rel output
int	note_depend;	// Print names of files included
int	separator;	// Character that separates multiple statements.
int	firstcol;
int	logcol;
int	coloncnt;
int first_always_label;
int	full_exprs;	// expression parsing mode allowing almost any identifier
struct item *label, pristine_label; // 
int	list_dollarsign;// flag used to change list output for some operations
int	list_addr;	// address to display for operation if !list_dollarsign

// Include file search path
char	*incpath[MAXINCPATH];
int	incpath_cnt;

/* program counter save for PHASE/DEPHASE */
int	phdollar, phbegin, phaseflag ;

char	*src_name[NEST_IN] ;
int	linein[NEST_IN] ;
int linepeek[NEST_IN];
int	now_in ;


// These first 5 errors are singled out in lsterr1() for reasons I don't
// quite understand.
#define bflag	0	/* balance error */
#define eflag	1	/* expression error */
#define fflag	2	/* syntax error */
#define iflag	3	/* bad digits */
#define mflag	4	/* multiply defined */

#define pflag	5	/* phase error */
#define uflag	6	/* undeclared used */
#define vflag	7	/* value out of range */
#define oflag	8	/* phase/dephase error */
#define aflag	9	/* assert failed */
#define jflag	10	/* JP could be JR */
#define rflag	11	/* expression not relocatable */
#define gflag	12	/* incorrect register */
#define zflag	13	/* invalid instruction */

#define FIRSTWARN	warn_hex
#define	warn_hex	14
#define warn_notimpl	15
#define warn_general	16

#define FLAGS	17	/* number of errors and warnings */

char	err[FLAGS];
int	keeperr[FLAGS];
char	errlet[FLAGS]="BEFIMPUVOAJRGZHNW";
char	*errname[FLAGS]={
	"Balance",
	"Expression",
	"Syntax",
	"Digit",
	"Mult. def.",
	"Phase",
	"Undeclared",
	"Value",
	"Phase/Dephase",
	"Assertion failure",
	"Use JR",
	"Not relocatable",
	"Register usage",
	"Invalid instruction",
	"$hex constant interpreted as symbol",
	"Not implemented",
	"General"
};
char	errdetail[FLAGS][TEMPBUFSIZE * 2];
char	detail[TEMPBUFSIZE * 2];


unsigned char inpbuf[LINEBUFFERSIZE];
unsigned char inpbuf_insert[LINEBUFFERSIZE];
unsigned char *inpptr;

char	linebuf[LINEBUFFERSIZE];
char	*lineptr;
char	*linemax = linebuf+LINEBUFFERSIZE;

char	outbin[BINPERLINE];
char	*outbinp = outbin;
char	*outbinm = outbin+BINPERLINE;

char	outoth[256];
int	outoth_cnt = 0;

unsigned char	emitbuf[EMITBUFFERSIZE];
unsigned char	*emitptr;

char	ifstack[IFSTACKSIZE];
char	*ifptr;
char	*ifstmax = ifstack+IFSTACKSIZE-1;


char	hexadec[] = "0123456789ABCDEF" ;


int	nitems;
int	nbytes;
int	invented;
int	npass;
int	njrpromo;
int	multiline;
int	prev_multiline;


char	tempbuf[TEMPBUFSIZE];
char	*tempmax = tempbuf+TEMPBUFSIZE-1;

char	arg_flag;
struct argparse arg_state;
void	arg_start();
void	arg_reset();
int	str_getch(struct argparse *ap);

int	mras_undecl_ok;
int	mras_param[10];
int	mras_param_set[10];

struct cl_symbol {
	char *name;
	struct cl_symbol *next;
} *cl_symbol_list;

char	inmlex;
char	mlex_list_on;
int	parm_number;
int	exp_number;
char	symlong[] = "Symbol/number too long";
int	raw;

int	disp;

int	param_parse;
#define PARAMTABSIZE (PARMMAX * 2)
struct item paramtab[PARAMTABSIZE];

#define FLOC	PARMMAX
#define TEMPNUM	PARMMAX+1
#define REPNUM	PARMMAX+2
#define MSTART	PARMMAX+3
#define MSTR	PARMMAX+4
#define MARGP	PARMMAX+5
#define MIF	PARMMAX+6

#define PAREXT	7

union exprec {
	char *param;
	int value;
	struct argparse *ap;
};
union exprec	*est;
union exprec	*est2;
union exprec	*expstack[MAXEXP];
int	expptr;

int	floc;
int	mfptr;
FILE	*mfile;

char	*writesyms;


char	*title;
char	titlespace[TITLELEN];
char	*timp;
char	*sourcef;
/* changed to cope with filenames longer than 14 chars -rjm 1998-12-15 */
char	src[1024];
char	bin[1024];
char	listf[1024];
char	oth[1024];

char	copt = 1,	/* cycle counts in listings by default */
	edef = 1,
	eopt = 1,
	fdef = 0,
	fopt = 0,
	gdef = 1,
	gopt = 1,
	iopt = 0 ,	/* list include files */
	jopt = 0,
	JPopt = 0,
	lstoff = 0,
	lston = 0,	/* flag to force listing on */
	mdef = 0,
	mopt = 0,
	nopt = 1 ,	/* line numbers on as default */
	popt = 1,	/* form feed as default page eject */
	sopt = 0,	/* turn on symbol table listing */
	topt = 1,	/* terse, only error count to terminal */
	printer_output = 0, // GWP - printer style output
	z80 = 1,
	saveopt;

char default_jopt, default_JPopt, default_z80 = 1;

char	xeq_flag = 0;
int	xeq;

time_t	now;
int	line;
int	page = 1;

struct stab {
	char	t_name[MAXSYMBOLSIZE+1];
	int	t_value;
	int	t_token;
};

// GWP - support for cycle count tracking (and opens door for efficient .cmd, etc. output)

unsigned char memory[1 << 16];
char memflag[1 << 16];
enum {
	MEM_DATA = 1,
	MEM_INST = 2,
	MEM_T_SET = 4
};
int tstatesum[1 << 16];
int ocfsum[1 << 16];

// GWP - expression handling extensions for .rel output.
void advance_segment(int step);
void expr_reloc_check(struct expr *ex);
void expr_number_check(struct expr *ex);
void expr_scope_same(struct expr *ex1, struct expr *ex2);
void expr_word_check(struct expr *ex);
int is_number(struct expr *ex);
int is_external(struct expr *ex);
struct expr *expr_num(int value);
struct expr *expr_alloc(void);
struct expr *expr_var(struct item *var);
struct expr *expr_mk(struct expr *left, int token, struct expr *right);
struct expr *expr_op(struct expr *left, int token, struct expr *right, int value);
struct expr *expr_op_sc(struct expr *left, int token, struct expr *right, int value);
void expr_free(struct expr *ex);
int can_extend_link(struct expr *ex);
void extend_link(struct expr *ex);
void putrelop(int op);
#define RELOP_BYTE	(1)
#define RELOP_WORD	(2)
#define RELOP_HIGH	(3)
#define RELOP_LOW	(4)
#define RELOP_NOT	(5)
#define RELOP_NEG	(6)
#define RELOP_SUB	(7)
#define RELOP_ADD	(8)
#define RELOP_MUL	(9)
#define RELOP_DIV	(10)
#define RELOP_MOD	(11)
struct item *item_lookup(char *name, struct item *table, int table_size);
struct item *item_substr_lookup(char *name, int token, struct item *table, int table_size);
struct item *locate(char *name);
// Data descriptions for emit()
#define E_CODE		(0)
#define E_DATA		(1)
#define E_CODE8		(2)
#define E_CODE16	(3)
int segment;
#define SEGCHAR(s) " '\"!"[s]
#define SEG_ABS		(0)
#define SEG_CODE	(1)
#define SEG_DATA	(2)
#define SEG_COMMON	(3)
int seg_pos[4]; // may eventually support SEG_COMMON
int seg_size[4];
int rel_main;
int segchange;
struct item *cur_common;
void putout(int value);
int outrec;
int outlen;
unsigned char outbuf[1024 * 1024];
void bookmark(int delay);
void listfrombookmark();


/*
 *  push back character
 */
#define NOPEEK (EOF - 1)
int	peekc;
int	nextline_peek;

/* function prototypes */
void error(char *as);
void usage(char *msg, char *param);
void help();
void list_out(int optarg, char *line_str, char type);
void erreport();
void errorprt(int errnum);
void errwarn(int errnum, char *message);
void mlex(char *look);
void popsi();
void suffix(char *str, char *suff);
char *basename(char *filename);
char *getsuffix(char *str);
void outpath(char *out, char *src, char *suff);
void casname(char *out, char *src, int maxlen);
void putm(int c);
void insymtab(char *name);
void outsymtab(char *name);
void compactsymtab();
void putsymtab();
void clear();
void clear_instdata_flags();
void setmem(int addr, int value, int type);
void setvars();
void flushbin();
void flushoth();
void lineout();
void puthex(int byte, FILE *buf);
void putcas(int byte);
void putrelbits(int count, int bits);
void putrel(int byte);
void putrelname(char *str);
void putrelextaddr(int extaddr);
void putrelcmd(int cmd);
void putrelsegref(int scope, int addr);
void flushrel(void);
void lsterr1();
void lsterr2(int lst);
void copyname(char *st1, char *st2);
void next_source(char *sp, int always);
void incbin(char *filename);
void dc(int count, int value);
char *getmraslocal();
void write_tap(int len, int org, unsigned char *data);
void write_250(int low, int high);
void writewavs(int pad250, int pad500, int pad1500);
void reset_import();
int imported(char *filename);
int sized_byteswap(int value);

#define RELCMD_PUBLIC	(0)
#define RELCMD_COMMON	(1)
#define RELCMD_PROGNAME	(2)
#define RELCMD_LIBLOOK	(3)
#define RELCMD_EXTLINK	(4)
#define RELCMD_COMSIZE	(5)
#define RELCMD_EXTCHAIN	(6)
#define RELCMD_PUBVALUE	(7)
#define RELCMD_EXTMINUS	(8)
#define RELCMD_EXTPLUS	(9)
#define RELCMD_DATASIZE	(10)
#define RELCMD_SETLOC	(11)
#define RELCMD_CODESIZE	(13)
#define RELCMD_ENDMOD	(14)
#define RELCMD_ENDPROG	(15)

/*
 *  add a character to the output line buffer
 */
void addtoline(int ac)
{
	/* check for EOF from stdio */
	if (ac == -1)
		ac = 0 ;
	if (lineptr >= linemax)
		error("line buffer overflow");
	*lineptr++ = ac;
}

int get_tstates(unsigned char *inst, int *low, int *high, int *fetch, char *dis)
{
	return zi_tstates(inst, z80, low, high, fetch, dis);
}

/*
 *  put values in buffer for outputing
 */

void emit(int bytes, int desc, struct expr *data, ...)
{
	int type, i, args, dsize;
	va_list ap;

	if (relopt && segchange) {
		segchange = 0;
		putrelcmd(RELCMD_SETLOC);
		putrelsegref(segment, seg_pos[segment]);
	}

	// External references only supported in .rel output.
	if (!relopt && data && (data->e_scope & SCOPE_EXTERNAL)) {
		sprintf(detail, "External references only allowed in .rel output\n");
		errwarn(uflag, detail);
	}

	va_start(ap, data);

	type = desc == E_DATA ? MEM_DATA : MEM_INST;

	// Check emit is not adding instructions to the buffer.
	if (desc != E_DATA && emitptr != emitbuf)
		fprintf(stderr, "internal inconsistency in t-state counting\n");

	dsize = 0;
	args = bytes;
	if (desc == E_DATA) {
		args = 0;
		dsize = bytes;
	}
	else if (desc == E_CODE16)
		dsize = 2;
	else if (desc == E_CODE8)
		dsize = 1;

	for (i = 0; i < args; i++)
	{
		if (emitptr >= &emitbuf[EMITBUFFERSIZE])
			error("emit buffer overflow");
		else {
			int addr = (emit_addr + (emitptr - emitbuf)) & 0xffff;
			*emitptr = va_arg(ap, int);
			if (segment == SEG_CODE) 
				setmem(addr, *emitptr, type);
			putrel(*emitptr);
			putout(*emitptr);
			emitptr++;
		}
	}

	va_end(ap);

	for (i = 0; i < dsize; i++) {
		int addr = (emit_addr + (emitptr - emitbuf)) & 0xffff;
		*emitptr = data->e_value >> (i * 8);
		if (segment == SEG_CODE)
			setmem(addr, *emitptr, type);
		putout(*emitptr);
		emitptr++;
	}

	if (desc != E_DATA)
	{
		int eaddr = emit_addr, low, fetch, addr_after;

		get_tstates(emitbuf, &low, 0, &fetch, 0);

		// emitbuf is OK since this only happens with single emits
		int op = emitbuf[0] & 0xff;

		switch (z80) {
		case 0:
			// 8080 mode, error if Z-80 instructions.
			if (op == 0x08 || op == 0x10 || op == 0x18 ||
			    op == 0x20 || op == 0x28 || op == 0x30 ||
			    op == 0x38 || op == 0xCB || op == 0xD9 ||
			    op == 0xDD || op == 0xED || op == 0xFD)
			{
				errwarn(zflag, "Invalid 8080 instruction");
			}
			break;
		case 1: // Z-80 mode, error if Z-180 instructions
			if (op == 0xED && (
			    (emitbuf[1] & 0xC6) == 0 || // IN0, OUT0
			    (emitbuf[1] & 0xC7) == 4 || // TST r
			    (emitbuf[1] & 0xCF) == 0x4C || // MLT rr
			    emitbuf[1] == 0x64 || // TST m
			    emitbuf[1] == 0x74 || // TSTIO (m)
			    emitbuf[1] == 0x83 || // OTIM
			    emitbuf[1] == 0x93 || // OTIMR
			    emitbuf[1] == 0x8B || // OTDM
			    emitbuf[1] == 0x9B || // OTDMR
			    emitbuf[1] == 0x76)) // SLP
			{
				errwarn(zflag, "Invalid Z-80 instruction");
			}
			break;
		case 2:
			// Z-180 mode, error if undocumented Z-80 instructions
			// So many undocumented Z-80 instructions that I lean
			// on get_states() to answer.
			if (low <= 0)
				errwarn(zflag, "Invalid Z-180 instruction");
			break;
		}

		// Special case to catch promotion of djnz to DEC B JP NZ
		// Even update the tstatesum[] counter though that seems to
		// me to be above and beyond.
		if (emitbuf[0] == 5 && args == 2) {
			tstatesum[eaddr] = tstates;
			ocfsum[eaddr] = ocf;
			memflag[eaddr] |= MEM_T_SET;
			eaddr++;
			tstates += low;
			ocf += fetch;
			low = 10;
			// still 1 fetch
		}

		// Sanity check
		if (low <= 0 && !err[zflag])
		{
			fprintf(stderr, "undefined instruction on %02x %02x (assembler or disassembler broken)\n",
				emitbuf[0], emitbuf[1]);
		}


		// Double setting of both sides of tstatesum[] seems like too
		// much, but must be done in the isolated instruction case:
		// org x ; inc a ; org y

		tstatesum[eaddr] = tstates;
		ocfsum[eaddr] = ocf;
		memflag[eaddr] |= MEM_T_SET;

		// Well, OK, should we default to high or low???
		// Guess it should be whatever makes sense for you
		// to get here which, generally, is the low.

		// low it is.

		tstates += low;
		ocf += fetch;

		addr_after = (emit_addr + (emitptr - emitbuf)) & 0xffff;

		tstatesum[addr_after] = tstates;
		ocfsum[addr_after] = ocf;
		memflag[addr_after] |= MEM_T_SET;
	}

	if (relopt && outpass && dsize > 0) {
		if (dsize == 1) {
			if (is_number(data))
				putrel(data->e_value);
			else if (can_extend_link(data)) {
				extend_link(data);
				putrelop(RELOP_BYTE);
				putrel(0);
			}
			else {
				err[rflag]++;

				putrel(0);
			}
		}
		else if (dsize == 2) {
			int handled = 0;
			if (data->e_scope & SCOPE_EXTERNAL) {
				struct item *var = 0;
				int offset = 0;
				// Simple external reference.
				if (is_external(data))
					var = data->e_item;
				else if (is_external(data->e_left) &&
					data->e_token == '+' &&
					is_number(data->e_right))
				{
					var = data->e_left->e_item;
					offset = data->e_right->e_value;
				}
				else if (is_number(data->e_left) &&
					data->e_token == '+' &&
					is_external(data->e_right))
				{
					offset = data->e_left->e_value;
					var = data->e_right->e_item;
				}
				else if (is_external(data->e_left) &&
					data->e_token == '-' &&
					is_number(data->e_right))
				{
					var = data->e_left->e_item;
					offset = data->e_right->e_value;
				}

				if (var && offset) {
					putrelcmd(data->e_token == '-' ?
						RELCMD_EXTMINUS : RELCMD_EXTPLUS);
					// Theoretically we could put a
					// program or data relative value here...
					putrelsegref(SEG_ABS, offset);
				}

				if (var) {
					if (var->i_chain == 0) {
						putrel(0);
						putrel(0);
					}
					else {
						putrelbits(1, 1);
						putrelextaddr(var->i_chain);
					}
					var->i_chain = (segment << 16) |
						((dollarsign + args) & 0xffff);
					handled = 1;
				}
			}
			else if ((data->e_scope & ~SCOPE_PUBLIC) == 0) {
				// nice constant value
				putrel(data->e_value);
				putrel(data->e_value >> 8);
				handled = 1;
			}
			else if (!(data->e_scope & SCOPE_NORELOC)) {
				// relocatable value
				putrelbits(1, 1);
				putrelbits(2, data->e_scope);
				putrelbits(8, data->e_value);
				putrelbits(8, data->e_value >> 8);
				handled = 1;
			}

			if (!handled) {
				if (can_extend_link(data)) {
					extend_link(data);
					putrelop(RELOP_WORD);
					putrel(0);
					putrel(0);
				}
				else {
					err[rflag]++;
					putrel(data->e_value);
					putrel(data->e_value >> 8);
				}
			}
		}
		else if (dsize == 4) {
			// Only numbers are allowed.
			if (data->e_scope != 0) {
				err[vflag]++;
				if (data->e_scope & SCOPE_NORELOC)
					err[rflag]++;
			}
			for (i = 0; i < dsize; i++)
				putrel(data->e_value >> (i * 8));
		}
		else
			error("internal dsize error");
	}
}

#define ET_NOARG_DISP	(0)
#define ET_NOARG	(1)
#define ET_BYTE		(2)
#define ET_WORD		(5)

void emit1(int opcode, int regvalh, struct expr *data, int type)
{
	if (type == ET_BYTE && (data->e_value < -128 || data->e_value > 255))
		err[vflag]++;

	if (regvalh & 0x10000) {
		switch (type) {
		case ET_NOARG_DISP:
			emit(2, E_CODE, 0, regvalh >> 8, opcode);
			break;
		case ET_BYTE:
			emit(2, E_CODE8, data, regvalh >> 8, opcode);
			break;
		}
	}
	else if (regvalh & 0x8000) {
		switch (type) {
		case ET_NOARG_DISP:
			if (opcode & 0x8000)
				emit(4, E_CODE, 0, regvalh >> 8, opcode >> 8, disp, opcode);
			else
				emit(3, E_CODE, 0, regvalh >> 8, opcode, disp);
			break;
		case ET_NOARG:
			emit(2, E_CODE, 0, regvalh >> 8, opcode);
			break;
		case ET_BYTE:
			emit(3, E_CODE8, data, regvalh >> 8, opcode, disp);
			break;
		case ET_WORD:
			emit(2, E_CODE16, data, regvalh >> 8, opcode);
		}
	} else
		switch(type) {
		case ET_NOARG_DISP:
		case ET_NOARG:
			if (opcode & 0100000)
				emit(2, E_CODE, 0, opcode >> 8, opcode);
			else
				emit(1, E_CODE, 0, opcode);
			break;
		case ET_BYTE:
			emit(1, E_CODE8, data, opcode);
			break;
		case ET_WORD:
			if (opcode & 0100000)
				emit(2, E_CODE16, data, opcode >> 8, opcode);
			else
				emit(1, E_CODE16, data, opcode);
		}
}




void emitdad(int rp1,int rp2)
{
	if (rp1 & 0x8000)
		emit(2, E_CODE, 0, rp1 >> 8, rp2 + 9);
	else
		emit(1, E_CODE, 0, rp2 + 9);
}


void emitjr(int opcode, struct expr *dest)
{
	int disp = dest->e_value - dollarsign - 2;

	if (dest->e_scope & SCOPE_NORELOC)
		err[rflag]++;

	// Can't relative jump to other segments or an external
	// However, without .rel output we default to the code segment
	// for various reasons thus we let "jr 0" (and such) be acceptable
	// in those cases.
	if (((relopt && (dest->e_scope & SCOPE_SEGMASK) != segment) ||
		(dest->e_scope & SCOPE_EXTERNAL) ||
		disp > 127 || disp < -128) && z80)
	{
		if (npass > 1 && jopt) {
			njrpromo++;
			switch (opcode) {
			case 0x10: // DJNZ
				emit(2, E_CODE16, dest, 0x05, 0xC2); // DEC B, JP NZ
				break;
			case 0x18: // JR
				emit(1, E_CODE16, dest, 0xC3); // JP
				break;
			case 0x20: // JR NZ
				emit(1, E_CODE16, dest, 0xC2); // JP NZ
				break;
			case 0x28: // JR Z
				emit(1, E_CODE16, dest, 0xCA); // JP Z
				break;
			case 0x30: // JR NC
				emit(1, E_CODE16, dest, 0xD2); // JP NC
				break;
			case 0x38: // JR C
				emit(1, E_CODE16, dest, 0xDA); // JP C
				break;
			default:
				err[vflag]++;	// shouldn't happen!
				expr_free(dest);
				break;
			}
		}
		else {
			emit(2, E_CODE, 0, opcode, -2);  // branch to itself
			err[vflag]++;
			expr_free(dest);
		}
	}
	else {
		emit(2, E_CODE, 0, opcode, disp);
		expr_free(dest);
	}
}

void checkjp(int op, struct expr *dest)
{
	op &= 0x030;
	// Only applies to Z-80 output and if JP optimization checking is on.
	// JR only has z, nz, nc, c
	// A jump to the current segment might have been optimizable
	if (z80 && JPopt && (op == 0 || op == 010 || op == 020 || op == 030) &&
		(dest->e_scope & (SCOPE_SEGMASK | SCOPE_EXTERNAL)) == segment)
	{
		int disp = dest->e_value - dollarsign - 2;
		if (disp >= -128 && disp <= 127)
			err[jflag]++;
	}
}

/*
 *  put out a byte of binary 
 */
void putbin(int v)
{
	if(!outpass) return;
	*outbinp++ = v;
	if (outbinp >= outbinm) flushbin();

	outoth[outoth_cnt++] = v;
	if (outoth_cnt == 256) flushoth();
}



/*
 *  output one line of binary in INTEL standard form
 */
void flushbin()
{
	char *p;
	int check=outbinp-outbin;

	if (!outpass)
		return;
	nbytes += check;
	if (check) {
		if (fbuf) {
			putc(':', fbuf);
			puthex(check, fbuf);
			puthex(olddollar>>8, fbuf);
			puthex(olddollar, fbuf);
			puthex(0, fbuf);
			check += (olddollar >> 8) + olddollar;
			olddollar += (outbinp-outbin);
			for (p=outbin; p<outbinp; p++) {
				puthex(*p, fbuf);
				check += *p;
			}
			puthex(256-check, fbuf);
			putc('\n', fbuf);
		}
		outbinp = outbin;
	}
}



/*
 *  put out one byte of hex
 */
void puthex(int byte, FILE *buf)
{
	putc(hexadec[(byte >> 4) & 017], buf);
	putc(hexadec[byte & 017], buf);
}

// Case-independent string comparisons.

int ci_strcompare(char *s1, char *s2, int len)
{
	int c1, c2;
	do {
		if (len == 0)
			return 0;

		c1 = *s1++;
		if (c1 >= 'A' && c1 <= 'Z') c1 += 'a' - 'A';
		c2 = *s2++;
		if (c2 >= 'A' && c2 <= 'Z') c2 += 'a' - 'A';
		if (c1 != c2)
			return c1 - c2;

		if (len > 0)
			if (--len == 0)
				return 0;

	} while (c1 && c2);

	return 0;
}

int ci_strcmp(char *s1, char *s2)
{
	return ci_strcompare(s1, s2, -1);
}

void flushoth()
{
	int i, checksum;

	if (!outpass || outoth_cnt == 0)
		return;

	if (fcmd) {
		fprintf(fcmd, "%c%c%c%c", 1, outoth_cnt + 2, oldothdollar, oldothdollar >> 8);
		fwrite(outoth, outoth_cnt, 1, fcmd);
	}

	putcas(0x3c);
	putcas(outoth_cnt);
	putcas(oldothdollar);
	putcas(oldothdollar >> 8);
	checksum = oldothdollar + (oldothdollar >> 8);
	for (i = 0; i < outoth_cnt; i++) {
		putcas(outoth[i]);
		checksum += outoth[i];
		if (fmds)
			fprintf(fmds, "b@$%04x=$%02x\n", oldothdollar + i, outoth[i] & 0xff);
	}
	putcas(checksum);

	oldothdollar += outoth_cnt;
	outoth_cnt = 0;
}

int casbit, casbitcnt = 0;

void putcas(int byte)
{
	if (flcas)
		fputc(byte, flcas);

	if (flnwcas)
		fputc(byte, flnwcas);

	if (fcas) {
		// Buffer 0 stop bit and the 8 data bits.
		casbit = (casbit << 9) | (byte & 0xff);
		casbitcnt += 9;
		while (casbitcnt >= 8) {
			casbitcnt -= 8;
			fputc(casbit >> casbitcnt, fcas);
		}
	}
}

void casname(char *out, char *src, int maxlen)
{
	char *base = basename(src);
	int i;

	out[0] = 'N';
	for (i = 1; i < maxlen; i++)
		out[i] = ' ';

	for (i = 0; *base && i < maxlen; base++) {
		if (ci_strcmp(base, ".z") == 0 || ci_strcmp(base, ".asm") == 0)
			break;

		if (*base >= 'a' && *base <= 'z') {
			*out++ = *base - ('a' - 'A');
			i++;
		}
		else if (*base >= 'A' && *base <= 'Z') {
			*out++ = *base;
			i++;
		}
	}
}

int relbit, relbitcnt = 0;

void putrelbits(int count, int bits)
{
	if (!outpass || !relopt)
		return;

	relbit <<= count;
	relbit |= bits & ((1 << count) - 1);
	relbitcnt += count;

	while (relbitcnt >= 8) {
		relbitcnt -= 8;
		fputc(relbit >> relbitcnt, frel);
	}
}

void putrel(int byte)
{
	// Add 0 bit indicating byte to load at next counter
	putrelbits(1, 0);
	// Add byte to load
	putrelbits(8, byte);
}

void putrelname(char *str)
{
	int len = strlen(str);
	int maxlen = mras ? 7 : relopt;

	// .rel file format can do strings 7 long but for compatibility
	// we restrict them to 6.  I believe this is important because
	// extended link functions require a character when they wish to
	// operate on an external symbol.
	if (len > maxlen)
		len = maxlen;
	putrelbits(3, len);
	while (len-- > 0) {
		int ch = *str++;
		if (ch >= 'a' && ch <= 'z')
			ch -= 'a' - 'A';
		putrelbits(8, ch);
	}
}

void putrelsegref(int scope, int addr)
{
	putrelbits(2, scope);
	putrelbits(8, addr);
	putrelbits(8, addr >> 8);
}

void putrelextaddr(int extaddr)
{
	putrelsegref(extaddr >> 16, extaddr);
}


void putrelcmd(int relcmd)
{
	putrelbits(1, 1);
	putrelbits(2, 0);
	putrelbits(4, relcmd);
}

void flushrel(void)
{
	if (relbitcnt > 0)
		putrelbits(8 - relbitcnt, 0);

	if (relopt)
		fflush(frel);
}

void list_cap_line()
{
	if (multiline) {
		if (lineptr > linebuf)
			lineptr[-1] = separator;
		addtoline('\n');
	}
	addtoline('\0');

	prev_multiline = multiline;
	multiline = 0;
}

/*
 *  put out a line of output -- also put out binary
 */
void list(int optarg)
{
	list_cap_line();

	list_out(optarg, linebuf, ' ');

	lineptr = linebuf;
}

void delayed_list(int optarg)
{
	int delay = iflist() && !mopt;
	FILE *tmp = fout;

	fout = delay ? NULL : tmp;
	list(optarg);
	fout = tmp;
	bookmark(delay);
}

void list_optarg(int optarg, int seg, int type)
{
	if (seg < 0 || !relopt)
		seg = relopt ? segment : SEG_ABS;

	puthex(optarg >> 8, fout);
	puthex(optarg, fout);
	fputc(SEGCHAR(seg), fout);
	if (type)
		fputc(type, fout);
}

void bds_perm(int dollar, int addr, int len)
{
	while (len > 0) {
		int blklen;
		int usage = memflag[addr & 0xffff] & (MEM_INST | MEM_DATA);

		for (blklen = 0; blklen < len; blklen++) {
			int u = memflag[(addr + blklen) & 0xffff] & (MEM_INST | MEM_DATA);
			if (u != usage)
				break;
		}

		int bu = 0;
		if (usage & MEM_INST) bu |= 1;
		if (usage & MEM_DATA) bu |= 2;

		while (blklen > 0) {
			int size = blklen;
			if (size > 255) size = 255;
			fprintf(fbds, "%04x %04x u %02x %02x\n",
				dollar, addr & 0xffff, size, bu);

			addr += size;
			dollar += size;
			len -= size;

			blklen -= size;
		}
	}
}

void list_out(int optarg, char *line_str, char type)
{
	unsigned char *	p;
	int	i;
	int  lst;

	if (outpass) {
		lst = iflist();
		if (lst) {
			lineout();
			if (nopt)
				fprintf(fout, "%4d:", linein[now_in]);

			if (copt)
			{
			    if (emitptr > emitbuf && (memflag[emit_addr] & MEM_INST))
			    {
			        int low, high, fetch;
			        get_tstates(memory + emit_addr, &low, &high, &fetch, 0);

				// Special case to catch promotion of djnz to DEC B JP NZ
				if (memory[emit_addr] == 5 && emitptr - emitbuf == 4) {
					low += 10;
					high += 10;
				}

			    	fprintf(fout, nopt ? "%5d" : "%4d", tstatesum[emit_addr]);

				fprintf(fout, "+%d", low);
				if (low != high)
				    fprintf(fout, "+%d", high - low);
			    }
			    else
			    {
			        fprintf(fout, nopt ? "%5s-" : "%4s-", "");
			    }
			}

			if (nopt || copt)
				fprintf(fout, "\t");

			list_optarg(optarg, -1, type);

			for (p = emitbuf; (p < emitptr) && (p - emitbuf < 4); p++) {
				puthex(*p, fout);
			}
			for (i = 4 - (p-emitbuf); i > 0; i--)
				fputs("  ", fout);

			putc('\t', fout);
			fputs(line_str, fout);
		}

		if (fbds) {
			if (emitptr > emitbuf) {
				fprintf(fbds, "%04x %04x d ", dollarsign, emit_addr);
				for (p = emitbuf; p < emitptr; p++)
					fprintf(fbds, "%02x", *p & 0xff);
				fprintf(fbds, "\n");

				bds_perm(dollarsign, emit_addr, emitptr - emitbuf);
			}
			fprintf(fbds, "%04x %04x s %s", dollarsign, emit_addr, line_str);
		}

		for (p = emitbuf; p < emitptr; p++)
			putbin(*p);

		p = emitbuf+4;
		while (lst && gopt && p < emitptr) {
			lineout();
			if (nopt) putc('\t', fout);
			fputs("      ", fout);
			if (copt) fputs("        ", fout);
			for (i = 0; (i < 4) && (p < emitptr);i++) {
				puthex(*p, fout);
				p++;
			}
			putc('\n', fout);
		}

		lsterr2(lst);
	} else
		lsterr1();

	dollarsign += emitptr - emitbuf;
	emit_addr += emitptr - emitbuf;
	dollarsign &= 0xffff;
	emit_addr &= 0xffff;
	emitptr = emitbuf;
}



/*
 *  keep track of line numbers and put out headers as necessary
 */
void lineout()
{
	if (!printer_output || !fout)
		return;

	if (line == 60) {
		if (popt)
			putc('\014', fout);	/* send the form feed */
		else
			fputs("\n\n\n\n\n", fout);
		line = 0;
	}
	if (line == 0) {
		fprintf(fout, "\n\n%s %s\t%s\t Page %d\n\n\n",
			&timp[4], &timp[20], title, page++);
		line = 4;
	}
	line++;
}


/*
 *  cause a page eject
 */
void eject()
{
	if (!printer_output)
		return;

	if (outpass && iflist()) {
		if (popt) {
			putc('\014', fout);	/* send the form feed */
		} else {
			while (line < 65) {
				line++;
				putc('\n', fout);
			}
		}
	}
	line = 0;
}


/*
 *  space n lines on the list file
 */
void space(int n)
{
	int	i ;
	if (outpass && iflist() && fout)
		for (i = 0; i<n; i++) {
			lineout();
			putc('\n', fout);
		}
}

/*
 *  Error handling - pass 1
 */
void lsterr1()
{
	int i;
	for (i = 0; i <= mflag; i++)
		if (err[i]) {
			if (topt)
				errorprt(i);
			passfail = 1;
			err[i] = 0;
		}
}


/*
 *  Error handling - pass 2.
 */
void lsterr2(int lst)
{
	int i;
	for (i=0; i<FLAGS; i++)
		if (err[i]) {
			if (i < FIRSTWARN)
				passfail = 1;
			if (lst) {
				char *desc = errname[i];
				char *type = i < FIRSTWARN ? " error" : " warning";
				if (errdetail[i][0]) {
					desc = errdetail[i];
					type = "";
				}
				lineout();
				if (fout)
					fprintf(fout, "%c %s%s\n",
						errlet[i], desc, type);
			}
			err[i] = 0;
			keeperr[i]++;
			if (i > mflag && topt)
				errorprt(i);
		}

	if (fout)
		fflush(fout);	/*to avoid putc(har) mix bug*/
}

/*
 *  print diagnostic to error terminal
 */
void errorprt(int errnum)
{
	char *desc = errname[errnum];
	char *type = errnum < FIRSTWARN ? " error" : " warning";
	if (errdetail[errnum][0]) {
		desc = errdetail[errnum];
		type = "";
	}
	fprintf(stderr, "%s(%d) : %s%s",
		src_name[now_in], linein[now_in], desc, type);

	errdetail[errnum][0] = '\0';

	fprintf(stderr, "\n");
	fprintf(stderr, "%s\n", linebuf);
	fflush(stderr) ;
}

void errwarn(int errnum, char *message)
{
	if (errnum == uflag && mras_undecl_ok)
		return;

	err[errnum]++;
	strcpy(errdetail[errnum], message);
}

/*
 *  list without address -- for comments and if skipped lines
 */
void list1()
{
	int lst;

	list_cap_line();
	lineptr = linebuf;
	if (outpass) {
		if ((lst = iflist())) {
			lineout();
			if (nopt)
				fprintf(fout, "%4d:\t", linein[now_in]);
			if (copt)
				fprintf(fout, "\t");
			fprintf(fout, "\t\t%s", linebuf);
			lsterr2(lst);
		}
		if (fbds)
			fprintf(fbds, "%04x %04x s %s", dollarsign, emit_addr, linebuf);
	}
	else
		lsterr1();
}

void delayed_list1()
{
	int delay = iflist() && !mopt;
	FILE *tmp = fout;

	fout = delay ? NULL : tmp;
	list1();
	fout = tmp;

	bookmark(delay);
}

/*
 *  see if listing is desired
 */
int iflist()
{
	int problem;

	if (!fout)
		return 0;

	if (inmlex)
		return mlex_list_on;

	if (lston)
		return(1) ;
	if (*ifptr && !fopt)
		return(0);
	if (!lstoff && !expptr)
		return(1);
	problem = countwarn() + counterr();
	if (expptr) {
		if (problem) return(1);
		if (!mopt) return(0);
		if (mopt == 1) return(1);
		return(emitptr > emitbuf);
	}
	if (eopt && problem)
		return(1);
	return(0);
}

void do_equ(struct item *sym, struct expr *val, int call_list);
void do_defl(struct item *sym, struct expr *val, int call_list);

// GWP - This avoids an apparent bug in bison as it tries to start out the
// Not needed under gcc which defines __STDC__ so I guard it to prevent
// warnings.
// yyparse() function with yyparse() ; { }.
#ifndef __STDC__
#define __STDC__
#endif

#define PSTITL	(0)	/* title */
#define PSRSYM	(1)	/* rsym */
#define PSWSYM	(2)	/* wsym */
#define PSINC	(3)	/* include file */
#define PSMACLIB (4)	/* maclib (similar to include) */
#define PSIMPORT (5)	/* import file */
#define PSCMN	(6)	/* common block */

#define SPTITL	(0)	/* title */
#define SPSBTL	(1)	/* sub title */
#define SPNAME	(2)	/* name */
#define SPCOM	(3)	/* comment */
#define SPPRAGMA (4)	/* pragma */


#line 1948 "zmac.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    STRING = 258,                  /* STRING  */
    NOOPERAND = 259,               /* NOOPERAND  */
    ARITHC = 260,                  /* ARITHC  */
    ADD = 261,                     /* ADD  */
    LOGICAL = 262,                 /* LOGICAL  */
    AND = 263,                     /* AND  */
    OR = 264,                      /* OR  */
    XOR = 265,                     /* XOR  */
    ANDAND = 266,                  /* ANDAND  */
    OROR = 267,                    /* OROR  */
    BIT = 268,                     /* BIT  */
    CALL = 269,                    /* CALL  */
    INCDEC = 270,                  /* INCDEC  */
    DJNZ = 271,                    /* DJNZ  */
    EX = 272,                      /* EX  */
    IM = 273,                      /* IM  */
    PHASE = 274,                   /* PHASE  */
    DEPHASE = 275,                 /* DEPHASE  */
    TK_IN = 276,                   /* TK_IN  */
    JR = 277,                      /* JR  */
    LD = 278,                      /* LD  */
    TK_OUT = 279,                  /* TK_OUT  */
    PUSHPOP = 280,                 /* PUSHPOP  */
    RET = 281,                     /* RET  */
    SHIFT = 282,                   /* SHIFT  */
    RST = 283,                     /* RST  */
    REGNAME = 284,                 /* REGNAME  */
    IXYLH = 285,                   /* IXYLH  */
    ACC = 286,                     /* ACC  */
    TK_C = 287,                    /* TK_C  */
    RP = 288,                      /* RP  */
    HL = 289,                      /* HL  */
    INDEX = 290,                   /* INDEX  */
    AF = 291,                      /* AF  */
    TK_F = 292,                    /* TK_F  */
    AFp = 293,                     /* AFp  */
    SP = 294,                      /* SP  */
    MISCREG = 295,                 /* MISCREG  */
    COND = 296,                    /* COND  */
    SPCOND = 297,                  /* SPCOND  */
    NUMBER = 298,                  /* NUMBER  */
    UNDECLARED = 299,              /* UNDECLARED  */
    END = 300,                     /* END  */
    ORG = 301,                     /* ORG  */
    ASSERT = 302,                  /* ASSERT  */
    TSTATE = 303,                  /* TSTATE  */
    T = 304,                       /* T  */
    TILO = 305,                    /* TILO  */
    TIHI = 306,                    /* TIHI  */
    SETOCF = 307,                  /* SETOCF  */
    OCF = 308,                     /* OCF  */
    LOW = 309,                     /* LOW  */
    HIGH = 310,                    /* HIGH  */
    DC = 311,                      /* DC  */
    DEFB = 312,                    /* DEFB  */
    DEFD = 313,                    /* DEFD  */
    DEFS = 314,                    /* DEFS  */
    DEFW = 315,                    /* DEFW  */
    DEF3 = 316,                    /* DEF3  */
    EQU = 317,                     /* EQU  */
    DEFL = 318,                    /* DEFL  */
    LABEL = 319,                   /* LABEL  */
    EQUATED = 320,                 /* EQUATED  */
    WASEQUATED = 321,              /* WASEQUATED  */
    DEFLED = 322,                  /* DEFLED  */
    COMMON = 323,                  /* COMMON  */
    MULTDEF = 324,                 /* MULTDEF  */
    SHL = 325,                     /* SHL  */
    SHR = 326,                     /* SHR  */
    LT = 327,                      /* LT  */
    EQ = 328,                      /* EQ  */
    LE = 329,                      /* LE  */
    GE = 330,                      /* GE  */
    NE = 331,                      /* NE  */
    NOT = 332,                     /* NOT  */
    MROP_ADD = 333,                /* MROP_ADD  */
    MROP_SUB = 334,                /* MROP_SUB  */
    MROP_MUL = 335,                /* MROP_MUL  */
    MROP_DIV = 336,                /* MROP_DIV  */
    MROP_MOD = 337,                /* MROP_MOD  */
    MROP_AND = 338,                /* MROP_AND  */
    MROP_OR = 339,                 /* MROP_OR  */
    MROP_XOR = 340,                /* MROP_XOR  */
    MROP_NE = 341,                 /* MROP_NE  */
    MROP_EQ = 342,                 /* MROP_EQ  */
    MROP_LT = 343,                 /* MROP_LT  */
    MROP_GT = 344,                 /* MROP_GT  */
    MROP_LE = 345,                 /* MROP_LE  */
    MROP_GE = 346,                 /* MROP_GE  */
    MROP_SHIFT = 347,              /* MROP_SHIFT  */
    MROP_SHL = 348,                /* MROP_SHL  */
    MROP_SHR = 349,                /* MROP_SHR  */
    MROP_NOT = 350,                /* MROP_NOT  */
    MROP_LOW = 351,                /* MROP_LOW  */
    MROP_HIGH = 352,               /* MROP_HIGH  */
    IF_TK = 353,                   /* IF_TK  */
    IF_DEF_TK = 354,               /* IF_DEF_TK  */
    IF_CP_TK = 355,                /* IF_CP_TK  */
    ELSE_TK = 356,                 /* ELSE_TK  */
    ENDIF_TK = 357,                /* ENDIF_TK  */
    ARGPSEUDO = 358,               /* ARGPSEUDO  */
    INCBIN = 359,                  /* INCBIN  */
    LIST = 360,                    /* LIST  */
    MINMAX = 361,                  /* MINMAX  */
    MACRO = 362,                   /* MACRO  */
    MNAME = 363,                   /* MNAME  */
    ARG = 364,                     /* ARG  */
    ENDM = 365,                    /* ENDM  */
    ONECHAR = 366,                 /* ONECHAR  */
    TWOCHAR = 367,                 /* TWOCHAR  */
    JRPROMOTE = 368,               /* JRPROMOTE  */
    JPERROR = 369,                 /* JPERROR  */
    PUBLIC = 370,                  /* PUBLIC  */
    EXTRN = 371,                   /* EXTRN  */
    MRAS_MOD = 372,                /* MRAS_MOD  */
    SETSEG = 373,                  /* SETSEG  */
    INSTSET = 374,                 /* INSTSET  */
    LXI = 375,                     /* LXI  */
    DAD = 376,                     /* DAD  */
    STAX = 377,                    /* STAX  */
    STA = 378,                     /* STA  */
    SHLD = 379,                    /* SHLD  */
    LDAX = 380,                    /* LDAX  */
    LHLD = 381,                    /* LHLD  */
    LDA = 382,                     /* LDA  */
    MVI = 383,                     /* MVI  */
    MOV = 384,                     /* MOV  */
    INXDCX = 385,                  /* INXDCX  */
    INRDCR = 386,                  /* INRDCR  */
    PSW = 387,                     /* PSW  */
    JUMP8 = 388,                   /* JUMP8  */
    JP = 389,                      /* JP  */
    CALL8 = 390,                   /* CALL8  */
    ALUI8 = 391,                   /* ALUI8  */
    SPECIAL = 392,                 /* SPECIAL  */
    RAWTOKEN = 393,                /* RAWTOKEN  */
    LOCAL = 394,                   /* LOCAL  */
    LD_XY = 395,                   /* LD_XY  */
    ST_XY = 396,                   /* ST_XY  */
    MV_XY = 397,                   /* MV_XY  */
    ALU_XY = 398,                  /* ALU_XY  */
    BIT_XY = 399,                  /* BIT_XY  */
    SHIFT_XY = 400,                /* SHIFT_XY  */
    INP = 401,                     /* INP  */
    OUTP = 402,                    /* OUTP  */
    JR_COND = 403,                 /* JR_COND  */
    LDST16 = 404,                  /* LDST16  */
    ARITH16 = 405,                 /* ARITH16  */
    REPT = 406,                    /* REPT  */
    IRPC = 407,                    /* IRPC  */
    IRP = 408,                     /* IRP  */
    EXITM = 409,                   /* EXITM  */
    NUL = 410,                     /* NUL  */
    MPARM = 411,                   /* MPARM  */
    TK_IN0 = 412,                  /* TK_IN0  */
    TK_OUT0 = 413,                 /* TK_OUT0  */
    MLT = 414,                     /* MLT  */
    TST = 415,                     /* TST  */
    TSTIO = 416,                   /* TSTIO  */
    GT = 417,                      /* GT  */
    UNARY = 418                    /* UNARY  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 1878 "zmac.y"

	struct expr *exprptr;
	struct item *itemptr;
	int ival;
	char *cval;
	

#line 2166 "zmac.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);



/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_STRING = 3,                     /* STRING  */
  YYSYMBOL_NOOPERAND = 4,                  /* NOOPERAND  */
  YYSYMBOL_ARITHC = 5,                     /* ARITHC  */
  YYSYMBOL_ADD = 6,                        /* ADD  */
  YYSYMBOL_LOGICAL = 7,                    /* LOGICAL  */
  YYSYMBOL_AND = 8,                        /* AND  */
  YYSYMBOL_OR = 9,                         /* OR  */
  YYSYMBOL_XOR = 10,                       /* XOR  */
  YYSYMBOL_ANDAND = 11,                    /* ANDAND  */
  YYSYMBOL_OROR = 12,                      /* OROR  */
  YYSYMBOL_BIT = 13,                       /* BIT  */
  YYSYMBOL_CALL = 14,                      /* CALL  */
  YYSYMBOL_INCDEC = 15,                    /* INCDEC  */
  YYSYMBOL_DJNZ = 16,                      /* DJNZ  */
  YYSYMBOL_EX = 17,                        /* EX  */
  YYSYMBOL_IM = 18,                        /* IM  */
  YYSYMBOL_PHASE = 19,                     /* PHASE  */
  YYSYMBOL_DEPHASE = 20,                   /* DEPHASE  */
  YYSYMBOL_TK_IN = 21,                     /* TK_IN  */
  YYSYMBOL_JR = 22,                        /* JR  */
  YYSYMBOL_LD = 23,                        /* LD  */
  YYSYMBOL_TK_OUT = 24,                    /* TK_OUT  */
  YYSYMBOL_PUSHPOP = 25,                   /* PUSHPOP  */
  YYSYMBOL_RET = 26,                       /* RET  */
  YYSYMBOL_SHIFT = 27,                     /* SHIFT  */
  YYSYMBOL_RST = 28,                       /* RST  */
  YYSYMBOL_REGNAME = 29,                   /* REGNAME  */
  YYSYMBOL_IXYLH = 30,                     /* IXYLH  */
  YYSYMBOL_ACC = 31,                       /* ACC  */
  YYSYMBOL_TK_C = 32,                      /* TK_C  */
  YYSYMBOL_RP = 33,                        /* RP  */
  YYSYMBOL_HL = 34,                        /* HL  */
  YYSYMBOL_INDEX = 35,                     /* INDEX  */
  YYSYMBOL_AF = 36,                        /* AF  */
  YYSYMBOL_TK_F = 37,                      /* TK_F  */
  YYSYMBOL_AFp = 38,                       /* AFp  */
  YYSYMBOL_SP = 39,                        /* SP  */
  YYSYMBOL_MISCREG = 40,                   /* MISCREG  */
  YYSYMBOL_COND = 41,                      /* COND  */
  YYSYMBOL_SPCOND = 42,                    /* SPCOND  */
  YYSYMBOL_NUMBER = 43,                    /* NUMBER  */
  YYSYMBOL_UNDECLARED = 44,                /* UNDECLARED  */
  YYSYMBOL_END = 45,                       /* END  */
  YYSYMBOL_ORG = 46,                       /* ORG  */
  YYSYMBOL_ASSERT = 47,                    /* ASSERT  */
  YYSYMBOL_TSTATE = 48,                    /* TSTATE  */
  YYSYMBOL_T = 49,                         /* T  */
  YYSYMBOL_TILO = 50,                      /* TILO  */
  YYSYMBOL_TIHI = 51,                      /* TIHI  */
  YYSYMBOL_SETOCF = 52,                    /* SETOCF  */
  YYSYMBOL_OCF = 53,                       /* OCF  */
  YYSYMBOL_LOW = 54,                       /* LOW  */
  YYSYMBOL_HIGH = 55,                      /* HIGH  */
  YYSYMBOL_DC = 56,                        /* DC  */
  YYSYMBOL_DEFB = 57,                      /* DEFB  */
  YYSYMBOL_DEFD = 58,                      /* DEFD  */
  YYSYMBOL_DEFS = 59,                      /* DEFS  */
  YYSYMBOL_DEFW = 60,                      /* DEFW  */
  YYSYMBOL_DEF3 = 61,                      /* DEF3  */
  YYSYMBOL_EQU = 62,                       /* EQU  */
  YYSYMBOL_DEFL = 63,                      /* DEFL  */
  YYSYMBOL_LABEL = 64,                     /* LABEL  */
  YYSYMBOL_EQUATED = 65,                   /* EQUATED  */
  YYSYMBOL_WASEQUATED = 66,                /* WASEQUATED  */
  YYSYMBOL_DEFLED = 67,                    /* DEFLED  */
  YYSYMBOL_COMMON = 68,                    /* COMMON  */
  YYSYMBOL_MULTDEF = 69,                   /* MULTDEF  */
  YYSYMBOL_SHL = 70,                       /* SHL  */
  YYSYMBOL_SHR = 71,                       /* SHR  */
  YYSYMBOL_LT = 72,                        /* LT  */
  YYSYMBOL_EQ = 73,                        /* EQ  */
  YYSYMBOL_LE = 74,                        /* LE  */
  YYSYMBOL_GE = 75,                        /* GE  */
  YYSYMBOL_NE = 76,                        /* NE  */
  YYSYMBOL_NOT = 77,                       /* NOT  */
  YYSYMBOL_MROP_ADD = 78,                  /* MROP_ADD  */
  YYSYMBOL_MROP_SUB = 79,                  /* MROP_SUB  */
  YYSYMBOL_MROP_MUL = 80,                  /* MROP_MUL  */
  YYSYMBOL_MROP_DIV = 81,                  /* MROP_DIV  */
  YYSYMBOL_MROP_MOD = 82,                  /* MROP_MOD  */
  YYSYMBOL_MROP_AND = 83,                  /* MROP_AND  */
  YYSYMBOL_MROP_OR = 84,                   /* MROP_OR  */
  YYSYMBOL_MROP_XOR = 85,                  /* MROP_XOR  */
  YYSYMBOL_MROP_NE = 86,                   /* MROP_NE  */
  YYSYMBOL_MROP_EQ = 87,                   /* MROP_EQ  */
  YYSYMBOL_MROP_LT = 88,                   /* MROP_LT  */
  YYSYMBOL_MROP_GT = 89,                   /* MROP_GT  */
  YYSYMBOL_MROP_LE = 90,                   /* MROP_LE  */
  YYSYMBOL_MROP_GE = 91,                   /* MROP_GE  */
  YYSYMBOL_MROP_SHIFT = 92,                /* MROP_SHIFT  */
  YYSYMBOL_MROP_SHL = 93,                  /* MROP_SHL  */
  YYSYMBOL_MROP_SHR = 94,                  /* MROP_SHR  */
  YYSYMBOL_MROP_NOT = 95,                  /* MROP_NOT  */
  YYSYMBOL_MROP_LOW = 96,                  /* MROP_LOW  */
  YYSYMBOL_MROP_HIGH = 97,                 /* MROP_HIGH  */
  YYSYMBOL_IF_TK = 98,                     /* IF_TK  */
  YYSYMBOL_IF_DEF_TK = 99,                 /* IF_DEF_TK  */
  YYSYMBOL_IF_CP_TK = 100,                 /* IF_CP_TK  */
  YYSYMBOL_ELSE_TK = 101,                  /* ELSE_TK  */
  YYSYMBOL_ENDIF_TK = 102,                 /* ENDIF_TK  */
  YYSYMBOL_ARGPSEUDO = 103,                /* ARGPSEUDO  */
  YYSYMBOL_INCBIN = 104,                   /* INCBIN  */
  YYSYMBOL_LIST = 105,                     /* LIST  */
  YYSYMBOL_MINMAX = 106,                   /* MINMAX  */
  YYSYMBOL_MACRO = 107,                    /* MACRO  */
  YYSYMBOL_MNAME = 108,                    /* MNAME  */
  YYSYMBOL_ARG = 109,                      /* ARG  */
  YYSYMBOL_ENDM = 110,                     /* ENDM  */
  YYSYMBOL_ONECHAR = 111,                  /* ONECHAR  */
  YYSYMBOL_TWOCHAR = 112,                  /* TWOCHAR  */
  YYSYMBOL_JRPROMOTE = 113,                /* JRPROMOTE  */
  YYSYMBOL_JPERROR = 114,                  /* JPERROR  */
  YYSYMBOL_PUBLIC = 115,                   /* PUBLIC  */
  YYSYMBOL_EXTRN = 116,                    /* EXTRN  */
  YYSYMBOL_MRAS_MOD = 117,                 /* MRAS_MOD  */
  YYSYMBOL_SETSEG = 118,                   /* SETSEG  */
  YYSYMBOL_INSTSET = 119,                  /* INSTSET  */
  YYSYMBOL_LXI = 120,                      /* LXI  */
  YYSYMBOL_DAD = 121,                      /* DAD  */
  YYSYMBOL_STAX = 122,                     /* STAX  */
  YYSYMBOL_STA = 123,                      /* STA  */
  YYSYMBOL_SHLD = 124,                     /* SHLD  */
  YYSYMBOL_LDAX = 125,                     /* LDAX  */
  YYSYMBOL_LHLD = 126,                     /* LHLD  */
  YYSYMBOL_LDA = 127,                      /* LDA  */
  YYSYMBOL_MVI = 128,                      /* MVI  */
  YYSYMBOL_MOV = 129,                      /* MOV  */
  YYSYMBOL_INXDCX = 130,                   /* INXDCX  */
  YYSYMBOL_INRDCR = 131,                   /* INRDCR  */
  YYSYMBOL_PSW = 132,                      /* PSW  */
  YYSYMBOL_JUMP8 = 133,                    /* JUMP8  */
  YYSYMBOL_JP = 134,                       /* JP  */
  YYSYMBOL_CALL8 = 135,                    /* CALL8  */
  YYSYMBOL_ALUI8 = 136,                    /* ALUI8  */
  YYSYMBOL_SPECIAL = 137,                  /* SPECIAL  */
  YYSYMBOL_RAWTOKEN = 138,                 /* RAWTOKEN  */
  YYSYMBOL_LOCAL = 139,                    /* LOCAL  */
  YYSYMBOL_LD_XY = 140,                    /* LD_XY  */
  YYSYMBOL_ST_XY = 141,                    /* ST_XY  */
  YYSYMBOL_MV_XY = 142,                    /* MV_XY  */
  YYSYMBOL_ALU_XY = 143,                   /* ALU_XY  */
  YYSYMBOL_BIT_XY = 144,                   /* BIT_XY  */
  YYSYMBOL_SHIFT_XY = 145,                 /* SHIFT_XY  */
  YYSYMBOL_INP = 146,                      /* INP  */
  YYSYMBOL_OUTP = 147,                     /* OUTP  */
  YYSYMBOL_JR_COND = 148,                  /* JR_COND  */
  YYSYMBOL_LDST16 = 149,                   /* LDST16  */
  YYSYMBOL_ARITH16 = 150,                  /* ARITH16  */
  YYSYMBOL_REPT = 151,                     /* REPT  */
  YYSYMBOL_IRPC = 152,                     /* IRPC  */
  YYSYMBOL_IRP = 153,                      /* IRP  */
  YYSYMBOL_EXITM = 154,                    /* EXITM  */
  YYSYMBOL_NUL = 155,                      /* NUL  */
  YYSYMBOL_MPARM = 156,                    /* MPARM  */
  YYSYMBOL_TK_IN0 = 157,                   /* TK_IN0  */
  YYSYMBOL_TK_OUT0 = 158,                  /* TK_OUT0  */
  YYSYMBOL_MLT = 159,                      /* MLT  */
  YYSYMBOL_TST = 160,                      /* TST  */
  YYSYMBOL_TSTIO = 161,                    /* TSTIO  */
  YYSYMBOL_162_ = 162,                     /* '?'  */
  YYSYMBOL_163_ = 163,                     /* ':'  */
  YYSYMBOL_164_ = 164,                     /* '|'  */
  YYSYMBOL_165_ = 165,                     /* '^'  */
  YYSYMBOL_166_ = 166,                     /* '&'  */
  YYSYMBOL_167_ = 167,                     /* '='  */
  YYSYMBOL_168_ = 168,                     /* '<'  */
  YYSYMBOL_169_ = 169,                     /* '>'  */
  YYSYMBOL_GT = 170,                       /* GT  */
  YYSYMBOL_171_ = 171,                     /* '+'  */
  YYSYMBOL_172_ = 172,                     /* '-'  */
  YYSYMBOL_173_ = 173,                     /* '*'  */
  YYSYMBOL_174_ = 174,                     /* '/'  */
  YYSYMBOL_175_ = 175,                     /* '%'  */
  YYSYMBOL_176_ = 176,                     /* '!'  */
  YYSYMBOL_177_ = 177,                     /* '~'  */
  YYSYMBOL_UNARY = 178,                    /* UNARY  */
  YYSYMBOL_179_n_ = 179,                   /* '\n'  */
  YYSYMBOL_180_ = 180,                     /* ','  */
  YYSYMBOL_181_ = 181,                     /* '('  */
  YYSYMBOL_182_ = 182,                     /* ')'  */
  YYSYMBOL_183_ = 183,                     /* '#'  */
  YYSYMBOL_184_ = 184,                     /* '$'  */
  YYSYMBOL_185_ = 185,                     /* '['  */
  YYSYMBOL_186_ = 186,                     /* ']'  */
  YYSYMBOL_YYACCEPT = 187,                 /* $accept  */
  YYSYMBOL_statements = 188,               /* statements  */
  YYSYMBOL_statement = 189,                /* statement  */
  YYSYMBOL_190_1 = 190,                    /* $@1  */
  YYSYMBOL_191_2 = 191,                    /* $@2  */
  YYSYMBOL_192_3 = 192,                    /* $@3  */
  YYSYMBOL_193_4 = 193,                    /* $@4  */
  YYSYMBOL_194_5 = 194,                    /* $@5  */
  YYSYMBOL_195_6 = 195,                    /* $@6  */
  YYSYMBOL_196_7 = 196,                    /* $@7  */
  YYSYMBOL_197_8 = 197,                    /* $@8  */
  YYSYMBOL_198_9 = 198,                    /* $@9  */
  YYSYMBOL_199_10 = 199,                   /* $@10  */
  YYSYMBOL_maybecolon = 200,               /* maybecolon  */
  YYSYMBOL_201_label_part = 201,           /* label.part  */
  YYSYMBOL_202_public_list = 202,          /* public.list  */
  YYSYMBOL_203_public_part = 203,          /* public.part  */
  YYSYMBOL_204_extrn_list = 204,           /* extrn.list  */
  YYSYMBOL_205_extrn_part = 205,           /* extrn.part  */
  YYSYMBOL_varop = 206,                    /* varop  */
  YYSYMBOL_operation = 207,                /* operation  */
  YYSYMBOL_208_11 = 208,                   /* $@11  */
  YYSYMBOL_209_12 = 209,                   /* $@12  */
  YYSYMBOL_210_13 = 210,                   /* $@13  */
  YYSYMBOL_211_14 = 211,                   /* $@14  */
  YYSYMBOL_212_parm_list = 212,            /* parm.list  */
  YYSYMBOL_213_parm_single = 213,          /* parm.single  */
  YYSYMBOL_214_15 = 214,                   /* $@15  */
  YYSYMBOL_maybeocto = 215,                /* maybeocto  */
  YYSYMBOL_216_parm_element = 216,         /* parm.element  */
  YYSYMBOL_locals = 217,                   /* locals  */
  YYSYMBOL_local_decls = 218,              /* local_decls  */
  YYSYMBOL_219_16 = 219,                   /* $@16  */
  YYSYMBOL_220_local_list = 220,           /* local.list  */
  YYSYMBOL_221_local_element = 221,        /* local.element  */
  YYSYMBOL_222_arg_list = 222,             /* arg.list  */
  YYSYMBOL_223_arg_element = 223,          /* arg.element  */
  YYSYMBOL_224_17 = 224,                   /* $@17  */
  YYSYMBOL_allreg = 225,                   /* allreg  */
  YYSYMBOL_reg = 226,                      /* reg  */
  YYSYMBOL_ixylhreg = 227,                 /* ixylhreg  */
  YYSYMBOL_reg8 = 228,                     /* reg8  */
  YYSYMBOL_m = 229,                        /* m  */
  YYSYMBOL_realreg = 230,                  /* realreg  */
  YYSYMBOL_mem = 231,                      /* mem  */
  YYSYMBOL_memxy = 232,                    /* memxy  */
  YYSYMBOL_dxy = 233,                      /* dxy  */
  YYSYMBOL_evenreg = 234,                  /* evenreg  */
  YYSYMBOL_evenreg8 = 235,                 /* evenreg8  */
  YYSYMBOL_pushable = 236,                 /* pushable  */
  YYSYMBOL_pushable8 = 237,                /* pushable8  */
  YYSYMBOL_bcdesp = 238,                   /* bcdesp  */
  YYSYMBOL_bcdehlsp = 239,                 /* bcdehlsp  */
  YYSYMBOL_bcdehl = 240,                   /* bcdehl  */
  YYSYMBOL_mar = 241,                      /* mar  */
  YYSYMBOL_aliasable = 242,                /* aliasable  */
  YYSYMBOL_condition = 243,                /* condition  */
  YYSYMBOL_spcondition = 244,              /* spcondition  */
  YYSYMBOL_245_db_list = 245,              /* db.list  */
  YYSYMBOL_246_db_list_element = 246,      /* db.list.element  */
  YYSYMBOL_247_dw_list = 247,              /* dw.list  */
  YYSYMBOL_248_dw_list_element = 248,      /* dw.list.element  */
  YYSYMBOL_249_d3_list = 249,              /* d3.list  */
  YYSYMBOL_250_d3_list_element = 250,      /* d3.list.element  */
  YYSYMBOL_251_dd_list = 251,              /* dd.list  */
  YYSYMBOL_252_dd_list_element = 252,      /* dd.list.element  */
  YYSYMBOL_expression = 253,               /* expression  */
  YYSYMBOL_parenexpr = 254,                /* parenexpr  */
  YYSYMBOL_noparenexpr = 255,              /* noparenexpr  */
  YYSYMBOL_256_18 = 256,                   /* $@18  */
  YYSYMBOL_symbol = 257,                   /* symbol  */
  YYSYMBOL_al = 258,                       /* al  */
  YYSYMBOL_arg_on = 259,                   /* arg_on  */
  YYSYMBOL_arg_off = 260,                  /* arg_off  */
  YYSYMBOL_mras_undecl_on = 261,           /* mras_undecl_on  */
  YYSYMBOL_mras_undecl_off = 262           /* mras_undecl_off  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;


/* Second part of user prologue.  */
#line 2024 "zmac.y"

char  *cp;
int  i;

void do_equ(struct item *sym, struct expr *val, int call_list)
{
	expr_reloc_check(val);
	switch(sym->i_token) {
	case UNDECLARED: case WASEQUATED:
		if (sym->i_token == WASEQUATED &&
			(sym->i_value != val->e_value ||
			 ((sym->i_scope ^ val->e_scope) & SCOPE_SEGMASK)))
		{
			if (outpass) {
				if (sym->i_value != val->e_value)
					sprintf(detail, "%s error - %s went from $%04x to $%04x",
						errname[pflag], sym->i_string, sym->i_value, val->e_value);
				else
					sprintf(detail, "%s error - %s changed scope",
						errname[pflag], sym->i_string);
				errwarn(pflag, detail);
			}
			else
				passretry = 1;
		}

		sym->i_token = EQUATED;
		sym->i_pass = npass;
		sym->i_value = val->e_value;
		sym->i_scope |= val->e_scope;
		break;
	default:
		// m80 allows multiple equates as long as the value
		// does not change.  So does newer zmac.
		if (sym->i_value != val->e_value ||
			((sym->i_scope ^ val->e_scope) & SCOPE_SEGMASK))
		{
			err[mflag]++;
			sym->i_token = MULTDEF;
			sym->i_pass = npass;
		}
	}
	sym->i_scope &= ~SCOPE_BUILTIN;
	if (call_list)
		list(val->e_value);
	expr_free(val);
}

void do_defl(struct item *sym, struct expr *val, int call_list)
{
	expr_reloc_check(val);
	switch(sym->i_token) {
	case UNDECLARED: case DEFLED:
		sym->i_token = DEFLED;
		sym->i_pass = npass;
		sym->i_value = val->e_value;
		sym->i_scope = (sym->i_scope & SCOPE_SEGMASK) | val->e_scope;
		break;
	default:
		err[mflag]++;
		sym->i_token = MULTDEF;
		sym->i_pass = npass;
		break;
	}
	if (call_list)
		list(val->e_value);
	expr_free(val);
}

void do_end(struct expr *entry)
{
	if (entry) {
		expr_reloc_check(entry);
		xeq_flag++;
		xeq = entry->e_value & 0xffff;
		rel_main = ((entry->e_scope & SCOPE_SEGMASK) << 16) | xeq;
		expr_free(entry);
	}

// TODO - no reason no to treat END as a simple mras or not.
// At least, give this a try and see how it goes.
//	if (mras) {
		while (expptr)
			popsi();

		peekc = NOPEEK;
		nextline_peek = EOF;
//	}
//	else
//		peekc = 0;

}

void common_block(char *unparsed_id)
{
	struct item *it;
	char *id = unparsed_id;
	char *p;
	int unnamed;

	if (*id == '/') {
		id++;
		for (p = id; *p; p++)
			if (*p == '/')
				*p = '\0';
	}

	unnamed = 1;
	for (p = id; *p; p++)
		if (*p != ' ')
			unnamed = 0;

	id = unnamed ? " " : id;

	it = locate(id);
	switch (it->i_token) {
	case 0:
		nitems++;
	case UNDECLARED:
	case COMMON:
		it->i_value = 0;
		it->i_token = COMMON;
		it->i_pass = npass;
		it->i_scope = SCOPE_COMBLOCK;
		it->i_uses = 0;
		if (!it->i_string)
			it->i_string = strdup(id);
		break;
	default:
		err[mflag]++;
		it->i_token = MULTDEF;
		it->i_pass = npass;
			it->i_string = strdup(id);
		break;
	}

	// Even if we change to the same COMMON block the address is
	// reset back to 0.
	if (relopt) {
		segment = SEG_COMMON;
		segchange = 1;
		dollarsign = seg_pos[SEG_COMMON] = seg_size[SEG_COMMON] = 0;
		// May not be necessary but too much trouble to suppress.
		putrelcmd(RELCMD_COMMON);
		putrelname(it->i_string);
	}

	cur_common = it;
}

int at_least_once; // global to avoid repetition in macro repeat count processing

void dolopt(struct item *itm, int enable)
{
	if (outpass) {
		lineptr = linebuf;
		switch (itm->i_value) {
		case 0:	/* list */
			if (enable < 0) lstoff = 1;
			if (enable > 0) lstoff = 0;
			break;

		case 1:	/* eject */
			if (enable) eject();
			break;

		case 2:	/* space */
			if ((line + enable) > 60) eject();
			else space(enable);
			break;

		case 3:	/* elist */
			eopt = edef;
			if (enable < 0) eopt = 0;
			if (enable > 0) eopt = 1;
			break;

		case 4:	/* fopt */
			fopt = fdef;
			if (enable < 0) fopt = 0;
			if (enable > 0) fopt = 1;
			break;

		case 5:	/* gopt */
			gopt = gdef;
			if (enable < 0) gopt = 1;
			if (enable > 0) gopt = 0;
			break;

		case 6: /* mopt */
			mopt = mdef;
			if (enable < 0) mopt = 0;
			if (enable > 0) mopt = 1;
		}
	}
}


#line 2653 "zmac.c"


#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5046

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  187
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  76
/* YYNRULES -- Number of rules.  */
#define YYNRULES  388
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  701

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   418


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     179,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   176,     2,   183,   184,   175,   166,     2,
     181,   182,   173,   171,   180,   172,     2,   174,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   163,     2,
     168,   167,   169,   162,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   185,     2,   186,   165,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   164,     2,   177,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   170,   178
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  2225,  2225,  2228,  2233,  2250,  2250,  2254,  2260,  2267,
    2276,  2280,  2284,  2288,  2292,  2317,  2331,  2348,  2364,  2373,
    2379,  2384,  2389,  2414,  2429,  2431,  2433,  2444,  2453,  2505,
    2516,  2520,  2520,  2578,  2581,  2606,  2613,  2620,  2624,  2628,
    2639,  2648,  2653,  2653,  2653,  2677,  2677,  2698,  2698,  2732,
    2733,  2732,  2767,  2768,  2767,  2815,  2823,  2851,  2853,  2855,
    2860,  2862,  2915,  2917,  2921,  2935,  2937,  2941,  2958,  2959,
    2960,  2961,  2962,  2963,  2964,  2965,  2966,  2967,  2968,  2969,
    2971,  2972,  2973,  2974,  2975,  2976,  2977,  2978,  2979,  2980,
    2981,  2985,  2988,  2997,  3005,  3019,  3022,  3034,  3037,  3042,
    3045,  3048,  3051,  3054,  3063,  3066,  3069,  3072,  3075,  3078,
    3081,  3084,  3087,  3090,  3093,  3096,  3099,  3102,  3105,  3108,
    3111,  3114,  3117,  3120,  3123,  3126,  3129,  3132,  3135,  3138,
    3143,  3146,  3149,  3152,  3159,  3162,  3171,  3173,  3178,  3196,
    3199,  3202,  3205,  3208,  3211,  3226,  3236,  3247,  3257,  3263,
    3269,  3272,  3275,  3278,  3281,  3284,  3287,  3290,  3293,  3296,
    3299,  3330,  3335,  3340,  3346,  3349,  3354,  3357,  3365,  3371,
    3383,  3389,  3392,  3398,  3404,  3410,  3419,  3422,  3428,  3434,
    3443,  3449,  3458,  3464,  3474,  3480,  3490,  3493,  3496,  3509,
    3517,  3525,  3532,  3535,  3538,  3541,  3544,  3551,  3559,  3566,
    3569,  3572,  3584,  3592,  3603,  3617,  3627,  3662,  3674,  3684,
    3694,  3694,  3696,  3696,  3698,  3698,  3700,  3700,  3702,  3705,
    3707,  3709,  3712,  3712,  3714,  3714,  3717,  3727,  3741,  3743,
    3743,  3746,  3748,  3750,  3754,  3764,  3767,  3769,  3774,  3784,
    3784,  3795,  3797,  3800,  3802,  3805,  3811,  3813,  3816,  3819,
    3824,  3829,  3835,  3840,  3843,  3848,  3854,  3859,  3869,  3871,
    3874,  3876,  3879,  3884,  3889,  3894,  3897,  3899,  3902,  3907,
    3913,  3915,  3921,  3926,  3932,  3937,  3942,  3942,  3942,  3942,
    3942,  3942,  3942,  3942,  3942,  3942,  3945,  3947,  3953,  3958,
    3962,  3964,  3967,  3974,  3993,  3995,  4000,  4010,  4012,  4017,
    4027,  4029,  4034,  4042,  4044,  4048,  4065,  4071,  4076,  4081,
    4088,  4093,  4098,  4103,  4108,  4114,  4141,  4153,  4153,  4157,
    4158,  4159,  4160,  4161,  4162,  4163,  4164,  4165,  4166,  4167,
    4168,  4169,  4170,  4171,  4172,  4173,  4174,  4175,  4176,  4177,
    4178,  4179,  4180,  4182,  4183,  4184,  4185,  4186,  4187,  4188,
    4189,  4190,  4191,  4192,  4193,  4194,  4195,  4196,  4197,  4199,
    4209,  4223,  4226,  4229,  4232,  4235,  4238,  4241,  4244,  4247,
    4254,  4263,  4272,  4279,  4284,  4289,  4294,  4301,  4303,  4305,
    4307,  4309,  4311,  4313,  4318,  4331,  4335,  4339,  4343
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "STRING", "NOOPERAND",
  "ARITHC", "ADD", "LOGICAL", "AND", "OR", "XOR", "ANDAND", "OROR", "BIT",
  "CALL", "INCDEC", "DJNZ", "EX", "IM", "PHASE", "DEPHASE", "TK_IN", "JR",
  "LD", "TK_OUT", "PUSHPOP", "RET", "SHIFT", "RST", "REGNAME", "IXYLH",
  "ACC", "TK_C", "RP", "HL", "INDEX", "AF", "TK_F", "AFp", "SP", "MISCREG",
  "COND", "SPCOND", "NUMBER", "UNDECLARED", "END", "ORG", "ASSERT",
  "TSTATE", "T", "TILO", "TIHI", "SETOCF", "OCF", "LOW", "HIGH", "DC",
  "DEFB", "DEFD", "DEFS", "DEFW", "DEF3", "EQU", "DEFL", "LABEL",
  "EQUATED", "WASEQUATED", "DEFLED", "COMMON", "MULTDEF", "SHL", "SHR",
  "LT", "EQ", "LE", "GE", "NE", "NOT", "MROP_ADD", "MROP_SUB", "MROP_MUL",
  "MROP_DIV", "MROP_MOD", "MROP_AND", "MROP_OR", "MROP_XOR", "MROP_NE",
  "MROP_EQ", "MROP_LT", "MROP_GT", "MROP_LE", "MROP_GE", "MROP_SHIFT",
  "MROP_SHL", "MROP_SHR", "MROP_NOT", "MROP_LOW", "MROP_HIGH", "IF_TK",
  "IF_DEF_TK", "IF_CP_TK", "ELSE_TK", "ENDIF_TK", "ARGPSEUDO", "INCBIN",
  "LIST", "MINMAX", "MACRO", "MNAME", "ARG", "ENDM", "ONECHAR", "TWOCHAR",
  "JRPROMOTE", "JPERROR", "PUBLIC", "EXTRN", "MRAS_MOD", "SETSEG",
  "INSTSET", "LXI", "DAD", "STAX", "STA", "SHLD", "LDAX", "LHLD", "LDA",
  "MVI", "MOV", "INXDCX", "INRDCR", "PSW", "JUMP8", "JP", "CALL8", "ALUI8",
  "SPECIAL", "RAWTOKEN", "LOCAL", "LD_XY", "ST_XY", "MV_XY", "ALU_XY",
  "BIT_XY", "SHIFT_XY", "INP", "OUTP", "JR_COND", "LDST16", "ARITH16",
  "REPT", "IRPC", "IRP", "EXITM", "NUL", "MPARM", "TK_IN0", "TK_OUT0",
  "MLT", "TST", "TSTIO", "'?'", "':'", "'|'", "'^'", "'&'", "'='", "'<'",
  "'>'", "GT", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "UNARY",
  "'\\n'", "','", "'('", "')'", "'#'", "'$'", "'['", "']'", "$accept",
  "statements", "statement", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6",
  "$@7", "$@8", "$@9", "$@10", "maybecolon", "label.part", "public.list",
  "public.part", "extrn.list", "extrn.part", "varop", "operation", "$@11",
  "$@12", "$@13", "$@14", "parm.list", "parm.single", "$@15", "maybeocto",
  "parm.element", "locals", "local_decls", "$@16", "local.list",
  "local.element", "arg.list", "arg.element", "$@17", "allreg", "reg",
  "ixylhreg", "reg8", "m", "realreg", "mem", "memxy", "dxy", "evenreg",
  "evenreg8", "pushable", "pushable8", "bcdesp", "bcdehlsp", "bcdehl",
  "mar", "aliasable", "condition", "spcondition", "db.list",
  "db.list.element", "dw.list", "dw.list.element", "d3.list",
  "d3.list.element", "dd.list", "dd.list.element", "expression",
  "parenexpr", "noparenexpr", "$@18", "symbol", "al", "arg_on", "arg_off",
  "mras_undecl_on", "mras_undecl_off", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-579)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-258)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -579,   665,  -579,  -579,   -82,  -579,  -579,  -579,  -579,  -579,
    -579,  4692,  -579,  4692,  -141,  -123,  -120,  4692,  4692,    55,
      55,   -89,   -76,  -579,  -579,   200,  2359,  -579,  -579,  -579,
    4692,  4692,  4692,  4692,  4692,  4692,  -579,  -579,  -579,  -579,
    -579,  -579,  4692,  4692,  4692,  4692,  4692,  -579,  -579,  -579,
    4692,  4692,  4692,  4692,  4692,  -579,  4692,  1358,  -579,  -579,
      -4,  1012,  -579,  -579,  -579,  4692,  1383,  1506,  -579,  -108,
    -579,  -579,   -86,  -579,  -579,  -579,  -579,    31,  4543,   387,
    4692,  -579,  -579,  -579,    -6,  4692,  -579,  -579,    14,  -579,
    2453,  -579,  -579,  4692,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  4692,    50,  -579,
    -579,  -579,    39,    51,  -579,  -579,  -579,   -49,    58,   -96,
      97,    97,    97,    97,    97,    97,  -579,  -579,  -579,  -579,
    -579,    95,    97,    97,    97,    97,   844,   819,  4692,  4692,
    4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,
    4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,
    4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,
    4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,  4692,
    -579,  -579,  4692,  2072,  -579,  -579,  -579,    55,  -579,    55,
    -579,  -579,  1531,    68,    70,    72,  1037,   987,   128,   148,
    -579,  -579,  2072,    80,    79,    98,  -579,  4692,  2942,  2847,
    3020,  3186,  3281,  3352,  4692,  3845,  2752,  4692,    -7,  4692,
    4692,  -579,  3447,  3923,  2681,  4763,    75,    47,  3613,  4692,
    4692,  4692,  4692,  4692,  -579,  -579,  -579,  -579,  -579,   268,
     268,    83,  4692,  4692,    83,  4692,  4692,   195,   195,   268,
     195,  4692,  3767,  4692,  4692,    83,    83,  4692,  4692,  4692,
    4692,    83,    83,  4692,  4692,   278,    34,   117,    67,  3613,
     117,   122,  1556,  1160,  -579,   123,   135,  2586,  4692,  4692,
    -579,   -54,   159,  -579,  -579,  -579,  -579,  4808,  2244,   668,
    2219,  2194,  1054,  1054,   201,  4871,   201,   201,  4871,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  2047,  2244,   668,  4808,
    4871,   201,   201,   201,  1227,  1227,    97,    97,    97,   137,
    1678,   140,  -579,  -579,  -579,  -579,  -579,  -579,  4692,  -579,
    4692,  -579,   141,  -579,   -94,   145,  -579,  -579,  -579,  2072,
    -579,  -579,   153,  -579,   154,  -579,  4394,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,   144,   869,   155,  -579,  -579,  -579,
    -579,   156,   869,   157,  -579,  -579,   869,   164,  -579,   869,
     166,  -579,   869,   167,  -579,   869,  1185,  -579,  -579,  -579,
     168,  -579,  2072,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    2072,  2072,   169,   170,   316,  2072,  2072,   176,  4001,   177,
    2072,   179,  2072,   181,  4240,   182,   183,   184,  4079,  2072,
     186,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,   191,  2072,  2072,  2072,  2072,  2072,   483,  4692,  4692,
    4692,  -579,  -579,   197,  -579,  -579,  2072,  2072,  -579,  2072,
    2072,   198,  -579,  -579,   203,  -579,  -579,  2072,  4465,  -579,
     204,  2072,  2072,  2072,   207,   208,  1210,  -579,  1333,  -579,
    -579,  -579,  2072,  2072,  -579,  -579,   209,  -579,   211,  -579,
    -579,  -579,  -579,   869,  -579,  -579,  -579,  4692,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
     202,  1703,  1728,  1850,  -579,    79,  -579,  4692,  -579,  -579,
    -579,  1875,  1900,   206,  -579,   213,  -579,  -579,    37,  -579,
    -579,  -579,  -579,  3518,   205,   212,  4614,   345,  3518,   278,
    3518,  3518,  3518,  3518,  3613,  4692,   359,   357,   214,   217,
     218,   223,  4692,   368,   219,  3115,  4465,   230,   226,   374,
      83,  -579,   229,  -579,  2072,   231,  -579,  2072,   232,  -579,
    2072,   233,  -579,  2072,  4692,  4692,   195,   234,  4692,  4692,
    4692,  4692,  4692,   117,    83,  2022,  -579,  -579,  -579,  -579,
    -579,  -579,  2072,  -579,  -579,  -579,  -579,  4692,  -579,   -94,
    -579,   -94,   -94,  -579,   869,  -579,  -579,  -579,  -579,  -579,
     235,   236,  -579,   869,  -579,  -579,  -579,   869,  -579,   869,
    -579,   869,  -579,   869,  -579,   239,  2072,  -579,  -579,   240,
     382,  -579,  4157,  -579,  2072,  -579,   241,  -579,  4311,  -579,
     227,   243,  -579,  2072,   244,   245,  -579,  -579,   252,  -579,
    -579,   483,  4692,  4692,  4692,  2072,  2072,  -579,  -579,  2072,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,   276,  2072,
    -579,  -579,  -579,  -579,  -579,  -579,    83,   220,   251,   253,
     394,   257,  3684,  -579,  -579,  -579,  -579,  -579,   255,   264,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  2072,   288,  -579,
    -579,  -579,    94,  -579,  -579,  -579,  -579,   288,  -579,  -579,
    -579
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     1,    56,   377,   378,   380,   381,   382,   383,
     379,     0,   385,     0,     0,     0,   387,     0,     0,     0,
       0,     0,     0,    31,     3,     5,    57,    42,   307,   315,
       0,     0,     0,     0,     0,     0,   306,   310,   311,   312,
     313,   316,     0,     0,     0,     0,     0,   308,   309,   317,
       0,     0,     0,     0,     0,   314,     0,     0,   303,   304,
       0,     0,    18,    19,    33,     0,     0,     0,   377,     0,
      62,    64,     0,    65,    67,    39,    41,     0,     0,     0,
       0,   385,   385,   384,     0,     0,   222,   222,     0,     4,
       0,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    90,    88,    89,     0,    58,    74,
      75,    73,    68,    69,    70,    71,    72,    61,     0,   219,
     369,   370,   371,   372,   373,   375,   366,   368,   363,   374,
     376,     0,   365,   367,   364,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      15,   386,     0,   388,    35,    36,    37,     0,    38,     0,
      32,    20,     0,     0,   308,   309,     0,     0,   386,     0,
      45,    40,   384,     0,   224,     0,    55,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   205,     0,     0,     0,     0,     0,   158,    93,     0,
       0,     0,     0,     0,   210,   216,   212,   214,   218,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
     225,     0,     0,   220,   318,   305,   361,   325,   327,   329,
     341,   342,   330,   331,   333,   335,   338,   340,   339,   343,
     344,   346,   345,   347,   348,   349,   350,   357,   354,   353,
     355,   356,   358,   359,   351,   352,     0,   326,   328,   324,
     334,   332,   336,   337,   319,   320,   322,   321,   323,     0,
       0,     0,    63,    66,    21,    26,    24,    25,     0,    22,
       0,   386,     0,   386,   235,     0,    49,   223,    52,    92,
     249,   245,   250,   251,     0,   248,     0,   115,   241,   242,
     117,   243,   244,   253,     0,   101,   250,   274,   275,   112,
     114,     0,    99,   250,   118,   119,   103,   250,   120,   104,
     250,   121,   105,   250,   122,   106,   144,   289,   287,   288,
       0,   286,    95,   250,   268,   269,   131,   139,   258,   259,
     257,   157,     0,     0,     0,   203,   204,     0,     0,     0,
     191,     0,   154,     0,     0,     0,     0,   303,     0,   198,
     303,   262,   263,   264,   267,   266,   142,   143,   265,   159,
     128,   253,    96,   206,   207,   208,   209,     0,     0,     0,
       0,   261,   260,     0,   136,   172,   174,   182,   168,   180,
     170,     0,   247,   246,     0,   140,   132,   149,     0,   151,
       0,    94,   153,    97,     0,     0,     0,    98,     0,   129,
     193,   200,   156,   184,   137,   138,     0,   190,     0,   272,
     273,   141,   123,   107,   202,     6,    10,     0,    12,    13,
     276,   277,   278,   281,   282,   283,   279,   284,   285,   280,
       0,     0,     0,     0,    43,   224,   226,     0,    17,    16,
      34,     0,     0,     0,    29,     0,   238,   239,     0,   236,
      47,   384,   384,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   292,   211,   290,   293,   217,   300,   302,   213,   294,
     296,   215,   297,   299,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     8,     7,     9,    11,
     228,   221,   360,    27,    23,    28,    30,     0,    46,     0,
     228,     0,     0,   116,   102,   271,   270,   133,   252,   255,
       0,     0,   113,   100,   134,   135,   124,   108,   125,   109,
     126,   110,   127,   111,   145,   253,   152,   185,   186,     0,
       0,   195,     0,   188,   155,   176,     0,   175,     0,   160,
     303,   304,   183,     0,   303,   304,   173,   181,     0,   196,
     130,     0,     0,     0,     0,   178,   166,   163,   150,   148,
     161,   162,   165,   146,   189,   197,    14,    44,   227,   240,
     237,    48,   386,   386,   254,   256,     0,     0,     0,     0,
       0,     0,     0,   291,   301,   295,   298,   229,     0,     0,
     147,   187,   194,   192,   171,   167,   199,   201,   231,    50,
      53,   234,     0,   232,   228,   228,   230,     0,    51,    54,
     233
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,   258,  -579,   259,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,   362,  -579,  -579,  -196,
    -578,  -579,  -579,  -579,  -251,  -579,  -397,  -579,  -163,  -191,
    -579,  -234,    20,  -164,  -579,  -224,  -230,  -219,  -176,  -579,
    -579,  -254,  -579,  -579,  -208,  -579,  -157,   237,  -579,  -184,
    -579,  -193,  -579,  -186,  -579,  -181,   -11,  -215,  -269,  -579,
      16,  -199,   224,  -171,  -579,  -579
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    24,    90,    77,   119,   580,   344,   590,   521,
     694,   522,   695,   117,    25,    69,    70,    72,    73,   118,
     271,   437,   439,   440,   438,   281,   203,   204,   282,   283,
     657,   658,   688,   692,   693,   518,   519,   587,   357,   358,
     359,   451,   452,   361,   362,   363,   364,   397,   443,   426,
     427,   398,   597,   481,   399,   500,   390,   391,   552,   553,
     558,   559,   561,   562,   555,   556,   400,    58,    59,   131,
      71,   200,    60,   329,    65,   331
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      57,   371,    61,   345,   431,   416,    66,    67,   347,   417,
     420,   474,   661,   277,   454,   516,   456,    26,   428,   120,
     121,   122,   123,   124,   125,    27,   402,   342,   467,   403,
     469,   126,   127,   128,   129,   130,    74,   430,    62,   132,
     133,   134,   135,   136,   459,   137,   369,   374,   378,   381,
     384,   477,   478,   396,   183,   484,    63,   475,   409,    64,
    -224,   415,   425,   350,   444,   393,   353,   192,   196,   197,
     429,   186,   187,   455,   202,   442,   442,   445,   482,   387,
     448,   517,   272,   453,   453,   442,   453,   280,   388,   389,
      75,   464,   465,   188,   189,   460,   273,   470,   471,    68,
     479,   480,   476,    76,   350,   181,   393,   353,   421,   367,
     368,   422,   350,   423,   393,   353,   698,   699,   278,     5,
       6,     7,     8,     9,    10,   504,   505,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   190,
     513,   330,   515,   201,   404,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   660,   206,   662,   663,   349,   365,   372,   376,
     379,   382,   385,   386,   392,    74,   401,   424,   405,   406,
     275,   410,   412,   274,   419,    54,   588,   589,   432,   433,
     434,   435,   436,   276,   350,   279,   393,   353,   360,   370,
     375,   446,   447,   284,   449,   450,   355,   341,   394,   595,
     457,   461,   462,   463,   395,    78,   466,   335,   468,   336,
     567,   337,   472,   473,   367,   368,    79,   343,   483,    80,
     346,   636,   280,   394,   367,   368,   501,   502,   503,   395,
     596,   143,   144,   696,   697,   604,   631,   635,   348,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   600,   350,    54,   393,
     353,   485,   488,    81,    82,   198,   199,   441,    83,   581,
     615,   394,   367,   368,   489,   506,   508,   395,    84,   510,
     514,   605,   591,   592,   520,   527,   623,   511,   637,   512,
     630,   634,   647,   523,   524,   528,   529,   530,   632,   650,
     651,   652,   653,   614,   531,   136,   532,   533,   535,   536,
     537,    85,    86,    87,    88,   538,   539,   541,   654,   542,
     593,   543,   545,   546,   547,   602,   549,   606,   608,   610,
     612,   550,   175,   176,   177,   178,   179,   564,   565,    89,
     601,   576,   629,   566,   568,   585,   640,   569,   570,   573,
     193,   574,   586,   617,   598,   618,   619,   136,   620,   625,
     621,   626,   453,   136,   622,   639,  -169,   136,   638,   641,
     655,   642,   643,   644,   668,   677,   648,   664,   665,   666,
     667,   670,  -164,  -179,  -177,   684,   554,   557,   560,   563,
      28,    29,   672,   682,   689,   683,    30,    31,    32,   685,
      33,    34,    35,   690,   691,   332,   700,   136,   333,   205,
     675,    36,    37,    38,    39,    40,    41,   673,   676,   681,
     411,   674,     0,     0,     0,    42,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   575,     0,     0,     0,
       0,     0,    44,    45,    46,     0,   551,     0,     0,     0,
       0,   678,   679,     0,     0,     0,   582,     0,   194,   195,
       0,     0,   680,     0,     0,     0,     0,     0,   686,     0,
       0,     0,   594,     0,     0,     0,     0,   603,     0,   607,
     609,   611,   613,     0,   616,     0,    28,    29,     0,     0,
       0,   624,    30,    31,    32,   633,    33,    34,    35,     0,
       0,     0,    49,     0,     0,     0,     0,    36,    37,    38,
      39,    40,    41,   645,   646,     0,     0,   649,    50,    51,
       0,    42,    43,    52,    53,     0,     0,     0,    54,     0,
       0,    55,    56,     0,     0,     0,   659,     0,    44,    45,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,    48,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   136,     0,     0,     0,     0,     0,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     554,   557,   560,   563,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,     0,     0,     0,    52,
      53,   687,     0,     0,    54,     2,     3,    55,    56,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   138,     0,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
     -60,   -60,   -60,   -60,     0,     0,     0,   -60,     0,     0,
       0,   -60,   -60,   -60,   -60,   -60,   -60,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   143,   144,
     145,   146,   147,   148,   149,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    11,    12,    13,    14,    15,   -60,   -60,
      16,     0,     0,   -60,     0,   -60,     0,     0,    17,    18,
      19,    20,    21,   -60,    22,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,     0,   -60,   -60,
     -60,   -60,    23,     0,     0,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,
       0,     0,   -60,   -60,   -60,   -60,   -60,   138,   139,   140,
     141,   142,     0,     0,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   -60,     0,     0,     0,     0,     0,
       0,     0,   138,   139,   140,   141,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   138,   139,   140,
     141,   142,     0,     0,     0,     0,     0,     0,     0,   143,
     144,   145,   146,   147,   148,   149,     0,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   143,   144,   145,   146,   147,   148,
     149,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   143,
     144,   145,   146,   147,   148,   149,     0,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,     0,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   138,   139,   140,   141,   142,
       0,     0,     0,     0,     0,   286,   167,     0,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     138,   139,   140,   141,   142,     0,   285,     0,     0,     0,
       0,   167,     0,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   138,   139,   140,   141,   142,
    -257,     0,     0,     0,     0,     0,     0,   143,   144,   145,
     146,   147,   148,   149,     0,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   143,   144,   145,   146,   147,   148,   149,     0,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   143,   144,   145,
     146,   147,   148,   149,     0,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
       0,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,     0,     0,     0,   339,   340,   138,   139,
     140,   141,   142,     0,   167,     0,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,     0,
       0,     0,   182,   138,   139,   140,   141,   142,     0,   167,
       0,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,     0,     0,     0,     0,   338,   138,   139,
     140,   141,   142,     0,     0,   175,   176,   177,   178,   179,
     143,   144,   145,   146,   147,   148,   149,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   143,   144,   145,   146,   147,
     148,   149,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     143,   144,   145,   146,   147,   148,   149,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,     0,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,     0,     0,     0,
     487,   138,   139,   140,   141,   142,     0,   167,     0,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,     0,     0,     0,   534,   138,   139,   140,   141,
     142,     0,   167,     0,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,     0,     0,     0,
     571,   138,   139,   140,   141,   142,     0,     0,     0,     0,
     177,   178,   179,   143,   144,   145,   146,   147,   148,   149,
       0,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   143,   144,
     145,   146,   147,   148,   149,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   143,   144,   145,   146,   147,   148,   149,
       0,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   167,     0,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     0,
       0,     0,     0,   572,   138,   139,   140,   141,   142,     0,
     167,     0,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,     0,     0,   180,     0,   138,
     139,   140,   141,   142,     0,   167,     0,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     0,
       0,     0,   184,     0,   138,   139,   140,   141,   142,     0,
       0,     0,     0,     0,     0,     0,   143,   144,   145,   146,
     147,   148,   149,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   143,   144,   145,   146,   147,   148,   149,     0,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   143,   144,   145,   146,
     147,   148,   149,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   167,     0,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,     0,     0,   185,   138,   139,   140,   141,
     142,     0,     0,   167,     0,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,     0,     0,
     334,   138,   139,   140,   141,   142,     0,     0,   167,     0,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,     0,     0,   486,   138,   139,   140,   141,
     142,     0,     0,     0,     0,     0,     0,     0,   143,   144,
     145,   146,   147,   148,   149,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   143,   144,   145,   146,   147,   148,   149,
       0,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   143,   144,
     145,   146,   147,   148,   149,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     167,     0,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,     0,     0,   509,   138,   139,
     140,   141,   142,     0,     0,   167,     0,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     0,
       0,     0,   577,   138,   139,   140,   141,   142,     0,     0,
     167,     0,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,     0,     0,   578,   138,   139,
     140,   141,   142,     0,     0,     0,     0,     0,     0,     0,
     143,   144,   145,   146,   147,   148,   149,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   143,   144,   145,   146,   147,
     148,   149,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     143,   144,   145,   146,   147,   148,   149,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   167,     0,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,     0,     0,   579,
     138,   139,   140,   141,   142,     0,     0,   167,     0,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,     0,     0,   583,   138,   139,   140,   141,   142,
       0,     0,   167,     0,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,     0,     0,   584,
     138,   139,   140,   141,   142,     0,     0,     0,     0,     0,
       0,     0,   143,   144,   145,   146,   147,   148,   149,     0,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   143,   144,   145,
     146,   147,   148,   149,     0,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   143,   144,   145,   146,   147,   148,   149,     0,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,     0,
       0,   656,   138,   139,   140,   141,     0,     0,     0,   167,
     507,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,     0,     0,     0,     0,   138,   139,   140,
       0,     0,     0,     0,   167,     0,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,     0,
       0,     0,   138,     0,   140,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   143,   144,   145,   146,   147,   148,
     149,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   143,
     144,   145,   146,   147,   148,   149,     0,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   143,   144,   145,   146,   147,   148,
     149,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
      91,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
       0,     0,    93,     0,     0,     0,     0,     0,     0,    94,
      95,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,   101,   102,   103,     0,     0,     0,     0,     0,
       0,   104,   105,   106,     0,     0,     0,   207,   208,   209,
     210,   211,   212,   213,     0,   107,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   230,
     231,   232,     0,     0,     0,   233,     0,     0,     0,     0,
     234,   235,     0,   236,   237,     0,     0,     0,     0,     0,
       0,     0,   108,   109,   110,   111,     0,     0,     0,     0,
     112,   113,   114,   115,   116,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,     0,   251,   252,   253,   254,
       0,     0,     0,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,     0,     0,     0,     0,     0,     0,
     266,   267,   268,   269,   270,   490,     0,   491,   492,   493,
     494,   495,   496,     0,     0,   497,     0,   498,     0,    28,
      29,     0,     0,     0,     0,    30,    31,    32,     0,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,     0,
      36,    37,    38,    39,    40,    41,     0,     0,     0,     0,
       0,     0,     0,     0,    42,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    44,    45,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,   351,   393,   353,   394,   367,   368,     0,   499,     0,
     395,   413,     0,     0,    28,    29,     0,     0,     0,     0,
      30,    31,    32,     0,    33,    34,    35,     0,     0,     0,
       0,    49,     0,     0,     0,    36,    37,    38,    39,    40,
      41,     0,     0,     0,     0,     0,     0,    50,    51,    42,
      43,     0,    52,    53,     0,     0,     0,    54,     0,     0,
      55,    56,     0,     0,     0,     0,    44,    45,    46,     0,
       0,   350,   351,   393,   353,   394,   367,   368,     0,     0,
       0,   395,    47,    48,     0,    28,    29,     0,     0,     0,
       0,    30,    31,    32,     0,    33,    34,    35,     0,     0,
       0,     0,     0,     0,     0,     0,    36,    37,    38,    39,
      40,    41,     0,     0,     0,     0,     0,     0,     0,     0,
      42,    43,     0,     0,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    44,    45,    46,
       0,     0,    50,    51,     0,     0,     0,    52,    53,     0,
       0,     0,   414,    47,    48,    55,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,   351,   366,   353,
       0,   367,   368,     0,     0,     0,     0,     0,   355,     0,
      28,    29,     0,     0,     0,     0,    30,    31,    32,     0,
      33,    34,    35,     0,     0,     0,     0,    49,     0,     0,
       0,    36,    37,    38,    39,    40,    41,     0,     0,     0,
       0,     0,     0,    50,    51,    42,    43,     0,    52,    53,
       0,     0,     0,   356,     0,     0,    55,    56,     0,     0,
       0,     0,    44,    45,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,   351,   352,   353,     0,   354,     0,     0,     0,
       0,     0,     0,   355,     0,    28,    29,     0,     0,     0,
       0,    30,    31,    32,     0,    33,    34,    35,     0,     0,
       0,     0,    49,     0,     0,     0,    36,    37,    38,    39,
      40,    41,     0,     0,     0,     0,     0,     0,    50,    51,
      42,    43,     0,    52,    53,     0,     0,     0,   356,     0,
       0,    55,    56,     0,     0,     0,     0,    44,    45,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
     351,   373,   353,    47,    48,     0,     0,     0,     0,     0,
       0,   355,     0,    28,    29,     0,     0,     0,     0,    30,
      31,    32,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,    36,    37,    38,    39,    40,    41,
       0,     0,     0,     0,     0,     0,     0,    49,    42,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,    44,    45,    46,    52,    53,
       0,     0,     0,   356,     0,     0,    55,    56,     0,     0,
       0,    47,    48,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,   351,   393,   353,     0,     0,
       0,     0,     0,     0,     0,   627,     0,     0,    28,    29,
       0,     0,     0,     0,    30,    31,    32,     0,    33,    34,
      35,     0,     0,     0,     0,    49,     0,     0,     0,    36,
      37,    38,    39,    40,    41,     0,     0,     0,     0,     0,
       0,    50,    51,    42,    43,     0,    52,    53,     0,     0,
       0,   356,     0,     0,    55,    56,     0,     0,     0,     0,
      44,    45,    46,     0,     0,   350,   351,   377,   353,     0,
       0,     0,     0,     0,     0,     0,    47,    48,     0,    28,
      29,     0,     0,     0,     0,    30,    31,    32,     0,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,     0,
      36,    37,    38,    39,    40,    41,     0,     0,     0,     0,
       0,     0,     0,     0,    42,    43,     0,     0,     0,     0,
      49,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    44,    45,    46,     0,     0,    50,    51,     0,     0,
       0,    52,    53,     0,     0,     0,   628,    47,    48,    55,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,   351,   380,   353,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,    29,     0,     0,     0,     0,
      30,    31,    32,     0,    33,    34,    35,     0,     0,     0,
       0,    49,     0,     0,     0,    36,    37,    38,    39,    40,
      41,     0,     0,     0,     0,     0,     0,    50,    51,    42,
      43,     0,    52,    53,     0,     0,     0,   356,     0,     0,
      55,    56,     0,     0,     0,     0,    44,    45,    46,     0,
       0,   350,   351,   383,   353,     0,     0,     0,     0,     0,
       0,     0,    47,    48,     0,    28,    29,     0,     0,     0,
       0,    30,    31,    32,     0,    33,    34,    35,     0,     0,
       0,     0,     0,     0,     0,     0,    36,    37,    38,    39,
      40,    41,     0,     0,     0,     0,     0,     0,     0,     0,
      42,    43,     0,     0,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    44,    45,    46,
       0,     0,    50,    51,     0,     0,     0,    52,    53,     0,
       0,     0,   356,    47,    48,    55,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,     0,   393,   353,
       0,     0,     0,     0,   407,     0,     0,     0,     0,     0,
      28,    29,     0,     0,     0,     0,    30,    31,    32,     0,
      33,    34,    35,     0,     0,     0,     0,    49,     0,     0,
       0,    36,    37,    38,    39,    40,    41,     0,     0,     0,
       0,     0,     0,    50,    51,    42,    43,     0,    52,    53,
       0,     0,     0,   356,     0,     0,    55,    56,     0,     0,
       0,     0,    44,    45,    46,     0,     0,   350,   351,   393,
     353,     0,     0,     0,     0,     0,     0,     0,    47,    48,
       0,    28,    29,     0,     0,     0,     0,    30,    31,    32,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     0,
       0,     0,    36,    37,    38,    39,    40,    41,     0,     0,
       0,     0,     0,     0,     0,     0,    42,    43,     0,     0,
       0,     0,    49,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,    45,    46,     0,     0,    50,    51,
       0,     0,     0,    52,    53,     0,     0,     0,   408,    47,
      48,    55,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   350,     0,   393,   353,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    28,    29,     0,     0,
       0,     0,    30,    31,    32,     0,    33,    34,    35,     0,
       0,     0,     0,    49,     0,     0,     0,    36,    37,    38,
      39,    40,    41,     0,     0,     0,     0,     0,     0,    50,
      51,    42,    43,     0,    52,    53,     0,     0,     0,   356,
       0,     0,    55,    56,     0,     0,     0,     0,    44,    45,
      46,     0,     0,   350,     0,   393,   353,     0,     0,     0,
       0,     0,     0,     0,    47,    48,     0,    28,    29,     0,
       0,     0,     0,    30,    31,    32,     0,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,    36,    37,
      38,    39,    40,    41,     0,     0,     0,     0,     0,     0,
       0,     0,    42,    43,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    44,
      45,    46,     0,     0,    50,    51,     0,     0,     0,    52,
      53,     0,     0,     0,   356,    47,    48,    55,    56,   387,
       0,   367,   368,     0,     0,     0,     0,     0,   388,   389,
      28,    29,     0,     0,     0,     0,    30,    31,    32,     0,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,     0,     0,    49,
       0,     0,     0,     0,     0,    42,    43,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
      52,    53,    44,    45,    46,    54,     0,     0,    55,    56,
       0,     0,     0,     0,     0,     0,     0,   387,    47,    48,
       0,     0,     0,     0,     0,     0,   388,   389,    28,    29,
       0,     0,     0,     0,    30,    31,    32,     0,    33,    34,
      35,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,    39,    40,    41,     0,     0,     0,     0,     0,
       0,     0,    49,    42,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      44,    45,    46,    52,    53,     0,     0,     0,   458,     0,
       0,    55,    56,     0,     0,   387,    47,    48,     0,     0,
       0,     0,     0,     0,     0,   389,    28,    29,     0,     0,
       0,     0,    30,    31,    32,     0,    33,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    37,    38,
      39,    40,    41,     0,     0,     0,     0,     0,     0,     0,
      49,    42,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    44,    45,
      46,    52,    53,     0,     0,     0,    54,     0,     0,    55,
      56,     0,     0,   540,    47,    48,     0,     0,     0,     0,
       0,     0,     0,     0,    28,    29,     0,     0,     0,     0,
      30,    31,    32,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    37,    38,    39,    40,
      41,     0,     0,     0,     0,     0,     0,     0,    49,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,    44,    45,    46,    52,
      53,     0,     0,     0,    54,     0,     0,    55,    56,     0,
       0,   548,    47,    48,     0,     0,     0,     0,     0,     0,
       0,     0,    28,    29,     0,     0,     0,     0,    30,    31,
      32,     0,    33,    34,    35,     0,     0,     0,     0,     0,
       0,     0,     0,    36,    37,    38,    39,    40,    41,     0,
       0,     0,     0,     0,     0,     0,    49,    42,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    44,    45,    46,    52,    53,     0,
       0,     0,    54,     0,     0,    55,    56,     0,     0,   669,
      47,    48,     0,     0,     0,     0,     0,     0,     0,     0,
      28,    29,     0,     0,     0,     0,    30,    31,    32,     0,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,     0,     0,     0,
       0,     0,     0,     0,    49,    42,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    44,    45,    46,    52,    53,     0,     0,     0,
      54,     0,     0,    55,    56,     0,     0,     0,    47,    48,
       0,     0,     0,   544,   525,   526,     0,     0,     0,     0,
       0,     0,     0,    28,    29,     0,     0,     0,     0,    30,
      31,    32,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,    36,    37,    38,    39,    40,    41,
       0,     0,    49,     0,     0,     0,     0,     0,    42,    43,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,     0,     0,    52,    53,    44,    45,    46,    54,     0,
       0,    55,    56,     0,   671,   525,   526,     0,     0,     0,
       0,    47,    48,     0,    28,    29,     0,     0,     0,     0,
      30,    31,    32,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    37,    38,    39,    40,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    42,
      43,     0,     0,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,    45,    46,     0,
       0,    50,    51,     0,     0,     0,    52,    53,     0,     0,
       0,    54,    47,    48,    55,    56,     0,     0,   525,   526,
       0,     0,     0,     0,     0,     0,     0,    28,    29,     0,
       0,     0,     0,    30,    31,    32,     0,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,    36,    37,
      38,    39,    40,    41,     0,     0,    49,     0,     0,     0,
       0,     0,    42,    43,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,    52,    53,    44,
      45,    46,    54,     0,     0,    55,    56,     0,     0,   367,
     368,     0,     0,     0,     0,    47,    48,     0,    28,    29,
       0,     0,     0,     0,    30,    31,    32,     0,    33,    34,
      35,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,    39,    40,    41,     0,     0,     0,     0,     0,
       0,     0,     0,    42,    43,     0,     0,     0,     0,    49,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,    45,    46,     0,     0,    50,    51,     0,     0,     0,
      52,    53,     0,     0,     0,    54,    47,    48,    55,    56,
       0,     0,     0,     0,     0,     0,    28,    29,     0,     0,
       0,     0,    30,    31,    32,     0,    33,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    37,    38,
      39,    40,    41,     0,     0,     0,     0,     0,     0,     0,
      49,    42,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    44,    45,
      46,    52,    53,     0,     0,     0,    54,     0,     0,    55,
      56,     0,     0,     0,    47,    48,     0,    28,    29,     0,
       0,     0,     0,    30,    31,    32,     0,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,    36,    37,
      38,    39,    40,    41,     0,     0,     0,     0,     0,     0,
       0,     0,    42,    43,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    44,
      45,    46,     0,     0,    50,    51,     0,     0,     0,    52,
      53,     0,   191,     0,    54,    47,    48,    55,    56,     0,
       0,     0,     0,     0,     0,    28,    29,     0,     0,     0,
       0,    30,    31,    32,     0,    33,    34,    35,     0,     0,
       0,     0,     0,     0,     0,     0,    36,    37,    38,    39,
      40,    41,     0,     0,     0,     0,     0,     0,     0,    49,
      42,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    44,    45,    46,
      52,    53,     0,     0,     0,    54,   599,     0,    55,    56,
       0,     0,     0,    47,    48,     0,    28,    29,     0,     0,
       0,     0,    30,    31,    32,     0,    33,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    37,    38,
      39,    40,    41,     0,     0,     0,     0,     0,     0,     0,
       0,    42,    43,     0,     0,     0,     0,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    44,    45,
      46,     0,     0,    50,    51,     0,     0,     0,    52,    53,
       0,     0,     0,    54,    47,    48,    55,    56,   143,   144,
     145,   146,   147,   148,   149,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    51,     0,     0,     0,    52,
      53,   143,   144,   145,   418,   147,   148,    55,    56,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   172,
     173,   174,   175,   176,   177,   178,   179
};

static const yytype_int16 yycheck[] =
{
      11,   209,    13,   202,   228,   224,    17,    18,   204,   224,
     225,   265,   590,    62,   248,   109,   250,     1,   226,    30,
      31,    32,    33,    34,    35,   107,    33,   198,   258,    36,
     260,    42,    43,    44,    45,    46,    20,   228,   179,    50,
      51,    52,    53,    54,   252,    56,   209,   210,   211,   212,
     213,   266,   267,   216,    65,   270,   179,   265,   222,   179,
     156,   224,   226,    29,   240,    31,    32,    78,    79,    80,
     227,   179,   180,   249,    85,   239,   240,   241,   269,    32,
     244,   175,    93,   247,   248,   249,   250,   183,    41,    42,
     179,   255,   256,   179,   180,   252,   107,   261,   262,    44,
      33,    34,   266,   179,    29,   109,    31,    32,    33,    34,
      35,    36,    29,    38,    31,    32,   694,   695,   167,    64,
      65,    66,    67,    68,    69,   179,   180,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   138,
     341,   182,   343,   179,   181,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,   589,   179,   591,   592,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   189,   217,   132,   219,   220,
     171,   222,   223,   163,   225,   181,   179,   180,   229,   230,
     231,   232,   233,   172,    29,   167,    31,    32,   208,   209,
     210,   242,   243,   138,   245,   246,    41,   109,    33,    34,
     251,   252,   253,   254,    39,    45,   257,   179,   259,   179,
     458,   179,   263,   264,    34,    35,    56,   109,   269,    59,
     180,    31,   183,    33,    34,    35,   277,   278,   279,    39,
     524,    70,    71,   179,   180,   529,   545,   546,   180,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   526,    29,   181,    31,
      32,   179,   179,   103,   104,    81,    82,    39,   108,   505,
     534,    33,    34,    35,   179,   156,   179,    39,   118,   179,
     179,   529,   521,   522,   179,   181,   541,   338,   547,   340,
     545,   546,   566,   180,   180,   180,   180,   180,   546,   569,
     570,   571,   572,   534,   180,   356,   180,   180,   180,   180,
     180,   151,   152,   153,   154,    39,   180,   180,   573,   180,
     523,   180,   180,   180,   180,   528,   180,   530,   531,   532,
     533,   180,   171,   172,   173,   174,   175,   180,   180,   179,
      35,   179,   545,   180,   180,   179,   550,   180,   180,   180,
       3,   180,   179,    34,   182,    38,   182,   408,   181,    31,
     182,   182,   566,   414,   181,    31,   179,   418,   182,   180,
     574,   180,   180,   180,    32,   139,   182,   182,   182,   180,
     180,   180,   179,   179,   179,    31,   437,   438,   439,   440,
      43,    44,   180,   182,   179,   182,    49,    50,    51,   182,
      53,    54,    55,   179,   156,   187,   697,   458,   189,    87,
     643,    64,    65,    66,    67,    68,    69,   641,   644,   667,
     223,   642,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   487,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    -1,     3,    -1,    -1,    -1,
      -1,   662,   663,    -1,    -1,    -1,   507,    -1,   111,   112,
      -1,    -1,   666,    -1,    -1,    -1,    -1,    -1,   672,    -1,
      -1,    -1,   523,    -1,    -1,    -1,    -1,   528,    -1,   530,
     531,   532,   533,    -1,   535,    -1,    43,    44,    -1,    -1,
      -1,   542,    49,    50,    51,   546,    53,    54,    55,    -1,
      -1,    -1,   155,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,   564,   565,    -1,    -1,   568,   171,   172,
      -1,    78,    79,   176,   177,    -1,    -1,    -1,   181,    -1,
      -1,   184,   185,    -1,    -1,    -1,   587,    -1,    95,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   622,    -1,    -1,    -1,    -1,    -1,   628,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     641,   642,   643,   644,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,   172,    -1,    -1,    -1,   176,
     177,   672,    -1,    -1,   181,     0,     1,   184,   185,     4,
       5,     6,     7,     8,     9,    10,     8,    -1,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      45,    46,    47,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    56,    57,    58,    59,    60,    61,    -1,    -1,    64,
      65,    66,    67,    68,    69,    -1,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    -1,   133,   134,
     135,   136,   137,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,   157,   158,   159,   160,   161,     8,     9,    10,
      11,    12,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     8,     9,    10,    11,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,   186,   162,    -1,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       8,     9,    10,    11,    12,    -1,   182,    -1,    -1,    -1,
      -1,   162,    -1,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,     8,     9,    10,    11,    12,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,   162,
      -1,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,    -1,    -1,    -1,   179,   180,     8,     9,
      10,    11,    12,    -1,   162,    -1,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,    -1,    -1,
      -1,    -1,   180,     8,     9,    10,    11,    12,    -1,   162,
      -1,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,    -1,    -1,    -1,    -1,   180,     8,     9,
      10,    11,    12,    -1,    -1,   171,   172,   173,   174,   175,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,   162,    -1,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,    -1,    -1,    -1,    -1,
     180,     8,     9,    10,    11,    12,    -1,   162,    -1,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,    -1,    -1,    -1,    -1,   180,     8,     9,    10,    11,
      12,    -1,   162,    -1,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,    -1,    -1,    -1,    -1,
     180,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
     173,   174,   175,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,    -1,
      -1,    -1,    -1,   180,     8,     9,    10,    11,    12,    -1,
     162,    -1,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,    -1,    -1,    -1,   179,    -1,     8,
       9,    10,    11,    12,    -1,   162,    -1,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,    -1,
      -1,    -1,   179,    -1,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,    -1,    -1,    -1,   179,     8,     9,    10,    11,
      12,    -1,    -1,   162,    -1,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,    -1,    -1,    -1,
     179,     8,     9,    10,    11,    12,    -1,    -1,   162,    -1,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,    -1,    -1,    -1,   179,     8,     9,    10,    11,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,    -1,    -1,    -1,   179,     8,     9,
      10,    11,    12,    -1,    -1,   162,    -1,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,    -1,
      -1,    -1,   179,     8,     9,    10,    11,    12,    -1,    -1,
     162,    -1,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,    -1,    -1,    -1,   179,     8,     9,
      10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,    -1,    -1,    -1,   179,
       8,     9,    10,    11,    12,    -1,    -1,   162,    -1,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,    -1,    -1,    -1,   179,     8,     9,    10,    11,    12,
      -1,    -1,   162,    -1,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,    -1,    -1,    -1,   179,
       8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,    -1,    -1,
      -1,   179,     8,     9,    10,    11,    -1,    -1,    -1,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,    -1,    -1,    -1,    -1,     8,     9,    10,
      -1,    -1,    -1,    -1,   162,    -1,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    -1,    -1,    -1,     4,     5,     6,
       7,     8,     9,    10,    -1,   106,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      57,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
     171,   172,   173,   174,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,   133,   134,   135,   136,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
     157,   158,   159,   160,   161,    29,    -1,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    41,    -1,    43,
      44,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    65,    66,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    -1,   132,    -1,
      39,    40,    -1,    -1,    43,    44,    -1,    -1,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,   171,   172,    78,
      79,    -1,   176,   177,    -1,    -1,    -1,   181,    -1,    -1,
     184,   185,    -1,    -1,    -1,    -1,    95,    96,    97,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    39,   111,   112,    -1,    43,    44,    -1,    -1,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      -1,    -1,   171,   172,    -1,    -1,    -1,   176,   177,    -1,
      -1,    -1,   181,   111,   112,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,    32,
      -1,    34,    35,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      43,    44,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,   171,   172,    78,    79,    -1,   176,   177,
      -1,    -1,    -1,   181,    -1,    -1,   184,   185,    -1,    -1,
      -1,    -1,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    30,    31,    32,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    -1,
      -1,    -1,   155,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,   171,   172,
      78,    79,    -1,   176,   177,    -1,    -1,    -1,   181,    -1,
      -1,   184,   185,    -1,    -1,    -1,    -1,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,   111,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    66,    67,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   171,   172,    95,    96,    97,   176,   177,
      -1,    -1,    -1,   181,    -1,    -1,   184,   185,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    43,    44,
      -1,    -1,    -1,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    64,
      65,    66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,   171,   172,    78,    79,    -1,   176,   177,    -1,    -1,
      -1,   181,    -1,    -1,   184,   185,    -1,    -1,    -1,    -1,
      95,    96,    97,    -1,    -1,    29,    30,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    43,
      44,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    65,    66,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    -1,    -1,   171,   172,    -1,    -1,
      -1,   176,   177,    -1,    -1,    -1,   181,   111,   112,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    44,    -1,    -1,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,   171,   172,    78,
      79,    -1,   176,   177,    -1,    -1,    -1,   181,    -1,    -1,
     184,   185,    -1,    -1,    -1,    -1,    95,    96,    97,    -1,
      -1,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    43,    44,    -1,    -1,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      -1,    -1,   171,   172,    -1,    -1,    -1,   176,   177,    -1,
      -1,    -1,   181,   111,   112,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    31,    32,
      -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      43,    44,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,   171,   172,    78,    79,    -1,   176,   177,
      -1,    -1,    -1,   181,    -1,    -1,   184,   185,    -1,    -1,
      -1,    -1,    95,    96,    97,    -1,    -1,    29,    30,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    43,    44,    -1,    -1,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    65,    66,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    -1,    -1,   171,   172,
      -1,    -1,    -1,   176,   177,    -1,    -1,    -1,   181,   111,
     112,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    44,    -1,    -1,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    55,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,   171,
     172,    78,    79,    -1,   176,   177,    -1,    -1,    -1,   181,
      -1,    -1,   184,   185,    -1,    -1,    -1,    -1,    95,    96,
      97,    -1,    -1,    29,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    43,    44,    -1,
      -1,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    -1,    -1,   171,   172,    -1,    -1,    -1,   176,
     177,    -1,    -1,    -1,   181,   111,   112,   184,   185,    32,
      -1,    34,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,    -1,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,   172,    -1,    -1,    -1,
     176,   177,    95,    96,    97,   181,    -1,    -1,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    43,    44,
      -1,    -1,    -1,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      65,    66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,   172,
      95,    96,    97,   176,   177,    -1,    -1,    -1,   181,    -1,
      -1,   184,   185,    -1,    -1,    32,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   171,   172,    95,    96,
      97,   176,   177,    -1,    -1,    -1,   181,    -1,    -1,   184,
     185,    -1,    -1,    32,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    44,    -1,    -1,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,   172,    95,    96,    97,   176,
     177,    -1,    -1,    -1,   181,    -1,    -1,   184,   185,    -1,
      -1,    32,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    43,    44,    -1,    -1,    -1,    -1,    49,    50,
      51,    -1,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,   172,    95,    96,    97,   176,   177,    -1,
      -1,    -1,   181,    -1,    -1,   184,   185,    -1,    -1,    32,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    44,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     171,   172,    95,    96,    97,   176,   177,    -1,    -1,    -1,
     181,    -1,    -1,   184,   185,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    44,    -1,    -1,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    66,    67,    68,    69,
      -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,   172,
      -1,    -1,    -1,   176,   177,    95,    96,    97,   181,    -1,
      -1,   184,   185,    -1,    33,    34,    35,    -1,    -1,    -1,
      -1,   111,   112,    -1,    43,    44,    -1,    -1,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    -1,
      -1,   171,   172,    -1,    -1,    -1,   176,   177,    -1,    -1,
      -1,   181,   111,   112,   184,   185,    -1,    -1,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    44,    -1,
      -1,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    68,    69,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,   172,    -1,    -1,    -1,   176,   177,    95,
      96,    97,   181,    -1,    -1,   184,   185,    -1,    -1,    34,
      35,    -1,    -1,    -1,    -1,   111,   112,    -1,    43,    44,
      -1,    -1,    -1,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      65,    66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    -1,    -1,   171,   172,    -1,    -1,    -1,
     176,   177,    -1,    -1,    -1,   181,   111,   112,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    44,    -1,    -1,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   171,   172,    95,    96,
      97,   176,   177,    -1,    -1,    -1,   181,    -1,    -1,   184,
     185,    -1,    -1,    -1,   111,   112,    -1,    43,    44,    -1,
      -1,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    -1,    -1,   171,   172,    -1,    -1,    -1,   176,
     177,    -1,   179,    -1,   181,   111,   112,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    43,    44,    -1,    -1,    -1,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,   172,    95,    96,    97,
     176,   177,    -1,    -1,    -1,   181,   182,    -1,   184,   185,
      -1,    -1,    -1,   111,   112,    -1,    43,    44,    -1,    -1,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    -1,    -1,   171,   172,    -1,    -1,    -1,   176,   177,
      -1,    -1,    -1,   181,   111,   112,   184,   185,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,   172,    -1,    -1,    -1,   176,
     177,    70,    71,    72,   181,    74,    75,   184,   185,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,   168,   169,   170,   171,
     172,   173,   174,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
     169,   170,   171,   172,   173,   174,   175
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   188,     0,     1,    44,    64,    65,    66,    67,    68,
      69,    98,    99,   100,   101,   102,   105,   113,   114,   115,
     116,   117,   119,   137,   189,   201,   257,   107,    43,    44,
      49,    50,    51,    53,    54,    55,    64,    65,    66,    67,
      68,    69,    78,    79,    95,    96,    97,   111,   112,   155,
     171,   172,   176,   177,   181,   184,   185,   253,   254,   255,
     259,   253,   179,   179,   179,   261,   253,   253,    44,   202,
     203,   257,   204,   205,   257,   179,   179,   191,    45,    56,
      59,   103,   104,   108,   118,   151,   152,   153,   154,   179,
     190,    11,    12,    63,    70,    71,    78,    79,    80,    81,
      82,    83,    84,    85,    92,    93,    94,   106,   163,   164,
     165,   166,   171,   172,   173,   174,   175,   200,   206,   192,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   256,   253,   253,   253,   253,   253,   253,     8,     9,
      10,    11,    12,    70,    71,    72,    73,    74,    75,    76,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,   162,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     179,   109,   180,   253,   179,   179,   179,   180,   179,   180,
     138,   179,   253,     3,   111,   112,   253,   253,   259,   259,
     258,   179,   253,   213,   214,   213,   179,     4,     5,     6,
       7,     8,     9,    10,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      46,    47,    48,    52,    57,    58,    60,    61,   110,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   133,   134,   135,   136,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   157,   158,   159,   160,
     161,   207,   253,   253,   163,   171,   172,    62,   167,   167,
     183,   212,   215,   216,   138,   182,   186,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   260,
     253,   262,   203,   205,   179,   179,   179,   179,   180,   179,
     180,   109,   260,   109,   194,   258,   180,   216,   180,   253,
      29,    30,    31,    32,    34,    41,   181,   225,   226,   227,
     229,   230,   231,   232,   233,   253,    31,    34,    35,   225,
     229,   241,   253,    31,   225,   229,   253,    31,   225,   253,
      31,   225,   253,    31,   225,   253,   253,    32,    41,    42,
     243,   244,   253,    31,    33,    39,   225,   234,   238,   241,
     253,   253,    33,    36,   181,   253,   253,    37,   181,   230,
     253,   244,   253,    40,   181,   225,   234,   254,   181,   253,
     254,    33,    36,    38,   132,   230,   236,   237,   241,   243,
     226,   232,   253,   253,   253,   253,   253,   208,   211,   209,
     210,    39,   230,   235,   235,   230,   253,   253,   230,   253,
     253,   228,   229,   230,   228,   235,   228,   253,   181,   241,
     243,   253,   253,   253,   230,   230,   253,   233,   253,   233,
     230,   230,   253,   253,   238,   241,   230,   254,   254,    33,
      34,   240,   226,   253,   254,   179,   179,   180,   179,   179,
      29,    31,    32,    33,    34,    35,    36,    39,    41,   132,
     242,   253,   253,   253,   179,   180,   156,   163,   179,   179,
     179,   253,   253,   260,   179,   260,   109,   175,   222,   223,
     179,   196,   198,   180,   180,    34,    35,   181,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,    39,   180,
      32,   180,   180,   180,    33,   180,   180,   180,    32,   180,
     180,     3,   245,   246,   253,   251,   252,   253,   247,   248,
     253,   249,   250,   253,   180,   180,   180,   241,   180,   180,
     180,   180,   180,   180,   180,   253,   179,   179,   179,   179,
     193,   216,   253,   179,   179,   179,   179,   224,   179,   180,
     195,   258,   258,   225,   253,    34,   238,   239,   182,   182,
     233,    35,   225,   253,   238,   241,   225,   253,   225,   253,
     225,   253,   225,   253,   226,   232,   253,    34,    38,   182,
     181,   182,   181,   254,   253,    31,   182,    40,   181,   225,
     254,   255,   241,   253,   254,   255,    31,   234,   182,    31,
     230,   180,   180,   180,   180,   253,   253,   228,   182,   253,
     233,   233,   233,   233,   254,   230,   179,   217,   218,   253,
     223,   217,   223,   223,   182,   182,   180,   180,    32,    32,
     180,    33,   180,   246,   252,   248,   250,   139,   260,   260,
     230,   241,   182,   182,    31,   182,   230,   253,   219,   179,
     179,   156,   220,   221,   197,   199,   179,   180,   217,   217,
     221
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   187,   188,   188,   189,   190,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   191,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   192,   193,   189,   194,   189,   195,   189,   196,
     197,   189,   198,   199,   189,   189,   189,   200,   200,   200,
     201,   201,   202,   202,   203,   204,   204,   205,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     208,   207,   209,   207,   210,   207,   211,   207,   207,   212,
     212,   212,   214,   213,   215,   215,   216,   217,   218,   219,
     218,   220,   220,   220,   221,   222,   222,   222,   223,   224,
     223,   225,   225,   226,   226,   227,   228,   228,   229,   230,
     230,   230,   231,   231,   232,   232,   232,   233,   234,   234,
     235,   235,   236,   236,   236,   236,   237,   237,   238,   238,
     239,   239,   240,   240,   241,   241,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   243,   243,   244,   244,
     245,   245,   246,   246,   247,   247,   248,   249,   249,   250,
     251,   251,   252,   253,   253,   254,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   256,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   257,   257,   257,
     257,   257,   257,   257,   258,   259,   260,   261,   262
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     0,     4,     5,     5,     5,
       4,     5,     4,     4,     6,     3,     5,     5,     2,     2,
       3,     4,     4,     6,     4,     4,     4,     6,     6,     5,
       6,     0,     3,     2,     5,     3,     3,     3,     3,     2,
       3,     2,     0,     0,     7,     0,     6,     0,     7,     0,
       0,    11,     0,     0,    11,     3,     1,     0,     1,     2,
       0,     2,     1,     3,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     2,     2,     2,     2,     2,
       4,     2,     4,     2,     2,     2,     2,     2,     4,     4,
       4,     4,     2,     4,     2,     2,     4,     2,     2,     2,
       2,     2,     2,     2,     4,     4,     4,     4,     2,     2,
       4,     2,     2,     4,     4,     4,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     4,     4,     6,     4,     2,
       4,     2,     4,     2,     2,     4,     2,     2,     1,     2,
       4,     4,     4,     4,     4,     4,     4,     6,     2,     4,
       2,     6,     2,     4,     2,     4,     4,     4,     4,     4,
       2,     4,     2,     4,     2,     4,     4,     6,     4,     4,
       2,     2,     6,     2,     6,     4,     4,     4,     2,     6,
       2,     6,     2,     2,     2,     1,     2,     2,     2,     2,
       0,     3,     0,     3,     0,     3,     0,     3,     1,     0,
       1,     3,     0,     2,     0,     1,     2,     1,     0,     0,
       5,     0,     1,     3,     1,     0,     1,     3,     1,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     3,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       5,     3,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     0,     0,     0,     0,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* statement: label.part '\n'  */
#line 2233 "zmac.y"
                        { 
		// An identfier without a colon all by itself on a line
		// will be interpreted as a label.  But there's a very
		// good chance it is a misspelling of an instruction or
		// pseudo-op name creating silent errors.  Since the condition
		// is unusual we print a warning.  Unless it is followed by
		// a colon in which case there's no ambiguity.
		if ((yyvsp[-1].itemptr) && !firstcol && coloncnt == 0 && outpass) {
			fprintf(stderr, "%s(%d): warning: '%s' treated as label (instruction typo?)\n",
				src_name[now_in], linein[now_in], (yyvsp[-1].itemptr)->i_string);
			fprintf(stderr, "\tAdd a colon or move to first column to stop this warning.\n");
		}

		if ((yyvsp[-1].itemptr)) list(dollarsign);
		else  list1();
	}
#line 5006 "zmac.c"
    break;

  case 5: /* $@1: %empty  */
#line 2250 "zmac.y"
                   { list_dollarsign = 1; }
#line 5012 "zmac.c"
    break;

  case 6: /* statement: label.part $@1 operation '\n'  */
#line 2250 "zmac.y"
                                                           {
		list(list_dollarsign ? dollarsign : list_addr);
	}
#line 5020 "zmac.c"
    break;

  case 7: /* statement: symbol maybecolon EQU expression '\n'  */
#line 2254 "zmac.y"
                                              {
		do_equ((yyvsp[-4].itemptr), (yyvsp[-1].exprptr), 1);
		if ((yyvsp[-3].ival) == 2)
			(yyvsp[-4].itemptr)->i_scope |= SCOPE_PUBLIC;
	}
#line 5030 "zmac.c"
    break;

  case 8: /* statement: symbol maybecolon EQU aliasable '\n'  */
#line 2260 "zmac.y"
                                             {
		do_equ((yyvsp[-4].itemptr), expr_num(keyword_index((yyvsp[-1].itemptr))), 1);
		(yyvsp[-4].itemptr)->i_scope |= SCOPE_ALIAS;
		if ((yyvsp[-3].ival) == 2)
			(yyvsp[-4].itemptr)->i_scope |= SCOPE_PUBLIC;
	}
#line 5041 "zmac.c"
    break;

  case 9: /* statement: symbol maybecolon '=' expression '\n'  */
#line 2267 "zmac.y"
                                              {
		do_defl((yyvsp[-4].itemptr), (yyvsp[-1].exprptr), 1); // TODO: is '=' equate or defl?
		// I don't even recall what assembler I saw that allows '='
		// Not MACR0-80.  Not MRAS.  Not MAC.
		// I choose "defl" since it works so nicely with +=, etc.
		if ((yyvsp[-3].ival) == 2)
			(yyvsp[-4].itemptr)->i_scope |= SCOPE_PUBLIC;
	}
#line 5054 "zmac.c"
    break;

  case 10: /* statement: symbol DEFL expression '\n'  */
#line 2276 "zmac.y"
                                    {
		do_defl((yyvsp[-3].itemptr), (yyvsp[-1].exprptr), 1);
	}
#line 5062 "zmac.c"
    break;

  case 11: /* statement: symbol varop '=' expression '\n'  */
#line 2280 "zmac.y"
                                         {
		do_defl((yyvsp[-4].itemptr), expr_mk(expr_var((yyvsp[-4].itemptr)), (yyvsp[-3].ival), (yyvsp[-1].exprptr)), 1);
	}
#line 5070 "zmac.c"
    break;

  case 12: /* statement: symbol '+' '+' '\n'  */
#line 2284 "zmac.y"
                            {
		do_defl((yyvsp[-3].itemptr), expr_mk(expr_var((yyvsp[-3].itemptr)), '+', expr_num(1)), 1);
	}
#line 5078 "zmac.c"
    break;

  case 13: /* statement: symbol '-' '-' '\n'  */
#line 2288 "zmac.y"
                            {
		do_defl((yyvsp[-3].itemptr), expr_mk(expr_var((yyvsp[-3].itemptr)), '-', expr_num(1)), 1);
	}
#line 5086 "zmac.c"
    break;

  case 14: /* statement: symbol MINMAX expression ',' expression '\n'  */
#line 2292 "zmac.y"
                                                     {
		int val3 = (yyvsp[-3].exprptr)->e_value;
		int val5 = (yyvsp[-1].exprptr)->e_value;
		expr_reloc_check((yyvsp[-3].exprptr));
		expr_reloc_check((yyvsp[-1].exprptr));
		expr_scope_same((yyvsp[-3].exprptr), (yyvsp[-1].exprptr));
		switch ((yyvsp[-5].itemptr)->i_token) {
		case UNDECLARED: case DEFLED:
			(yyvsp[-5].itemptr)->i_token = DEFLED;
			(yyvsp[-5].itemptr)->i_pass = npass;
			(yyvsp[-5].itemptr)->i_scope |= (yyvsp[-3].exprptr)->e_scope;
			if ((yyvsp[-4].itemptr)->i_value)	/* max */
				list((yyvsp[-5].itemptr)->i_value = (val3 > val5? val3:val5));
			else list((yyvsp[-5].itemptr)->i_value = (val3 < val5? val3:val5));
			break;
		default:
			err[mflag]++;
			(yyvsp[-5].itemptr)->i_token = MULTDEF;
			(yyvsp[-5].itemptr)->i_pass = npass;
			list((yyvsp[-5].itemptr)->i_value);
		}
		expr_free((yyvsp[-3].exprptr));
		expr_free((yyvsp[-1].exprptr));
	}
#line 5115 "zmac.c"
    break;

  case 15: /* statement: IF_TK expression '\n'  */
#line 2317 "zmac.y"
                              {
		expr_number_check((yyvsp[-1].exprptr));
		if (ifptr >= ifstmax)
			error("Too many ifs");
		else
			*++ifptr = !((yyvsp[-1].exprptr)->e_value);

		saveopt = fopt;
		fopt = 1;
		list((yyvsp[-1].exprptr)->e_value);
		fopt = saveopt;
		expr_free((yyvsp[-1].exprptr));
	}
#line 5133 "zmac.c"
    break;

  case 16: /* statement: IF_CP_TK expression ',' expression '\n'  */
#line 2331 "zmac.y"
                                                {
		// Unpleasant duplication of IF_TK work.
		struct expr *compare = expr_mk((yyvsp[-3].exprptr), (yyvsp[-4].itemptr)->i_value , (yyvsp[-1].exprptr));
		expr_number_check(compare);
		if (ifptr >= ifstmax)
			error("Too many ifs");
		else
			*++ifptr = !(compare->e_value);

		saveopt = fopt;
		fopt = 1;
		list(compare->e_value);
		fopt = saveopt;
		expr_free(compare);
	}
#line 5153 "zmac.c"
    break;

  case 17: /* statement: IF_DEF_TK arg_on ARG arg_off '\n'  */
#line 2348 "zmac.y"
                                          {
		struct item *ip = locate(tempbuf);
		int declared = ip && ip->i_pass == npass;
		int value = declared == (yyvsp[-4].itemptr)->i_value;

		if (ifptr >= ifstmax)
			error("Too many ifs");
		else
			*++ifptr = !value;

		saveopt = fopt;
		fopt = 1;
		list(value);
		fopt = saveopt;
	}
#line 5173 "zmac.c"
    break;

  case 18: /* statement: ELSE_TK '\n'  */
#line 2364 "zmac.y"
                     {
		/* FIXME: it would be nice to spot repeated ELSEs, but how? */
		*ifptr = !*ifptr;
		saveopt = fopt;
		fopt = 1;
		list1();
		fopt = saveopt;
	}
#line 5186 "zmac.c"
    break;

  case 19: /* statement: ENDIF_TK '\n'  */
#line 2373 "zmac.y"
                      {
		if (ifptr == ifstack) err[bflag]++;
		else --ifptr;
		list1();
	}
#line 5196 "zmac.c"
    break;

  case 20: /* statement: label.part END '\n'  */
#line 2379 "zmac.y"
                            {
		list(dollarsign);
		do_end(NULL);
	}
#line 5205 "zmac.c"
    break;

  case 21: /* statement: label.part END expression '\n'  */
#line 2384 "zmac.y"
                                       {
		list((yyvsp[-1].exprptr)->e_value);
		do_end((yyvsp[-1].exprptr));
	}
#line 5214 "zmac.c"
    break;

  case 22: /* statement: label.part DEFS expression '\n'  */
#line 2389 "zmac.y"
                                        {
		expr_number_check((yyvsp[-1].exprptr));
		if ((yyvsp[-1].exprptr)->e_value < 0) err[vflag]++;
		if ((yyvsp[-1].exprptr)->e_value > 0) {
			if (!phaseflag) {
				list(dollarsign);
				flushbin();
				flushoth();
				dollarsign += (yyvsp[-1].exprptr)->e_value;
				olddollar += (yyvsp[-1].exprptr)->e_value;
				oldothdollar += (yyvsp[-1].exprptr)->e_value;
				emit_addr += (yyvsp[-1].exprptr)->e_value;
				advance_segment((yyvsp[-1].exprptr)->e_value);
				putrelcmd(RELCMD_SETLOC);
				putrelsegref(segment, seg_pos[segment]);
			}
			else
				dc((yyvsp[-1].exprptr)->e_value, 0);
		}
		else
			list1();

		expr_free((yyvsp[-1].exprptr));
	}
#line 5243 "zmac.c"
    break;

  case 23: /* statement: label.part DEFS expression ',' expression '\n'  */
#line 2414 "zmac.y"
                                                       {
		expr_number_check((yyvsp[-3].exprptr));
		expr_number_check((yyvsp[-1].exprptr));
		if ((yyvsp[-3].exprptr)->e_value < 0) err[vflag]++;
		if ((yyvsp[-1].exprptr)->e_value < -128 || (yyvsp[-1].exprptr)->e_value > 127) err[vflag]++;
		if ((yyvsp[-3].exprptr)->e_value > 0) {
			dc((yyvsp[-3].exprptr)->e_value, (yyvsp[-1].exprptr)->e_value);
		}
		else
			list1();

		expr_free((yyvsp[-3].exprptr));
		expr_free((yyvsp[-1].exprptr));
	}
#line 5262 "zmac.c"
    break;

  case 24: /* statement: label.part DC ONECHAR '\n'  */
#line 2429 "zmac.y"
                                   { emit(1, E_DATA, expr_num((yyvsp[-1].ival) | 0x80)); list(dollarsign); }
#line 5268 "zmac.c"
    break;

  case 25: /* statement: label.part DC TWOCHAR '\n'  */
#line 2431 "zmac.y"
                                   { emit(1, E_DATA, expr_num((yyvsp[-1].ival))); emit(1, E_DATA, expr_num(((yyvsp[-1].ival) >> 8) | 0x80)); list(dollarsign); }
#line 5274 "zmac.c"
    break;

  case 26: /* statement: label.part DC STRING '\n'  */
#line 2434 "zmac.y"
                {
			for (cp = (yyvsp[-1].cval); *cp != '\0'; cp++)
				if (!cp[1])
					emit(1, E_DATA, expr_num(*cp | 0x80));
				else
					emit(1, E_DATA, expr_num(*cp));

			list(dollarsign);
		}
#line 5288 "zmac.c"
    break;

  case 27: /* statement: label.part DC expression ',' expression '\n'  */
#line 2445 "zmac.y"
                {
			expr_number_check((yyvsp[-3].exprptr));
			expr_number_check((yyvsp[-1].exprptr));
			dc((yyvsp[-3].exprptr)->e_value, (yyvsp[-1].exprptr)->e_value);
			expr_free((yyvsp[-3].exprptr));
			expr_free((yyvsp[-1].exprptr));
		}
#line 5300 "zmac.c"
    break;

  case 28: /* statement: label.part ARGPSEUDO arg_on ARG arg_off '\n'  */
#line 2453 "zmac.y"
                                                     {
		list1();
		switch ((yyvsp[-4].itemptr)->i_value) {

		case PSTITL:	/* title */
			lineptr = linebuf;
			cp = tempbuf;
			title = titlespace;
			while ((*title++ = *cp++) && (title < &titlespace[TITLELEN]));
			*title = 0;
			title = titlespace;
			break;

		case PSRSYM:	/* rsym */
			if (pass2) break;
			insymtab(tempbuf);
			break;

		case PSWSYM:	/* wsym */
			writesyms = malloc(strlen(tempbuf)+1);
			strcpy(writesyms, tempbuf);
			break;
		case PSINC:	/* include file */
			if (mras) {
				// Allow for FILE/EXT; TRS-80 used / for extension.
				char *slash = strchr(tempbuf, '/');
				// Must have only one slash and short extension.
				if (slash && !strchr(slash + 1, '/') &&
					strlen(slash + 1) <= 3)
				{
					*slash = '.';
				}

				// MRAS appends "asm" suffix if not present.
				if (!strchr(tempbuf, '.'))
					strcat(tempbuf, ".asm");
			}
			next_source(tempbuf, 1);
			break ;
		case PSIMPORT:	/* import file */
			next_source(tempbuf, 0);
			break;
		case PSMACLIB:
			strcat(tempbuf, ".lib");
			next_source(tempbuf, 1);
			break;
		case PSCMN:
			common_block(tempbuf);
			break;
		}
	}
#line 5356 "zmac.c"
    break;

  case 29: /* statement: label.part ARGPSEUDO arg_on arg_off '\n'  */
#line 2505 "zmac.y"
                                                 {
		if ((yyvsp[-3].itemptr)->i_value == PSCMN) {
			common_block(" ");
		}
		else {
			fprintf(stderr, "Missing argument of '%s'\n", (yyvsp[-3].itemptr)->i_string);
			err[fflag]++;
		}
		list1();
	}
#line 5371 "zmac.c"
    break;

  case 30: /* statement: label.part INCBIN arg_on ARG arg_off '\n'  */
#line 2516 "zmac.y"
                                                  {
		incbin(tempbuf);
	}
#line 5379 "zmac.c"
    break;

  case 31: /* $@2: %empty  */
#line 2520 "zmac.y"
                { raw = 1; }
#line 5385 "zmac.c"
    break;

  case 32: /* statement: SPECIAL $@2 RAWTOKEN  */
#line 2520 "zmac.y"
                                      {
		int quote = 0;
		char *p, *q;
		switch ((yyvsp[-2].itemptr)->i_value) {
		case SPTITL:
			cp = tempbuf;
			title = titlespace;
			if (*cp == '\'' || *cp == '"')
				quote = *cp++;
			while ((*title++ = *cp++) && (title < &titlespace[TITLELEN]));
			if (quote && title > titlespace + 1 && title[-2] == quote)
				title[-2] = '\0';
			title = titlespace;
			list1();
			break;
		case SPSBTL:
			err[warn_notimpl]++;
			list1();
			break;
		case SPNAME:
			// Drop surrounding ('') if present
			p = tempbuf;
			q = strchr(tempbuf, '\0') - 1;
			if (*p == '(' && *q == ')' && q > p) p++, q--;
			if (*p == '\'' && *q == '\'' && q > p) p++, q--;
			q[1] = '\0';
			strncpy(progname, p, sizeof progname);
			progname[sizeof progname - 1] = '\0';
			list1();
			break;
		case SPCOM:
			quote = *tempbuf;
			list1();
			for (;;) {
				raw = 1;
				yychar = yylex();
				list1();
				if (yychar == 0)
					break;
				if (*tempbuf == quote) {
					yychar = yylex();
					break;
				}
			}
			break;

		case SPPRAGMA:
			if (strncmp(tempbuf, "bds", 3) == 0 && fbds && outpass) {
				fprintf(fbds, "%s\n", tempbuf + 4);
			}
			if (strncmp(tempbuf, "mds", 3) == 0 && fmds && outpass) {
				fprintf(fmds, "%s\n", tempbuf + 4);
			}
			list1();
			break;
		}
	}
#line 5447 "zmac.c"
    break;

  case 33: /* statement: LIST '\n'  */
#line 2578 "zmac.y"
                  {
		dolopt((yyvsp[-1].itemptr), 1); }
#line 5454 "zmac.c"
    break;

  case 34: /* statement: LIST mras_undecl_on expression mras_undecl_off '\n'  */
#line 2581 "zmac.y"
                                                            {
		int enable = (yyvsp[-2].exprptr)->e_value;

		if (mras) {
			if (ci_strcmp(tempbuf, "on") == 0)
				enable = 1;
			else if (ci_strcmp(tempbuf, "off") == 0)
				enable = -1;
			else {
				fprintf(stderr, "LIST not given 'on' or 'off'\n");
				err[fflag]++;
				list(dollarsign);
				goto dolopt_done;
			}
		}
		else {
			expr_number_check((yyvsp[-2].exprptr));
			expr_free((yyvsp[-2].exprptr));
		}

		dolopt((yyvsp[-4].itemptr), enable);

	dolopt_done: ;
	}
#line 5483 "zmac.c"
    break;

  case 35: /* statement: JRPROMOTE expression '\n'  */
#line 2606 "zmac.y"
                                  {
		expr_number_check((yyvsp[-1].exprptr));
		jopt = !!(yyvsp[-1].exprptr)->e_value;
		list1();
		expr_free((yyvsp[-1].exprptr));
	}
#line 5494 "zmac.c"
    break;

  case 36: /* statement: JPERROR expression '\n'  */
#line 2613 "zmac.y"
                                {
		expr_number_check((yyvsp[-1].exprptr));
		JPopt = !!(yyvsp[-1].exprptr)->e_value;
		list1();
		expr_free((yyvsp[-1].exprptr));
	}
#line 5505 "zmac.c"
    break;

  case 37: /* statement: PUBLIC public.list '\n'  */
#line 2620 "zmac.y"
                                {
		list1();
	}
#line 5513 "zmac.c"
    break;

  case 38: /* statement: EXTRN extrn.list '\n'  */
#line 2624 "zmac.y"
                              {
		list1();
	}
#line 5521 "zmac.c"
    break;

  case 39: /* statement: MRAS_MOD '\n'  */
#line 2628 "zmac.y"
                      {
		char *p = strchr(modstr, '\0') - 1;
		for (; p >= modstr; p--) {
			(*p)++;
			if (*p < 'Z')
				break;
			*p = 'A';
		}
		list1();
	}
#line 5536 "zmac.c"
    break;

  case 40: /* statement: label.part SETSEG '\n'  */
#line 2639 "zmac.y"
                               {
		if (relopt && segment != (yyvsp[-1].itemptr)->i_value) {
			segment = (yyvsp[-1].itemptr)->i_value;
			segchange = 1;
			dollarsign = seg_pos[(yyvsp[-1].itemptr)->i_value];
		}
		list1();
	}
#line 5549 "zmac.c"
    break;

  case 41: /* statement: INSTSET '\n'  */
#line 2648 "zmac.y"
                     {
		z80 = (yyvsp[-1].itemptr)->i_value;
		list1();
	}
#line 5558 "zmac.c"
    break;

  case 42: /* $@3: %empty  */
#line 2653 "zmac.y"
                         { param_parse = 1; }
#line 5564 "zmac.c"
    break;

  case 43: /* $@4: %empty  */
#line 2653 "zmac.y"
                                                             {
		param_parse = 0;
		(yyvsp[-4].itemptr)->i_token = MNAME;
		(yyvsp[-4].itemptr)->i_pass = npass;
		(yyvsp[-4].itemptr)->i_value = mfptr;
		if (keyword((yyvsp[-4].itemptr)->i_string) && !driopt) {
			sprintf(detail, "Macro '%s' will override the built-in '%s'",
				(yyvsp[-4].itemptr)->i_string, (yyvsp[-4].itemptr)->i_string);
			errwarn(warn_general, detail);
		}
#ifdef M_DEBUG
		fprintf (stderr, "%s(%d) [UNDECLARED MACRO %s]\n",
			src_name[now_in], linein[now_in], (yyvsp[-4].itemptr)->i_string);
#endif
		list1();
	}
#line 5585 "zmac.c"
    break;

  case 44: /* statement: UNDECLARED MACRO $@3 parm.list '\n' $@4 locals  */
#line 2669 "zmac.y"
               {
		mlex_list_on += !iopt;
		mfseek(mfile, (long)mfptr, 0);
		mlex((yyvsp[0].cval));
		mlex_list_on -= !iopt;
		parm_number = 0;
	}
#line 5597 "zmac.c"
    break;

  case 45: /* $@5: %empty  */
#line 2677 "zmac.y"
                            { arg_state.macarg = 1; }
#line 5603 "zmac.c"
    break;

  case 46: /* statement: label.part MNAME al $@5 arg.list '\n'  */
#line 2677 "zmac.y"
                                                                    {
#ifdef M_DEBUG
		fprintf (stderr, "%s(%d) [MNAME %s]\n",
			src_name[now_in], linein[now_in], (yyvsp[-4].itemptr)->i_string);
#endif
		(yyvsp[-4].itemptr)->i_uses++ ;
		arg_reset();
		parm_number = 0;
		delayed_list(dollarsign);
		expptr++;
		est = est2;
		est2 = NULL; // GWP - this may leak, but it avoids double-free crashes
		est[FLOC].value = floc;
		est[TEMPNUM].value = exp_number++;
		est[MIF].param = ifptr;
		est[REPNUM].value = 0;
		est[MSTR].param = NULL;
		floc = (yyvsp[-4].itemptr)->i_value;
		mfseek(mfile, (long)floc, 0);
	}
#line 5628 "zmac.c"
    break;

  case 47: /* $@6: %empty  */
#line 2698 "zmac.y"
                                           {
		expr_reloc_check((yyvsp[-2].exprptr));
		// MRAS compat would require treating the repeat count
		// as a byte value with 0 == 256.
		at_least_once = (yyvsp[-2].exprptr)->e_value > 0;

		if (at_least_once)
			delayed_list1();
		else
			list1();

		arg_reset();
	}
#line 5646 "zmac.c"
    break;

  case 48: /* statement: label.part REPT expression al '\n' $@6 locals  */
#line 2711 "zmac.y"
               {
		int pos = mfptr;
		mfseek(mfile, (long)mfptr, 0);
		mlex((yyvsp[0].cval));
		parm_number = 0;

		if (at_least_once) {
			expptr++;
			est = est2;
			est2 = NULL;
			est[FLOC].value = floc;
			est[TEMPNUM].value = exp_number++;
			est[MIF].param = ifptr;
			est[REPNUM].value = (yyvsp[-4].exprptr)->e_value - 1;
			est[MSTART].value = pos;
			est[MSTR].param = NULL;
			floc = pos;
			mfseek(mfile, (long)floc, 0);
		}
	}
#line 5671 "zmac.c"
    break;

  case 49: /* $@7: %empty  */
#line 2732 "zmac.y"
                                        { parm_number = 0; }
#line 5677 "zmac.c"
    break;

  case 50: /* $@8: %empty  */
#line 2733 "zmac.y"
        {
		at_least_once = est2[0].param[0];

		if (at_least_once)
			delayed_list1();
		else
			list1();
	}
#line 5690 "zmac.c"
    break;

  case 51: /* statement: label.part IRPC parm.single ',' $@7 al arg.element arg_off '\n' $@8 locals  */
#line 2741 "zmac.y"
               {
		int pos = mfptr;

		mfseek(mfile, (long)mfptr, 0);
		mlex((yyvsp[0].cval));

		parm_number = 0;

		if (at_least_once) {
			expptr++;
			est = est2;
			est2 = NULL;
			est[FLOC].value = floc;
			est[TEMPNUM].value = exp_number++;
			est[MIF].param = ifptr;
			est[REPNUM].value = 0;
			est[MSTART].value = pos;
			est[MSTR].param = est[0].param;
			est[0].param = malloc(2);
			est[0].param[0] = est[MSTR].param[0];
			est[0].param[1] = '\0';
			floc = pos;
			mfseek(mfile, (long)floc, 0);
		}
	}
#line 5720 "zmac.c"
    break;

  case 52: /* $@9: %empty  */
#line 2767 "zmac.y"
                                       { parm_number = 0; }
#line 5726 "zmac.c"
    break;

  case 53: /* $@10: %empty  */
#line 2768 "zmac.y"
        {
		// if the sub list is not empty
		at_least_once = est2[0].param[0] && est2[0].param[0] != ';'
			&& est2[0].param[0] != '\n';

		if (at_least_once)
			delayed_list1();
		else
			list1();
	}
#line 5741 "zmac.c"
    break;

  case 54: /* statement: label.part IRP parm.single ',' $@9 al arg.element arg_off '\n' $@10 locals  */
#line 2778 "zmac.y"
               {
		int pos = mfptr;
		mfseek(mfile, (long)mfptr, 0);
		mlex((yyvsp[0].cval));

		parm_number = 0;
		if (at_least_once) {
			expptr++;
			est = est2;
			est2 = NULL;
			est[FLOC].value = floc;
			est[TEMPNUM].value = exp_number++;
			est[MIF].param = ifptr;
			est[REPNUM].value = -1;
			est[MSTART].value = pos;
			est[MSTR].param = NULL;

			est[MARGP].ap = malloc(sizeof *est[MARGP].ap);
			est[MARGP].ap->arg = malloc(TEMPBUFSIZE);
			est[MARGP].ap->argsize = TEMPBUFSIZE;
			est[MARGP].ap->getch = str_getch;
			est[MARGP].ap->user_ptr = est[0].param;
			est[MARGP].ap->user_int = 0;
			est[MARGP].ap->user_peek = -1;
			est[MARGP].ap->peek = &est[MARGP].ap->user_peek;
			est[MARGP].ap->macarg = 0;
			est[MARGP].ap->didarg = 0;
			est[MARGP].ap->numarg = 0;

			est[0].param = est[MARGP].ap->arg;
			getarg(est[MARGP].ap);

			floc = pos;
			mfseek(mfile, (long)floc, 0);
		}
	}
#line 5782 "zmac.c"
    break;

  case 55: /* statement: label.part EXITM '\n'  */
#line 2815 "zmac.y"
                              {
		// XXX - popsi() is not safe, There is type-specific cleanup.
		//  But for now...
		// popsi() must be made safe as others use it.
		list1();
		popsi();
	}
#line 5794 "zmac.c"
    break;

  case 56: /* statement: error  */
#line 2823 "zmac.y"
              {
		err[fflag]++;
		arg_reset();
		parm_number = 0;
		param_parse = 0;

		if (est2)
		{
			int i;
			for (i=0; i<PARMMAX; i++) {
				if (est2[i].param) {
#ifdef M_DEBUG
	fprintf (stderr, "[Freeing2 arg%u(%p)]\n", i, est2[i].param),
#endif
					free(est2[i].param);
				}
			}
			free(est2);
			est2 = NULL;
		}

		while(yychar != '\n' && yychar != '\0') yychar = yylex();
		list(dollarsign);
		yyclearin;yyerrok;
	}
#line 5824 "zmac.c"
    break;

  case 57: /* maybecolon: %empty  */
#line 2851 "zmac.y"
                    { (yyval.ival) = 0; }
#line 5830 "zmac.c"
    break;

  case 58: /* maybecolon: ':'  */
#line 2853 "zmac.y"
            { (yyval.ival) = 1; }
#line 5836 "zmac.c"
    break;

  case 59: /* maybecolon: ':' ':'  */
#line 2855 "zmac.y"
                { (yyval.ival) = 2; }
#line 5842 "zmac.c"
    break;

  case 60: /* label.part: %empty  */
#line 2860 "zmac.y"
         {	label = (yyval.itemptr) = NULL;	}
#line 5848 "zmac.c"
    break;

  case 61: /* label.part: symbol maybecolon  */
#line 2862 "zmac.y"
                          {
		coloncnt = (yyvsp[0].ival);
		itemcpy(&pristine_label, (yyvsp[-1].itemptr));
		label = coloncnt == 0 ? (yyvsp[-1].itemptr) : NULL;
		(yyvsp[-1].itemptr)->i_scope |= segment;
		if ((yyvsp[0].ival) == 2)
			(yyvsp[-1].itemptr)->i_scope |= SCOPE_PUBLIC;

		if ((yyvsp[-1].itemptr)->i_string[0] != '.')
			llseq++;

		switch((yyvsp[-1].itemptr)->i_token) {
		case UNDECLARED:
			if (pass2) {
				sprintf(detail, "%s error - label '%s' not declared",
					errname[pflag], (yyvsp[-1].itemptr)->i_string);
				errwarn(pflag, detail);
			}
			else {
				(yyvsp[-1].itemptr)->i_token = LABEL;
				(yyvsp[-1].itemptr)->i_pass = npass;
				(yyvsp[-1].itemptr)->i_value = dollarsign;
			}
			break;
		case LABEL:
			if (!pass2) {
				(yyvsp[-1].itemptr)->i_token = MULTDEF;
				(yyvsp[-1].itemptr)->i_pass = npass;
				err[mflag]++;
			} else if ((yyvsp[-1].itemptr)->i_value != dollarsign) {
				// XXX - perhaps only allow retrys if JR promotions are in play?
				if (outpass) {
					if (!passfail) {
						sprintf(detail, "%s error - label '%s' changed from $%04x to $%04x",
							errname[pflag], (yyvsp[-1].itemptr)->i_string, (yyvsp[-1].itemptr)->i_value, dollarsign);
						errwarn(pflag, detail);
					}
				}
				else {
					(yyvsp[-1].itemptr)->i_value = dollarsign;
					passretry = 1;
				}
			}
			break;
		default:
			err[mflag]++;
			(yyvsp[-1].itemptr)->i_token = MULTDEF;
			(yyvsp[-1].itemptr)->i_pass = npass;
		}
	}
#line 5903 "zmac.c"
    break;

  case 64: /* public.part: symbol  */
#line 2921 "zmac.y"
               {
		if (!((yyvsp[0].itemptr)->i_scope & SCOPE_COMBLOCK))
			(yyvsp[0].itemptr)->i_scope |= SCOPE_PUBLIC;
		// Just being "public" does not #ifdef define a symbol.
		if (pass2) {
			if ((yyvsp[0].itemptr)->i_token == UNDECLARED) {
				sprintf(detail, "'%s' %s", (yyvsp[0].itemptr)->i_string, errname[uflag]);
				errwarn(uflag, detail);
			}
		}
	}
#line 5919 "zmac.c"
    break;

  case 67: /* extrn.part: symbol  */
#line 2941 "zmac.y"
               {
		if (pass2 && (yyvsp[0].itemptr)->i_scope != SCOPE_NONE && !((yyvsp[0].itemptr)->i_scope & SCOPE_EXTERNAL)) {
			fprintf(stderr, "Label scope change\n");
			err[fflag]++;
		}
		(yyvsp[0].itemptr)->i_pass = npass; // external labels seen as defined for #ifdef
		(yyvsp[0].itemptr)->i_scope |= SCOPE_EXTERNAL;
		if (pass2) {
			if ((yyvsp[0].itemptr)->i_token != UNDECLARED) {
				fprintf(stderr, "External label defined locally.\n");
				err[fflag]++;
			}
		}
	}
#line 5938 "zmac.c"
    break;

  case 68: /* varop: '+'  */
#line 2958 "zmac.y"
            { (yyval.ival) = '+'; }
#line 5944 "zmac.c"
    break;

  case 69: /* varop: '-'  */
#line 2959 "zmac.y"
            { (yyval.ival) = '-'; }
#line 5950 "zmac.c"
    break;

  case 70: /* varop: '*'  */
#line 2960 "zmac.y"
            { (yyval.ival) = '*'; }
#line 5956 "zmac.c"
    break;

  case 71: /* varop: '/'  */
#line 2961 "zmac.y"
            { (yyval.ival) = '/'; }
#line 5962 "zmac.c"
    break;

  case 72: /* varop: '%'  */
#line 2962 "zmac.y"
            { (yyval.ival) = '%'; }
#line 5968 "zmac.c"
    break;

  case 73: /* varop: '&'  */
#line 2963 "zmac.y"
            { (yyval.ival) = '&'; }
#line 5974 "zmac.c"
    break;

  case 74: /* varop: '|'  */
#line 2964 "zmac.y"
            { (yyval.ival) = '|'; }
#line 5980 "zmac.c"
    break;

  case 75: /* varop: '^'  */
#line 2965 "zmac.y"
            { (yyval.ival) = '^'; }
#line 5986 "zmac.c"
    break;

  case 76: /* varop: ANDAND  */
#line 2966 "zmac.y"
               { (yyval.ival) = ANDAND; }
#line 5992 "zmac.c"
    break;

  case 77: /* varop: OROR  */
#line 2967 "zmac.y"
             { (yyval.ival) = OROR; }
#line 5998 "zmac.c"
    break;

  case 78: /* varop: SHL  */
#line 2968 "zmac.y"
            { (yyval.ival) = SHL; }
#line 6004 "zmac.c"
    break;

  case 79: /* varop: SHR  */
#line 2969 "zmac.y"
            { (yyval.ival) = SHR; }
#line 6010 "zmac.c"
    break;

  case 80: /* varop: MROP_ADD  */
#line 2971 "zmac.y"
                 { (yyval.ival) = '+'; }
#line 6016 "zmac.c"
    break;

  case 81: /* varop: MROP_SUB  */
#line 2972 "zmac.y"
                 { (yyval.ival) = '-'; }
#line 6022 "zmac.c"
    break;

  case 82: /* varop: MROP_MUL  */
#line 2973 "zmac.y"
                 { (yyval.ival) = '*'; }
#line 6028 "zmac.c"
    break;

  case 83: /* varop: MROP_DIV  */
#line 2974 "zmac.y"
                 { (yyval.ival) = '/'; }
#line 6034 "zmac.c"
    break;

  case 84: /* varop: MROP_MOD  */
#line 2975 "zmac.y"
                 { (yyval.ival) = '%'; }
#line 6040 "zmac.c"
    break;

  case 85: /* varop: MROP_AND  */
#line 2976 "zmac.y"
                 { (yyval.ival) = '&'; }
#line 6046 "zmac.c"
    break;

  case 86: /* varop: MROP_OR  */
#line 2977 "zmac.y"
                 { (yyval.ival) = '|'; }
#line 6052 "zmac.c"
    break;

  case 87: /* varop: MROP_XOR  */
#line 2978 "zmac.y"
                 { (yyval.ival) = '^'; }
#line 6058 "zmac.c"
    break;

  case 88: /* varop: MROP_SHL  */
#line 2979 "zmac.y"
                 { (yyval.ival) = MROP_SHL; }
#line 6064 "zmac.c"
    break;

  case 89: /* varop: MROP_SHR  */
#line 2980 "zmac.y"
                 { (yyval.ival) = MROP_SHR; }
#line 6070 "zmac.c"
    break;

  case 90: /* varop: MROP_SHIFT  */
#line 2981 "zmac.y"
                   { (yyval.ival) = MROP_SHIFT; }
#line 6076 "zmac.c"
    break;

  case 91: /* operation: NOOPERAND  */
#line 2986 "zmac.y"
                { emit1((yyvsp[0].itemptr)->i_value, 0, 0, ET_NOARG); }
#line 6082 "zmac.c"
    break;

  case 92: /* operation: NOOPERAND expression  */
#line 2989 "zmac.y"
                {
			// XXX - maybe splitting out CPI is better?
			if (!z80 && (yyvsp[-1].itemptr)->i_value == 0166641)
				emit1(0376, 0, (yyvsp[0].exprptr), ET_BYTE);
			else
				err[fflag]++;
		}
#line 6094 "zmac.c"
    break;

  case 93: /* operation: SHIFT  */
#line 2998 "zmac.y"
                {
			if (!z80 && ((yyvsp[0].itemptr)->i_value & 070) < 020)
				emit(1, E_CODE, 0, 007 | ((yyvsp[0].itemptr)->i_value & 070));
			else
				err[fflag]++;
		}
#line 6105 "zmac.c"
    break;

  case 94: /* operation: JP expression  */
#line 3006 "zmac.y"
        {
		if ((yyvsp[0].exprptr)->e_parenthesized)
			errwarn(warn_general, "JP (addr) is equivalent to JP addr");

		if (z80 || (yyvsp[-1].itemptr)->i_value == 0303) {
			checkjp(0, (yyvsp[0].exprptr));
			emit(1, E_CODE16, (yyvsp[0].exprptr), 0303);
		}
		else
			// can't optimize jump on plus
			emit(1, E_CODE16, (yyvsp[0].exprptr), 0362);
	}
#line 6122 "zmac.c"
    break;

  case 95: /* operation: CALL expression  */
#line 3020 "zmac.y"
                {	emit(1, E_CODE16, (yyvsp[0].exprptr), 0315);	}
#line 6128 "zmac.c"
    break;

  case 96: /* operation: RST expression  */
#line 3023 "zmac.y"
        {
		// accepts rst 0-7 or rst 0,8,16,...,56
		int vec = (yyvsp[0].exprptr)->e_value;
		expr_number_check((yyvsp[0].exprptr));
		if ((vec > 7 || vec < 0) && (vec & ~(7 << 3)))
			err[vflag]++;
		if (vec > 7) vec >>= 3;
		emit(1, E_CODE, 0, (yyvsp[-1].itemptr)->i_value + ((vec & 7) << 3));
		expr_free((yyvsp[0].exprptr));
	}
#line 6143 "zmac.c"
    break;

  case 97: /* operation: ALUI8 expression  */
#line 3035 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value, 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6149 "zmac.c"
    break;

  case 98: /* operation: ALU_XY dxy  */
#line 3038 "zmac.y"
                {
			emit(3, E_CODE, 0, (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value, disp);
		}
#line 6157 "zmac.c"
    break;

  case 99: /* operation: ADD expression  */
#line 3043 "zmac.y"
                { emit1(0306, 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6163 "zmac.c"
    break;

  case 100: /* operation: ADD ACC ',' expression  */
#line 3046 "zmac.y"
                { emit1(0306, 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6169 "zmac.c"
    break;

  case 101: /* operation: ARITHC expression  */
#line 3049 "zmac.y"
                { emit1(0306 + ((yyvsp[-1].itemptr)->i_value & 070), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6175 "zmac.c"
    break;

  case 102: /* operation: ARITHC ACC ',' expression  */
#line 3052 "zmac.y"
                { emit1(0306 + ((yyvsp[-3].itemptr)->i_value & 070), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6181 "zmac.c"
    break;

  case 103: /* operation: LOGICAL expression  */
#line 3055 "zmac.y"
                {
			// CP is CALL P in 8080
			if (!z80 && (yyvsp[-1].itemptr)->i_value == 0270 && (yyvsp[-1].itemptr)->i_string[1] == 'p')
				emit(1, E_CODE16, (yyvsp[0].exprptr), 0364);
			else
				emit1(0306 | ((yyvsp[-1].itemptr)->i_value & 070), 0, (yyvsp[0].exprptr), ET_BYTE);
		}
#line 6193 "zmac.c"
    break;

  case 104: /* operation: AND expression  */
#line 3064 "zmac.y"
                { emit1(0306 | ((yyvsp[-1].itemptr)->i_value & 070), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6199 "zmac.c"
    break;

  case 105: /* operation: OR expression  */
#line 3067 "zmac.y"
                { emit1(0306 | ((yyvsp[-1].itemptr)->i_value & 070), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6205 "zmac.c"
    break;

  case 106: /* operation: XOR expression  */
#line 3070 "zmac.y"
                { emit1(0306 | ((yyvsp[-1].itemptr)->i_value & 070), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6211 "zmac.c"
    break;

  case 107: /* operation: TST expression  */
#line 3073 "zmac.y"
                { emit(2, E_CODE8, (yyvsp[0].exprptr), 0xED, 0x60 | (yyvsp[-1].itemptr)->i_value); }
#line 6217 "zmac.c"
    break;

  case 108: /* operation: LOGICAL ACC ',' expression  */
#line 3076 "zmac.y"
                { emit1(0306 | ((yyvsp[-3].itemptr)->i_value & 070), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6223 "zmac.c"
    break;

  case 109: /* operation: AND ACC ',' expression  */
#line 3079 "zmac.y"
                { emit1(0306 | ((yyvsp[-3].itemptr)->i_value & 070), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6229 "zmac.c"
    break;

  case 110: /* operation: OR ACC ',' expression  */
#line 3082 "zmac.y"
                { emit1(0306 | ((yyvsp[-3].itemptr)->i_value & 070), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6235 "zmac.c"
    break;

  case 111: /* operation: XOR ACC ',' expression  */
#line 3085 "zmac.y"
                { emit1(0306 | ((yyvsp[-3].itemptr)->i_value  & 070), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 6241 "zmac.c"
    break;

  case 112: /* operation: ADD allreg  */
#line 3088 "zmac.y"
                { emit1(0200 + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6247 "zmac.c"
    break;

  case 113: /* operation: ADD ACC ',' allreg  */
#line 3091 "zmac.y"
                { emit1(0200 + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6253 "zmac.c"
    break;

  case 114: /* operation: ADD m  */
#line 3094 "zmac.y"
                { emit(1, E_CODE, 0, 0206); }
#line 6259 "zmac.c"
    break;

  case 115: /* operation: ARITHC allreg  */
#line 3097 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6265 "zmac.c"
    break;

  case 116: /* operation: ARITHC ACC ',' allreg  */
#line 3100 "zmac.y"
                { emit1((yyvsp[-3].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6271 "zmac.c"
    break;

  case 117: /* operation: ARITHC m  */
#line 3103 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6277 "zmac.c"
    break;

  case 118: /* operation: LOGICAL allreg  */
#line 3106 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6283 "zmac.c"
    break;

  case 119: /* operation: LOGICAL m  */
#line 3109 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6289 "zmac.c"
    break;

  case 120: /* operation: AND allreg  */
#line 3112 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6295 "zmac.c"
    break;

  case 121: /* operation: OR allreg  */
#line 3115 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6301 "zmac.c"
    break;

  case 122: /* operation: XOR allreg  */
#line 3118 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6307 "zmac.c"
    break;

  case 123: /* operation: TST reg  */
#line 3121 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) << 3), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6313 "zmac.c"
    break;

  case 124: /* operation: LOGICAL ACC ',' allreg  */
#line 3124 "zmac.y"
                { emit1((yyvsp[-3].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6319 "zmac.c"
    break;

  case 125: /* operation: AND ACC ',' allreg  */
#line 3127 "zmac.y"
                { emit1((yyvsp[-3].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6325 "zmac.c"
    break;

  case 126: /* operation: OR ACC ',' allreg  */
#line 3130 "zmac.y"
                { emit1((yyvsp[-3].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6331 "zmac.c"
    break;

  case 127: /* operation: XOR ACC ',' allreg  */
#line 3133 "zmac.y"
                { emit1((yyvsp[-3].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6337 "zmac.c"
    break;

  case 128: /* operation: SHIFT reg  */
#line 3136 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6343 "zmac.c"
    break;

  case 129: /* operation: SHIFT_XY dxy  */
#line 3139 "zmac.y"
                {
			emit(4, E_CODE, 0, (yyvsp[-1].itemptr)->i_value >> 24, (yyvsp[-1].itemptr)->i_value >> 16, disp, (yyvsp[-1].itemptr)->i_value);
		}
#line 6351 "zmac.c"
    break;

  case 130: /* operation: SHIFT memxy ',' realreg  */
#line 3144 "zmac.y"
                { emit1((yyvsp[-3].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[-2].ival), 0, ET_NOARG_DISP); }
#line 6357 "zmac.c"
    break;

  case 131: /* operation: INCDEC allreg  */
#line 3147 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + (((yyvsp[0].ival) & 0377) << 3), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6363 "zmac.c"
    break;

  case 132: /* operation: INRDCR reg8  */
#line 3150 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + (((yyvsp[0].ival) & 0377) << 3), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6369 "zmac.c"
    break;

  case 133: /* operation: ARITHC HL ',' bcdehlsp  */
#line 3153 "zmac.y"
                { if ((yyvsp[-3].itemptr)->i_value == 0210)
				emit(2,E_CODE,0,0355,0112+(yyvsp[0].ival));
			else
				emit(2,E_CODE,0,0355,0102+(yyvsp[0].ival));
		}
#line 6379 "zmac.c"
    break;

  case 134: /* operation: ADD mar ',' bcdesp  */
#line 3160 "zmac.y"
                { emitdad((yyvsp[-2].ival),(yyvsp[0].ival)); }
#line 6385 "zmac.c"
    break;

  case 135: /* operation: ADD mar ',' mar  */
#line 3163 "zmac.y"
                {
			if ((yyvsp[-2].ival) != (yyvsp[0].ival)) {
				fprintf(stderr,"ADD mar, mar error\n");
				err[gflag]++;
			}
			emitdad((yyvsp[-2].ival),(yyvsp[0].ival));
		}
#line 6397 "zmac.c"
    break;

  case 136: /* operation: DAD evenreg8  */
#line 3171 "zmac.y"
                     { emitdad(040, (yyvsp[0].ival)); }
#line 6403 "zmac.c"
    break;

  case 137: /* operation: ARITH16 bcdesp  */
#line 3174 "zmac.y"
                {
			emit(2, E_CODE, 0, (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value | (yyvsp[0].ival));
		}
#line 6411 "zmac.c"
    break;

  case 138: /* operation: ARITH16 mar  */
#line 3179 "zmac.y"
                {
			int dst = (yyvsp[-1].itemptr)->i_value >> 8;
			int reg = (yyvsp[0].ival) >> 8;
			if (!reg) reg = 0xed;

			if (dst != reg) {
				if (dst == 0xed)
					fprintf(stderr, "dadc/dsbc cannot take ix or iy\n");
				else if (dst == 0xdd)
					fprintf(stderr, "dadx cannot take hl or iy\n");
				else
					fprintf(stderr, "dady cannot take hl or ix\n");
				err[gflag]++;
			}
			emit(2, E_CODE, 0, (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value | (yyvsp[0].ival));
		}
#line 6432 "zmac.c"
    break;

  case 139: /* operation: INCDEC evenreg  */
#line 3197 "zmac.y"
                { emit1((((yyvsp[-1].itemptr)->i_value & 1) << 3) + ((yyvsp[0].ival) & 0377) + 3, (yyvsp[0].ival), 0, ET_NOARG); }
#line 6438 "zmac.c"
    break;

  case 140: /* operation: INXDCX evenreg8  */
#line 3200 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG); }
#line 6444 "zmac.c"
    break;

  case 141: /* operation: MLT bcdehl  */
#line 3203 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG); }
#line 6450 "zmac.c"
    break;

  case 142: /* operation: PUSHPOP pushable  */
#line 3206 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG); }
#line 6456 "zmac.c"
    break;

  case 143: /* operation: PUSHPOP pushable8  */
#line 3209 "zmac.y"
                { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG); }
#line 6462 "zmac.c"
    break;

  case 144: /* operation: BIT expression  */
#line 3212 "zmac.y"
                {
			if (strcmp((yyvsp[-1].itemptr)->i_string, "set") == 0 && label) {
				// Clear error that label definition will have been set
				err[mflag] = 0;
				itemcpy(label, &pristine_label);
				do_defl(label, (yyvsp[0].exprptr), 0);
				list_dollarsign = 0;
				list_addr = label->i_value;
			}
			else {
				err[fflag]++;
			}
		}
#line 6480 "zmac.c"
    break;

  case 145: /* operation: BIT expression ',' reg  */
#line 3227 "zmac.y"
                {
			int bit = (yyvsp[-2].exprptr)->e_value;
			expr_number_check((yyvsp[-2].exprptr));
			expr_free((yyvsp[-2].exprptr));
			if (bit < 0 || bit > 7)
				err[vflag]++;
			emit1((yyvsp[-3].itemptr)->i_value + ((bit & 7) << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP);
		}
#line 6493 "zmac.c"
    break;

  case 146: /* operation: BIT_XY expression ',' dxy  */
#line 3237 "zmac.y"
                {
			int bit = (yyvsp[-2].exprptr)->e_value;
			expr_number_check((yyvsp[-2].exprptr));
			expr_free((yyvsp[-2].exprptr));
			if (bit < 0 || bit > 7)
				err[vflag]++;
			emit(4, E_CODE, 0, (yyvsp[-3].itemptr)->i_value >> 24, (yyvsp[-3].itemptr)->i_value >> 16, disp,
				(yyvsp[-3].itemptr)->i_value | (bit << 3));
		}
#line 6507 "zmac.c"
    break;

  case 147: /* operation: BIT expression ',' memxy ',' realreg  */
#line 3248 "zmac.y"
                {
			int bit = (yyvsp[-4].exprptr)->e_value;
			expr_number_check((yyvsp[-4].exprptr));
			expr_free((yyvsp[-4].exprptr));
			if (bit < 0 || bit > 7)
				err[vflag]++;
			emit1((yyvsp[-5].itemptr)->i_value + ((bit & 7) << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[-2].ival), 0, ET_NOARG_DISP);
		}
#line 6520 "zmac.c"
    break;

  case 148: /* operation: JP condition ',' expression  */
#line 3258 "zmac.y"
        {
		checkjp((yyvsp[-2].ival), (yyvsp[0].exprptr));
		emit(1, E_CODE16, (yyvsp[0].exprptr), 0302 + (yyvsp[-2].ival));
	}
#line 6529 "zmac.c"
    break;

  case 149: /* operation: JUMP8 expression  */
#line 3264 "zmac.y"
        {
		checkjp((yyvsp[-1].itemptr)->i_value, (yyvsp[0].exprptr));
		emit(1, E_CODE16, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value);
	}
#line 6538 "zmac.c"
    break;

  case 150: /* operation: JP '(' mar ')'  */
#line 3270 "zmac.y"
                { emit1(0351, (yyvsp[-1].ival), 0, ET_NOARG); }
#line 6544 "zmac.c"
    break;

  case 151: /* operation: JP mar  */
#line 3273 "zmac.y"
                { emit1(0351, (yyvsp[0].ival), 0, ET_NOARG); }
#line 6550 "zmac.c"
    break;

  case 152: /* operation: CALL condition ',' expression  */
#line 3276 "zmac.y"
                { emit(1, E_CODE16, (yyvsp[0].exprptr), 0304 + (yyvsp[-2].ival)); }
#line 6556 "zmac.c"
    break;

  case 153: /* operation: CALL8 expression  */
#line 3279 "zmac.y"
                { emit(1, E_CODE16, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value); }
#line 6562 "zmac.c"
    break;

  case 154: /* operation: JR expression  */
#line 3282 "zmac.y"
                { emitjr(030,(yyvsp[0].exprptr)); }
#line 6568 "zmac.c"
    break;

  case 155: /* operation: JR spcondition ',' expression  */
#line 3285 "zmac.y"
                { emitjr((yyvsp[-3].itemptr)->i_value + (yyvsp[-2].ival), (yyvsp[0].exprptr)); }
#line 6574 "zmac.c"
    break;

  case 156: /* operation: JR_COND expression  */
#line 3288 "zmac.y"
                { emitjr((yyvsp[-1].itemptr)->i_value, (yyvsp[0].exprptr)); }
#line 6580 "zmac.c"
    break;

  case 157: /* operation: DJNZ expression  */
#line 3291 "zmac.y"
                { emitjr((yyvsp[-1].itemptr)->i_value, (yyvsp[0].exprptr)); }
#line 6586 "zmac.c"
    break;

  case 158: /* operation: RET  */
#line 3294 "zmac.y"
                { emit(1, E_CODE, 0, (yyvsp[0].itemptr)->i_value); }
#line 6592 "zmac.c"
    break;

  case 159: /* operation: RET condition  */
#line 3297 "zmac.y"
                { emit(1, E_CODE, 0, 0300 + (yyvsp[0].ival)); }
#line 6598 "zmac.c"
    break;

  case 160: /* operation: LD allreg ',' allreg  */
#line 3300 "zmac.y"
                {
			// Many constraints on byte access to IX/IY registers.
			if (((yyvsp[-2].ival) | (yyvsp[0].ival)) >> 16) {
				int a = (yyvsp[-2].ival);
				int b = (yyvsp[0].ival);

				// Only ixh,ixh; ixh,ixl; ixl,ixh; ixl,ixl allowed.
				if (a >> 16 && b >> 16) {
					if (a >> 8 != b >> 8) {
						fprintf(stderr, "LD cannot move between ix and iy\n");
						err[gflag]++;
					}
				}
				else {
					int c = b >> 16 ? a : b;
					// No access to h, l, (hl), (ix), (iy)
					if (c == 4 || c == 5 || (c & 0xff) == 6) {
						fprintf(stderr, "LD cannot combine i/xy/lh and h,l,(hl),(ix) or (iy).\n");
						err[gflag]++;
					}
				}
			}

			if (((yyvsp[-2].ival) & 0377) == 6 && ((yyvsp[0].ival) & 0377) == 6) {
				fprintf(stderr,"LD reg, reg error: can't do memory to memory\n");
				err[gflag]++;
			}
			emit1(0100 + (((yyvsp[-2].ival) & 7) << 3) + ((yyvsp[0].ival) & 7), (yyvsp[-2].ival) | (yyvsp[0].ival), 0, ET_NOARG_DISP);
		}
#line 6632 "zmac.c"
    break;

  case 161: /* operation: LD_XY realreg ',' dxy  */
#line 3331 "zmac.y"
                {
			emit(3, E_CODE, 0, (yyvsp[-3].itemptr)->i_value >> 8, (yyvsp[-3].itemptr)->i_value | ((yyvsp[-2].ival) << 3), disp);
		}
#line 6640 "zmac.c"
    break;

  case 162: /* operation: ST_XY realreg ',' dxy  */
#line 3336 "zmac.y"
                {
			emit(3, E_CODE, 0, (yyvsp[-3].itemptr)->i_value >> 8, (yyvsp[-3].itemptr)->i_value | (yyvsp[-2].ival), disp);
		}
#line 6648 "zmac.c"
    break;

  case 163: /* operation: MOV reg8 ',' reg8  */
#line 3341 "zmac.y"
                {
			if ((yyvsp[-2].ival) == 6 && (yyvsp[0].ival) == 6) err[gflag]++;
			emit1(0100 + (((yyvsp[-2].ival) & 7) << 3) + ((yyvsp[0].ival) & 7),(yyvsp[-2].ival) | (yyvsp[0].ival), 0, ET_NOARG_DISP);
		}
#line 6657 "zmac.c"
    break;

  case 164: /* operation: LD allreg ',' noparenexpr  */
#line 3347 "zmac.y"
                { emit1(6 + (((yyvsp[-2].ival) & 0377) << 3), (yyvsp[-2].ival), (yyvsp[0].exprptr), ET_BYTE); }
#line 6663 "zmac.c"
    break;

  case 165: /* operation: MV_XY expression ',' dxy  */
#line 3350 "zmac.y"
                {
			emit(3, E_CODE8, (yyvsp[-2].exprptr), (yyvsp[-3].itemptr)->i_value >> 8, (yyvsp[-3].itemptr)->i_value, disp);
		}
#line 6671 "zmac.c"
    break;

  case 166: /* operation: MVI reg8 ',' expression  */
#line 3355 "zmac.y"
                { emit1(6 + (((yyvsp[-2].ival) & 0377) << 3), (yyvsp[-2].ival), (yyvsp[0].exprptr), ET_BYTE); }
#line 6677 "zmac.c"
    break;

  case 167: /* operation: LD allreg ',' '(' RP ')'  */
#line 3358 "zmac.y"
                {	if ((yyvsp[-4].ival) != 7) {
				fprintf(stderr,"LD reg, (RP) error\n");
				err[gflag]++;
			}
			else emit(1, E_CODE, 0, 012 + (yyvsp[-1].itemptr)->i_value);
		}
#line 6688 "zmac.c"
    break;

  case 168: /* operation: LDAX realreg  */
#line 3366 "zmac.y"
                {
			if ((yyvsp[0].ival) != 0 && (yyvsp[0].ival) != 2) err[gflag]++;
			emit(1, E_CODE, 0, 012 + ((yyvsp[0].ival) << 3));
		}
#line 6697 "zmac.c"
    break;

  case 169: /* operation: LD allreg ',' parenexpr  */
#line 3372 "zmac.y"
                {
			if ((yyvsp[-2].ival) != 7) {
				fprintf(stderr,"LD reg, (expr) error: A only valid destination\n");
				err[gflag]++;
			}
			else {
				expr_word_check((yyvsp[0].exprptr));
				emit(1, E_CODE16, (yyvsp[0].exprptr), 072);
			}
		}
#line 6712 "zmac.c"
    break;

  case 170: /* operation: LDA expression  */
#line 3384 "zmac.y"
                {
			expr_word_check((yyvsp[0].exprptr));
			emit(1, E_CODE16, (yyvsp[0].exprptr), 072);
		}
#line 6721 "zmac.c"
    break;

  case 171: /* operation: LD '(' RP ')' ',' ACC  */
#line 3390 "zmac.y"
                { emit(1, E_CODE, 0, 2 + (yyvsp[-3].itemptr)->i_value); }
#line 6727 "zmac.c"
    break;

  case 172: /* operation: STAX realreg  */
#line 3393 "zmac.y"
                {
			if ((yyvsp[0].ival) != 0 && (yyvsp[0].ival) != 2) err[gflag]++;
			emit(1, E_CODE, 0, 2 + ((yyvsp[0].ival) << 3));
		}
#line 6736 "zmac.c"
    break;

  case 173: /* operation: LD parenexpr ',' ACC  */
#line 3399 "zmac.y"
                {
			expr_word_check((yyvsp[-2].exprptr));
			emit(1, E_CODE16, (yyvsp[-2].exprptr), 062);
		}
#line 6745 "zmac.c"
    break;

  case 174: /* operation: STA expression  */
#line 3405 "zmac.y"
                {
			expr_word_check((yyvsp[0].exprptr));
			emit(1, E_CODE16, (yyvsp[0].exprptr), 062);
		}
#line 6754 "zmac.c"
    break;

  case 175: /* operation: LD allreg ',' MISCREG  */
#line 3411 "zmac.y"
                {
			if ((yyvsp[-2].ival) != 7) {
				fprintf(stderr,"LD reg, MISCREG error: A only valid destination\n");
				err[gflag]++;
			}
			else emit(2, E_CODE, 0, 0355, 0127 + (yyvsp[0].itemptr)->i_value);
		}
#line 6766 "zmac.c"
    break;

  case 176: /* operation: LD MISCREG ',' ACC  */
#line 3420 "zmac.y"
                { emit(2, E_CODE, 0, 0355, 0107 + (yyvsp[-2].itemptr)->i_value); }
#line 6772 "zmac.c"
    break;

  case 177: /* operation: LD evenreg ',' noparenexpr  */
#line 3423 "zmac.y"
                {
			expr_word_check((yyvsp[0].exprptr));
			emit1(1 + ((yyvsp[-2].ival) & 060), (yyvsp[-2].ival), (yyvsp[0].exprptr), ET_WORD);
		}
#line 6781 "zmac.c"
    break;

  case 178: /* operation: LXI evenreg8 ',' expression  */
#line 3429 "zmac.y"
                {
			expr_word_check((yyvsp[0].exprptr));
			emit1(1 + ((yyvsp[-2].ival) & 060), (yyvsp[-2].ival), (yyvsp[0].exprptr), ET_WORD);
		}
#line 6790 "zmac.c"
    break;

  case 179: /* operation: LD evenreg ',' parenexpr  */
#line 3435 "zmac.y"
                {
			expr_word_check((yyvsp[0].exprptr));
			if (((yyvsp[-2].ival) & 060) == 040)
				emit1(052, (yyvsp[-2].ival), (yyvsp[0].exprptr), ET_WORD);
			else
				emit(2, E_CODE16, (yyvsp[0].exprptr), 0355, 0113 + (yyvsp[-2].ival));
		}
#line 6802 "zmac.c"
    break;

  case 180: /* operation: LHLD expression  */
#line 3444 "zmac.y"
                {
			expr_word_check((yyvsp[0].exprptr));
			emit1(052, 040, (yyvsp[0].exprptr), ET_WORD);
		}
#line 6811 "zmac.c"
    break;

  case 181: /* operation: LD parenexpr ',' evenreg  */
#line 3450 "zmac.y"
                {
			expr_word_check((yyvsp[-2].exprptr));
			if (((yyvsp[0].ival) & 060) == 040)
				emit1(042, (yyvsp[0].ival), (yyvsp[-2].exprptr), ET_WORD);
			else
				emit(2, E_CODE16, (yyvsp[-2].exprptr), 0355, 0103 + (yyvsp[0].ival));
		}
#line 6823 "zmac.c"
    break;

  case 182: /* operation: SHLD expression  */
#line 3459 "zmac.y"
                {
			expr_word_check((yyvsp[0].exprptr));
			emit1(042, 040, (yyvsp[0].exprptr), ET_WORD);
		}
#line 6832 "zmac.c"
    break;

  case 183: /* operation: LD evenreg ',' mar  */
#line 3465 "zmac.y"
                {
			if ((yyvsp[-2].ival) != 060) {
				fprintf(stderr,"LD evenreg error\n");
				err[gflag]++;
			}
			else
				emit1(0371, (yyvsp[0].ival), 0, ET_NOARG);
		}
#line 6845 "zmac.c"
    break;

  case 184: /* operation: LDST16 expression  */
#line 3475 "zmac.y"
                {
			expr_word_check((yyvsp[0].exprptr));
			emit(2, E_CODE16, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value);
		}
#line 6854 "zmac.c"
    break;

  case 185: /* operation: EX RP ',' HL  */
#line 3481 "zmac.y"
                {
			if ((yyvsp[-2].itemptr)->i_value != 020) {
				fprintf(stderr,"EX RP, HL error\n");
				err[gflag]++;
			}
			else
				emit(1, E_CODE, 0, 0353);
		}
#line 6867 "zmac.c"
    break;

  case 186: /* operation: EX AF ',' AFp  */
#line 3491 "zmac.y"
                { emit(1, E_CODE, 0, 010); }
#line 6873 "zmac.c"
    break;

  case 187: /* operation: EX '(' SP ')' ',' mar  */
#line 3494 "zmac.y"
                { emit1(0343, (yyvsp[0].ival), 0, ET_NOARG); }
#line 6879 "zmac.c"
    break;

  case 188: /* operation: TK_IN realreg ',' parenexpr  */
#line 3497 "zmac.y"
                {
			if ((yyvsp[-2].ival) != 7) {
				fprintf(stderr,"IN reg, (expr) error\n");
				err[gflag]++;
			}
			else	{
				if ((yyvsp[0].exprptr)->e_value < 0 || (yyvsp[0].exprptr)->e_value > 255)
					err[vflag]++;
				emit(1, E_CODE8, (yyvsp[0].exprptr), (yyvsp[-3].itemptr)->i_value);
			}
		}
#line 6895 "zmac.c"
    break;

  case 189: /* operation: TK_IN0 realreg ',' parenexpr  */
#line 3510 "zmac.y"
                {
			if ((yyvsp[0].exprptr)->e_value < 0 || (yyvsp[0].exprptr)->e_value > 255)
				err[vflag]++;
			emit(2, E_CODE8, (yyvsp[0].exprptr), (yyvsp[-3].itemptr)->i_value >> 8,
				(yyvsp[-3].itemptr)->i_value | ((yyvsp[-2].ival) << 3));
		}
#line 6906 "zmac.c"
    break;

  case 190: /* operation: TK_IN0 parenexpr  */
#line 3518 "zmac.y"
                {
			if ((yyvsp[0].exprptr)->e_value < 0 || (yyvsp[0].exprptr)->e_value > 255)
				err[vflag]++;
			emit(2, E_CODE8, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value >> 8,
				(yyvsp[-1].itemptr)->i_value | (6 << 3));
		}
#line 6917 "zmac.c"
    break;

  case 191: /* operation: TK_IN expression  */
#line 3526 "zmac.y"
                {
			if ((yyvsp[0].exprptr)->e_value < 0 || (yyvsp[0].exprptr)->e_value > 255)
				err[vflag]++;
			emit(1, E_CODE8, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value);
		}
#line 6927 "zmac.c"
    break;

  case 192: /* operation: TK_IN realreg ',' '(' TK_C ')'  */
#line 3533 "zmac.y"
                { emit(2, E_CODE, 0, 0355, 0100 + ((yyvsp[-4].ival) << 3)); }
#line 6933 "zmac.c"
    break;

  case 193: /* operation: INP realreg  */
#line 3536 "zmac.y"
                { emit(2, E_CODE, 0, 0355, 0100 + ((yyvsp[0].ival) << 3)); }
#line 6939 "zmac.c"
    break;

  case 194: /* operation: TK_IN TK_F ',' '(' TK_C ')'  */
#line 3539 "zmac.y"
                { emit(2, E_CODE, 0, 0355, 0160); }
#line 6945 "zmac.c"
    break;

  case 195: /* operation: TK_IN '(' TK_C ')'  */
#line 3542 "zmac.y"
                { emit(2, E_CODE, 0, 0355, 0160); }
#line 6951 "zmac.c"
    break;

  case 196: /* operation: TK_OUT parenexpr ',' ACC  */
#line 3545 "zmac.y"
                {
			if ((yyvsp[-2].exprptr)->e_value < 0 || (yyvsp[-2].exprptr)->e_value > 255)
				err[vflag]++;
			emit(1, E_CODE8, (yyvsp[-2].exprptr), (yyvsp[-3].itemptr)->i_value);
		}
#line 6961 "zmac.c"
    break;

  case 197: /* operation: TK_OUT0 parenexpr ',' realreg  */
#line 3552 "zmac.y"
                {
			if ((yyvsp[-2].exprptr)->e_value < 0 || (yyvsp[-2].exprptr)->e_value > 255)
				err[vflag]++;
			emit(2, E_CODE8, (yyvsp[-2].exprptr), (yyvsp[-3].itemptr)->i_value >> 8,
				(yyvsp[-3].itemptr)->i_value | ((yyvsp[0].ival) << 3));
		}
#line 6972 "zmac.c"
    break;

  case 198: /* operation: TK_OUT expression  */
#line 3560 "zmac.y"
                {
			if ((yyvsp[0].exprptr)->e_value < 0 || (yyvsp[0].exprptr)->e_value > 255)
				err[vflag]++;
			emit(1, E_CODE8, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value);
		}
#line 6982 "zmac.c"
    break;

  case 199: /* operation: TK_OUT '(' TK_C ')' ',' realreg  */
#line 3567 "zmac.y"
                { emit(2, E_CODE, 0, 0355, 0101 + ((yyvsp[0].ival) << 3)); }
#line 6988 "zmac.c"
    break;

  case 200: /* operation: OUTP realreg  */
#line 3570 "zmac.y"
                { emit(2, E_CODE, 0, 0355, 0101 + ((yyvsp[0].ival) << 3)); }
#line 6994 "zmac.c"
    break;

  case 201: /* operation: TK_OUT '(' TK_C ')' ',' expression  */
#line 3573 "zmac.y"
                {
			expr_number_check((yyvsp[0].exprptr));
			if ((yyvsp[0].exprptr)->e_value != 0) {
				fprintf(stderr, "Can only output 0 to port C with OUT\n");
				err[vflag]++;
			}
			expr_free((yyvsp[0].exprptr));

			emit(2, E_CODE, 0, 0355, 0101 + (6 << 3));
		}
#line 7009 "zmac.c"
    break;

  case 202: /* operation: TSTIO parenexpr  */
#line 3585 "zmac.y"
                {
			if ((yyvsp[0].exprptr)->e_value < 0 || (yyvsp[0].exprptr)->e_value > 255)
				err[vflag]++;

			emit(2, E_CODE8, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value);
		}
#line 7020 "zmac.c"
    break;

  case 203: /* operation: IM expression  */
#line 3593 "zmac.y"
                {
			int im = (yyvsp[0].exprptr)->e_value;
			expr_number_check((yyvsp[0].exprptr));
			expr_free((yyvsp[0].exprptr));
			if (im > 2 || im < 0)
				err[vflag]++;
			else
				emit(2, E_CODE, 0, (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value + ((im + (im > 0)) << 3));
		}
#line 7034 "zmac.c"
    break;

  case 204: /* operation: PHASE expression  */
#line 3604 "zmac.y"
                {
			expr_number_check((yyvsp[0].exprptr));
			if (phaseflag) {
				err[oflag]++;
			} else {
				phaseflag = 1;
				phdollar = dollarsign;
				dollarsign = (yyvsp[0].exprptr)->e_value;
				phbegin = dollarsign;
			}
			expr_free((yyvsp[0].exprptr));
		}
#line 7051 "zmac.c"
    break;

  case 205: /* operation: DEPHASE  */
#line 3618 "zmac.y"
                {
			if (!phaseflag) {
				err[oflag]++;
			} else {
				phaseflag = 0;
				dollarsign = phdollar + dollarsign - phbegin;
			}
		}
#line 7064 "zmac.c"
    break;

  case 206: /* operation: ORG expression  */
#line 3628 "zmac.y"
                {
			expr_reloc_check((yyvsp[0].exprptr));
			// Cannot org to the other segment (but absolute values are OK)
			if (relopt && segment &&
				((yyvsp[0].exprptr)->e_scope & SCOPE_SEGMASK) &&
				((yyvsp[0].exprptr)->e_scope & SCOPE_SEGMASK) != segment)
			{
				err[rflag]++;
			}
			if (phaseflag) {
				err[oflag]++;
				dollarsign = phdollar + dollarsign - phbegin;
				phaseflag = 0;
			}
			if ((yyvsp[0].exprptr)->e_value-dollarsign) {
				flushbin();
				flushoth();
				olddollar = (yyvsp[0].exprptr)->e_value;
				oldothdollar = (yyvsp[0].exprptr)->e_value;
				dollarsign = (yyvsp[0].exprptr)->e_value;
				emit_addr = (yyvsp[0].exprptr)->e_value;
				seg_pos[segment] = dollarsign;
				if (seg_pos[segment] > seg_size[segment]) {
					seg_size[segment] = seg_pos[segment];
					if (segment == SEG_COMMON && cur_common)
						cur_common->i_value = seg_size[segment];
				}
				putrelcmd(RELCMD_SETLOC);
				putrelsegref(segment, seg_pos[segment]);
				segchange = 0;
			}
			expr_free((yyvsp[0].exprptr));
		}
#line 7102 "zmac.c"
    break;

  case 207: /* operation: ASSERT expression  */
#line 3663 "zmac.y"
                {
			list_dollarsign = 0;
			list_addr = (yyvsp[0].exprptr)->e_value;
			expr_number_check((yyvsp[0].exprptr));
			if (outpass && !(yyvsp[0].exprptr)->e_value)
			{
				err[aflag]++;
			}
			expr_free((yyvsp[0].exprptr));
		}
#line 7117 "zmac.c"
    break;

  case 208: /* operation: TSTATE expression  */
#line 3675 "zmac.y"
                {
			list_dollarsign = 0;
			list_addr = (yyvsp[0].exprptr)->e_value;
			expr_number_check((yyvsp[0].exprptr));
			tstates = (yyvsp[0].exprptr)->e_value;
			tstatesum[emit_addr] = tstates;
			expr_free((yyvsp[0].exprptr));
		}
#line 7130 "zmac.c"
    break;

  case 209: /* operation: SETOCF expression  */
#line 3685 "zmac.y"
                {
			list_dollarsign = 0;
			list_addr = (yyvsp[0].exprptr)->e_value;
			expr_number_check((yyvsp[0].exprptr));
			ocf = (yyvsp[0].exprptr)->e_value;
			ocfsum[emit_addr] = ocf;
			expr_free((yyvsp[0].exprptr));
		}
#line 7143 "zmac.c"
    break;

  case 210: /* $@11: %empty  */
#line 3694 "zmac.y"
             { full_exprs = 1; }
#line 7149 "zmac.c"
    break;

  case 211: /* operation: DEFB $@11 db.list  */
#line 3694 "zmac.y"
                                         { full_exprs = 0; }
#line 7155 "zmac.c"
    break;

  case 212: /* $@12: %empty  */
#line 3696 "zmac.y"
             { full_exprs = 1; }
#line 7161 "zmac.c"
    break;

  case 213: /* operation: DEFW $@12 dw.list  */
#line 3696 "zmac.y"
                                         { full_exprs = 0; }
#line 7167 "zmac.c"
    break;

  case 214: /* $@13: %empty  */
#line 3698 "zmac.y"
             { full_exprs = 1; }
#line 7173 "zmac.c"
    break;

  case 215: /* operation: DEF3 $@13 d3.list  */
#line 3698 "zmac.y"
                                         { full_exprs = 0; }
#line 7179 "zmac.c"
    break;

  case 216: /* $@14: %empty  */
#line 3700 "zmac.y"
             { full_exprs = 1; }
#line 7185 "zmac.c"
    break;

  case 217: /* operation: DEFD $@14 dd.list  */
#line 3700 "zmac.y"
                                         { full_exprs = 0; }
#line 7191 "zmac.c"
    break;

  case 222: /* $@15: %empty  */
#line 3712 "zmac.y"
             { param_parse = 1; }
#line 7197 "zmac.c"
    break;

  case 223: /* parm.single: $@15 parm.element  */
#line 3712 "zmac.y"
                                               { param_parse = 0; }
#line 7203 "zmac.c"
    break;

  case 224: /* maybeocto: %empty  */
#line 3714 "zmac.y"
           { (yyval.ival) = 0; }
#line 7209 "zmac.c"
    break;

  case 225: /* maybeocto: '#'  */
#line 3714 "zmac.y"
                             { (yyval.ival) = 1; }
#line 7215 "zmac.c"
    break;

  case 226: /* parm.element: maybeocto MPARM  */
#line 3718 "zmac.y"
                {
			if (parm_number >= PARMMAX)
				error("Too many parameters");
			(yyvsp[0].itemptr)->i_value = parm_number++;
			(yyvsp[0].itemptr)->i_scope = (yyvsp[-1].ival);
			(yyvsp[0].itemptr)->i_chain = 0;
		}
#line 7227 "zmac.c"
    break;

  case 227: /* locals: local_decls  */
#line 3727 "zmac.y"
                    {
		static char macpush[LINEBUFFERSIZE];
		// Because of locals the parser has to look ahead.
		// We'll have buffered that as we usually do so just a
		// matter of picking that up and cancelling any look-ahead.
		*lineptr = '\0';
		strcpy(macpush, linebuf);
		lineptr = linebuf;
		peekc = NOPEEK;
		yychar = YYEMPTY;
		(yyval.cval) = macpush;
	}
#line 7244 "zmac.c"
    break;

  case 229: /* $@16: %empty  */
#line 3743 "zmac.y"
                          { param_parse = 1; }
#line 7250 "zmac.c"
    break;

  case 230: /* local_decls: local_decls LOCAL $@16 local.list '\n'  */
#line 3743 "zmac.y"
                                                               { param_parse = 0; list1(); }
#line 7256 "zmac.c"
    break;

  case 234: /* local.element: MPARM  */
#line 3755 "zmac.y"
                {
			if (parm_number >= PARMMAX)
				error("Too many parameters");
			(yyvsp[0].itemptr)->i_value = parm_number++;
			(yyvsp[0].itemptr)->i_scope = 0;
			(yyvsp[0].itemptr)->i_chain = 1;
		}
#line 7268 "zmac.c"
    break;

  case 238: /* arg.element: ARG  */
#line 3775 "zmac.y"
                {
			cp = malloc(strlen(tempbuf)+1);
#ifdef M_DEBUG
			fprintf (stderr, "[Arg%u(%p): %s]\n", parm_number, cp, tempbuf);
#endif
			est2[parm_number++].param = cp;
			strcpy(cp, tempbuf);
		}
#line 7281 "zmac.c"
    break;

  case 239: /* $@17: %empty  */
#line 3784 "zmac.y"
            { arg_flag = 0; }
#line 7287 "zmac.c"
    break;

  case 240: /* arg.element: '%' $@17 expression  */
#line 3785 "zmac.y"
                {
			arg_flag = 1;
			expr_reloc_check((yyvsp[0].exprptr));
			sprintf(tempbuf, "%d", (yyvsp[0].exprptr)->e_value);
			est2[parm_number++].param = strdup(tempbuf);
			expr_free((yyvsp[0].exprptr));
		}
#line 7299 "zmac.c"
    break;

  case 245: /* ixylhreg: IXYLH  */
#line 3806 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7307 "zmac.c"
    break;

  case 248: /* m: COND  */
#line 3816 "zmac.y"
             { if ((yyvsp[0].itemptr)->i_value != 070) err[gflag]++; (yyval.ival) = 6; }
#line 7313 "zmac.c"
    break;

  case 249: /* realreg: REGNAME  */
#line 3820 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7321 "zmac.c"
    break;

  case 250: /* realreg: ACC  */
#line 3825 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7329 "zmac.c"
    break;

  case 251: /* realreg: TK_C  */
#line 3830 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7337 "zmac.c"
    break;

  case 252: /* mem: '(' HL ')'  */
#line 3836 "zmac.y"
                {
			(yyval.ival) = 6;
		}
#line 7345 "zmac.c"
    break;

  case 254: /* memxy: '(' INDEX dxy ')'  */
#line 3844 "zmac.y"
                {
			(yyval.ival) = ((yyvsp[-2].itemptr)->i_value & 0177400) | 6;
		}
#line 7353 "zmac.c"
    break;

  case 255: /* memxy: '(' INDEX ')'  */
#line 3849 "zmac.y"
                {
			disp = 0;
			(yyval.ival) = ((yyvsp[-1].itemptr)->i_value & 0177400) | 6;
		}
#line 7362 "zmac.c"
    break;

  case 256: /* memxy: dxy '(' INDEX ')'  */
#line 3855 "zmac.y"
                {
			(yyval.ival) = ((yyvsp[-1].itemptr)->i_value & 0177400) | 6;
		}
#line 7370 "zmac.c"
    break;

  case 257: /* dxy: expression  */
#line 3860 "zmac.y"
                {
			expr_number_check((yyvsp[0].exprptr));
			disp = (yyvsp[0].exprptr)->e_value;
			expr_free((yyvsp[0].exprptr));
			if (disp > 127 || disp < -128)
				err[vflag]++;
		}
#line 7382 "zmac.c"
    break;

  case 260: /* evenreg8: realreg  */
#line 3874 "zmac.y"
                { if ((yyvsp[0].ival) & 1) err[gflag]++; (yyval.ival) = (yyvsp[0].ival) << 3; }
#line 7388 "zmac.c"
    break;

  case 261: /* evenreg8: SP  */
#line 3876 "zmac.y"
           { (yyval.ival) = (yyvsp[0].itemptr)->i_value; }
#line 7394 "zmac.c"
    break;

  case 262: /* pushable: RP  */
#line 3880 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7402 "zmac.c"
    break;

  case 263: /* pushable: AF  */
#line 3885 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7410 "zmac.c"
    break;

  case 264: /* pushable: AFp  */
#line 3890 "zmac.y"
                {
			(yyval.ival) = 060;
		}
#line 7418 "zmac.c"
    break;

  case 266: /* pushable8: realreg  */
#line 3897 "zmac.y"
                { if (((yyvsp[0].ival) & 1) && (yyvsp[0].ival) != 7) err[gflag]++; (yyval.ival) = ((yyvsp[0].ival) & 6) << 3; }
#line 7424 "zmac.c"
    break;

  case 267: /* pushable8: PSW  */
#line 3899 "zmac.y"
            { (yyval.ival) = (yyvsp[0].itemptr)->i_value; }
#line 7430 "zmac.c"
    break;

  case 268: /* bcdesp: RP  */
#line 3903 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7438 "zmac.c"
    break;

  case 269: /* bcdesp: SP  */
#line 3908 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7446 "zmac.c"
    break;

  case 271: /* bcdehlsp: HL  */
#line 3916 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7454 "zmac.c"
    break;

  case 272: /* bcdehl: RP  */
#line 3922 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7462 "zmac.c"
    break;

  case 273: /* bcdehl: HL  */
#line 3927 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7470 "zmac.c"
    break;

  case 274: /* mar: HL  */
#line 3933 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7478 "zmac.c"
    break;

  case 275: /* mar: INDEX  */
#line 3938 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7486 "zmac.c"
    break;

  case 287: /* condition: COND  */
#line 3948 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7494 "zmac.c"
    break;

  case 288: /* spcondition: SPCOND  */
#line 3954 "zmac.y"
                {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7502 "zmac.c"
    break;

  case 289: /* spcondition: TK_C  */
#line 3959 "zmac.y"
                {	(yyval.ival) = 030;	}
#line 7508 "zmac.c"
    break;

  case 292: /* db.list.element: STRING  */
#line 3968 "zmac.y"
                {
			cp = (yyvsp[0].cval);
			while (*cp != '\0')
				emit(1,E_DATA,expr_num(*cp++));
		}
#line 7518 "zmac.c"
    break;

  case 293: /* db.list.element: expression  */
#line 3975 "zmac.y"
                {
			// Allow a single "TWOCHAR" value...
			if ((yyvsp[0].exprptr)->e_scope & SCOPE_TWOCHAR) {
				emit(1, E_DATA, expr_num((yyvsp[0].exprptr)->e_value & 0xff));
				emit(1, E_DATA, expr_num((yyvsp[0].exprptr)->e_value >> 8));
			}
			else {
				if (is_number((yyvsp[0].exprptr)) && ((yyvsp[0].exprptr)->e_value < -128 || (yyvsp[0].exprptr)->e_value > 255)) {
					err[vflag]++;
				}

				emit(1, E_DATA, (yyvsp[0].exprptr));
			}
		}
#line 7537 "zmac.c"
    break;

  case 296: /* dw.list.element: expression  */
#line 4001 "zmac.y"
                {
			if ((yyvsp[0].exprptr)->e_value < -32768 || (yyvsp[0].exprptr)->e_value > 65535) {
				err[vflag]++;
			}
			emit(2, E_DATA, (yyvsp[0].exprptr));
		}
#line 7548 "zmac.c"
    break;

  case 299: /* d3.list.element: expression  */
#line 4018 "zmac.y"
                {
			if ((yyvsp[0].exprptr)->e_value < -0x800000 || (yyvsp[0].exprptr)->e_value > 0xffffff) {
				err[vflag]++;
			}
			emit(3, E_DATA, (yyvsp[0].exprptr));
		}
#line 7559 "zmac.c"
    break;

  case 302: /* dd.list.element: expression  */
#line 4035 "zmac.y"
                {
			// Can't check overflow as I only have 32 bit ints.
			emit(4, E_DATA, (yyvsp[0].exprptr));
		}
#line 7568 "zmac.c"
    break;

  case 305: /* parenexpr: '(' expression ')'  */
#line 4049 "zmac.y"
                {	(yyval.exprptr) = (yyvsp[-1].exprptr); (yyval.exprptr)->e_parenthesized = 1;	}
#line 7574 "zmac.c"
    break;

  case 306: /* noparenexpr: LABEL  */
#line 4066 "zmac.y"
                {
			(yyval.exprptr) = expr_var((yyvsp[0].itemptr));
			(yyvsp[0].itemptr)->i_uses++;
		}
#line 7583 "zmac.c"
    break;

  case 307: /* noparenexpr: NUMBER  */
#line 4072 "zmac.y"
                {
			(yyval.exprptr) = expr_num((yyvsp[0].ival));
		}
#line 7591 "zmac.c"
    break;

  case 308: /* noparenexpr: ONECHAR  */
#line 4077 "zmac.y"
                {
			(yyval.exprptr) = expr_num((yyvsp[0].ival));
		}
#line 7599 "zmac.c"
    break;

  case 309: /* noparenexpr: TWOCHAR  */
#line 4082 "zmac.y"
                {
			(yyval.exprptr) = expr_num((yyvsp[0].ival));
			// Mark as twochar for db.list hackery
			(yyval.exprptr)->e_scope |= SCOPE_TWOCHAR;
		}
#line 7609 "zmac.c"
    break;

  case 310: /* noparenexpr: EQUATED  */
#line 4089 "zmac.y"
                {
			(yyval.exprptr) = expr_var((yyvsp[0].itemptr));
		}
#line 7617 "zmac.c"
    break;

  case 311: /* noparenexpr: WASEQUATED  */
#line 4094 "zmac.y"
                {
			(yyval.exprptr) = expr_var((yyvsp[0].itemptr));
		}
#line 7625 "zmac.c"
    break;

  case 312: /* noparenexpr: DEFLED  */
#line 4099 "zmac.y"
                {
			(yyval.exprptr) = expr_var((yyvsp[0].itemptr));
		}
#line 7633 "zmac.c"
    break;

  case 313: /* noparenexpr: COMMON  */
#line 4104 "zmac.y"
                {
			(yyval.exprptr) = expr_var((yyvsp[0].itemptr));
		}
#line 7641 "zmac.c"
    break;

  case 314: /* noparenexpr: '$'  */
#line 4109 "zmac.y"
                {
			(yyval.exprptr) = expr_num(dollarsign + emitptr - emitbuf);
			(yyval.exprptr)->e_scope = segment;
		}
#line 7650 "zmac.c"
    break;

  case 315: /* noparenexpr: UNDECLARED  */
#line 4115 "zmac.y"
                {
			int chkext = 1;
			(yyval.exprptr) = expr_alloc();
			(yyval.exprptr)->e_token = 'u';
			(yyval.exprptr)->e_item = (yyvsp[0].itemptr);
			(yyval.exprptr)->e_scope = (yyvsp[0].itemptr)->i_scope;
			(yyval.exprptr)->e_value = 0;

			if (!nmnvopt) {
				// Allow use of JMP, RET, etc. as values.
				struct item *i = keyword((yyvsp[0].itemptr)->i_string);
				if (item_is_verb(i)) {
					chkext = 0;
					(yyvsp[0].itemptr) = i;
					(yyval.exprptr)->e_item = i;
					(yyval.exprptr)->e_value = item_value(i);
				}
			}

			if (chkext && !((yyvsp[0].itemptr)->i_scope & SCOPE_EXTERNAL)) {
				sprintf(detail, "'%s' %s", (yyvsp[0].itemptr)->i_string, errname[uflag]);
				// Would be nice to add to list of undeclared errors
				errwarn(uflag, detail);
			}
		}
#line 7680 "zmac.c"
    break;

  case 316: /* noparenexpr: MULTDEF  */
#line 4142 "zmac.y"
                {
			(yyval.exprptr) = expr_alloc();
			(yyval.exprptr)->e_token = 'm';
			(yyval.exprptr)->e_item = (yyvsp[0].itemptr);
			(yyval.exprptr)->e_scope = (yyvsp[0].itemptr)->i_scope;
			// Use the current value.  Harmless enough as this
			// will normally error out yet vital to allow
			// "var set var+1" to function.
			(yyval.exprptr)->e_value = (yyvsp[0].itemptr)->i_value;
		}
#line 7695 "zmac.c"
    break;

  case 317: /* $@18: %empty  */
#line 4153 "zmac.y"
            { raw = 2; }
#line 7701 "zmac.c"
    break;

  case 318: /* noparenexpr: NUL $@18 RAWTOKEN  */
#line 4154 "zmac.y"
                {
			(yyval.exprptr) = expr_num(tempbuf[0] ? -1 : 0);
		}
#line 7709 "zmac.c"
    break;

  case 319: /* noparenexpr: expression '+' expression  */
#line 4157 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '+', (yyvsp[0].exprptr)); }
#line 7715 "zmac.c"
    break;

  case 320: /* noparenexpr: expression '-' expression  */
#line 4158 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '-', (yyvsp[0].exprptr)); }
#line 7721 "zmac.c"
    break;

  case 321: /* noparenexpr: expression '/' expression  */
#line 4159 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '/', (yyvsp[0].exprptr)); }
#line 7727 "zmac.c"
    break;

  case 322: /* noparenexpr: expression '*' expression  */
#line 4160 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '*', (yyvsp[0].exprptr)); }
#line 7733 "zmac.c"
    break;

  case 323: /* noparenexpr: expression '%' expression  */
#line 4161 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '%', (yyvsp[0].exprptr)); }
#line 7739 "zmac.c"
    break;

  case 324: /* noparenexpr: expression '&' expression  */
#line 4162 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '&', (yyvsp[0].exprptr)); }
#line 7745 "zmac.c"
    break;

  case 325: /* noparenexpr: expression AND expression  */
#line 4163 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '&', (yyvsp[0].exprptr)); }
#line 7751 "zmac.c"
    break;

  case 326: /* noparenexpr: expression '|' expression  */
#line 4164 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '|', (yyvsp[0].exprptr)); }
#line 7757 "zmac.c"
    break;

  case 327: /* noparenexpr: expression OR expression  */
#line 4165 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '|', (yyvsp[0].exprptr)); }
#line 7763 "zmac.c"
    break;

  case 328: /* noparenexpr: expression '^' expression  */
#line 4166 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '^', (yyvsp[0].exprptr)); }
#line 7769 "zmac.c"
    break;

  case 329: /* noparenexpr: expression XOR expression  */
#line 4167 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '^', (yyvsp[0].exprptr)); }
#line 7775 "zmac.c"
    break;

  case 330: /* noparenexpr: expression SHL expression  */
#line 4168 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHL, (yyvsp[0].exprptr)); }
#line 7781 "zmac.c"
    break;

  case 331: /* noparenexpr: expression SHR expression  */
#line 4169 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHR, (yyvsp[0].exprptr)); }
#line 7787 "zmac.c"
    break;

  case 332: /* noparenexpr: expression '<' expression  */
#line 4170 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '<', (yyvsp[0].exprptr)); }
#line 7793 "zmac.c"
    break;

  case 333: /* noparenexpr: expression LT expression  */
#line 4171 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '<', (yyvsp[0].exprptr)); }
#line 7799 "zmac.c"
    break;

  case 334: /* noparenexpr: expression '=' expression  */
#line 4172 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '=', (yyvsp[0].exprptr)); }
#line 7805 "zmac.c"
    break;

  case 335: /* noparenexpr: expression EQ expression  */
#line 4173 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '=', (yyvsp[0].exprptr)); }
#line 7811 "zmac.c"
    break;

  case 336: /* noparenexpr: expression '>' expression  */
#line 4174 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '>', (yyvsp[0].exprptr)); }
#line 7817 "zmac.c"
    break;

  case 337: /* noparenexpr: expression GT expression  */
#line 4175 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '>', (yyvsp[0].exprptr)); }
#line 7823 "zmac.c"
    break;

  case 338: /* noparenexpr: expression LE expression  */
#line 4176 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), LE, (yyvsp[0].exprptr)); }
#line 7829 "zmac.c"
    break;

  case 339: /* noparenexpr: expression NE expression  */
#line 4177 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), NE, (yyvsp[0].exprptr)); }
#line 7835 "zmac.c"
    break;

  case 340: /* noparenexpr: expression GE expression  */
#line 4178 "zmac.y"
                                  { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), GE, (yyvsp[0].exprptr)); }
#line 7841 "zmac.c"
    break;

  case 341: /* noparenexpr: expression ANDAND expression  */
#line 4179 "zmac.y"
                                     { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), ANDAND, (yyvsp[0].exprptr)); }
#line 7847 "zmac.c"
    break;

  case 342: /* noparenexpr: expression OROR expression  */
#line 4180 "zmac.y"
                                   { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), OROR, (yyvsp[0].exprptr)); }
#line 7853 "zmac.c"
    break;

  case 343: /* noparenexpr: expression MROP_ADD expression  */
#line 4182 "zmac.y"
                                       { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '+', (yyvsp[0].exprptr)); }
#line 7859 "zmac.c"
    break;

  case 344: /* noparenexpr: expression MROP_SUB expression  */
#line 4183 "zmac.y"
                                       { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '-', (yyvsp[0].exprptr)); }
#line 7865 "zmac.c"
    break;

  case 345: /* noparenexpr: expression MROP_DIV expression  */
#line 4184 "zmac.y"
                                       { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '/', (yyvsp[0].exprptr)); }
#line 7871 "zmac.c"
    break;

  case 346: /* noparenexpr: expression MROP_MUL expression  */
#line 4185 "zmac.y"
                                       { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '*', (yyvsp[0].exprptr)); }
#line 7877 "zmac.c"
    break;

  case 347: /* noparenexpr: expression MROP_MOD expression  */
#line 4186 "zmac.y"
                                       { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '%', (yyvsp[0].exprptr)); }
#line 7883 "zmac.c"
    break;

  case 348: /* noparenexpr: expression MROP_AND expression  */
#line 4187 "zmac.y"
                                       { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '&', (yyvsp[0].exprptr)); }
#line 7889 "zmac.c"
    break;

  case 349: /* noparenexpr: expression MROP_OR expression  */
#line 4188 "zmac.y"
                                       { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '|', (yyvsp[0].exprptr)); }
#line 7895 "zmac.c"
    break;

  case 350: /* noparenexpr: expression MROP_XOR expression  */
#line 4189 "zmac.y"
                                       { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '^', (yyvsp[0].exprptr)); }
#line 7901 "zmac.c"
    break;

  case 351: /* noparenexpr: expression MROP_SHL expression  */
#line 4190 "zmac.y"
                                       { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHL, (yyvsp[0].exprptr)); }
#line 7907 "zmac.c"
    break;

  case 352: /* noparenexpr: expression MROP_SHR expression  */
#line 4191 "zmac.y"
                                       { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHR, (yyvsp[0].exprptr)); }
#line 7913 "zmac.c"
    break;

  case 353: /* noparenexpr: expression MROP_LT expression  */
#line 4192 "zmac.y"
                                      { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '<', (yyvsp[0].exprptr)); }
#line 7919 "zmac.c"
    break;

  case 354: /* noparenexpr: expression MROP_EQ expression  */
#line 4193 "zmac.y"
                                      { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '=', (yyvsp[0].exprptr)); }
#line 7925 "zmac.c"
    break;

  case 355: /* noparenexpr: expression MROP_GT expression  */
#line 4194 "zmac.y"
                                      { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '>', (yyvsp[0].exprptr)); }
#line 7931 "zmac.c"
    break;

  case 356: /* noparenexpr: expression MROP_LE expression  */
#line 4195 "zmac.y"
                                      { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), LE, (yyvsp[0].exprptr)); }
#line 7937 "zmac.c"
    break;

  case 357: /* noparenexpr: expression MROP_NE expression  */
#line 4196 "zmac.y"
                                      { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), NE, (yyvsp[0].exprptr)); }
#line 7943 "zmac.c"
    break;

  case 358: /* noparenexpr: expression MROP_GE expression  */
#line 4197 "zmac.y"
                                      { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), GE, (yyvsp[0].exprptr)); }
#line 7949 "zmac.c"
    break;

  case 359: /* noparenexpr: expression MROP_SHIFT expression  */
#line 4200 "zmac.y"
                {
			if ((yyvsp[0].exprptr)->e_value < 0) {
				(yyvsp[0].exprptr)->e_value = -(yyvsp[0].exprptr)->e_value;
				(yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHR, (yyvsp[0].exprptr));
			}
			else
				(yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHL, (yyvsp[0].exprptr));
		}
#line 7962 "zmac.c"
    break;

  case 360: /* noparenexpr: expression '?' expression ':' expression  */
#line 4210 "zmac.y"
                {
			expr_number_check((yyvsp[-4].exprptr));
			if ((yyvsp[-4].exprptr)->e_value) {
				(yyval.exprptr) = (yyvsp[-2].exprptr);
				expr_free((yyvsp[0].exprptr));
			}
			else {
				(yyval.exprptr) = (yyvsp[0].exprptr);
				expr_free((yyvsp[-2].exprptr));
			}
			expr_free((yyvsp[-4].exprptr));
		}
#line 7979 "zmac.c"
    break;

  case 361: /* noparenexpr: '[' expression ']'  */
#line 4224 "zmac.y"
                {	(yyval.exprptr) = (yyvsp[-1].exprptr);	}
#line 7985 "zmac.c"
    break;

  case 362: /* noparenexpr: '~' expression  */
#line 4227 "zmac.y"
                {	(yyval.exprptr) = expr_op((yyvsp[0].exprptr), '~', 0, ~(yyvsp[0].exprptr)->e_value);	}
#line 7991 "zmac.c"
    break;

  case 363: /* noparenexpr: MROP_NOT expression  */
#line 4230 "zmac.y"
                {	(yyval.exprptr) = expr_op((yyvsp[0].exprptr), '~', 0, ~(yyvsp[0].exprptr)->e_value);	}
#line 7997 "zmac.c"
    break;

  case 364: /* noparenexpr: '!' expression  */
#line 4233 "zmac.y"
                {	(yyval.exprptr) = expr_op((yyvsp[0].exprptr), '!', 0, !(yyvsp[0].exprptr)->e_value * trueval);	}
#line 8003 "zmac.c"
    break;

  case 365: /* noparenexpr: '+' expression  */
#line 4236 "zmac.y"
                {	(yyval.exprptr) = (yyvsp[0].exprptr); /* no effect */	}
#line 8009 "zmac.c"
    break;

  case 366: /* noparenexpr: MROP_ADD expression  */
#line 4239 "zmac.y"
                {	(yyval.exprptr) = (yyvsp[0].exprptr); /* no effect */	}
#line 8015 "zmac.c"
    break;

  case 367: /* noparenexpr: '-' expression  */
#line 4242 "zmac.y"
                {	(yyval.exprptr) = expr_op((yyvsp[0].exprptr), '-', 0, -(yyvsp[0].exprptr)->e_value);	}
#line 8021 "zmac.c"
    break;

  case 368: /* noparenexpr: MROP_SUB expression  */
#line 4245 "zmac.y"
                {	(yyval.exprptr) = expr_op((yyvsp[0].exprptr), '-', 0, -(yyvsp[0].exprptr)->e_value);	}
#line 8027 "zmac.c"
    break;

  case 369: /* noparenexpr: T expression  */
#line 4248 "zmac.y"
                {
			expr_reloc_check((yyvsp[0].exprptr));
			(yyval.exprptr) = expr_num(tstatesum[phaseaddr((yyvsp[0].exprptr)->e_value)]);
			expr_free((yyvsp[0].exprptr));
		}
#line 8037 "zmac.c"
    break;

  case 370: /* noparenexpr: TILO expression  */
#line 4255 "zmac.y"
                {
			int low;
			expr_reloc_check((yyvsp[0].exprptr));
			get_tstates(memory + phaseaddr((yyvsp[0].exprptr)->e_value), &low, 0, 0, 0);
			(yyval.exprptr) = expr_num(low);
			expr_free((yyvsp[0].exprptr));
		}
#line 8049 "zmac.c"
    break;

  case 371: /* noparenexpr: TIHI expression  */
#line 4264 "zmac.y"
                {
			int high;
			expr_reloc_check((yyvsp[0].exprptr));
			get_tstates(memory + phaseaddr((yyvsp[0].exprptr)->e_value), 0, &high, 0, 0);
			(yyval.exprptr) = expr_num(high);
			expr_free((yyvsp[0].exprptr));
		}
#line 8061 "zmac.c"
    break;

  case 372: /* noparenexpr: OCF expression  */
#line 4273 "zmac.y"
                {
			expr_reloc_check((yyvsp[0].exprptr));
			(yyval.exprptr) = expr_num(ocfsum[phaseaddr((yyvsp[0].exprptr)->e_value)]);
			expr_free((yyvsp[0].exprptr));
		}
#line 8071 "zmac.c"
    break;

  case 373: /* noparenexpr: LOW expression  */
#line 4280 "zmac.y"
                {
			(yyval.exprptr) = expr_op((yyvsp[0].exprptr), LOW, 0, (yyvsp[0].exprptr)->e_value & 0xff);
		}
#line 8079 "zmac.c"
    break;

  case 374: /* noparenexpr: MROP_LOW expression  */
#line 4285 "zmac.y"
                {
			(yyval.exprptr) = expr_op((yyvsp[0].exprptr), LOW, 0, (yyvsp[0].exprptr)->e_value & 0xff);
		}
#line 8087 "zmac.c"
    break;

  case 375: /* noparenexpr: HIGH expression  */
#line 4290 "zmac.y"
                {
			(yyval.exprptr) = expr_op((yyvsp[0].exprptr), HIGH, 0, ((yyvsp[0].exprptr)->e_value >> 8) & 0xff);
		}
#line 8095 "zmac.c"
    break;

  case 376: /* noparenexpr: MROP_HIGH expression  */
#line 4295 "zmac.y"
                {
			(yyval.exprptr) = expr_op((yyvsp[0].exprptr), HIGH, 0, ((yyvsp[0].exprptr)->e_value >> 8) & 0xff);
		}
#line 8103 "zmac.c"
    break;

  case 384: /* al: %empty  */
#line 4318 "zmac.y"
        { int i;
		if (expptr >= MAXEXP)
			error("Macro expansion level too deep");
		est2 = (union exprec *) malloc((PARMMAX + PAREXT) * sizeof *est2);
		expstack[expptr] = est2;
		for (i=0; i<PARMMAX; i++)
			est2[i].param = 0;
		arg_start();
	}
#line 8117 "zmac.c"
    break;

  case 385: /* arg_on: %empty  */
#line 4331 "zmac.y"
        {	arg_start();	}
#line 8123 "zmac.c"
    break;

  case 386: /* arg_off: %empty  */
#line 4335 "zmac.y"
        {	arg_reset();	}
#line 8129 "zmac.c"
    break;

  case 387: /* mras_undecl_on: %empty  */
#line 4339 "zmac.y"
        {	if (mras) mras_undecl_ok = 1; }
#line 8135 "zmac.c"
    break;

  case 388: /* mras_undecl_off: %empty  */
#line 4343 "zmac.y"
        {	if (mras) mras_undecl_ok = 0; }
#line 8141 "zmac.c"
    break;


#line 8145 "zmac.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 4346 "zmac.y"

/*extern int	yylval;*/

#define F_END	0
#define OTHER	1
#define SPACE	2
#define DIGIT	3
#define LETTER	4
#define STARTER 5
#define DOLLAR	6


/*
 *  This is the table of character classes.  It is used by the lexical
 *  analyser. (yylex())
 */
char	charclass[] = {
	F_END,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
	OTHER,	SPACE,	OTHER,	OTHER,	OTHER,	SPACE,	OTHER,	OTHER,
	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
	SPACE,	OTHER,	OTHER,	OTHER,	DOLLAR,	OTHER,	OTHER,	OTHER,	//  !"#$%&'
	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	STARTER,OTHER,	// ()*+,-./
	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	// 01234567
	DIGIT,	DIGIT,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	STARTER,// 89:;<=>?
	STARTER,LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// @ABCDEFG
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// HIJKLMNO
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// PQRSTUVW
	LETTER, LETTER, LETTER, OTHER,	OTHER,	OTHER,	OTHER,	LETTER,	// XYZ[\]^_
	OTHER,	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// `abcdefg
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// hijklmno
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// pqrstuvw
	LETTER, LETTER, LETTER, OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	// xyz{|}~
};
#define CHARCLASS(x) charclass[(x) & 0xff]


/*
 *  the following table tells which characters are parts of numbers.
 *  The entry is non-zero for characters which can be parts of numbers.
 */
char	numpart[] = {
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	'0',	'1',	'2',	'3',	'4',	'5',	'6',	'7',
	'8',	'9',	0,	0,	0,	0,	0,	0,
	0,	'A',	'B',	'C',	'D',	'E',	'F',	0,
	'H',	0,	0,	0,	0,	0,	0,	'O',
	0,	'Q',	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	'a',	'b',	'c',	'd',	'e',	'f',	0,
	'h',	0,	0,	0,	0,	0,	0,	'o',
	0,	'q',	0,	0,	0,	0,	0,	0,
	'x',	0,	0,	0,	0,	0,	0,	0,
	0};




/*
 *  the following table is a list of assembler mnemonics;
 *  for each mnemonic the associated machine-code bit pattern
 *  and symbol type are given.
 *
 *  The i_uses field is overloaded to indicate the possible uses for
 *  a token.
 */

#define VERB	(1)	/* opcode or psuedo-op */
#define I8080	(2)	/* used in 8080 instructions */
#define Z80	(4)	/* used in Z80 instructions */
#define UNDOC	(8)	/* used only in undocumented instructions */
#define TERM	(16)	/* can appear in expressions (not all marked) */
#define ZNONSTD	(32)	/* non-standard Z-80 mnemonic (probably TDL or DRI) */
#define COL0	(64)	/* will always be recognized in logical column 0 */
#define MRASOP	(128)	/* dig operator out of identifiers */
#define Z180	(256)	/* used in Z180 (HD64180) instructions */

struct	item	keytab[] = {
	{"*get",	PSINC,	ARGPSEUDO,	VERB | COL0 },
	{"*include",	PSINC,	ARGPSEUDO,	VERB | COL0 },
	{"*list",	0,	LIST,		VERB | COL0 },
	{"*mod",	0,	MRAS_MOD,	VERB },
	{".8080",	0,	INSTSET,	VERB },
	{"a",		7,	ACC,		I8080 | Z80 },
	{"aci",		0316,	ALUI8,		VERB | I8080 },
	{"adc",		0210,	ARITHC,		VERB | I8080 | Z80  },
	{"adcx",	0xdd8e,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"adcy",	0xfd8e,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"add",		0200,	ADD,		VERB | I8080 | Z80  },
	{"addx",	0xdd86,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"addy",	0xfd86,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"adi",		0306,	ALUI8,		VERB | I8080 },
	{"af",		060,	AF,		Z80 },
	{"ana",		0240,	ARITHC,		VERB | I8080},
	{"and",		0240,	AND,		VERB | Z80 | TERM },
	{".and.",	0,	MROP_AND,	TERM | MRASOP },
	{"andx",	0xdda6,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"andy",	0xfda6,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"ani",		0346,	ALUI8,		VERB | I8080 },
	{".ascii",	0,	DEFB,		VERB },
	{".aseg",	SEG_ABS,SETSEG,		VERB },
	{".aset",	0,	DEFL,		VERB },
	{".assert",	0,	ASSERT,		VERB },
	{"b",		0,	REGNAME,	I8080 | Z80 },
	{"bc",		0,	RP,		Z80 },
	{"bit",		0145500,BIT,		VERB | Z80 },
	{"bitx",	0xddcb0046,BIT_XY,	VERB | Z80 | ZNONSTD },
	{"bity",	0xfdcb0046,BIT_XY,	VERB | Z80 | ZNONSTD },
	{".block",	0,	DEFS,		VERB },
	{".byte",	0,	DEFB,		VERB },
	{".bytes",	0,	DC,		VERB },
	{"c",		1,	TK_C,		I8080 | Z80 },
	{"call",	0315,	CALL,		VERB | I8080 | Z80 },
	{"cc",		0334,	CALL8,		VERB | I8080 },
	{"ccd",		0xeda9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"ccdr",	0xedb9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"ccf",		077,	NOOPERAND,	VERB | Z80 },
	{"cci",		0xeda1,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"ccir",	0xedb1,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"cm",		0374,	CALL8,		VERB | I8080 },
	{"cma",		057,	NOOPERAND,	VERB | I8080 },
	{"cmc",		077,	NOOPERAND,	VERB | I8080 },
	{"cmp",		0270,	LOGICAL,	VERB | I8080 },
	{"cmpx",	0xddbe,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"cmpy",	0xfdbe,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"cnc",		0324,	CALL8,		VERB | I8080 },
	{"cnz",		0304,	CALL8,		VERB | I8080 },
	{".comment",	SPCOM,	SPECIAL,	VERB },
	{".common",	PSCMN,	ARGPSEUDO,	VERB },
	{".cond",	0,	IF_TK,		VERB },
	{"cp",		0270,	LOGICAL,	VERB | I8080 | Z80 },
	{"cpd",		0166651,NOOPERAND,	VERB | Z80 },
	{"cpdr",	0166671,NOOPERAND,	VERB | Z80 },
	{"cpe",		0354,	CALL8,		VERB | I8080 },
	{"cpi",		0166641,NOOPERAND,	VERB | I8080 | Z80 },
	{"cpir",	0166661,NOOPERAND,	VERB | Z80 },
	{"cpl",		057,	NOOPERAND,	VERB | Z80 },
	{"cpo",		0344,	CALL8,		VERB | I8080 },
	{".cseg",	SEG_CODE,SETSEG,	VERB },
	{"cz",		0314,	CALL8,		VERB | I8080 },
	{"d",		2,	REGNAME,	I8080 | Z80 },
	{".d3",		0,	DEF3,		VERB },
	{"daa",		0047,	NOOPERAND,	VERB | I8080 | Z80 },
	{"dad",		9,	DAD,		VERB | I8080 },
	{"dadc",	0xed4a,	ARITH16,	VERB | Z80 | ZNONSTD },
	{"dadx",	0xdd09,	ARITH16,	VERB | Z80 | ZNONSTD },
	{"dady",	0xfd09,	ARITH16,	VERB | Z80 | ZNONSTD },
	{".db",		0,	DEFB,		VERB },
	{".dc",		0,	DC,		VERB },
	{"dcr",		5,	INRDCR,		VERB | I8080 },
	{"dcrx",	0xdd35,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"dcry",	0xfd35,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"dcx",		0xb,	INXDCX,		VERB | I8080 },
	{"dcxix",	0xdd2b,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"dcxiy",	0xfd2b,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"de",		020,	RP,		Z80 },
	{"dec",		5,	INCDEC,		VERB | I8080 | Z80 },
	{".def3",	0,	DEF3,		VERB },
	{".defb",	0,	DEFB,		VERB },
	{".defd",	0,	DEFD,		VERB },
	{".defl",	0,	DEFL,		VERB },
	{".defm",	0,	DEFB,		VERB },
	{".defs",	0,	DEFS,		VERB },
	{".defw",	0,	DEFW,		VERB },
	{".dephase",	0,	DEPHASE,	VERB },
	{"di",		0363,	NOOPERAND,	VERB | I8080 | Z80 },
	{"djnz",	020,	DJNZ,		VERB | Z80 },
	{".dm",		0,	DEFB,		VERB },
	{".ds",		0,	DEFS,		VERB },
	{"dsbc",	0xed42,	ARITH16,	VERB | Z80 | ZNONSTD },
	{".dseg",	SEG_DATA,SETSEG,	VERB },
	{".dw",		0,	DEFW,		VERB },
	{".dword",	0,	DEFD,		VERB },
	{"e",		3,	REGNAME,	I8080 | Z80 },
	{"ei",		0373,	NOOPERAND,	VERB | I8080 | Z80 },
	{".eject",	1,	LIST,		VERB },
	{".elist",	3,	LIST,		VERB },
	{".else",	0,	ELSE_TK,	VERB },
	{".end",	0,	END,		VERB },
	{".endc",	0,	ENDIF_TK,	VERB },
	{".endif",	0,	ENDIF_TK,	VERB },
	{".endm", 	0,	ENDM,		VERB },
	{".entry",	0,	PUBLIC,		VERB },
	{"eq",		0,	EQ,		0 },
	{".eq.",	0,	MROP_EQ,	TERM | MRASOP },
	{".equ",	0,	EQU,		VERB },
	{"ex",		0xEB,	EX,		VERB | Z80 },
	{"exaf",	0x08,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{".exitm", 	0,	EXITM,		VERB },
	{".ext",	0,	EXTRN,		VERB },
	{".extern",	0,	EXTRN,		VERB },
	{".extrn",	0,	EXTRN,		VERB },
	{"exx",		0331,	NOOPERAND,	VERB | Z80 },
	{"f",		0,	TK_F,		Z80 },
	{".flist",	4,	LIST,		VERB },
	{"ge",		0,	GE,		0 },
	{".ge.",	0,	MROP_GE,	TERM | MRASOP },
	{".glist",	5,	LIST,		VERB },
	{".global",	0,	PUBLIC,		VERB },
	{"gt",		0,	GT,		0 },
	{".gt.",	0,	MROP_GT,	TERM | MRASOP },
	{"h",		4,	REGNAME,	I8080 | Z80 },
	{"halt",	0166,	NOOPERAND,	VERB | Z80 },
	{"high",	0,	HIGH,		0 },
	{".high.",	0,	MROP_HIGH,	TERM | MRASOP },
	{"hl",		040,	HL,		Z80 },
	{"hlt",		0166,	NOOPERAND,	VERB | I8080 },
	{"hx",   	0x1DD04,IXYLH,		Z80 | UNDOC },
	{"hy",   	0x1FD04,IXYLH,		Z80 | UNDOC },
	{"i",		0,	MISCREG,	Z80 },
	{".if",		0,	IF_TK,		VERB | COL0 },
	{".ifdef",	1,	IF_DEF_TK,	VERB | COL0 },
	{".ifeq",	'=',	IF_CP_TK,	VERB | COL0 },
	{".ifgt",	'>',	IF_CP_TK,	VERB | COL0 },
	{".iflt",	'<',	IF_CP_TK,	VERB | COL0 },
	{".ifndef",	0,	IF_DEF_TK,	VERB | COL0 },
	{".ifne",	NE,	IF_CP_TK,	VERB | COL0 },
	{"im",		0166506,IM,		VERB | Z80 },
	{"im0",		0xed46,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"im1",		0xed56,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"im2",		0xed5e,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{".import",	PSIMPORT,ARGPSEUDO,	VERB | COL0 },
	{"in",		0333,	TK_IN,		VERB | I8080 | Z80 },
	{"in0",		0xED00,	TK_IN0,		VERB | Z180 },
	{"inc",		4,	INCDEC,		VERB | Z80 },
	{".incbin", 	0, 	INCBIN,		VERB },
	{".include",	PSINC,	ARGPSEUDO,	VERB | COL0 },	// COL0 only needed for MRAS mode
	{"ind",		0166652,NOOPERAND,	VERB | Z80 },
	{"indr",	0166672,NOOPERAND,	VERB | Z80 },
	{"ini",		0166642,NOOPERAND,	VERB | Z80 },
	{"inir",	0166662,NOOPERAND,	VERB | Z80 },
	{"inp",		0xed40,	INP,		VERB | Z80 | ZNONSTD },
	{"inr",		4,	INRDCR,		VERB | I8080 },
	{"inrx",	0xdd34,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"inry",	0xfd34,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"inx",		3,	INXDCX,		VERB | I8080 },
	{"inxix",	0xdd23,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"inxiy",	0xfd23,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"irp",		0,	IRP,		VERB },
	{"irpc",	0,	IRPC,		VERB },
	{"ix",		0156440,INDEX,		Z80 },
	{"ixh",		0x1DD04,IXYLH,		Z80 | UNDOC },
	{"ixl",		0x1DD05,IXYLH,		Z80 | UNDOC },
	{"iy",		0176440,INDEX,		Z80 },
	{"iyh",		0x1FD04,IXYLH,		Z80 | UNDOC },
	{"iyl",		0x1FD05,IXYLH,		Z80 | UNDOC },
	{"jc",		0332,	JUMP8,		VERB | I8080 },
	{"jm",		0372,	JUMP8,		VERB | I8080 },
	{"jmp",		0303,	JP,		VERB | I8080 },
	{"jnc",		0322,	JUMP8,		VERB | I8080 },
	{"jnz",		0302,	JUMP8,		VERB | I8080 },
	{"jp",		0362,	JP,		VERB | I8080 | Z80 },
	{"jpe",		0352,	JUMP8,		VERB | I8080 },
	{".jperror",	0,	JPERROR,	VERB },
	{"jpo",		0342,	JUMP8,		VERB | I8080 },
	{"jr",		040,	JR,		VERB | Z80 },
	{"jrc",		0x38,	JR_COND,	VERB | Z80 | ZNONSTD },
	{"jrnc",	0x30,	JR_COND,	VERB | Z80 | ZNONSTD },
	{"jrnz",	0x20,	JR_COND,	VERB | Z80 | ZNONSTD },
	{".jrpromote",	0,	JRPROMOTE,	VERB },
	{"jrz",		0x28,	JR_COND,	VERB | Z80 | ZNONSTD },
	{"jz",		0312,	JUMP8,		VERB | I8080 },
	{"l",		5,	REGNAME,	I8080 | Z80 },
	{"lbcd",	0xed4b,	LDST16,		VERB | Z80 | ZNONSTD },
	{"ld",		0x40,	LD,		VERB | Z80 },
	{"lda",		0x3a,	LDA,		VERB | I8080 },
	{"ldai",	0xed57,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"ldar",	0xed5f,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"ldax",	0xA,	LDAX,		VERB | I8080 },
	{"ldd",		0166650,NOOPERAND,	VERB | Z80 },
	{"lddr",	0166670,NOOPERAND,	VERB | Z80 },
	{"lded",	0xed5b,	LDST16,		VERB | Z80 | ZNONSTD },
	{"ldi",		0166640,NOOPERAND,	VERB | Z80 },
	{"ldir",	0166660,NOOPERAND,	VERB | Z80 },
	{"ldx",		0xdd46,	LD_XY,		VERB | Z80 | ZNONSTD},
	{"ldy",		0xfd46,	LD_XY,		VERB | Z80 | ZNONSTD},
	{"le",		0,	LE,		0 },
	{".le.",	0,	MROP_LE,	TERM | MRASOP },
	{"lhld",	0x2a,	LHLD,		VERB | I8080 },
	{".list",	0,	LIST,		VERB | COL0 }, // COL0 only needed for MRAS
	{"lixd",	0xdd2a,	LDST16,		VERB | Z80 | ZNONSTD },
	{"liyd",	0xfd2a,	LDST16,		VERB | Z80 | ZNONSTD },
	{".local",	0,	LOCAL,		VERB },
	{"low",		0,	LOW,		0 },
	{".low.",	0,	MROP_LOW,	TERM | MRASOP },
	{"lspd",	0xed7b,	LDST16,		VERB | Z80 | ZNONSTD },
	{"lt",		0,	LT,		0 },
	{".lt.",	0,	MROP_LT,	TERM | MRASOP },
	{"lx",   	0x1DD05,IXYLH,		Z80 | UNDOC },
	{"lxi",		1,	LXI,		VERB | I8080 },
	{"lxix",	0xdd21,	LDST16,		VERB | Z80 | ZNONSTD },
	{"lxiy",	0xfd21,	LDST16,		VERB | Z80 | ZNONSTD },
	{"ly",   	0x1FD05,IXYLH,		Z80 | UNDOC },
	{"m",		070,	COND,		I8080 | Z80 },
	{".maclib",	PSMACLIB,ARGPSEUDO,	VERB },
	{".macro",	0,	MACRO,		VERB },
	{".max",	1,	MINMAX,		VERB },
	{".min",	0,	MINMAX,		VERB },
	{".mlist",	6,	LIST,		VERB },
	{"mlt",		0xED4C,	MLT,		VERB | Z180 },
	{"mod",		0,	'%',		0 },
	{".mod.",	0,	MROP_MOD,	TERM | MRASOP },
	{"mov",		0x40,	MOV,		VERB | I8080 },
	{"mvi",		6,	MVI,		VERB | I8080 },
	{"mvix",	0xdd36,	MV_XY,		VERB | Z80 | ZNONSTD },
	{"mviy",	0xfd36,	MV_XY,		VERB | Z80 | ZNONSTD },
	{".name",	SPNAME,	SPECIAL,	VERB },
	{"nc",		020,	SPCOND,		0 },
	{"ne",		0,	NE,		0 },
	{".ne.",	0,	MROP_NE,	TERM | MRASOP },
	{"neg",		0166504,NOOPERAND,	VERB | Z80 },
	{".nolist",	-1,	LIST,		VERB },
	{"nop",		0,	NOOPERAND,	VERB | I8080 | Z80 },
	{"not",		0,	'~',		0 },
	{".not.",	0,	MROP_NOT,	TERM | MRASOP },
	{"nul",		0,	NUL,		0 },
	{"nv",		040,	COND,		Z80 },
	{"nz",		0,	SPCOND,		Z80 },
	{"ocf",		0,	OCF,		0 },
	{"or",		0260,	OR,		VERB | Z80 | TERM },
	{".or.",	6,	MROP_OR,	TERM | MRASOP },
	{"ora",		0260,	LOGICAL,	VERB | I8080 },
	{".org",	0,	ORG,		VERB },
	{"ori",		0366,	ALUI8,		VERB | I8080 },
	{"orx",		0xddb6,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"ory",		0xfdb6,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"otdm",	0xED8B,	NOOPERAND,	VERB | Z180 },
	{"otdmr",	0xED9B,	NOOPERAND,	VERB | Z180 },
	{"otdr",	0166673,NOOPERAND,	VERB | Z80 },
	{"otim",	0xED83,	NOOPERAND,	VERB | Z180 },
	{"otimr",	0xED93,	NOOPERAND,	VERB | Z180 },
	{"otir",	0166663,NOOPERAND,	VERB | Z80 },
	{"out",		0323,	TK_OUT,		VERB | I8080 | Z80 },
	{"out0",	0xED01,	TK_OUT0,	VERB | Z180 },
	{"outd",	0166653,NOOPERAND,	VERB | Z80 },
	{"outdr",	0166673,NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"outi",	0166643,NOOPERAND,	VERB | Z80 },
	{"outir",	0166663,NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"outp",	0xED41,	OUTP,		VERB | Z80 | ZNONSTD },
	{"p",		060,	COND,		Z80 },
	{".page",	1,	LIST,		VERB },
	{"pchl",	0351,	NOOPERAND,	VERB | I8080 },
	{"pcix",	0xdde9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"pciy",	0xfde9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"pe",		050,	COND,		Z80 },
	{"pfix",	0xdd,	NOOPERAND,	VERB | Z80 | UNDOC },
	{"pfiy",	0xfd,	NOOPERAND,	VERB | Z80 | UNDOC },
	{".phase",	0,	PHASE,		VERB },
	{"po",		040,	COND,		Z80 },
	{"pop",		0301,	PUSHPOP,	VERB | I8080 | Z80 },
	{"popix",	0xdde1,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"popiy",	0xfde1,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"pragma",	SPPRAGMA,SPECIAL,	VERB },
	{"psw", 	060,	PSW,		I8080 },
	{".public",	0,	PUBLIC,		VERB },
	{"push",	0305,	PUSHPOP,	VERB | I8080 | Z80 },
	{"pushix",	0xdde5,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"pushiy",	0xfde5,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"r",		010,	MISCREG,	Z80 },
	{"ral",		027,	NOOPERAND,	VERB | I8080 },
	{"ralr",	0xCB10,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"ralx",	0xddcb0016,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"raly",	0xfdcb0016,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rar",		037,	NOOPERAND,	VERB | I8080 },
	{"rarr",	0xCB18,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"rarx",	0xddcb001e,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rary",	0xfdcb001e,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rc",		0330,	NOOPERAND,	VERB | I8080 },
	{".read",	PSINC,	ARGPSEUDO,	VERB },
	{"rept",	0,	REPT,		VERB },
	{"res",		0145600,BIT,		VERB | Z80 },
	{"resx",	0xddcb0086,BIT_XY,	VERB | Z80 | ZNONSTD },
	{"resy",	0xfdcb0086,BIT_XY,	VERB | Z80 | ZNONSTD },
	{"ret",		0311,	RET,		VERB | I8080 | Z80 },
	{"reti",	0166515,NOOPERAND,	VERB | Z80 },
	{"retn",	0166505,NOOPERAND,	VERB | Z80 },
	{"rl",		0xCB10,	SHIFT,		VERB | Z80 },
	{"rla",		027,	NOOPERAND,	VERB | Z80 },
	{"rlc",		0xCB00,	SHIFT,		VERB | I8080 | Z80 },
	{"rlca",	07,	NOOPERAND,	VERB | Z80 },
	{"rlcr",	0xCB00,	SHIFT,		VERB | I8080 | Z80 | ZNONSTD },
	{"rlcx",	0xddcb0006,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rlcy",	0xfdcb0006,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rld",		0166557,NOOPERAND,	VERB | Z80 },
	{"rm",		0370,	NOOPERAND,	VERB | I8080 },
	{".rmem",	0,	DEFS,		VERB },
	{"rnc",		0320,	NOOPERAND,	VERB | I8080 },
	{"rnz",		0300,	NOOPERAND,	VERB | I8080 },
	{"rp",		0360,	NOOPERAND,	VERB | I8080 },
	{"rpe",		0350,	NOOPERAND,	VERB | I8080 },
	{"rpo",		0340,	NOOPERAND,	VERB | I8080 },
	{"rr",		0xCB18,	SHIFT,		VERB | Z80 },
	{"rra",		037,	NOOPERAND,	VERB | Z80 },
	{"rrc",		0xCB08,	SHIFT,		VERB | I8080 | Z80 },
	{"rrca",	017,	NOOPERAND,	VERB | Z80 },
	{"rrcr",	0xCB08,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"rrcx",	0xddcb000e,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rrcy",	0xfdcb000e,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rrd",		0166547,NOOPERAND,	VERB | Z80 },
	{"rst",		0307,	RST,		VERB | I8080 | Z80 },
	{".rsym",	PSRSYM,	ARGPSEUDO,	VERB },
	{"rz",		0310,	NOOPERAND,	VERB | I8080 },
	{"sbb",		0230,	ARITHC,		VERB | I8080 },
	{"sbc",		0230,	ARITHC,		VERB | Z80 },
	{"sbcd",	0xed43,	LDST16,		VERB | Z80 | ZNONSTD },
	{"sbcx",	0xdd9e,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"sbcy",	0xfd9e,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"sbi",		0336,	ALUI8,		VERB | I8080 },
	{"scf",		067,	NOOPERAND,	VERB | Z80 },
	{"sded",	0xed53,	LDST16,		VERB | Z80 | ZNONSTD },
	{"set",		0145700,BIT,		VERB | Z80 },
	{"setb",	0145700,BIT,		VERB | Z80 | ZNONSTD },
	{".setocf",	0,	SETOCF,		VERB },
	{".sett",	0,	TSTATE,		VERB },
	{"setx",	0xddcb00c6,BIT_XY,	VERB | Z80 | ZNONSTD },
	{"sety",	0xfdcb00c6,BIT_XY,	VERB | Z80 | ZNONSTD },
	{"shl",		0,	SHL,		TERM },
	{".shl.",	0,	MROP_SHL,	TERM | MRASOP },
	{"shld",	0x22,	SHLD,		VERB | I8080 },
	{"shr",		0,	SHR,		TERM },
	{".shr.",	0,	MROP_SHR,	TERM | MRASOP },
	{"sixd",	0xdd22,	LDST16,		VERB | Z80 | ZNONSTD },
	{"siyd",	0xfd22,	LDST16,		VERB | Z80 | ZNONSTD },
	{"sl1",		0xCB30,	SHIFT,		VERB | Z80 | UNDOC },
	{"sla",		0xCB20,	SHIFT,		VERB | Z80 },
	{"slar",	0xCB20,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"slax",	0xddcb0026,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"slay",	0xfdcb0026,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"sll",		0xCB30,	SHIFT,		VERB | Z80 },
	{"slp",		0xED76,	NOOPERAND,	VERB | Z180 },
	{"sp",		060,	SP,		I8080 | Z80 },
	{".space",	2,	LIST,		VERB },
	{"sphl",	0371,	NOOPERAND,	VERB | I8080 },
	{"spix",	0xddf9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"spiy",	0xfdf9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"sra",		0xCB28,	SHIFT,		VERB | Z80 },
	{"srar",	0xCB28,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"srax",	0xddcb002e,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"sray",	0xfdcb002e,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"srl",		0xCB38,	SHIFT,		VERB | Z80 },
	{"srlr",	0xCB38,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"srlx",	0xddcb003e,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"srly",	0xfdcb003e,SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"sspd",	0xed73,	LDST16,		VERB | Z80 | ZNONSTD },
	{"sta",		0x32,	STA,		VERB | I8080 },
	{"stai",	0xed47,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"star",	0xed4f,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"stax",	2,	STAX,		VERB | I8080 },
	{"stc",		067,	NOOPERAND,	VERB | I8080 },
	{"stx",		0xdd70,	ST_XY,		VERB | Z80 | ZNONSTD},
	{"sty",		0xfd70,	ST_XY,		VERB | Z80 | ZNONSTD},
	{"sub",		0220,	LOGICAL,	VERB | I8080 | Z80 },
	{".subttl",	SPSBTL,	SPECIAL,	VERB },
	{"subx",	0xdd96,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"suby",	0xfd96,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"sui",		0326,	ALUI8,		VERB | I8080 },
	{"t",		0,	T,		0 },
	{".text",	0,	DEFB,		VERB },
	{"tihi",	0,	TIHI,		0 },
	{"tilo",	0,	TILO,		0 },
	{".title",	SPTITL,	SPECIAL,	VERB | COL0},
	{"tst",		0xED04,	TST,		VERB | Z180 },
	{".tstate",	0,	TSTATE,		VERB },
	{"tstio",	0xED74,	TSTIO,		VERB | Z180 },
	{"v",		050,	COND,		Z80 },
	{".word",	0,	DEFW,		VERB },
	{".wsym",	PSWSYM,	ARGPSEUDO,	VERB },
	{"xchg",	0353,	NOOPERAND,	VERB | I8080 },
	{"xh",   	0x1DD04,IXYLH,		Z80 | UNDOC },
	{"xl",   	0x1DD05,IXYLH,		Z80 | UNDOC },
	{"xor",		0250,	XOR,		VERB | Z80 | TERM },
	{".xor.",	0,	MROP_XOR,	TERM | MRASOP },
	{"xorx",	0xddae,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"xory",	0xfdae,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"xra",		0250,	LOGICAL,	VERB | I8080 },
	{"xri",		0356,	ALUI8,		VERB | I8080 },
	{"xthl",	0343,	NOOPERAND,	VERB | I8080 },
	{"xtix",	0xdde3,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"xtiy",	0xfde3,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"yh",   	0x1FD04,IXYLH,		Z80 | UNDOC },
	{"yl",   	0x1FD05,IXYLH,		Z80 | UNDOC },
	{"z",		010,	SPCOND,		Z80 },
	{".z180",	2,	INSTSET,	VERB },
	{".z80",	1,	INSTSET,	VERB },
};

/*
 *  user-defined items are tabulated in the following table.
 */

struct item	itemtab[ITEMTABLESIZE];
struct item	*itemmax = itemtab+ITEMTABLESIZE;

int item_is_verb(struct item *i)
{
	return i && (i->i_uses & VERB) == VERB;
}

int item_value(struct item *i)
{
	int value = i->i_value;

	// Some special cases for 8080 opcode values.
	if (!z80) {
		// CP is CALL P in 8080
		if (i->i_value == 0270 && i->i_string[1] == 'p')
			value = 0364;
		else if (i->i_value == 0166641) // CPI
			value = 0376;
		else if (i->i_token == SHIFT && (i->i_value & 070) < 020)
			value = 7 | (i->i_value & 070);
	}
	else {
		if (i->i_token == JR)
			value = 0x18;
		else if (i->i_token == JP)
			value = 0xC3;

		value = sized_byteswap(value);
	}

	return value;
}

/*
 *  lexical analyser, called by yyparse.
 */
int yylex()
{
	int lex();
	int token = lex();

	if (mras) {
		switch (token)
		{
		// Operator translations because of different precedence
		case '+': token = MROP_ADD; break;
		case '-': token = MROP_SUB; break;
		case '*': token = MROP_MUL; break;
		case '/': token = MROP_DIV; break;
		case '%': token = MROP_MOD; break;
		case '&': token = MROP_AND; break;
		case '|': token = MROP_OR; break;
		case '^': token = MROP_XOR; break;
		case SHL: token = MROP_SHL; break;
		case SHR: token = MROP_SHR; break;
		case LT:  token = MROP_LT; break;
		case EQ:  token = MROP_EQ; break;
		case '>': token = MROP_GT; break;
		case GE:  token = MROP_GE; break;
		case NE:  token = MROP_NE; break;
		case LE:  token = MROP_LE; break;
		case NOT: token = MROP_NOT; break;
		case HIGH: token = MROP_HIGH; break;
		case LOW: token = MROP_LOW; break;
		// Operator translations to completely different operator.
		case '<': token = MROP_SHIFT; break;
		case '!': token = MROP_OR; break;
		default: break;
		// Sadly, AND, OR, XOR and '=' can't be translated unilaterally
		// because they are also opcodes or psuedo-ops.
		}
	}

	return token;
}

int lex()
{
	int c;
	char *p;
	int radix;
	int sep;
	int exclude, include, overflow, token, endc;

	if (arg_flag) {
		yylval.cval = arg_state.arg;
		c = getarg(&arg_state);
		if (c == '\0' || c == '\n' || c == ';')
			c = skipline(c);

		return c;
	}

	if (raw == 2) {
		while (charclass[c = nextchar()] == SPACE)
			;

		*tempbuf = c == '\n' || c == '\0';

		peekc = skipline(c);

		raw = 0;

		return RAWTOKEN;
	}
	else if (raw) {
		int skip = 1;
		p = tempbuf;
		while ((c = nextchar()) != '\n' && c) {
			if (p >= tempmax) {
				*p = '\0';
				printf("was parsing '%s'\n", tempbuf);
				error(symlong);
			}
			if (!skip || charclass[c] != SPACE) {
				*p++ = c;
				skip = 0;
			}
		}
		if (c == 0)
			peekc = c;

		*p-- = '\0';

		while (p >= tempbuf && CHARCLASS(*p) == SPACE)
			*p-- = '\0';

		raw = 0;

		return RAWTOKEN;
	}

	for (;;) switch(charclass[c = nextchar()]) {
	case F_END:
		if (!expptr)
			return 0;

		if (est[MSTR].param) {
			int ch;
			est[REPNUM].value++;
			ch = est[MSTR].param[est[REPNUM].value];
			if (ch) {
				est[0].param[0] = ch;
				floc = est[MSTART].value;
				mfseek(mfile, (long)floc, 0);
				est[TEMPNUM].value = exp_number++;
			}
			else {
				free(est[MSTR].param);
				est[MSTR].param = 0;
				popsi();
			}
		}
		else if (est[REPNUM].value < 0) {
			int arg;
			do {
				switch (getarg(est[MARGP].ap)) {
				case ARG:
					arg = 1;
					break;
				case ',':
					arg = 0;
					break;
				default:
					arg = 2;
					break;
				}
			} while (!arg);

			if (arg == 1) {
				floc = est[MSTART].value;
				mfseek(mfile, (long)floc, 0);
				est[TEMPNUM].value = exp_number++;
			}
			else {
				// XXX - memory leak
				est[0].param = NULL;
				free(est[MARGP].ap);
				popsi();
			}
		}
		else if (est[REPNUM].value-- > 0) {
			floc = est[MSTART].value;
			mfseek(mfile, (long)floc, 0);
			est[TEMPNUM].value = exp_number++;
		}
		else
			popsi();

		continue;

	case SPACE:
		while (charclass[c = nextchar()] == SPACE)
			;
		peekc = c;
		logcol++;
		break;
	case LETTER:
	case STARTER:
	case DIGIT:
	case DOLLAR:
	spectok:
		firstcol = getcol() == 1;

		radix = -1; // might be a number
		p = tempbuf;
		do {
			if (p >= tempmax) {
				*tempmax = '\0';
				printf("was parsing '%s'\n", tempbuf);
				error(symlong);
			}

			if (driopt && c == '$') {
				c = nextchar();
				continue;
			}
			// GWP - don't lowercase
			//*p = (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
			*p = c;
			if (mras && *p == '?') {
				char *q;

				radix = 0; // can't be a number even if it looks like it

				if (expptr)
					q = getmraslocal();
				else
					for (q = modstr; *q == '@'; q++)
						;

				if (*q) {
					strcpy(p, q);
					p = strchr(p, '\0') - 1;
				}
				else
					*p = '?';
			}
			p++;
			c = nextchar();
		} while	(charclass[c]==LETTER || charclass[c]==DIGIT ||
			charclass[c]==STARTER || charclass[c]==DOLLAR);

		if (driopt && p == tempbuf)
			*p++ = '$'; // reverse overzelous stripping

		*p = '\0';

		// When parsing macro parameters we use a custom symbol table.
		// And, heck, almost anything goes.
		if (param_parse) {
			struct item *param = item_lookup(tempbuf, paramtab, PARAMTABSIZE);
			peekc = c;
			if (param->i_token) {
				sprintf(detail, "%s error.  Macro parameter '%s' repeated",
					errname[fflag], tempbuf);
				errwarn(fflag, detail);
			}

			param->i_token = MPARM;
			param->i_string = malloc(strlen(tempbuf) + 1);
			strcpy(param->i_string, tempbuf);

			yylval.itemptr = param;
			return param->i_token;
		}

		// Special case for AF'
		if (c == '\'' && ci_strcmp(tempbuf, "af") == 0)
			return AFp;

		endc = c;
		peekc = c;

		// Pass off '?' (XXX but, technically, should only be done in expression context)
		if (strcmp(tempbuf, "?") == 0)
			return '?';

		// Pass off '$'
		if (strcmp(tempbuf, "$") == 0)
			return '$';

		// Look ahead at what we have.
		while (charclass[c] == SPACE)
			c = nextchar();

		peekc = c;

		//printf("%d %s\n", logcol, tempbuf);
		// If logcol == 0 then if c == ':' we're a label for sure.
		// If logcol == 1 if c == ':' we're a label, change logcol
		//    otherwise we're op or pseudo
		// If logcol == 0 and c == '\n' or ';' then we're alone so
		//	we give tokenization a chance otherwise label
		// If logcol >= 2 we're in the arguments
		//
		// There is quite a lot of unrealized scope for error
		// detection and helpful warnings.

		 // Default to any tokenization.
		exclude = 0;
		include = 0;

		if (logcol >= 2) {
			// Arguments allow mnemonics and psuedo-ops
			exclude = VERB;
			include = TERM;
		}
		else if (logcol == 0 && first_always_label) {
			exclude = ~TERM;
		}
		else if (logcol <= 1 && c == ':') {
			exclude = ~TERM;
			logcol = 0;
		}
		else if (logcol == 0 && c != ';' && c != '\n') {
			exclude = ~TERM;
			include = COL0;
		}

		logcol++;

		// Look for possible numbers.
		// 0x<hex> $<hex> <hex>h <octal>o <octal>q <binary>b
		// <decimal> <decimal>d
		// Suffix formats must start with 0-9.

		if (radix)
			radix = convert(tempbuf, p, &overflow);

		// If we're in the first logical column and the token starts with
		// '$' then we'll force it to be a label even though it could be
		// a $hex constant. This will allow $FCB as a label.
		// Thus we must also allow symbol lookup a chance to override number
		// parsing if we start with a '$'.

		if (tempbuf[0] == '$') {
			if (logcol == 1 || locate(tempbuf)->i_token) {
				if (radix > 0) {
					sprintf(detail, "warning: $hex constant '%s' interpreted as symbol", tempbuf);
					errwarn(warn_hex, detail);
				}
				radix = 0;
			}
		}

		if (radix > 0) {
			// Might be line skipping time, though.
			if (*ifptr)
				return skipline(c);

			if (overflow) {
				err[iflag]++;
				yylval.ival = 0;
			}
			return NUMBER;
		}

		// Too late to do '$' concatenation of numbers.  But zmac
		// didn't allow for that previously at any rate.
		if (zcompat) {
			char *q = tempbuf;
			// Normal zmac operation requires we ignore $ in identifiers
			for (p = q; *p; p++)
				if (*p != '$')
					*q++ = *p;

			*q = '\0';
			p = q;
		}

		// GWP - boy, this should be a warning or error
		if (p - tempbuf > MAXSYMBOLSIZE) {
			p = tempbuf + MAXSYMBOLSIZE;
			*p = '\0';
		}

		token = tokenofitem(UNDECLARED, exclude, include);

		// Initial "priming" hack

		if (endc == '\'')
		{
			//printf("allowing <<%s%c>> at %d, endc=%c\n", tempbuf, peekc, logcol, endc);
			peekc = NOPEEK;
// A step towards emitting the instructions
#if 0
			// Only if auto-exx on on...
			if (tempbuf[0] == 'a') {
				emit1(8, 0, 0, ET_NOARG);
				list_out(dollarsign, "\tex\taf,af'\n", '*');
			}
			else {
				emit1(0xd9, 0, 0, ET_NOARG);
				list_out(dollarsign, "\texx\n", '*');
			}
#endif
		}

		return token;

	default:
		if (*ifptr)
			return(skipline(c));

		if (mras && getcol() == 1 && c == '*')
			goto spectok;

		switch(c) {
		int corig;
		case ':':
			if (logcol == 1) {
				// Make sure "label:ret", "label: ret",
				// "label: :ret", "label: : ret" work out OK.
				// But stop fooling around once we've done the VERB
				peekc = nextchar();
				if (charclass[peekc] == SPACE)
					logcol--;
			}
			return c;
		case ';':
			return(skipline(c));
		case '\'':
		case '"':
			sep = c;
			p = tempbuf;
			p[1] = 0;
			do	switch(c = nextchar())	{
			case '\0':
			case '\n':
				err[bflag]++;
				goto retstring;
			default:
				if (c == sep && (c = nextchar()) != sep) {
				retstring:
					peekc = c;
					*p = '\0';
					switch (p - tempbuf) {
					case 2:
						p = tempbuf;
						yylval.ival = *p++ & 255;
						yylval.ival |= (*p & 255) << 8;
						return TWOCHAR;
					case 1:
						p = tempbuf;
						yylval.ival = *p++;
						return ONECHAR;
					case 0:
						if (!full_exprs) {
							yylval.ival = 0;
							return NUMBER;
						}
						// fall through
					default:
						yylval.cval = tempbuf;
						return STRING;
					}
				}
				*p++ = c;
			} while (p < tempmax);
			/*
			 *  if we break out here, our string is longer than
			 *  our input line
			 */
			error("string buffer overflow");
		case '<':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return LE;
			case '<':
				return SHL;
			case '>':
				return NE;
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '>':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return GE;
			case '>':
				return SHR;
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '!':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return NE;
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '=':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return '=';
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */

		case '&':
			corig = c;
			if ((c = nextchar()) == '&')
				return ANDAND;
			else {
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '|':
			corig = c;
			if ((c = nextchar()) == '|')
				return OROR;
			else {
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		default:
			return(c);
		}
	}
}

// Convert string to number
// 0x<hex> $<hex> <hex>h <octal>o <octal>q <binary>b
// <decimal> <decimal>d
// Suffix formats must start with 0-9.
//
// Returns:
//	-1	not numeric
//	0	numeric but digit out of range
//	>0	radix of number,  yylval.ival holds value

int convert(char *buf, char *bufend, int *overflow)
{
	int radix = -1, dummy;
	char *d0, *dn;

	if (!bufend)
		bufend = strchr(buf, '\0');

	if (!overflow)
		overflow = &dummy;

	if (buf[0] == '0' && (buf[1] == 'x' || buf[1] == 'X') && buf[2]) {
		radix = 16;
		d0 = buf + 2;
		dn = bufend;
	} else if (buf[0] == '$') {
		radix = 16;
		d0 = buf + 1;
		dn = bufend;
	}
	else if (buf[0] >= '0' && buf[0] <= '9') {
		d0 = buf;
		dn = bufend - 1;
		switch (*dn) {
		case 'O':
		case 'o':
		case 'Q':
		case 'q':
			radix = 8;
			break;
		case 'D':
		case 'd':
			radix = 10;
			break;
		case 'H':
		case 'h':
			radix = 16;
			break;
		case 'B':
		case 'b':
			radix = 2;
			break;
		default:
			radix = 10;
			dn++;
			break;
		}
	}

	// We may have a number on our hands.
	if (radix > 0) {
		*overflow = 0;
		yylval.ival = 0;

		for (; d0 < dn; d0++) {
			unsigned int ovchk = (unsigned int)yylval.ival;
			int c = *d0;
			if (c >= 'a') c -= 'a' - 10;
			else if (c >= 'A') c -= 'A' - 10;
			else c -= '0';
			if (c < 0 || c >= radix) {
				radix = 0;
				break;
			}
			if (ovchk * radix / radix != ovchk)
				*overflow = 1;

			yylval.ival *= radix;
			yylval.ival += c;
		}
	}

	return radix;
}

// Verify keytab is in alphabetical order.
// And that all MRASOP keywords start with '.'

int check_keytab()
{
	int i;
	char *prev;

	for (i = 0; i < sizeof(keytab) / sizeof(keytab[0]); i++) {
		char *next = keytab[i].i_string;
		next += *next == '.';
		if (i != 0) {
			if (strcmp(prev, next) >= 0) {
				printf("keytab error: %s >= %s\n", prev, next);
				return 0;
			}
		}
		prev = next;

		if ((keytab[i].i_uses & MRASOP) && keytab[i].i_string[0] != '.') {
			printf("keytab error: %s does not start with '.'\n",
				keytab[i].i_string);
			return 0;
		}

		// Uncomment to liat all 8080 verbs to assist documentation.
		//if ((keytab[i].i_uses & (VERB | I8080)) == (VERB | I8080))
		//	printf("\tdb\t%s\n", keytab[i].i_string);
		// Uncomment to list all Z-80 verbs to assist documentation.
		//if ((keytab[i].i_uses & (VERB | Z80)) == (VERB | Z80))
		//	printf("%-6s   $%X\n", keytab[i].i_string,
		//		item_value(keytab + i));
		// Uncomment to list all Z-180 verbs to assist documentation.
		//if ((keytab[i].i_uses & (VERB | Z180)) == (VERB | Z180))
		//	printf("%-6s   $%X\n", keytab[i].i_string,
		//		item_value(keytab + i));
	}

	printf("keytab OK\n");

	return 1;
}


struct item *keyword(char *name)
{
	int  r, l, u;
	struct item *ip;

	/*
	 *  binary search
	 */
	l = 0;
	u = (sizeof keytab/sizeof keytab[0])-1;
	while (l <= u) {
		char *key;
		i = (l+u)/2;
		ip = &keytab[i];
		key = ip->i_string;
		r = ci_strcmp(name + (name[0] == '.'), key + (key[0] == '.'));
		if (r == 0) {
			// Do not allow ".foo" to match "foo"
			if (name[0] == '.' && key[0] != '.')
				break;

			return ip;
		}
		if (r < 0)
			u = i-1;
		else
			l = i+1;
	}

	// Check if there's an alias.
	ip = locate(name);
	if (ip && (ip->i_scope & SCOPE_ALIAS))
		return keytab + ip->i_value;

	return 0;
}

int keyword_index(struct item *k)
{
	return k - keytab;
}

// Find 'name' in an item table.  Returns an empty slot if not found.
// Uses case-independent comparisions which are largely wasted as
// there is only 1 case where 'name' has not already been lowercased.

struct item *item_lookup(char *name, struct item *table, int table_size)
{
	struct item *ip;
	/*
	 *  hash into item table
	 */
	unsigned long hash = 0;
	char *p = name;
	while (*p) {
		char ch = *p++;
		if (ch >= 'A' && ch <= 'Z') ch += 'a' - 'A';
		hash += hash * 67 + ch - 113;
	}
	hash %= table_size;
	ip = &table[hash];

	for (;;) {
		if (ip->i_token == 0)
			break;
		if (ci_strcmp(name, ip->i_string) == 0)
			break;
		if (++ip >= table + table_size)
			ip = table;
	}

	return ip;
}

struct item *locate(char *name)
{
	return item_lookup(name, itemtab, ITEMTABLESIZE);
}

// Return the longest token that matches the start of the given name.
// Currently used for MRAS which will substitute macro parameters inside
// identifiers.
struct item *item_substr_lookup(char *name, int token, struct item *table, int table_size)
{
	struct item *ip = 0;
	int i;

	for (i = 0; i < table_size; i++) {
		unsigned int len;

		if (table[i].i_token != token)
			continue;

		len = strlen(table[i].i_string);
		if (ci_strcompare(name, table[i].i_string, len) == 0) {
			if (!ip || len > strlen(ip->i_string)) {
				ip = table + i;
			}
		}
	}

	return ip;
}

/*
 *  return the token associated with the string pointed to by
 *  tempbuf.  if no token is associated with the string, associate
 *  deftoken with the string and return deftoken.
 *  in either case, cause yylval to point to the relevant
 *  symbol table entry.
 *
 *  Only keys not matching the keyexclude will be returned allowing
 *  context-dependent tokenization.  Unless they match keyinclude.
 */

int tokenofitem(int deftoken, int keyexclude, int keyinclude)
{
	struct item *ip;
	int  i;

#ifdef T_DEBUG
	fputs("'tokenofitem entry'	", stderr) ;
	fputs(tempbuf, stderr) ;
#endif

	// Allow macros to override built-ins
	// Maybe shouldn't be done for identifiers that will undergo
	// '.' and '_' expansion.
	ip = locate(tempbuf);
	if (ip->i_token == MNAME)
		goto found;

	if (full_exprs)
		keyexclude = ~TERM;

	ip = keyword(tempbuf);
	if (ip) {
		if (ip->i_uses & keyinclude)
			goto found;

		if (!(ip->i_uses & keyexclude))
			goto found;
	}

	// This is really my own thing rather than old zmac, but zmac
	// didn't support it and it does depend on '$' crushing a bit.
	if (zcompat) {
	    // '_' prefixed labels are local to the file
	    if (tempbuf[0] == '_') {
		    strcat(tempbuf, "$");
		    strcat(tempbuf, basename(src_name[now_in]));
	    }

	    // '.' prefixed labels are local between labels
	    if (tempbuf[0] == '.') {
		    char *p = tempbuf;
		    while (*p) p++;
		    sprintf(p, "$%d", llseq);
	    }
	}

	ip = locate(tempbuf);

	if (ip->i_token)
		goto found;

	if (!deftoken) {
		i = 0 ;
		goto token_done ;
	}
	if (++nitems > ITEMTABLESIZE-20)
		error("item table overflow");
	ip->i_string = malloc(strlen(tempbuf)+1);
	ip->i_token = deftoken;
	ip->i_uses = 0;
	strcpy(ip->i_string, tempbuf);

found:
	if (*ifptr) {
		if (ip->i_token == ENDIF_TK) {
			i = ENDIF_TK;
			goto token_done ;
		}
		if (ip->i_token == ELSE_TK) {
			/* We must only honour the ELSE if it is not
			   in a nested failed IF/ELSE */
			char forbid = 0;
			char *ifstackptr;
			for (ifstackptr = ifstack; ifstackptr != ifptr; ++ifstackptr) {
				if (*ifstackptr) {
					forbid = 1;
					break;
				}
			}
			if (!forbid) {
				i = ELSE_TK;
				goto token_done;
			}
		}
		if (ip->i_token == IF_TK || ip->i_token == IF_DEF_TK ||
			ip->i_token == IF_CP_TK)
		{
			if (ifptr >= ifstmax)
				error("Too many ifs");
			else *++ifptr = 1;
		}
		i = skipline(' ');
		goto token_done ;
	}
	yylval.itemptr = ip;
	i = ip->i_token;
token_done:
#ifdef T_DEBUG
	fputs("\t'tokenofitem exit'\n", stderr) ;
#endif
	return(i) ;
}

void itemcpy(struct item *dst, struct item *src)
{
	if (dst && src && dst != src) {
		dst->i_string = src->i_string;
		dst->i_value = src->i_value;
		dst->i_token = src->i_token;
		dst->i_uses = src->i_uses;
		dst->i_scope = src->i_scope;
		dst->i_chain = src->i_chain;
		dst->i_pass = src->i_pass;
	}
}

/*
 *  interchange two entries in the item table -- used by custom_qsort
 */
void interchange(int i, int j)
{
	struct item temp;

	itemcpy(&temp, itemtab + i);
	itemcpy(itemtab + i, itemtab + j);
	itemcpy(itemtab + j, &temp);
}



/*
 *  quick sort -- used by compactsymtab to sort the symbol table
 */
void custom_qsort(int m, int n)
{
	int  i, j;

	if (m < n) {
		i = m;
		j = n+1;
		for (;;) {
			do i++; while(strcmp(itemtab[i].i_string,
					itemtab[m].i_string) < 0);
			do j--; while(strcmp(itemtab[j].i_string,
					itemtab[m].i_string) > 0);
			if (i < j) interchange(i, j); else break;
		}
		interchange(m, j);
		custom_qsort(m, j-1);
		custom_qsort(j+1, n);
	}
}

int getcol()
{
	return inpptr - inpbuf;
}

void dri_setmacro(int op) {
	if (op == '-') {
		mopt = 0;
	} else if (op == '+') {
		mopt = 1;
	} else { // '*'
		mopt = 2;
	}
}

int mc_quote;
int mc_first;

void start_multi_check()
{
	mc_quote = -1;
	mc_first = 1;
}

int found_multi(int ch)
{

	if (ch == mc_quote && (mc_quote == '"' || mc_quote == '\''))
		mc_quote = -1;
	else if (mc_quote < 0 && (ch == '\'' || ch == '"' || ch == ';'))
		mc_quote = ch;
	else if (ch == '*' && mc_first)
		mc_quote = '*';

	mc_first = 0;
	if (ch == separator && mc_quote < 0)
		return 1;

	return 0;
}

/*
 *  get the next character
 */
int nextchar()
{
	int c, ch;
	unsigned char *p;
	char *getlocal(int c, int n);

	if (peekc != NOPEEK) {
		c = peekc;
		peekc = NOPEEK;
		return c;
	}

	if (inpptr) {
		int col = getcol();

		// Double nul indicates EOF for macros
		if (expptr && inpptr[0] == '\0' && inpptr[1] == '\0') {
			inpptr = 0;
			return 0;
		}

		if (col == 0) {
			void analyze_inpbuf(void);
			void mras_operator_separate(void);

			if (driopt && inpptr[0] == '*') {
				addtoline(*inpptr++);
				c = skipline(inpptr[0]);
				linein[now_in]++;
				return c;
			}

			if (!expptr && !prev_multiline)
				linein[now_in]++;

			analyze_inpbuf();

			// TODO - I think this code and comnt is unnecessary
			if (driopt && comnt) {
				addtoline(*inpptr++);
				c = skipline(*inpptr);
				linein[now_in]++;
				return c;
			}

			if (macopt) {
				dri_setmacro(macopt);
				addtoline(*inpptr++);
				c = skipline(inpptr[0]);
				linein[now_in]++;
				macopt = 0;
				return c;
			}

			if (mras)
				mras_operator_separate();
		}

		if (inpbuf_insert[col]) {
			c = inpbuf_insert[col];
			inpbuf_insert[col] = '\0';
		}
		else {
			c = *inpptr++;
			addtoline(c);
		}

		if (*inpptr == '\0')
			inpptr = 0;

		return c;
	}

	inpptr = inpbuf;
	logcol = 0;
	p = inpbuf;

	// XXX - should check for input line overflow!

	// If invoking a macro then pull the next line from it.
	if (expptr) {
		start_multi_check();
		for (;;) {
			ch = getm();

			if (ch == '\1') { /* expand argument */
				ch = getm() - 'A';
				if (ch >= 0 && ch < PARMMAX && est[ch].param) {
					strcpy((char *)p, est[ch].param);
					p = (unsigned char *)strchr((char *)p, '\0');
				}
			}
			else if (ch == '\2') {	/*  local symbol  */
				ch = getm() - 'A';
				if (ch >= 0 && ch < PARMMAX && est[ch].param)
					strcpy((char *)p, est[ch].param);
				else
					strcpy((char *)p, getlocal(ch, est[TEMPNUM].value));

				p = (unsigned char *)strchr((char *)p, '\0');
			}
			else {
				if (ch == 0)
					break;

				*p++ = ch;

				if (ch == '\n')
					break;

				if (found_multi(ch)) {
					p[-1] = '\n';
					multiline = 1;
					break;
				}
			}
		}
		*p = '\0';
		p[1] = ch;
	}
	else {
		start_multi_check();

		for (;;) {
			ch = nextline_peek != NOPEEK ? nextline_peek : getc(now_file);
			nextline_peek = NOPEEK;

			if (ch == '\r') {
				nextline_peek = getc(now_file);
				if (nextline_peek == '\n')
					nextline_peek = NOPEEK;

				ch = '\n';
			}

			if (ch == EOF)
				break;

			*p++ = ch;

			if (ch == '\n') 
				break;

			if (found_multi(ch) && !inmlex) {
				p[-1] = '\n';
				multiline = 1;
				break;
			}
		}

		*p = '\0';

		/* if EOF, check for include file */
		if (ch == EOF) {
			if (now_in) {
				fclose(fin[now_in]) ;
				free(src_name[now_in]);
				now_file = fin[--now_in];
				nextline_peek = linepeek[now_in];
			}
			else if (p == inpbuf)
				return 0;
	
			if (linein[now_in] < 0) {
				lstoff = 1;
				linein[now_in] = -linein[now_in];
			} else {
				lstoff = 0 ;
			}

			if (outpass) {
				if (iflist()) {
					lineout();
					fprintf(fout, "**** %s ****\n", src_name[now_in]) ;
				}
				if (fbds)
					fprintf(fbds, "%04x %04x f %s\n", dollarsign, emit_addr, src_name[now_in]);
			}

			if (p != inpbuf) {
				*p++='\n';
				*p = '\0';
			}
			else
				inpptr = 0;
		}
	}

	return nextchar();
}

char *skipspace(char *p)
{
	while (CHARCLASS(*p) == SPACE)
		p++;

	return p;
}

// Look at inpbuf and try to determine what logical column we are starting
// at.  We could put all of the work in here and keep yylex simple but for
// now we share the load.

void analyze_inpbuf(void)
{
	int cc;
	char *p, *q, save;
	char *word1, *word2;
	struct item *ip, *word1item;
	int triplecase = 1;

	macopt = 0;

	// No need to do this when pulling in data for a macro.  In fact,
	// in can be harmful to undef a macro.
	if (inmlex)
		return;

	// Default if we find nothing to override
	logcol = 0;

	// One case to check is when you start with whitespace yet there are
	// 3 columns.  If so then we change logcol to -1 to compensate.
	// If the 2nd column is a VERB.

	// If we start with whitespace then we can give up on triple word case.
	p = (char *)inpbuf;
	if (CHARCLASS(*p) != SPACE)
		triplecase = 0;

	p = skipspace(p);
	word1 = p;

	// Special comment if first non-space char is '*'
	if (driopt) {
		comnt = (*p == '*');
		if (comnt)
			return;
	}

	// Now skip over a token or abort if we don't find one

	cc = CHARCLASS(*p);
	if (cc != LETTER && cc != STARTER && cc != DIGIT && cc != DOLLAR)
		return;

	if (driopt && *p == '$' && (p[1] == '-' || p[1] == '+' || p[1] == '*') &&
		strncasecmp(p + 2, "macro", 5) == 0 &&
		(p[7] == 0 || p[7] == '\n' || CHARCLASS(p[7]) == SPACE))
	{
		macopt = p[1];
	}

	for (;;) {
		cc = CHARCLASS(*p);
		if (cc == LETTER || cc == STARTER || cc == DIGIT || cc == DOLLAR)
			p++;
		else
			break;
	}

	// We could skip space-separated colons now, but if we see a colon
	// both issues have been decided to do that because it is easier.
	if (*p == ':')
		return;

	save = *p;
	*p = '\0';
	word1item = locate(word1);
	*p = save;

	p = skipspace(p);

	// Another token to skip past.
	// But we need to examine it to see if it is a verb.

	cc = CHARCLASS(*p);
	if (cc != LETTER && cc != STARTER && cc != DIGIT && cc != DOLLAR)
		return;

	q = p;
	word2 = p;
	for (;;) {
		cc = CHARCLASS(*p);
		if (cc == LETTER || cc == STARTER || cc == DIGIT || cc == DOLLAR)
			p++;
		else
			break;
	}

	// Now we have a second word we can check for the "name macro" case.
	// Unless we're skipping.
	save = *p;
	*p = '\0';
	if (ci_strcmp(word2, "macro") == 0 && word1item->i_token && !*ifptr)
		word1item->i_token = UNDECLARED;

	*p = save;

	if (!triplecase)
		return;

	// Must have space to skip over
	if (CHARCLASS(*p) != SPACE)
		return;

	// This 2nd token must be a verb.
	cc = *p;
	*p = '\0';
	ip = keyword(q);
	*p = cc;
	if (!ip || !(ip->i_uses & VERB))
		return;

	// Now skip over space.  If there's anything but a comment or end
	// of the line then we've may have 3 logical columns.
	// "ld a, 5" can throw that off, but we've done the verb check.

	p = skipspace(p);

	if (*p != ';' && *p != '\n' && *p != '\0')
		logcol--;
}

void mras_operator_separate(void)
{
	int i, sep;

	// Only do operator separation after macro expansion.
	if (inmlex)
		return;

	// Apply operator separation across the entire line.
	// We could limit this to logical columns if need be,
	// but I imagine MRAS didn't allow "x.mod.y" as a label
	// or macro name.

	// Strings are handled in a simple but largly compatible way.
	// AF' is a problem which can be safely ignored since its appearance
	// will mean no expression is present.
	// My allowed salting of ' after identifiers is another matter.
	// But, again, original MRAS code won't do that.

	memset(inpbuf_insert, 0, strlen((char *)inpbuf));

	sep = '\0';
	for (i = 0; inpbuf[i]; i++) {
		int j;

		if (inpbuf[i] == sep) {
			sep = '\0';
			continue;
		}

		if (inpbuf[i] == '\'' || inpbuf[i] == '"')
			sep = inpbuf[i];

		// Don't do anthing if inside a string constant.
		if (sep)
			continue;

		if (inpbuf[i] == ';')
			break;

		// Slight nod to efficiency.  Assumption is tested in
		// check_keytab().

		if (inpbuf[i] != '.')
			continue;

		for (j = 0; j < sizeof(keytab) / sizeof(keytab[0]); j++) {
			char *p = keytab[j].i_string;
			int len = strlen(p);

			if (!(keytab[j].i_uses & MRASOP))
				continue;

			if (ci_strcompare((char *)inpbuf + i, p, len) == 0) {
				// Only need to add spaces if we're possibly
				// in an identifier.  But no harm if we're not.
				inpbuf_insert[i] = ' ';
				inpbuf_insert[i + len] = ' ';
				i += len - 1;
				break;
			}
		}
	}
}


/*
 *  skip to rest of the line -- comments and if skipped lines
 */
int skipline(int ac)
{
	int  c;

	c = ac;
	while (c != '\n' && c != '\0')
		c = nextchar();
	return('\n');
}

void add_incpath(char *dir)
{
	if (incpath_cnt >= MAXINCPATH) {
		fprintf(stderr, "Sorry, can only handle %d include paths\n", MAXINCPATH);
		exit(1);
	}

	incpath[incpath_cnt++] = strdup(dir);
}

FILE *open_incpath(char *filename, char *mode, char **path_used)
{
	char quote;
	int i;
	char path[1024];
	FILE *fp;

	// Due to the way parsing works the string can be specified
	// without quotes or will allow quotes but include them.  Instead
	// of fooling with the parsing I just strip the quotes.  I think
	// you can still include a file that starts with a single or double
	// quote by quoting it, but that's an awful thing to do to yourself.

	quote = *filename;
	if (quote == '"' || quote == '\'') {
		char *p;
		for (p = filename; *p; p++)
			p[0] = p[1];

		if (p[-2] == quote)
			p[-2] = '\0';
	}

	// First look for included file in same directory as source file.

	strcpy(path, src_name[now_in]);
	*basename(path) = '\0';
	strcat(path, filename);
	fp = fopen(path, mode);
	if (fp) {
		if (path_used)
			*path_used = strdup(path);

		if (note_depend && outpass)
			printf("%s\n", path);
		return fp;
	}

	for (i = 0; i < incpath_cnt; i++) {
		sprintf(path, "%s/%s", incpath[i], filename);
		fp = fopen(path, mode);
		if (fp) {
			if (path_used)
				*path_used = strdup(path);
			if (note_depend && outpass)
				printf("%s\n", path);
			return fp;
		}
	}

	if (note_depend && outpass)
		printf("%s\n", filename);

	fp = fopen(filename, mode);
	if (fp && path_used)
		*path_used = strdup(filename);

	return fp;
}

void version()
{
	fprintf(stderr, "zmac version " VERSION "        http://48k.ca/zmac.html\n");
}

//
// Print out a usage message and exit.
//
void usage(char *msg, char *param)
{
	fprintf(stderr, msg, param);
	fprintf(stderr, "\n");
	version();
	fprintf(stderr, "usage: zmac [-8bcefghijJlLmnopstz] [-I dir] [-Pk=n] [-Dsym] file[.z]\n");
	fprintf(stderr, "other opts: --rel[7] --mras --zmac --dri --nmnv --z180 --fcal --dep --help --doc --version\n");
	fprintf(stderr, "  zmac -h for more detail about options.\n");
	exit(1);
}

void help()
{
	version();

	fprintf(stderr, "   --version\tshow version number\n");
	fprintf(stderr, "   --help\tshow this help message\n");
	fprintf(stderr, "   -8\t\tuse 8080 timings and interpretation of mnemonics\n");
	fprintf(stderr, "   -b\t\tno binary (.hex,.cmd,.cas, etc.) output\n");
	fprintf(stderr, "   -c\t\tno cycle counts in listing\n");
	fprintf(stderr, "   -e\t\terror list only\n");
	fprintf(stderr, "   -f\t\tprint if skipped lines\n");
	fprintf(stderr, "   -g\t\tdo not list extra code\n");
	fprintf(stderr, "   -h\t\tshow this information about options and quit\n");
	fprintf(stderr, "   -i\t\tdo not list include files\n");
	fprintf(stderr, "   -I dir\tadd 'dir' to include file search path\n");
	fprintf(stderr, "   -j\t\tpromote relative jumps to absolute as needed\n");
	fprintf(stderr, "   -J\t\twarn when a jump could be relative\n");
	fprintf(stderr, "   -l\t\tlist to standard output\n");
	fprintf(stderr, "   -L\t\tforce listing of everything\n");
	fprintf(stderr, "   -m\t\tprint macro expansions\n");
	fprintf(stderr, "   -n\t\tput line numbers off\n");
	fprintf(stderr, "   -o file.hex\toutput only named file (multiple -o allowed)\n");
	fprintf(stderr, "   -p\t\tput out four \\n's for eject\n");
	fprintf(stderr, "   -P\t\tformat listing for a printer\n");
	fprintf(stderr, "   -s\t\tdon't produce a symbol list\n");
	fprintf(stderr, "   -t\t\toutput error count instead of list of errors\n");
	fprintf(stderr, "   -z\t\tuse Z-80 timings and interpretation of mnemonics\n");
	fprintf(stderr, "   -Pk=num\tset @@k to num before assembly (e.g., -P4=10)\n");
	fprintf(stderr, "   -Dsym\tset symbol sym to 1 before assembly (e.g., define it)\n");
	fprintf(stderr, "   --od\tdir\tdirectory unnamed output files (default \"zout\")\n");
	fprintf(stderr, "   --oo\thex,cmd\toutput only listed file suffix types\n");
	fprintf(stderr, "   --xo\tlst,cas\tdo not output listed file suffix types\n");
	fprintf(stderr, "   --nmnv\tdo not interpret mnemonics as values in expressions\n");
	fprintf(stderr, "   --z180\tuse Z-180 timings and extended instructions\n");
	fprintf(stderr, "   --dep\tlist files included\n");
	fprintf(stderr, "   --mras\tlimited MRAS/EDAS compatibility\n");
	fprintf(stderr, "   --rel\toutput .rel file only (--rel7 for 7 character symbol names)\n");
	fprintf(stderr, "   --zmac\tcompatibility with original zmac\n");
	fprintf(stderr, "   --fcal\tidentifier in first column is always a label\n");
	fprintf(stderr, "   --doc\toutput documentation as HTML file\n");

	exit(0);
}

int main(int argc, char *argv[])
{
	struct item *ip;
	int  i, j;
	int  files;
	int used_o;
	int used_oo;
	char *zmac_args_env;
#ifdef DBUG
	extern  yydebug;
#endif

	separator = '\\';

	fin[0] = stdin;
	now_file = stdin;
	files = 0;
	used_o = 0;
	used_oo = 0;

	// Special flag for unit testing.
	if (argc > 1 && strcmp(argv[1], "--test") == 0)
		exit(!check_keytab());

	// To avoid typing typical command-line arguments every time we
	// allow ZMAC_ARGS environment variable to augment the command-line.
	zmac_args_env = getenv("ZMAC_ARGS");
	if (zmac_args_env) {
		int new_argc = 0;
		char *arg;
		char **new_argv;
		static char *zmac_args;

		// Overestimate to size of new argv vector.
		new_argv = malloc((argc + strlen(zmac_args_env) + 1) *
			sizeof *new_argv);
		// Save a copy because we (1) mutate it and (2) use it in argv.
		zmac_args = strdup(zmac_args_env);

		if (!new_argv || !zmac_args)
			error("malloc to support ZMAC_ARGS failed");

		memcpy(new_argv, argv, sizeof(*new_argv) * argc);
		new_argc = argc;

		arg = strtok(zmac_args, " \t");
		while (arg != NULL) {
			new_argv[new_argc++] = arg;
			arg = strtok(NULL, " \t");
		}

		argv = new_argv;
		argc = new_argc;
	}

	for (i=1; i<argc; i++) {
		int skip = 0;
		if (strcmp(argv[i], "--mras") == 0) {
			mras = 1;
			trueval = -1;
			continue;
		}

		if (strcmp(argv[i], "--dri") == 0) {
			driopt = 1;
			separator = '!';
			continue;
		}

		if (strcmp(argv[i], "--rel") == 0) {
			relopt = 6;
			continue;
		}

		if (strcmp(argv[i], "--rel7") == 0) {
			relopt = 7;
			continue;
		}

		if (strcmp(argv[i], "--zmac") == 0) {
			zcompat = 1;
			continue;
		}

		if (strcmp(argv[i], "--dep") == 0) {
			note_depend = 1;
			continue;
		}

		if (strcmp(argv[i], "--nmnv") == 0) {
			nmnvopt = 1;
			continue;
		}

		if (strcmp(argv[i], "--fcal") == 0) {
			first_always_label = 1;
			continue;
		}

		if (strcmp(argv[i], "--help") == 0) {
			help();
			continue;
		}

		if (strcmp(argv[i], "--doc") == 0) {
			extern void doc(void);
			doc();
			exit(0);
			continue; // not reached
		}

		if (strcmp(argv[i], "--version") == 0) {
			version();
			exit(0);
			continue; // not reached
		}

		if (strcmp(argv[i], "--z180") == 0) {
			/* Equivalent to .z180 */
			default_z80 = 2;
			continue;
		}

		if (strcmp(argv[i], "--od") == 0) {
			output_dir = argv[i = getoptarg(argc, argv, i)];
			continue;
		}

		if (strcmp(argv[i], "--oo") == 0) {
			if (!used_oo)
				stop_all_outf();

			suffix_list(argv[i = getoptarg(argc, argv, i)], 0);

			used_oo = 1;
			continue;
		}

		if (strcmp(argv[i], "--xo") == 0) {
			suffix_list(argv[i = getoptarg(argc, argv, i)], 1);
			continue;
		}

		if (argv[i][0] == '-' && argv[i][1] == '-')
			usage("Unknown option: %s", argv[i]);

		if (argv[i][0] == '-' && argv[i][1] == 'P' &&
			argv[i][2] >= '0' && argv[i][2] <= '9')
		{
			if (argv[i][3] == '=') {
				int overflow;
				int radix;
				int sign = 1;
				char *str = argv[i] + 4;
				if (*str == '-') {
					sign = -1;
					str++;
				}

				radix = convert(str, NULL, &overflow);
				if (radix <= 0 || overflow)
					usage("bad -Pn= parameter value", 0);

				mras_param[argv[i][2] - '0'] = sign * yylval.ival;
				mras_param_set[argv[i][2] - '0'] = 1;
			}
			else if (argv[i][3] == '\0') {
				mras_param[argv[i][2] - '0'] = -1;
				mras_param_set[argv[i][2] - '0'] = 1;
			}
			else
				usage("-Pn syntax error", 0);

			continue;
		}

		if (argv[i][0] == '-' && argv[i][1] == 'D') {
			struct cl_symbol *sym = malloc(sizeof(struct cl_symbol));
			if (!argv[i][2])
				usage("missing symbol name for -D", 0);

			sym->name = argv[i] + 2;
			sym->next = cl_symbol_list;
			cl_symbol_list = sym;

			continue;
		}

		if (*argv[i] == '-') while (*++argv[i]) {
			switch(*argv[i]) {

			case '8':	/* Equivalent to .8080 */
				default_z80 = 0;
				continue;

			case 'b':	/*  no binary  */
				for (j = 0; j < CNT_OUTF; j++)
					if (strcmp(outf[j].suffix, "lst") != 0)
						outf[j].no_open = 1;
				continue;

			case 'c':	/*  no cycle counts in listing */
				copt-- ;
				continue;

#ifdef DBUG
			case 'd':	/*  debug  */
				yydebug++;
				continue;
#endif

			case 'e':	/*  error list only  */
				eopt = 0;
				edef = 0;
				continue;

			case 'f':	/*  print if skipped lines  */
				fopt++;
				fdef++;
				continue;

			case 'g':	/*  do not list extra code  */
				gopt = 0;
				gdef = 0;
				continue;

			case 'h':
				help();
				continue;

			case 'i':	/* do not list include files */
				iopt = 1 ;
				continue ;

			case 'I':
				if (argv[i][1])
					add_incpath(argv[i] + 1);
				else
					add_incpath(argv[i = getoptarg(argc, argv, i)]);
				skip = 1;
				break;

			case 'l':	/*  list to stdout */
				fout = stdout;
				continue;

			case 'L':	/*  force listing of everything */
				lston++;
				continue;

			case 'j':	// promote relative jumps to absolute as needed
				default_jopt = 1;
				continue;

			case 'J':	// error when JR instructions could replace JP
				default_JPopt = 1;
				continue;

			case 'm':	/*  print macro expansions  */
				mdef++;
				mopt++;
				continue;

			case 'n':	/*  put line numbers off */
				nopt-- ;
				continue;

			case 'o':	/*  select output */
				{
					char *outfile = 0;
					char *sfx;
					int found = 0;

					if (!used_o)
						stop_all_outf();

					if (argv[i][1])
						outfile = argv[i] + 1;
					else
						outfile = argv[i = getoptarg(argc, argv, i)];

					for (sfx = outfile; !found && *sfx; sfx++) {
						if (*sfx != '.')
							continue;

						for (j = 0; j < CNT_OUTF; j++) {
							if (ci_strcmp(sfx + 1, outf[j].suffix) == 0) {
								outf[j].no_open = 0;
								outf[j].wanted = 1;
								outf[j].filename = outfile;
								found = 1;
								break;
							}
						}
					}
					if (!found)
						usage("output file '%s' has unknown suffix", outfile);
				}
				used_o = 1;
				skip = 1;
				break;

			case 'p':	/*  put out four \n's for eject */
				popt-- ;
				continue;

			case 'P':	// GWP - printer style output (headers, page separation, etc.)
				printer_output = 1;
				continue;

			case 's':	/*  don't produce a symbol list  */
				sopt++;
				continue;

			case 't':	/*  output only number of errors */
				topt = 0;
				continue;

			case 'z':	/* Equivalent to .z80 */
				default_z80 = 1;
				continue;

			default:	/*  error  */
				{
					char badopt[2] = { argv[i][0], 0 };
					usage("Unknown option: %s", badopt);
				}
			}
			if (skip)
				break;
		}
		else if (files++ == 0) {
			sourcef = argv[i];
			strcpy(src, sourcef);
			if ((now_file = fopen(src, "r")) == NULL) {
				if (!*getsuffix(src))
					suffix(src, ".z");
				if ((now_file = fopen(src, "r")) == NULL)
					usage("Cannot open source file '%s'", src);
			}
			now_in = 0;
			fin[now_in] = now_file ;
			src_name[now_in] = src ;
		} else if (files)
			usage("Too many arguments", 0);
	}


	if (files == 0)
		usage("No source file", 0);

	// .wav file outputs must ensure their .cas antecedents are generated.
	// And also check for .rel output and set relopt while we're at it.
	for (j = 0; j < CNT_OUTF; j++) {
		char *p;

		if (strcmp(outf[j].suffix, "rel") == 0 && !outf[j].no_open && !relopt)
			relopt = 6;

		p = strchr(outf[j].suffix, '.');
		// Only .wav file that open matter and only if .cas doesn't open.
		if (!p || strcmp(p, ".wav") != 0 || outf[j].no_open || !outf[j + 1].no_open)
			continue;

		outf[j + 1].no_open = 0;
		outf[j + 1].temp = 1;
		if (outf[j].filename) {
			// Replace .wav suffix with .cas.  This is safe for
			// now as only -o can choose a filename and it must end
			// with outf[j]'s suffix to be put in outf[j].
			outf[j + 1].filename = strdup(outf[j].filename);
			p = strrchr(outf[j + 1].filename, '.');
			strcpy(p + 1, "cas");
		}
	}

	if (relopt) {
		for (j = 0; j < CNT_OUTF; j++) {
			if (strcmp(outf[j].suffix, "lst") != 0)
			{
				outf[j].no_open = strcmp(outf[j].suffix, "rel") != 0;
			}
		}
	}

	for (j = 0; j < CNT_OUTF; j++) {
		if (outf[j].no_open || *outf[j].fpp)
			continue;

		if (!outf[j].filename) {
			char suffix[32];
			strcpy(suffix, ".");
			strcat(suffix, outf[j].suffix);
			outpath(oth, sourcef, suffix);
			outf[j].filename = strdup(oth);
		}

		*outf[j].fpp = fopen(outf[j].filename, outf[j].mode);
		if (!*outf[j].fpp) {
			fprintf(stderr, "Cannot create file '%s'", outf[j].filename);
			clean_outf();
			exit(1);
		}
	}

	if (fbds) {
		fprintf(fbds, "binary-debuggable-source\n");
		fprintf(fbds, "%04x %04x f %s\n", dollarsign, emit_addr, src_name[now_in]);
	}

	// Tape header
	for (i = 0; i < 255; i++) {
		if (flcas) fputc(0, flcas);
		if (flnwcas) fputc(0, flnwcas);
		if (fcas) fputc(0x55, fcas);
		if (ftcas) fputc(0, ftcas);
	}
	if (flcas) fputc(0xA5, flcas);
	if (flnwcas) fputc(0xA5, flnwcas);
	if (fcas) fputc(0x7F, fcas);
	if (ftcas) fputc(0xA5, ftcas);

	casname(oth, sourcef, 6);
	putcas(0x55);
	for (i = 0; i < 6; i++)
		putcas(oth[i]);

	if (relopt) {
		strncpy(progname, basename(sourcef), sizeof progname);
		progname[sizeof progname - 1] = '\0';
	}

	// mfopen() is always in-memory not a temporary file.

	mfile = mfopen("does-not-matter","w+b") ;

	/*
	 *  get the time
	 */
	time(&now);
	timp = ctime(&now);
	timp[16] = 0;
	timp[24] = 0;

	title = sourcef;
	/*
	 * pass 1
	 */
#ifdef DEBUG
	fputs("DEBUG-pass 1\n", stderr) ;
#endif
	clear();
	setvars();
	npass = 1;
	outpass = 0;
	yyparse();

	// GWP - errors should stop us, but the listing is very useful.

	pass2++;

	for (npass = 2; npass < MAXPASS; npass++) {
		if (passfail || npass == MAXPASS - 1)
			outpass = 1;

		if (outpass) {
			putrelcmd(RELCMD_PROGNAME);
			putrelname(progname);
		}

		for (ip = itemtab - 1; ++ip < itemmax; ) {
			// Output list of public labels.  m80 will let
			// equates and aseg values be public so we do, too.
			if (outpass && ip->i_token && (ip->i_scope & SCOPE_PUBLIC)) {
				putrelcmd(RELCMD_PUBLIC);
				putrelname(ip->i_string);
			}

			/* reset use count */
			ip->i_uses = 0 ;

			/* set macro names, equated and defined names */
			switch	(ip->i_token) {
			case MNAME:
				ip->i_token = UNDECLARED;
				break;

			case EQUATED:
				ip->i_token = WASEQUATED;
				break;

			case DEFLED:
				if (zcompat)
					ip->i_token = UNDECLARED;
				break;
			}
		}

		if (outpass) {
			// m80 outputs data size as an absolute value, but
			// code size as code segment relative.  Odd, but
			// I'll follow suit.
			putrelcmd(RELCMD_DATASIZE);
			putrelsegref(SEG_ABS, seg_size[SEG_DATA]);

			putrelcmd(RELCMD_CODESIZE);
			putrelsegref(SEG_CODE, seg_size[SEG_CODE]);
		}

		for (ip = itemtab - 1; ++ip < itemmax; ) {
			if (ip->i_token != COMMON)
				continue;

			// TODO: perhaps have WASCOMMON but this will suffice
			ip->i_token = UNDECLARED;

			putrelcmd(RELCMD_COMSIZE);
			putrelsegref(SEG_ABS, ip->i_value);
			putrelname(ip->i_string);
		}

		// In case we hit 'end' inside an included file
		while (now_in > 0) {
			fclose(fin[now_in]);
			free(src_name[now_in]);
			now_file = fin[--now_in];
			nextline_peek = linepeek[now_in];
		}
		setvars();
		clear_instdata_flags();
		fseek(now_file, (long)0, 0);

	#ifdef DEBUG
		fprintf(stderr, "DEBUG- pass %d\n", npass) ;
	#endif

		yyparse();

		if (outpass || passfail)
			break;

		if (!passretry)
			outpass = 1;
	}

	flushbin();
	flushoth();

	if (fbuf)
		putc(':', fbuf);

	if (xeq_flag) {
		if (fbuf) {
			puthex(0, fbuf);
			puthex(xeq >> 8, fbuf);
			puthex(xeq, fbuf);
			puthex(1, fbuf);
			puthex(255-(xeq >> 8)-xeq, fbuf);
		}
		if (fcmd) {
			fprintf(fcmd, "%c%c%c%c", 2, 2, xeq, xeq >> 8);
			fflush(fcmd);
		}
		putcas(0x78);
		putcas(xeq);
		putcas(xeq >> 8);
		if (fmds)
			fprintf(fmds, "pc=$%04x\ng\n", xeq);
	}
	else {
		// SYSTEM cassette files must have an execution address.
		// Without one we simply do not output .cas or .wav SYSTEM.

		int i;
		for (i = 0; i < CNT_OUTF; i++) {
			if (*outf[i].fpp && outf[i].system) {
				fclose(*outf[i].fpp);
				*outf[i].fpp = NULL;
				// unlink is an intended implicit declaration -- silence the gcc warning.
				// Old gcc's don't permit #pragman in a function.
				// Uncomment to suppress the warning.
				//#pragma GCC diagnostic push
				//#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
				unlink(outf[i].filename);
				//#pragma GCC diagnostic pop
				if (outf[i].wanted)
					fprintf(stderr, "Warning: %s not output -- no entry address (forgot \"end label\")\n", outf[i].filename);
			}
		}

		if (fbuf) {
			puthex(0, fbuf);
			puthex(0, fbuf);
			puthex(0, fbuf);
			puthex(1, fbuf);
			puthex(255, fbuf);
		}
	}

	if (fbuf) {
		putc('\n', fbuf);
		fflush(fbuf);
	}

	// "Play Cas" seems to require trailing zeros to work
	// properly.  And we need to output at least one zero byte
	// to flush out the final high speed bits.
	#define CAS_PAD 6
	for (i = 0; i < CAS_PAD; i++)
		putcas(0);

	if (relopt) {
		struct item *ip;
		// Output external symbols and value of public symbols
		for (ip = itemtab; ip < itemmax; ip++) {
			if (ip->i_token == UNDECLARED && (ip->i_scope & SCOPE_EXTERNAL)) {
				putrelcmd(RELCMD_EXTCHAIN);
				// Chain value will have top two bits set appropriately
				putrelextaddr(ip->i_chain);
				putrelname(ip->i_string);
			}
			if (ip->i_scope & SCOPE_PUBLIC)
			{
				putrelcmd(RELCMD_PUBVALUE);
				putrelsegref(ip->i_scope, ip->i_value);
				putrelname(ip->i_string);
			}
		}

		// End module, entry address if any
		putrelcmd(RELCMD_ENDMOD);
		putrelextaddr(rel_main);
		flushrel(); // byte alignment expected after end module

		// End .rel file
		putrelcmd(RELCMD_ENDPROG);
		flushrel();
	}

	if (xeq_flag == 0) {
#if WIN32
		char *username = getenv("USERNAME");
		if (username && strcmp(username, "George") == 0 && !relopt) {
			CONSOLE_SCREEN_BUFFER_INFO inf;
			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(hOut, &inf);
			SetConsoleTextAttribute(hOut, FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
			fprintf(stderr, "Warning: no entry address (forgot \"end label\")\n");
			fflush(stderr);
			SetConsoleTextAttribute(hOut, inf.wAttributes);
		}
#endif
	}
	else if (fbds) {
		fprintf(fbds, "%04x e\n", xeq);
	}

	if (fcim || fams || ftap || ftcas) {
		int low = 0;
		int high = sizeof(memory) - 1;
		int chk;
		int filelen;
		int i;
		char leafname[] = "FILENAMEBIN";

		while (low < sizeof(memory) && (memflag[low] & (MEM_INST | MEM_DATA)) == 0)
			low++;

		while (high >= 0 && (memflag[high] & (MEM_INST | MEM_DATA)) == 0)
			high--;

		if (high >= low && fcim)
			fwrite(memory + low, high + 1 - low, 1, fcim);

		filelen = (high + 1) - low;

		// AMSDOS binary file output (A for Amstrad, code from zmac 1.3)
		if (fams) {
			chk = 0;
			putc(0, fams);
			for (i = 0; i < 11; i++) {
				putc(leafname[i], fams);
				chk += leafname[i];
			}
			for (i = 0; i < 6; i++)
				putc(0, fams);

			putc(2, fams); // Unprotected binary
			chk += 2;
			putc(0, fams);
			putc(0, fams);
			putc(low & 0xff, fams);
			chk += low & 0xff;
			putc(low >> 8, fams);
			chk += low >> 8;
			putc(0, fams);
			putc(filelen & 0xff, fams);
			chk += filelen & 0xff;
			putc(filelen >> 8, fams);
			chk += filelen >> 8;
			putc(xeq & 0xff, fams);
			chk += xeq & 0xff;
			putc(xeq >> 8, fams);
			chk += xeq >> 8;
			for (i = 28; i < 64; i++)
				putc(0, fams);

			putc(filelen & 0xff, fams);
			chk += filelen & 0xff;
			putc(filelen >> 8, fams);
			chk += filelen >> 8;
			putc(0, fams); // this would be used if filelen > 64K
			putc(chk & 0xff, fams);
			putc(chk >> 8, fams);

			for (i = 69; i < 128; i++)
				putc(0, fams);

			if (filelen > 0)
				fwrite(memory + low, filelen, 1, fams);

			if (filelen & 0x7f)
				putc(0x1a, fams); // CP/M EOF character
		}

		if (ftap)
			write_tap(filelen, low, memory + low);
 
		if (ftcas)
			write_250(low, high);
	}

	// Output .wav files noting the padding bytes to ignore.
	writewavs(0, CAS_PAD, CAS_PAD);

	if (fbds) {
		struct item *tp;

		for (tp = itemtab; tp < itemmax; tp++) {
			if (tp->i_token == LABEL)
				fprintf(fbds, "%04x a %s\n", tp->i_value, tp->i_string);
			else if (tp->i_token == EQUATED)
				fprintf(fbds, "%04x v %s\n", tp->i_value, tp->i_string);
		}
	}

	if (fout)
		fflush(fout);
	if (writesyms)
		outsymtab(writesyms);
	compactsymtab();
	if (eopt)
		erreport();
	if (!sopt)
		putsymtab();
	if (fout) {
		eject();
		fflush(fout);
	}
	// GWP - some things (like balance errors in macro definitions) do
	// not show up until you use them.  So just in case we print an error
	// count here as not to confuse the programmer who is unlikely to check
	// the listing for errors if none are shown in the command window.
	if (counterr() > 0)
		fprintf(stderr, "%d errors (see listing if no diagnostics appeared here)\n", counterr());
	if (countwarn() > 0)
		fprintf(stderr, "%d warnings (see listing if no diagnostics appeared here)\n", countwarn());

	clean_outf_temp();
	if (counterr() > 0)
		clean_outf();
	exit(counterr() > 0);
}

int getoptarg(int argc, char *argv[], int i)
{
	i++;
	if (i < argc)
		return i;

	usage("missing argument for %s option", argv[i - 1]);
	return i; // not actually reached
}

void stop_all_outf()
{
	int i;
	for (i = 0; i < CNT_OUTF; i++)
		outf[i].no_open = 1;
}

void clean_outf()
{
	int i;

	for (i = 0; i < CNT_OUTF; i++) {
		if (*outf[i].fpp) {
			int size = ftell(*outf[i].fpp);
			fclose(*outf[i].fpp);
			*outf[i].fpp = NULL;
			// Listing file can stick around, but not if empty.
			if (strcmp(outf[i].suffix, "lst") != 0 || size == 0)
				unlink(outf[i].filename);
		}
	}
}

void clean_outf_temp()
{
	int i;
	for (i = 0; i < CNT_OUTF; i++) {
		if (*outf[i].fpp && outf[i].temp) {
			fclose(*outf[i].fpp);
			*outf[i].fpp = NULL;
			unlink(outf[i].filename);
		}
	}
}

// Set output files to open or not using a comma-separated list of suffixes.
// Loops over the outf[] suffix so that "wav" can be used to eliminate all
// .wav files.
void suffix_list(char *sfx_lst, int no_open)
{
	while (sfx_lst) {
		int i;
		char *p = strchr(sfx_lst, ',');
		if (p)
			*p = '\0';

		// Allow prefix '.' in case user puts them in.
		while (*sfx_lst == '.')
			sfx_lst++;

		for (i = 0; i < CNT_OUTF; i++) {
			char *sfx;
			for (sfx = outf[i].suffix; sfx; ) {
				if (ci_strcmp(sfx, sfx_lst) == 0) {
					outf[i].no_open = no_open;
					if (!no_open)
						outf[i].wanted = 1;
				}
				sfx = strchr(sfx, '.');
				if (sfx)
					sfx++;
			}
		}

		sfx_lst = p ? p + 1 : 0;
	}
}

void equate(char *id, int value, int scope)
{
	struct item *it;

	it = locate(id);
	if (!it->i_token) {
		nitems++;
		it->i_value = value;
		it->i_token = EQUATED;
		it->i_pass = npass;
		it->i_scope = scope;
		it->i_uses = 0;
		it->i_string = malloc(strlen(id)+1);
		strcpy(it->i_string, id);
	}

	// This variable test true for ifdef
	// This is a slightly subtle way to ensure it->i_pass == npass
	// Setting it to npass or npass + 1 doesn't always work due to
	// the different contexts in which setvars() is called.
	if (scope & (SCOPE_CMD_D | SCOPE_CMD_P))
		it->i_pass++;
}

/*
 *  set some data values before each pass
 */
void setvars()
{
	int  i;
	struct cl_symbol *sym;

	peekc = NOPEEK;
	inpptr = 0;
	nextline_peek = NOPEEK;
	raw = 0;
	linein[now_in] = 0;
	exp_number = 0;
	emitptr = emitbuf;
	lineptr = linebuf;
	ifptr = ifstack;
	*ifptr = 0;
	dollarsign = 0;
	emit_addr = 0;
	olddollar = 0;
	oldothdollar = 0;
	phaseflag = 0;
	for (i=0; i<FLAGS; i++) err[i] = 0;
	tstates = 0;
	ocf = 0;
	llseq = 0;
	passfail = 0;
	passretry = 0;
	njrpromo = 0;
	jopt = default_jopt;
	JPopt = default_JPopt;
	strcpy(modstr, "@@@@");
	segment = SEG_CODE;
	cur_common = NULL;
	memset(seg_pos, 0, sizeof(seg_pos));
	memset(seg_size, 0, sizeof(seg_size));
	segchange = 0;
	z80 = default_z80;
	outrec = 0;
	outlen = 0;

	param_parse = 0;
	arg_reset();

	mfptr = 0;
	mfseek(mfile, mfptr, 0);

	// TODO - maybe these could be just as well handled lexically
	// like the 8080 opcodes in DRI mode?
	// These are slightly harmful, but not too bad.  Only
	// absolutely necessary for MAC compatibility.  But there's
	// some use in having them available always.

	equate("b", 0, SCOPE_BUILTIN);
	equate("c", 1, SCOPE_BUILTIN);
	equate("d", 2, SCOPE_BUILTIN);
	equate("e", 3, SCOPE_BUILTIN);
	equate("h", 4, SCOPE_BUILTIN);
	equate("l", 5, SCOPE_BUILTIN);
	equate("m", 6, SCOPE_BUILTIN);
	equate("a", 7, SCOPE_BUILTIN);

	equate("sp", 6, SCOPE_BUILTIN);
	equate("psw", 6, SCOPE_BUILTIN);

	// TODO - these are now handled lexically in --dri mode
	// There are a large number of symbols to add to support
	// "LXI H,MOV" and the like.

	// Technically only MRAS has these, but I'll wait and see if
	// anyone actually uses @@0 through @@9.  If so, then we can
	// DEFL them instead of EQU.
	for (i = 0; i < 10; i++) {
		char var[4];
		var[0] = '@';
		var[1] = '@';
		var[2] = '0' + i;
		var[3] = '\0';
		equate(var, mras_param[i], mras_param_set[i] ? SCOPE_CMD_P : SCOPE_BUILTIN);
	}

	for (sym = cl_symbol_list; sym; sym = sym->next)
		equate(sym->name, 1, SCOPE_CMD_D);

	reset_import();
}

//
// Clear out cycle counts and memory.
//

void clear()
{
	int i;

	for (i = 0; i < sizeof(memory) / sizeof(memory[0]); i++)
	{
		memory[i] = 0;
		memflag[i] = 0;
		tstatesum[i] = 0;
	}
}

void clear_instdata_flags()
{
	int i;

	for (i = 0; i < sizeof(memory) / sizeof(memory[0]); i++)
		memflag[i] &= ~(MEM_DATA | MEM_INST);
}

void setmem(int addr, int value, int type)
{
	value &= 0xff;
	memory[addr] = value;
	memflag[addr] |= type;
}

/*
 *  print out an error message and die
 */
void error(char *as)
{
	*linemax = 0;
	if (fout) {
		fprintf(fout, "%s\n", linebuf);
		fflush(fout);
	}
	fprintf(stderr, "%s\n", as) ;
	clean_outf();
	exit(1);
}


/*
 * Compact the symbol table, removing unused, UNDECLARED entries,
 * macros and built-in identifiers.
 */
void compactsymtab()
{
	struct item *tp, *fp;

	if (!nitems)
		return;

	tp = itemtab;
	tp--;
	for (fp = itemtab; fp<itemmax; fp++) {
		if (fp->i_token == UNDECLARED && !(fp->i_scope & SCOPE_EXTERNAL)) {
			nitems--;
			continue;
		}
		if (fp->i_token == 0)
			continue;

		// Don't list macros or internally defined symbols
		if (fp->i_token == MNAME || (fp->i_scope & SCOPE_BUILTIN)) {
			nitems--;
			continue;
		}

		tp++;
		itemcpy(tp, fp);
	}

	tp++;
	tp->i_string = "{";	/* } */

	/*  sort the table */
	custom_qsort(0, nitems-1);
}

/*
 *  output the symbol table
 */
void putsymtab()
{
	int  i, j, k, t, rows;
	char c, seg = ' '; //, c1;
	int numcol = printer_output ? 4 : 1;
	struct item *tp;

	if (!nitems || !fout)
		return;

	title = "**  Symbol Table  **";

	rows = (nitems+numcol-1) / numcol;
	if (rows+5+line > 60)
		eject();
	lineout();
	fprintf(fout,"\n\n\nSymbol Table:\n\n") ;
	line += 4;

	for (i=0; i<rows; i++) {
		for(j=0; j<numcol; j++) {
			k = rows*j+i;
			if (k < nitems) {
				tp = &itemtab[k];
				t = tp->i_token;
				c = ' ' ;
				if (t == EQUATED || t == DEFLED)
					c = '=' ;
				if (t == COMMON)
					c = '/';

				//if (tp->i_uses == 0)
				//	c1 = '+' ;
				//else
				//	c1 = ' ' ;

				// GWP - decided I don't care about uses
				// even if it were accurate.
				// TODO: Should use maxsymbol size in there,
				// but makes output harder to read.

				fprintf(fout, "%-15s%c", tp->i_string, c);

				if (tp->i_scope & SCOPE_ALIAS)
					fprintf(fout, "\"%s\"",
						keytab[tp->i_value].i_string);

				else {
					if (relopt)
						seg = SEGCHAR(tp->i_scope & SCOPE_SEGMASK);

					if (tp->i_value >> 16)
						fprintf(fout, "%08X%c", tp->i_value, seg);
					else if (tp->i_value >> 8)
						fprintf(fout, "%4X%c    ", tp->i_value, seg);
					else
						fprintf(fout, "%02X%c      ", tp->i_value, seg);

					fprintf(fout, " %d", tp->i_value);

					if (tp->i_scope & SCOPE_EXTERNAL)
						fprintf(fout, " (extern)");

					if (tp->i_scope & SCOPE_PUBLIC)
						fprintf(fout, " (public)");

					if (tp->i_scope & SCOPE_CMD_P)
						fprintf(fout, " (command line -P)");

					if (tp->i_scope & SCOPE_CMD_D)
						fprintf(fout, " (command line -D)");
				}
			}
		}
		lineout();
		putc('\n', fout);
	}
}




/*
 *  put out error report
 */
void erreport()
{
	int i, numerr, numwarn;

	if (!fout)
		return;

	if (line > 49) eject();
	lineout();
	numerr = 0;
	for (i=0; i<FIRSTWARN; i++) numerr += keeperr[i];
	numwarn = 0;
	for (i = FIRSTWARN; i < FLAGS; i++) numwarn += keeperr[i];
	if (numerr || numwarn) {
		fputs("\n\n\nError + Warning report:\n\n", fout);
		fprintf(fout, "%6d errors\n", numerr);
		fprintf(fout, "%6d warnings\n", numwarn);
		line += 6;
	} else {
		fputs("\n\n\nStatistics:\n", fout);
		line += 3;
	}

	for (i=0; i<FLAGS; i++)
		if (keeperr[i]) {
			lineout();
			fprintf(fout, "%6d %c -- %s %s\n",
				keeperr[i], errlet[i], errname[i],
				i < FIRSTWARN ? "error" : "warnings");
		}

	if (line > 52) eject();
	lineout();
	fprintf(fout, "\n%6d\tpasses\n", npass);
	fprintf(fout, "%6d\tjr promotions\n", njrpromo);
	fprintf(fout, "%6d\tsymbols\n", nitems);
	fprintf(fout, "%6d\tbytes\n", nbytes);
	line += 4;
	if (mfptr) {
		if (line > 53) eject();
		lineout();
		fprintf(fout, "\n%6d\tmacro calls\n", exp_number);
		fprintf(fout, "%6d\tmacro bytes\n", mfptr);
		fprintf(fout, "%6d\tinvented symbols\n", invented/2);
		line += 3;
	}
}

/*
 * count errors (GWP - added to set exit code)
 */
int counterr()
{
	int i, numerr = 0;
	for (i=0; i<FIRSTWARN; i++) numerr += keeperr[i];
	return numerr;
}

// Count warnings
int countwarn()
{
	int i, numwarn = 0;
	for (i = FIRSTWARN; i < FLAGS; i++)
		numwarn += keeperr[i];
	return numwarn;
}

char *mlook;

int nextmac()
{
	int ch;

	if (mlook) {
		if (*mlook) {
			ch = *mlook++;
			addtoline(ch);
		}
		else
			mlook = 0;
	}

	if (!mlook)
		ch = nextchar();

	return ch;
}

void putm_param_ref(struct item *param);
void putm_str(char *str, int look_for_param);

/*
 *  lexical analyser for macro definition
 */
void mlex(char *look)
{
	char  *p;
	int  c;
	int  t;
	int quote = 0, ampdrop = 0;
	struct item *param, *key;
	char symbuf[TEMPBUFSIZE];
	char *symbufmax = symbuf+TEMPBUFSIZE-1;

	/*
	 *  move text onto macro file, changing formal parameters
	 */
#ifdef	M_DEBUG
	fprintf(stderr,"enter 'mlex' at %d\n", mfptr) ;
#endif
	inmlex++;

	mlook = look;

	c = nextmac();
	for (;;) {
		int octo = 0, amp = 0, caret = 0;

		if (c == '#' || c == '&' || c == '^') {
			int esc = c;
			c = nextmac();
			if (charclass[c] != STARTER && charclass[c] != LETTER) {
				if (esc != '&' || !ampdrop)
					putm(esc);
				ampdrop = 0;
				continue;
			}
			if (esc == '#')
				octo = 1;
			else if (esc == '&')
				amp = 1;
			else
				caret = 1;
		}

		switch(charclass[c]) {

		case DIGIT:
			while (numpart[c]) {
				putm(c);
				c = nextmac();
			}
			continue;

		case STARTER:
		case LETTER:
			t = 0;
			p = symbuf;
			do {
				if (p >= symbufmax) {
					*symbufmax = '\0';
					printf("was parsing '%s' in macro definition\n", tempbuf);
					error(symlong);
				}
				*p++ = c;
				if (t < MAXSYMBOLSIZE)
					tempbuf[t++] = (c >= 'A' && c <= 'Z')  ?
						c+'a'-'A' : c;
				c = nextmac();
			} while	(charclass[c]==LETTER || charclass[c]==DIGIT || charclass[c]==STARTER);

			tempbuf[t] = 0;
			*p++ = '\0';
			p = symbuf;

			key = keyword(tempbuf);
			t = key ? key->i_token : 0;

			// Hmmm, that "item_lookup" is putting crap in the table, yes?
			if (mras)
				param = item_substr_lookup(tempbuf, MPARM, paramtab, PARAMTABSIZE);
			else
				param = item_lookup(tempbuf, paramtab, PARAMTABSIZE);

			// Accept almost anything as a parameter.  But a few
			// keywords will just screw things up royally.
			if (param && t != ENDM && t != REPT && t != IRPC && t != IRP && t != MACRO)
				t = param->i_token;

			// Caret escaping kills all expansion
			// And interpretation of ENDM, etc.  For better or worse.
			if (caret)
				t = 0;

			if (t == MPARM) {
				int pfx = octo || amp || c == '&';
				// # prefix only allowed if declared that way
				if (octo != param->i_scope)
					t = 0;
				else
					octo = 0;

				// Expansion in quoted strings only with prefix character.
				if (quote && !pfx && !zcompat)
					t = 0;

				amp = 0; // parameter substitution, eat '&'
			}
			else if (t && quote)
				t = 0;

			if (ampdrop)
				amp = 0;

			ampdrop = c == '&' && t == MPARM;

			if (octo) putm('#');
			if (amp) putm('&');

			if (t != MPARM) {
				putm(symbuf[0]);
				putm_str(symbuf + 1, mras && !quote);
			}
			else {
				putm_param_ref(param);
				// Only in MRAS will we have a remainder string
				putm_str(symbuf + strlen(param->i_string), mras && !quote);
			}

			if (t == ENDM) {
				if (--inmlex == 0)
					goto done;
			}
			else if (t == REPT || t == IRPC || t == IRP || t == MACRO) {
				inmlex++;
			}

			continue;

		case F_END:
			errwarn(warn_general, "macro definition went until end of file");
			if (expptr) {
				popsi();
				c = nextmac();
				continue;
			}

			goto done;

		default:
			switch (c) {
			case '\n':
				quote = 0;
				list1();
				break;
			case ';':
				if (!quote) {
					while (c != '\n' && c != 0) {
						putm(c);
						c = nextmac();
					}
					continue;
				}
				break;
			case '\'':
			case '"':
				if (c == quote)
					quote = 0;
				else
					quote = c;
				break;
			default:
				break;
			}
			if (c != '\1' && c != '`') putm(c);
			c = nextmac();
		}
	}

	/*
	 *  finish off the file entry
	 */
done:
	while(c != EOF && c != '\n' && c != '\0') c = nextmac();
	inmlex++;
	list1();
	inmlex--;
	// WHY two newlines??
//	putm('\n');
	putm('\n');
	putm(0);

	// TODO - here's where we could save parameter names for MRAS
	for (c = 0; c < PARAMTABSIZE; c++) {
		if (paramtab[c].i_token == MPARM) {
			free(paramtab[c].i_string);
			paramtab[c].i_string = NULL;
			paramtab[c].i_token = 0;
		}
	}
	inmlex = 0;
#ifdef	M_DEBUG
	fprintf(stderr,"exit 'mlex' at %d\n", mfptr) ;
#endif
}

void putm_param_ref(struct item *param)
{
	if (*(param->i_string) == '?' || param->i_chain)
		putm('\2');
	else
		putm('\1');

	putm(param->i_value + 'A');
}

void putm_str(char *str, int look_for_param)
{
	for (; *str; str++) {
		if (look_for_param) {
			struct item *param = item_substr_lookup(str, MPARM, paramtab, PARAMTABSIZE);
			if (param) {
				putm_param_ref(param);
				str += strlen(param->i_string) - 1;
				continue;
			}
		}
		putm(*str);
	}
}

int str_getch(struct argparse *ap)
{
	int ch = ap->user_peek;
	if (ch >= 0) {
		ap->user_peek = -1;
		return ch;
	}
	if (!ap->user_ptr || ap->user_ptr[ap->user_int] == '\0')
		return '\0';

	return ap->user_ptr[ap->user_int++];
}

int arg_getch(struct argparse *ap)
{
	(void)ap; // suppress warning
	return nextchar();
}

void arg_start()
{
	arg_reset();
	arg_flag = 1;
}

void arg_reset()
{
	arg_flag = 0;

	arg_state.arg = tempbuf;
	arg_state.argsize = sizeof tempbuf;
	arg_state.peek = &peekc;
	arg_state.getch = arg_getch;
	arg_state.macarg = 0;
	arg_state.user_ptr = 0;
	arg_state.user_int = 0;
	arg_state.didarg = 0;
	arg_state.numarg = 0;
}

/*
 *  lexical analyser for the arguments of a macro call
 */
int getarg(struct argparse *ap)
{
	int c;
	char *p;
	int quote;
	int depth;

	*ap->arg = 0;
	while (charclass[c = ap->getch(ap)] == SPACE);

	switch(c) {

	case '\0':
		if (!ap->user_ptr)
			popsi(); // a seemingly unlikely case?
	case '\n':
	case ';':
		if (!ap->didarg && ap->numarg) {
			*ap->peek = c;
			ap->didarg = 1;
			ap->numarg++;
			return ARG;
		}
		ap->didarg = 0;
		ap->numarg = 0;
		return c;

	case ',':
		if (!ap->didarg) {
			ap->didarg = 1;
			*ap->peek = c;
			ap->numarg++;
			return ARG;
		}
		ap->didarg = 0;
		return c;

	case '\'':
	case '\"':
		quote = c;
		p = ap->arg;
		if (!zcompat)
			*p++ = c;

		do {
			c = ap->getch(ap);
			if (c == '\0' || c == '\n') {
				*ap->peek = c;
				*p = 0;
				err[bflag]++;
				ap->didarg = 1;
				ap->numarg++;
				return ARG;
			}
			else if (c == quote) {
				if ((c = ap->getch(ap)) != quote) {
					if (!zcompat)
						*p++ = quote;
					*ap->peek = c;
					*p = '\0';
					ap->didarg = 1;
					ap->numarg++;
					return ARG;
				}
			}
			else
				*p++ = c;
		} while (p < ap->arg + ap->argsize - 1);
		ap->arg[ap->argsize - 1] = '\0';
		printf("was parsing macro argument '%s'\n", ap->arg);
		error(symlong);
		return 0; // not reached

	case '<':
		depth = 1;
		p = ap->arg;
		do {
			c = ap->getch(ap);
			if (c == '\0' || c == '\n') {
				*ap->peek = c;
				*p = 0;
				err[bflag]++;
				ap->didarg = 1;
				ap->numarg++;
				return ARG;
			}
			if (c == '>') {
				depth--;
				if (depth == 0) {
					*p = '\0';
					ap->didarg = 1;
					ap->numarg++;
					return ARG;
				}
			}
			else if (c == '<')
				depth++;

			*p++ = c;
		} while (p < ap->arg + ap->argsize - 1);
		ap->arg[ap->argsize - 1] = '\0';
		printf("was parsing macro argument '%s'\n", ap->arg);
		error(symlong);
		return 0; // not reached

	default:  /* unquoted string */
		if (c == '%' && ap->macarg) {
			ap->didarg = 1;
			ap->numarg++;
			return c;
		}

		p = ap->arg;
		*ap->peek = c;

		do {
			c = ap->getch(ap);
			switch(c) {
			case '\0':
			case '\n':
			case '\t':
			case ' ':
			case ',':
				*ap->peek = c;
				*p = '\0';
				ap->didarg = 1;
				ap->numarg++;
				return ARG;
			case '^':
				c = ap->getch(ap);
				switch (c) {
				case ',':
				case '^':
				case ' ':
				case '\t':
					*p++ = c;
					break;
				default:
					*p++ = '^';
					*ap->peek = c;
					break;
				}
				break;
			default:
				*p++ = c;
			}
		} while (p < ap->arg + ap->argsize - 1);
		ap->arg[ap->argsize - 1] = '\0';
		printf("was parsing macro argument '%s'\n", ap->arg);
		error("macro argument too long");
		return 0; // not reached
	}
}


/*
 *  add a suffix to a string
 */
void suffix(char *str, char *suff)
{
	strcpy(getsuffix(str), suff);
}

char *basename(char *filename)
{
	char *base, *p;

	base = filename;
	for (p = filename; *p; p++) {
		if (*p == '/' || *p == '\\') {
			base = p + 1;
		}
	}

	return base;
}

char *getsuffix(char *str)
{
	char *suffix = 0;
	str = basename(str);
	for (; *str; str++) {
		if (*str == '.')
			suffix = str;
	}
	return suffix ? suffix : str;
}

// Construct output file given input path.
// Essentially files for "file.z" are sent to "zout/file.suffix".
// And for "dir/file.z" they are "zout/file.suffix"
// Creates output directory as a side effect.

void outpath(char *out, char *src, char *suff)
{
	static int did_mkdir = 0;

	strcpy(out, output_dir);

	if (!did_mkdir) {
		char *dir = out;
		while (*dir) {
			char *p;
			int ch;
			for (p = dir; *p && *p != '/'
#ifdef WIN32
				 && *p != '\\'
#endif
				 ; p++) { };
			ch = *p;
			*p = '\0';
#ifdef WIN32
			_mkdir(out);
#else
			mkdir(out, 0777);
#endif
			*p = ch;
			dir = p;
			if (ch)
				dir++;
		}
		did_mkdir = 1;
	}

	if (!suff)
		return;

	if (*out)
		strcat(out, "/");
	strcat(out, basename(src));
	suffix(out, suff);
}


/*
 *  put out a byte to the macro file, keeping the offset
 */
void putm(int c)
{
	mfseek(mfile, mfptr, 0);
	mfptr++;
	mfputc(c, mfile);
}



/*
 *  get a byte from the macro file
 */
int getm()
{
	int ch;

	mfseek(mfile, floc, 0);
	floc++;
	ch = mfgetc(mfile);
	if (ch == EOF) {
		ch = 0;
		fprintf(stderr, "bad macro read\n");
	}
	return ch;
}



/*
 *  pop standard input
 */
void popsi()
{
	int  i;

	for (i=0; i<PARMMAX; i++) {
		if (est[i].param) free(est[i].param);
	}
	floc = est[FLOC].value;
	ifptr = est[MIF].param;
	free(est);
	expptr--;
	est = expptr ? expstack[expptr-1] : 0;
	mfseek(mfile, (long)floc, 0);
	if (lineptr > linebuf) lineptr--;

	listfrombookmark();
}



/*
 *  return a unique name for a local symbol
 *  c is the parameter number, n is the macro number.
 */

char *getlocal(int c, int n)
{
	static char local_label[10];

	invented++;
	if (c >= 26)
		c += 'a' - '0';
	sprintf(local_label, "?%c%04d", c+'a', n) ;
	return(local_label);
}

char *getmraslocal()
{
	static char mras_local[32];
	char *p = mras_local + sizeof mras_local - 1;
	int n = est[TEMPNUM].value;

	*p = '\0';
	for (; n > 0; n /= 26)
		*--p = 'A' + n % 26;


	return p;
}


/*
 *  read in a symbol table
 */
void insymtab(char *name)
{
	struct stab *t;
	int  s, i;
	FILE *sfile;

	t = (struct stab *) tempbuf;
	if (!(sfile = fopen(name, "rb")))
		return;
	fread((char *)t, 1, sizeof *t, sfile);
	if (t->t_value != SYMMAJIC)
		return;

	
	s = t->t_token;
	for (i=0; i<s; i++) {
		fread((char *)t, 1, sizeof *t, sfile);
		if (tokenofitem(UNDECLARED, 0, 0) != UNDECLARED)
			continue;
		yylval.itemptr->i_token = t->t_token;
		yylval.itemptr->i_value = t->t_value;
		if (t->t_token == MACRO)
			yylval.itemptr->i_value += mfptr;
	}

	while ((s = fread(tempbuf, 1, TEMPBUFSIZE, sfile)) > 0) {
		mfptr += s;
		mfwrite(tempbuf, 1, s, mfile) ;
	}
	fclose(sfile);
}



/*
 *  write out symbol table
 */
void outsymtab(char *name)
{
	struct stab *t;
	struct item *ip;
	int  i;
	FILE *sfile;

	t = (struct stab *) tempbuf;
	if (!(sfile = fopen(name, "wb")))
		return;
	for (ip=itemtab; ip<itemmax; ip++) {
		if (ip->i_token == UNDECLARED) {
			ip->i_token = 0;
			nitems--;
		}
	}

	copyname(title, (char *)t);
	t->t_value = SYMMAJIC;
	t->t_token = nitems;
	fwrite((char *)t, 1, sizeof *t, sfile);

	for (ip=itemtab; ip<itemmax; ip++) {
		if (ip->i_token != 0) {
			t->t_token = ip->i_token;
			t->t_value = ip->i_value;
			copyname(ip->i_string, (char *)t);
			fwrite((char *)t, 1, sizeof *t, sfile);
		}
	}

	mfseek(mfile, (long)0, 0);
	while((i = mfread(tempbuf, 1, TEMPBUFSIZE, mfile) ) > 0)
		fwrite(tempbuf, 1, i, sfile);

	fclose(sfile);
}



/*
 *  copy a name into the symbol file
 */
void copyname(char *st1, char *st2)
{
	char  *s1, *s2;
	int  i;

	i = (MAXSYMBOLSIZE+2) & ~01;
	s1 = st1;
	s2 = st2;

	while((*s2++ = *s1++)) i--;		/* -Wall-ishness :-) -RJM */
	while(--i > 0) *s2++ = '\0';
}

/* get the next source file */
void next_source(char *sp, int always)
{
	char *path;

	if (!always && imported(sp))
		return;

	if(now_in == NEST_IN -1)
		error("Too many nested includes") ;
	if ((now_file = open_incpath(sp, "r", &path)) == NULL) {
		char ebuf[1024] ;
		sprintf(ebuf,"Can't open include file: %s", sp) ;
		error(ebuf) ;
	}
	if (outpass && iflist()) {
		lineout() ;
		fprintf(fout, "**** %s ****\n", path) ;
	}

	if (outpass && fbds)
		fprintf(fbds, "%04x %04x f %s\n", dollarsign, emit_addr, sp);

	/* save the list control flag with the current line number */
	if (lstoff)
		linein[now_in] = - linein[now_in] ;

	/* no list if include files are turned off */
	lstoff |= iopt ;

	linepeek[now_in] = nextline_peek;
	nextline_peek = NOPEEK;
	/* save the new file descriptor. */
	fin[++now_in] = now_file ;
	/* start with line 0 */
	linein[now_in] = 0 ;
	/* save away the file name */
	src_name[now_in] = path;
}

int phaseaddr(int addr)
{
	if (!phaseflag)
		return addr;

	if (addr < phbegin || addr > dollarsign) {
		err[vflag]++;
		if (pass2)
			fprintf(stderr, "$%04x outside current phase area\n", addr);
		return 0;
	}

	return phdollar + (addr - phbegin);
}

// Include contents of named file as binary data.
void incbin(char *filename)
{
	FILE *fp = open_incpath(filename, "rb", NULL);
	int ch;
	int start = dollarsign;
	int last = start;
	int bds_count;
	int bds_dollar = dollarsign, bds_addr = emit_addr, bds_len;

	if (!fp) {
		char ebuf[1024];
		sprintf(ebuf, "Can't binary include file: %s", filename);
		error(ebuf);
		return;
	}

	addtoline('\0');
	if (outpass && fbds)
		fprintf(fbds, "%04x %04x s %s", dollarsign, emit_addr, linebuf);

	// Avoid emit() because it has a small buffer and it'll spam the listing.
	bds_count = 0;
	bds_len = 0;
	while ((ch = fgetc(fp)) != EOF) {
		if (outpass && fbds) {
			if (bds_count == 0)
				fprintf(fbds, "%04x %04x d ", dollarsign, emit_addr);
			fprintf(fbds, "%02x", ch);
			bds_len++;
			bds_count++;
			if (bds_count == 16) {
				fprintf(fbds, "\n");
				bds_count = 0;
			}
		}

		if (segment == SEG_CODE)
			setmem(emit_addr, ch, MEM_DATA);
		emit_addr++;
		emit_addr &= 0xffff;
		last = dollarsign;
		dollarsign++;
		dollarsign &= 0xffff;

		putbin(ch);
		putrel(ch);
		putout(ch);
	}
	if (outpass && fbds) {
		if (bds_count)
			fprintf(fbds, "\n");

		bds_perm(bds_dollar, bds_addr, bds_len);
	}

	fclose(fp);

	// Do our own list() work as we emit bytes manually.

	if (outpass && iflist()) {
		lineout();

		if (nopt)
			fprintf(fout, "%4d:", linein[now_in]);

		if (copt)
		        fprintf(fout, nopt ? "%5s-" : "%4s-", "");

		if (nopt || copt)
			fprintf(fout, "\t");

		list_optarg(start, -1, 0);
		fprintf(fout, "..");
		list_optarg(last, -1, 0);

		putc('\t', fout);

		fputs(linebuf, fout);

	}
	lineptr = linebuf;
}

void dc(int count, int value)
{
	int start = dollarsign;
	int bds_count;
	int bds_addr = emit_addr, bds_len = count;

	addtoline('\0');
	if (outpass && fbds)
		fprintf(fbds, "%04x %04x s %s", dollarsign, emit_addr, linebuf);

	// Avoid emit() because it has a small buffer and it'll spam the listing.
	bds_count = 0;
	while (count-- > 0) {
		if (outpass && fbds) {
			if (bds_count == 0)
				fprintf(fbds, "%04x %04x d ", dollarsign, emit_addr);
			fprintf(fbds, "%02x", value);
			bds_count++;
			if (bds_count == 16) {
				fprintf(fbds, "\n");
				bds_count = 0;
			}
		}

		if (segment == SEG_CODE)
			setmem(emit_addr, value, MEM_DATA);

		emit_addr++;
		emit_addr &= 0xffff;
		dollarsign++;
		dollarsign &= 0xffff;

		putbin(value);
		putrel(value);
		putout(value);
	}

	if (outpass && fbds) {
		if (bds_count)
			fprintf(fbds, "\n");

		bds_perm(start, bds_addr, bds_len);
	}

	// Do our own list() work as we emit bytes manually.

	if (outpass && iflist()) {
		lineout();

		if (nopt)
			fprintf(fout, "%4d:", linein[now_in]);

		if (copt)
		        fprintf(fout, nopt ? "%5s-" : "%4s-", "");

		if (nopt || copt)
			fprintf(fout, "\t");

		list_optarg(start, -1, 0);
		fprintf(fout, "..");
		list_optarg(dollarsign - 1, -1, 0);
		puthex(value, fout);
		putc('\t', fout);
		fputs(linebuf, fout);
		lsterr2(1);

	}
	else
		lsterr1();

	lineptr = linebuf;
}

#define OUTREC_SEG(rec)		outbuf[rec]
#define OUTREC_ADDR(rec)	((outbuf[rec + 1] << 8) | outbuf[rec + 2])
#define OUTREC_LEN(rec)		outbuf[rec + 3]
#define OUTREC_DATA(rec, len)	outbuf[rec + 4 + len]
#define OUTREC_SIZEOF	5

void new_outrec(void)
{
	OUTREC_LEN(outrec) = outlen;
	outrec += OUTREC_SIZEOF + outlen;

	outlen = 0;
	OUTREC_SEG(outrec) = segment;
	outbuf[outrec + 1] = seg_pos[segment] >> 8;
	outbuf[outrec + 2] = seg_pos[segment];
}

void putout(int value)
{
	int addr = (OUTREC_ADDR(outrec) + outlen) & 0xffff;
	if (OUTREC_SEG(outrec) != segment || addr != seg_pos[segment])
		new_outrec();

	if (pass2 && OUTREC_DATA(outrec, outlen) != value && !passfail) {
		char segname[2];
		if (relopt)
			sprintf(segname, "%c", SEGCHAR(segment));
		else
			segname[0] = '\0';

		sprintf(detail, "%s error - $%04x%s changed from $%02x to $%02x",
			errname[pflag], addr, segname, OUTREC_DATA(outrec, outlen), value);
		errwarn(pflag, detail);

		if (!outpass)
			passretry = 1;
	}
	OUTREC_DATA(outrec, outlen) = value;
	outlen++;

	if (outlen >= 255)
		new_outrec();

	advance_segment(1);
}

struct bookmark {
	int len, rec, tstates, linenum, listed;
	char *line;
}
bookstack[MAXEXP];

int mark;

void bookmark(int delay)
{
	struct bookmark *book;

	if (!outpass)
		return;

	if (mark >= MAXEXP) {
		error("Macro expansion level too deep");
		return;
	}

	book = bookstack + mark++;

	book->len = outlen;
	book->rec = outrec;
	book->tstates = tstates;
	book->linenum = linein[now_in];
	book->line = strdup(linebuf);
	book->listed = !delay;
}

int book_row, book_col, book_addr, book_seg;

void booklist(int seg, int addr, int data, struct bookmark *book)
{
	// Don't list beyond the first 4 bytes if told not to.
	if (!gopt && book_row > 0)
		return;

	if (book_addr == -1) {
		list_optarg(addr, seg, ' ');
		book_seg = seg;
		book_addr = addr;
		book_col = 0;
	}

	if (seg != book_seg || addr != book_addr || book_col < 0 || book_col == 4) {
		if (book_row == 0 && book->line) {
			fprintf(fout, "\t%s", book->line);
			free(book->line);
			book->line = 0;
		}
		else
			fputs("\n", fout);

		lineout();

		book_row++;
		book_col = 0;

		if (!gopt && !book->line)
			return;

		if (nopt) putc('\t', fout);
		if (copt) fputs("        ", fout);

		if (seg != book_seg || addr != book_addr)
			list_optarg(addr, seg, ' ');
		else
			fputs("      ", fout);

		book_seg = seg;
		book_addr = addr;
	}

	if (!gopt && book_row > 0)
		return;

	puthex(data, fout);
	book_addr++;
	book_col++;
}

void listfrombookmark()
{
	int t, n;
	struct bookmark *book;

	if (!outpass)
		return;

	if (mark < 1) {
		//error("Internal delayed listing underflow.");
		fprintf(stderr, "Internal delayed listing underflow at %d.", mark);
		return;
	}

	book = bookstack + --mark;

	// No delayed listing required?  bookstack clean is all we need.
	if (book->listed) {
		if (book->line) {
			free(book->line);
			book->line = 0;
		}
		return;
	}

	book->listed = 1;

	t = tstates - book->tstates;
	lineout(); // call before every output line

	if (nopt)
		fprintf(fout, "%4d:", book->linenum);

	if (copt) {
		if (t) {
			fprintf(fout, nopt ? "%5d+%d" : "%4d+%d", book->tstates, t);
		}
		else {
			fprintf(fout, nopt ? "%5s-" : "%4s-", "");
		}
	}

	if (nopt || copt)
		fprintf(fout, "\t");

	book_row = 0;
	book_col = -1;
	book_addr = -1;
	n = 0;
	while (book->rec <= outrec) {
		int len = book->rec == outrec ? outlen : OUTREC_LEN(book->rec);
		int addr = OUTREC_ADDR(book->rec) + book->len;
		int seg = OUTREC_SEG(book->rec);

		for (; book->len < len; book->len++) {
			booklist(seg, addr++, OUTREC_DATA(book->rec, book->len), book);
			n++;
		}

		book->len = 0;
		book->rec += OUTREC_SIZEOF + len;
	}

	if (book->line) {
		// pad with spaces up to 4 total hex bytes
		for (; n < 4; n++)
			fprintf(fout, "  ");

		fprintf(fout, "\t%s", book->line);
		free(book->line);
		book->line = 0;
	}
	else if (gopt)
		fputs("\n", fout);
}

void advance_segment(int step)
{
	int top = seg_pos[segment] += step;
	seg_pos[segment] &= 0xffff;
	if (top >= 0x10000)
		top = 0xffff;

	if (top > seg_size[segment]) {
		seg_size[segment] = top;
		if (segment == SEG_COMMON && cur_common)
			cur_common->i_value = top;
	}
}

void expr_reloc_check(struct expr *ex)
{
	if (!relopt) return;
	if (ex->e_scope & (SCOPE_EXTERNAL | SCOPE_NORELOC))
		err[rflag]++;
}

void expr_number_check(struct expr *ex)
{
	if (!relopt) return;
	expr_reloc_check(ex);
	if (ex->e_scope & SCOPE_SEGMASK)
		err[rflag]++;
}

void expr_scope_same(struct expr *ex1, struct expr *ex2)
{
	if (!relopt) return;
	if ((ex1->e_scope & SCOPE_SEGMASK) != (ex2->e_scope & SCOPE_SEGMASK))
		err[rflag]++;
}

void expr_word_check(struct expr *ex)
{
	if (ex->e_value < -32768 || ex->e_value > 65535) {
		err[vflag]++;
	}
}

int is_number(struct expr *ex)
{
	return ex && (ex->e_scope & ~SCOPE_PUBLIC) == 0;
}

int is_external(struct expr *ex)
{
	return ex && (ex->e_scope & SCOPE_EXTERNAL) && !ex->e_left && !ex->e_right &&
		ex->e_item;
}

struct expr *expr_alloc(void)
{
	struct expr *ex = malloc(sizeof *ex);

	ex->e_value = 0;
	ex->e_scope = 0;
	ex->e_token = 0;
	ex->e_item = 0;
	ex->e_left = 0;
	ex->e_right = 0;
	ex->e_parenthesized = 0;

	return ex;
}

struct expr *expr_var(struct item *it)
{
	struct expr *ex = expr_alloc();

	ex->e_token = 'v';
	ex->e_item = it;
	ex->e_scope = it->i_scope;
	ex->e_value = it->i_value;

	return ex;
}

struct expr *expr_num(int value)
{
	struct expr *ex = expr_alloc();
	ex->e_value = value;
	ex->e_token = '0';

	return ex;
}

// Build expression and update value based on the operator.
// Could be done inline in the grammar but there is a fair bit of
// repetition and MRAS operators have only made that worse.

struct expr *expr_mk(struct expr *left, int op, struct expr *right)
{
	struct expr *ex;
	int val = 0;
	int sc = 0;

	switch (op) {
	case '+':
		ex = expr_op(left, '+', right, left->e_value + right->e_value);

		// Can't operate on external labels.
		// But we can add constants to any scope.
		if (!((left->e_scope | right->e_scope) & SCOPE_EXTERNAL) &&
			((left->e_scope & SCOPE_SEGMASK) == 0 ||
			(right->e_scope & SCOPE_SEGMASK) == 0))
		{
			ex->e_scope &= ~(SCOPE_NORELOC | SCOPE_SEGMASK);
			ex->e_scope |= (left->e_scope | right->e_scope) & SCOPE_SEGMASK;
		}
		return ex;
	case '-':
		ex = expr_op_sc(left, '-', right, left->e_value - right->e_value);

		// But we can subtract a constant.
		if (!((left->e_scope | right->e_scope) & SCOPE_EXTERNAL) &&
			((right->e_scope & SCOPE_SEGMASK) == 0))
		{
			ex->e_scope &= ~(SCOPE_NORELOC | SCOPE_SEGMASK);
			ex->e_scope |= (left->e_scope & SCOPE_SEGMASK);
		}
		return ex;
	case '/':
		if (!(right->e_scope & SCOPE_EXTERNAL)) {
			if (right->e_value == 0)
				err[eflag]++;
			else
				val = left->e_value / right->e_value;
		}
		break;
	case '*':
		val = left->e_value * right->e_value;
		break;
	case '%':
		if (!(right->e_scope & SCOPE_EXTERNAL)) {
			if (right->e_value == 0)
				err[eflag]++;
			else
				val = left->e_value % right->e_value;
		}
		break;
	case '&':
		val = left->e_value & right->e_value;
		break;
	case '|':
		val = left->e_value | right->e_value;
		break;
	case '^':
		val = left->e_value ^ right->e_value;
		break;
	case SHL:
		val = left->e_value << right->e_value;
		break;
	case SHR:
		val = right->e_value == 0 ? left->e_value : ((left->e_value >> 1) & ((0x7fff << 16) | 0xffff)) >> (right->e_value - 1);
		break;
	case '<':
		val = (left->e_value < right->e_value) * trueval;
		sc = 1;
		break;
	case '=':
		val = (left->e_value == right->e_value) * trueval;
		sc = 1;
		break;
	case '>':
		val = (left->e_value > right->e_value) * trueval;
		sc = 1;
		break;
	case LE:
		val = (left->e_value <= right->e_value) * trueval;
		sc = 1;
		break;
	case NE:
		val = (left->e_value != right->e_value) * trueval;
		sc = 1;
		break;
	case GE:
		val = (left->e_value >= right->e_value) * trueval;
		sc = 1;
		break;
	case ANDAND:
		val = (left->e_value && right->e_value) * trueval;
		break;
	case OROR:
		val = (left->e_value || right->e_value) * trueval;
		break;
	default:
		fprintf(stderr, "internal expression evaluation error!\n");
		clean_outf();
		exit(-1);
		break;
	}

	if (sc)
		return expr_op_sc(left, op, right, val);

	return expr_op(left, op, right, val);
}


// Expression consruction for operators that subtract/compare.
// They produce a valid result if operating on numbers in the same segment.
struct expr *expr_op_sc(struct expr *left, int token, struct expr *right, int value)
{
	struct expr *ex = expr_op(left, token, right, value);

	if (!(ex->e_scope & SCOPE_EXTERNAL) &&
		((left->e_scope ^ right->e_scope) & SCOPE_SEGMASK) == 0)
	{
		// Result relocatable and a simple number
		ex->e_scope &= ~(SCOPE_NORELOC | SCOPE_SEGMASK);
	}

	return ex;
}

struct expr *expr_op(struct expr *left, int token, struct expr *right, int value)
{
	struct expr *ex = expr_alloc();

	ex->e_value = value;
	ex->e_token = token;
	ex->e_left = left;
	ex->e_right = right;

	// Combining two numbers will be fine as long as they're not
	// flagged as external or already not relocatable.  In which case
	// it is up to the particular operator to allow the value
	// to become valid.

	ex->e_scope = left->e_scope;
	if (left->e_scope & SCOPE_SEGMASK)
		ex->e_scope |= SCOPE_NORELOC;
	if (right) {
		ex->e_scope |= right->e_scope;
		if (right->e_scope & SCOPE_SEGMASK)
			ex->e_scope |= SCOPE_NORELOC;
	}

	return ex;
}

void expr_free(struct expr *ex)
{
	if (!ex)
		return;

	expr_free(ex->e_left);
	expr_free(ex->e_right);
	free(ex);
}

int synth_op(struct expr *ex, int gen)
{
	if (!is_number(ex->e_right))
		return 0;

	switch (ex->e_token) {
	case '&':
		if (ex->e_right->e_value == 255) {
			if (gen) {
				extend_link(ex->e_left);
				putrelop(RELOP_LOW);
				return 1;
			}
			return can_extend_link(ex->e_left);
		}
		break;
	case SHR:
		if (ex->e_right->e_value <= 15) {
			if (gen) {
				extend_link(ex->e_left);
				extend_link(expr_num(1 << ex->e_right->e_value));
				putrelop(RELOP_DIV);
			}
			return can_extend_link(ex->e_left);
		}
		break;
	case SHL:
		if (ex->e_right->e_value <= 15) {
			if (gen) {
				extend_link(ex->e_left);
				extend_link(expr_num(1 << ex->e_right->e_value));
				putrelop(RELOP_MUL);
			}
			return can_extend_link(ex->e_left);
		}
		break;
	default:
		break;
	}

	return 0;
}

int link_op(struct expr *ex)
{
	if (!ex)
		return 0;

	switch (ex->e_token) {
	case HIGH: return RELOP_HIGH;
	case LOW: return RELOP_LOW;
	case '~': return RELOP_NOT;
	case '-': return !ex->e_right ? RELOP_NEG : RELOP_SUB;
	case '+': return RELOP_ADD;
	case '*': return RELOP_MUL;
	case '/': return RELOP_DIV;
	case '%': return RELOP_MOD;
	default: return 0;
	}
}

int can_extend_link(struct expr *ex)
{
	if (!ex)
		return 1;

	// If we have a value available then we're good.
	if (!(ex->e_scope & SCOPE_NORELOC)) {
		//printf("HEY!\n");
		//if (ex->e_item && ex->e_item->i_string)
		//	printf("ext link says OK for '%s'\n", ex->e_item->i_string);
		return 1;
	}

	// Might be able to synthesize the operation.
	if (synth_op(ex, 0))
		return 1;

	// Otherwise, the operator must be supported and the children
	// must be linkable.

	return link_op(ex) && can_extend_link(ex->e_left) && can_extend_link(ex->e_right);
}

void extend_link(struct expr *ex)
{
	int op;

	if (!ex)
		return;

	if (synth_op(ex, 1))
		return;

	extend_link(ex->e_left);
	extend_link(ex->e_right);

	op = link_op(ex);
	if (op) {
		putrelop(op);
		return;
	}

	putrelcmd(RELCMD_EXTLINK);

	if (is_external(ex)) {
		char *str = ex->e_item->i_string;
		int len = strlen(str);

		if (len > 6)
			len = 6;

		putrelbits(3, 1 + len);
		putrelbits(8, 'B');
		while (len-- > 0) {
			int ch = *str++;
			if (ch >= 'a' && ch <= 'z')
				ch -= 'a' - 'A';
			putrelbits(8, ch);
		}
	}
	else {
		putrelbits(3, 4);
		putrelbits(8, 'C');
		putrelbits(8, ex->e_scope & SCOPE_SEGMASK);
		putrelbits(8, ex->e_value);
		putrelbits(8, ex->e_value >> 8);
	}
}

void putrelop(int op)
{
	putrelcmd(RELCMD_EXTLINK);

	putrelbits(3, 2);
	putrelbits(8, 'A');
	putrelbits(8, op);
}

void write_tap_block(int type, int len, unsigned char *data)
{
	int i, parity;

	fputc((len + 2) & 0xff, ftap);
	fputc((len + 2) >> 8, ftap);

	fputc(type, ftap);
	parity = type;
	for (i = 0; i < len; i++) {
		fputc(data[i], ftap);
		parity ^= data[i];
	}
	fputc(parity, ftap);
}

// One supposes .tap files could load multiple blocks into memory.
// However, doesn't seem to be a lot of point and we'd have to write
// extra loader code to make it happen.  For now we just load the
// assembled data as one contiguous block with the holes getting
// filled with zeros.

void write_tap(int len, int org, unsigned char *data)
{
	unsigned char block[32], *p, orglo, orghi;
	unsigned char basic_loader[] = {
		239, 34, 34, 175, 58, 249, 192, 176, // LOAD ""CODE:RANDOMIZE USR VAL
		'"', '2', '3', '2', '9', '6', '"', 13 // aka 0x5b00 - start of RAM
	};
	int entry = org;

	if (xeq_flag)
		entry = xeq;

	// .tap file output borrowed heavily from skoolkit's bin2tap.py
	// It loads a short basic program which auto-executes and loads
	// a short machine-language loader that reads the block of code
	// and jumps to it.
	// Constrast this with pasmo which doesn't have the short machine
	// code loader but uses a native code block.  I think that means
	// it can only execute at the beginning of the loaded data.

	p = block;

	*p++ = 0; // Program block
	casname((char *)p, sourcef, 10);
	p += 10;
	*p++ = 4 + sizeof basic_loader; *p++ = 0; // length of BASIC program
	*p++ = 10; *p++ = 0; // run line 10 after loading
	*p++ = 4 + sizeof basic_loader; *p++ = 0; // length of BASIC program

	write_tap_block(0, p - block, block);

	p = block;
	*p++ = 0; *p++ = 10; // line 10
	*p++ = sizeof(basic_loader); *p++ = 0;
	memcpy(p, basic_loader, sizeof basic_loader);
	p += sizeof basic_loader;
	write_tap_block(0xff, p - block, block);

	p = block;

	*p++ = 3; // Code block
	casname((char *)p, sourcef, 10);
	p += 10;
	*p++ = 19; *p++ = 0; // length of loader program
	*p++ = 0; *p++ = 0x5b; // 0x5b00 == 23296 - start of RAM
	*p++ = 0; *p++ = 0; // ?

	write_tap_block(0, p - block, block);

	p = block;

	orglo = org & 0xff;
	orghi = org >> 8;
	/* LD IX,org   */ *p++ = 0xdd; *p++ = 0x21; *p++ = orglo; *p++ = orghi;
	/* LD DE,len   */ *p++ = 0x11; *p++ = len & 0xff; *p++ = len >> 8;
	/* SCF         */ *p++ = 0x37;
	/* SBC A,A     */ *p++ = 0x9f;
	/* LD SP,org   */ *p++ = 0x31; *p++ = orglo; *p++ = orghi;
	/* LD BC,entry */ *p++ = 0x01; *p++ = entry & 0xff; *p++ = entry >> 8;
	/* PUSH BC     */ *p++ = 0xc5;
	/* JP $556     */ *p++ = 0xc3; *p++ = 0x56; *p++ = 0x05;

	write_tap_block(0xff, p - block, block);

	write_tap_block(0xff, len, data);
}

#define WORD(w) (w) & 255, (w) >> 8

void write_250(int low, int high)
{
	int load = low;
	int len = high - low + 1;
	int last;
	int chk;

	if (len <= 0) {
		// Nothing to output.  So we'll just delete the output file.
		int i;
		for (i = 0; i < CNT_OUTF; i++) {
			if (*outf[i].fpp && (*outf[i].fpp == ftcas || *outf[i].fpp == f250wav)) {
				fclose(*outf[i].fpp);
				*outf[i].fpp = NULL;
				unlink(outf[i].filename);
				if (outf[i].wanted)
					fprintf(stderr, "Warning: %s not output -- no code or data\n", outf[i].filename);
			}
		}
		return;
	}

	if (xeq_flag) {
		// Only add relocation if they don't already put their
		// execution address in to $41FE.  This means programs will
		// be unchanged if they seem to be aware of the structure.

		if (low > 0x41FE || high < 0x41FF ||
			memory[0x41FE] != (xeq & 0xff) ||
			memory[0x41FF] != xeq >> 8)
		{
			if (low >= 0x4200 && low <= 0x4200 + 14) {
				// A little too high.  More efficient to
				// just load a bit extra.  Plus we can't
				// easily fit in the "copy up" code.
				low = 0x41FE;
				memory[0x41FE] = xeq;
				memory[0x41FF] = xeq >> 8;
				load = low;
			}
			else if (low < 0x4200) {
				// Moving down.
				int src = 0x4200;
				int dst = low;
				unsigned char relo[] = {
					0x21, WORD(src),	// LD HL,nn
					0x11, WORD(dst),	// LD DE,nn
					0x01, WORD(len),	// LD BC,len
					0xED, 0xB0,		// LDIR
					0xC3, WORD(xeq)		// JP nn
				};
				high++;
				low -= 2;
				memory[low] = src + len;
				memory[low + 1] = (src + len) >> 8;
				memcpy(memory + high, relo, sizeof relo);
				high += sizeof relo - 1;
				load = 0x41FE;
			}
			else {
				// Moving up
				int src = 0x41FE + 2 + 14 + len - 1;
				int dst = low + len - 1;
				unsigned char relo[] = {
					WORD(0x4200),
					0x21, WORD(src),	// LD HL,nn
					0x11, WORD(dst),	// LD DE,nn
					0x01, WORD(len),	// LD BC,len
					0xED, 0xB8,		// LDDR
					0xC3, WORD(xeq)		// JP nn
				};
				low -= sizeof relo;
				memcpy(memory + low, relo, sizeof relo);
				load = 0x41FE;
			}
		}
	}

	len = high + 1 - low;
	last = load + len;
	// Yeah, it is big endian.
	fprintf(ftcas, "%c%c%c%c", load >> 8, load, last >> 8, last);
	fwrite(memory + low, len, 1, ftcas);
	chk = 0;
	for (i = 0; i < len; i++)
		chk += memory[low + i];
	fprintf(ftcas, "%c", -chk);
}

int bitgetbuf;
int bitgetcnt;

void bitget_rewind(FILE *fp)
{
	bitgetcnt = 0;
	fseek(fp, 0, SEEK_SET);
}

int bitget(FILE *fp)
{
	int bit;

	if (bitgetcnt == 0) {
		bitgetbuf = fgetc(fp);
		bitgetcnt = 8;
	}

	bit = !!(bitgetbuf & 0x80);
	bitgetbuf <<= 1;
	bitgetcnt--;

	return bit;
}

void writewavs(int pad250, int pad500, int pad1500)
{
	FILE *cas[] = { ftcas, flcas, flnwcas, fcas };
	FILE *wav[] = { f250wav, f500wav, f1000wav, f1500wav };
	int padbytes[] = { pad250, pad500, pad500, pad1500 };
#define	NFMT (sizeof padbytes / sizeof padbytes[0])
	int bits[NFMT];
	int i, j, k, m;
	unsigned char pulse[][2][13] = {
		{ { 2, 0xff, 2, 0, 42 - 4, 0x80, 0 },
		  { 2, 0xff, 2, 0, 17, 0x80, 2, 0xff, 2, 0, 17, 0x80, 0 } },

		{ { 3, 0xff, 3, 0, 44 - 6, 0x80, 0 },
		  { 3, 0xff, 3, 0, 16, 0x80, 3, 0xff, 3, 0, 16, 0x80, 0 } },

		{ { 3, 0xff, 3, 0, 44 - 6, 0x80, 0 },
		  { 3, 0xff, 3, 0, 16, 0x80, 3, 0xff, 3, 0, 16, 0x80, 0 } },

		{ { 8, 0, 8, 0xff, 0 },
		  { 4, 0, 4, 0xff, 0 } }
	};
	int hz[] = { 11025, 22050, 44100, 22050 };
	int pulse_len[NFMT][2];

	for (i = 0; i < NFMT; i++) {
		for (j = 0; j < 2; j++) {
			pulse_len[i][j] = 0;
			for (k = 0; pulse[i][j][k]; k += 2)
				pulse_len[i][j] += pulse[i][j][k];
		}
	}

	for (i = 0; i < NFMT; i++) {
		if (!cas[i] || !wav[i])
			continue;

		bits[i] = (ftell(cas[i]) - padbytes[i]) * 8;
		if (i == 2 && casbitcnt > 0)
			bits[i] -= 8 - casbitcnt;
	}

	for (i = 0; i < NFMT; i++) {
		int headPad = 10, tailPad = hz[i] / 2;
		int audio_bytes = headPad;
		unsigned char hzlo = hz[i] & 0xff;
		unsigned char hzhi = hz[i] >> 8;

		unsigned char waveHeader[] = {
			'R', 'I', 'F', 'F',
			0, 0, 0, 0,
			'W', 'A', 'V', 'E', 'f', 'm', 't', ' ',
			16, 0, 0, 0, // wav information length
			1, 0, // PCM
			1, 0, // Single channel
			hzlo, hzhi, 0, 0, // samples/second
			hzlo, hzhi, 0, 0, // average bytes/second
			1, 0, // block alignment
			8, 0, // bits/sample
			'd', 'a', 't', 'a',
			0, 0, 0, 0
		};
		int waveHeaderSize = sizeof waveHeader;

		if (!cas[i] || !wav[i])
			continue;

		bitget_rewind(cas[i]);
		for (j = 0; j < bits[i]; j++)
			audio_bytes += pulse_len[i][bitget(cas[i])];

		audio_bytes += tailPad;

		waveHeader[waveHeaderSize - 4] = audio_bytes;
		waveHeader[waveHeaderSize - 3] = audio_bytes >> 8;
		waveHeader[waveHeaderSize - 2] = audio_bytes >> 16;
		waveHeader[waveHeaderSize - 1] = audio_bytes >> 24;

		waveHeader[4] = (audio_bytes + 36);
		waveHeader[5] = (audio_bytes + 36) >> 8;
		waveHeader[6] = (audio_bytes + 36) >> 16;
		waveHeader[7] = (audio_bytes + 36) >> 24;

		bitget_rewind(cas[i]);

		fwrite(waveHeader, waveHeaderSize, 1, wav[i]);

		for (j = 0; j < headPad; j++)
			fputc(0x80, wav[i]);

		bitget_rewind(cas[i]);
		for (j = 0; j < bits[i]; j++) {
			int bit = bitget(cas[i]);
			for (k = 0; pulse[i][bit][k]; k += 2)
				for (m = 0; m < pulse[i][bit][k]; m++)
					fputc(pulse[i][bit][k + 1], wav[i]);
		}

		for (j = 0; j < tailPad; j++)
			fputc(0x80, wav[i]);
	}
}

int sized_byteswap(int value)
{
	int swapped = 0;

	for (; value; value = (value >> 8) & 0xffffff) {
		swapped <<= 8;
		swapped |= value & 0xff;
	}

	return swapped;
}

// Tracking whether a file has been imported.

struct import_list {
	struct import_list *next;
	char *filename;
	int imported;
} *imports;

void reset_import()
{
	struct import_list *il;
	for (il = imports; il; il = il->next)
		il->imported = 0;
}

// Returns 1 if filename has been imported.  Marks it as imported.
// Only uses the base name.

int imported(char *filename)
{
	struct import_list *il;
	int ret;
	char *p;

	for (p = filename; *p; p++)
		if (*p == '/' || *p == '\\')
			filename = p + 1;

	for (il = imports; il; il = il->next) {
		if (strcmp(filename, il->filename) == 0)
			break;
	}

	if (!il) {
		il = malloc(sizeof *il);
		il->filename = strdup(filename);
		il->imported = 0;
		il->next = imports;
		imports = il;
	}

	ret = il->imported;
	il->imported = 1;
	return ret;
}
