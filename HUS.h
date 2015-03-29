*/ TS-Labs Hummer Ultra Sound card interface header /*
*/   Amixgris (Red Triangle). Russia. 29.03.2015    /*

#define HUS_BASE	0x0000
__sfr __banked __at   HUS_BASE + 0 HUS_CPU_Control;
__sfr __banked __at   HUS_BASE + 1 HUS_Control;
__sfr __banked __at   HUS_BASE + 2 HUS_TickCntLo;
__sfr __banked __at   HUS_BASE + 3 HUS_TickCntHi;
__sfr __banked __at   HUS_BASE + 4 HUS_AudioTmrLo;
__sfr __banked __at   HUS_BASE + 5 HUS_AudioTmrHi;
__sfr __banked __at   HUS_BASE + 6 HUS_ChanNum;
__sfr __banked __at   HUS_BASE + 7 HUS_SampVol;

#define HUS_BW8			0
#define HUS_BW16		1 << 7
#define HUS_MONO		0
#define HUS_STEREO		1 << 6
#define HUS_UNSIGN		0
#define HUS_SIGNED		1 << 5
#define HUS_NORETRIG	0	   // ?
#define HUS_RETRIGGED	1 << 2 // ?
#define HUS_CH_OFF		0x00
#define HUS_CH_NOLOOP	0x01
#define HUS_CH_FWDLOOP	0x02
#define HUS_CH_BIDILOOP	0x03

typedef struct {
	unsigned char StartAddrLo;
	unsigned char Control;
	unsigned char StartAddrMi;
	unsigned char StartAddrHi;
	unsigned char EndAddrMi;
	unsigned char EndAddrLo;
	unsigned char LoopAddrLo;
	unsigned char EndAddrHi;
	unsigned char LoopAddrHi;
	unsigned char LoopAddrMi;
	unsigned char FractStepHi;
	unsigned char FractStepLo;
	unsigned char VolumeR;	
	unsigned char VolumeL;		
}	HUS_CHCMD;

void chcmd_StartAddr(HUS_CHCMD* chan, unsigned char* sample, unsigned char page) __naked
{	chan, sample, page;
__asm
	pop	af
	pop	hl	; struct ptr
	pop	de	; sample addr
	pop bc	; sample page in C
	push bc
	push de
	push hl
	push af
	ld (hl),e
	inc hl
	inc hl
	ld (hl),d
	inc hl
	ld	a,c
	and a,#0x3f
	ld (hl),a
	ret
__endasm;
}
void chcmd_EndtAddr(HUS_CHCMD* chan, unsigned char* sample, unsigned char page) __naked
{	chan, sample, page;
__asm
	pop	af
	pop	hl	; struct ptr
	pop	de	; sample addr
	pop bc	; sample page in C
	push bc
	push de
	push hl
	push af
	inc hl
	inc hl
	inc hl
	inc hl
	ld (hl),d
	inc hl
	ld (hl),e
	inc hl
	inc hl
	ld	a,c
	and a,#0x3f
	ld (hl),a
	ret
__endasm;
}
void chcmd_LoopAddr(HUS_CHCMD* chan, unsigned char* sample, unsigned char page) __naked
{	chan, sample, page;
__asm
	pop	af
	pop	hl	; struct ptr
	pop	de	; sample addr
	pop bc	; sample page in C
	push bc
	push de
	push hl
	push af

	ld a,#6
	add a,l
	ld l,a
	jr	nc,1$
	inc h
1$:	
	ld (hl),e
	inc hl
	inc hl
	ld	a,c
	and a,#0x3f
	ld (hl),a
	inc hl
	inc hl
	ld (hl),d
	ret
__endasm;
}
void chcmd_Control(HUS_CHCMD* chan, unsigned char cmd) __naked
{	chan, cmd;
__asm
	pop	af
	pop	hl	; struct ptr
	pop bc	; cmd in C
	push bc
	push hl
	push af
	inc hl
	ld a,c
	and a,#0xE7
	ld (hl),a
	ret
__endasm;
}
void chcmd_FractStep(HUS_CHCMD* chan, unsigned int step) __naked
{	chan, step;
__asm
	pop	af
	pop	hl	; struct ptr
	pop bc	; fract step
	push bc
	push hl
	push af
	ld de,#10
	add hl,de
	ld (hl),c
	inc hl
	ld (hl),b
	ret
__endasm;
}
void chcmd_VolRL(HUS_CHCMD* chan, unsigned char VL, unsigned char VR) __naked
{	chan, VL, VR;
__asm
	pop	af
	pop	hl	; struct ptr
	pop bc	; VL / VR
	push bc
	push hl
	push af
	ld de,#12
	add hl,de
	ld (hl),b
	inc hl
	ld (hl),c
	ret
__endasm;
}
void chcmd_VolR(HUS_CHCMD* chan, unsigned char VR) __naked
{	chan, VR;
__asm
	pop	af
	pop	hl	; struct ptr
	pop bc	; VR in C
	push bc
	push hl
	push af
	ld de,#12
	add hl,de
	ld (hl),c
	ret
__endasm;
}
void chcmd_VolL(HUS_CHCMD* chan, unsigned char VL) __naked
{	chan, VL;
__asm
	pop	af
	pop	hl	; struct ptr
	pop bc	; VL in C
	push bc
	push hl
	push af
	ld de,#13
	add hl,de
	ld (hl),c
	ret
__endasm;
}
void chcmd_SEND(HUS_CHCMD* table, HUS_CHCMD* dest) __naked
{	table, dest;
__asm
	pop	af
	pop	hl	; struct ptr
	pop de	; dest
	push de
	push hl
	push af
	ld bc,#0x200
	ldir
	ret
__endasm;
}
unsigned int Get_HUS_TickCnt(void) __naked
{
__asm
	ld bc,#_HUS_BASE+2
	in	l,(c)
	inc bc
	in	h,(c)
	ret
__endasm;
}
void Send_HUS_TickCnt(unsigned int ticks) __naked
{ ticks;
__asm
	pop af
	pop hl
	push hl
	push af
	ld bc,#_HUS_BASE+2
	out (c),l
	inc bc
	out (c),h
	ret
__endasm;
}
unsigned int Get_HUS_AudioTmr(void) __naked
{
__asm
	ld bc,#_HUS_BASE+4
	in	l,(c)
	inc bc
	in	h,(c)
	ret
__endasm;
}
void Send_HUS_TickCnt(unsigned int time) __naked
{ time;
__asm
	pop af
	pop hl
	push hl
	push af
	ld bc,#_HUS_BASE+4
	out (c),l
	inc bc
	out (c),h
	ret
__endasm;
}