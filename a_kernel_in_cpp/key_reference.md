#1 INTERRUPT

#1.0.0 interrupt descriptor


// interrupt descriptor
struct IDT_entry {
  unsigned short int offset_lowerbits;
  unsigned short int selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short int offset_higherbits;
};


The Interrupt Descriptor Table (IDT) is specific to the IA-32 architecture. It is the Protected mode counterpart to the Real Mode Interrupt Vector Table (IVT) telling where the Interrupt Service Routines (ISR) are located (one per interrupt vector). It is similar to the Global Descriptor Table in structure. 

struct IDTDescr {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_2; // offset bits 16..31
};

The offset is a 32 bit value, split in two parts. It represents the address of the entry point of the ISR.

The selector is a 16 bit value and must point to a valid descriptor in your GDT.

The type_attr is specified here: 

  7                           0
+---+---+---+---+---+---+---+---+
| P |  DPL  | S |    GateType   |
+---+---+---+---+---+---+---+---+


#example


// initialize basic idt
int idt_init() {

	// null out the idt
	memset ((void*)&IDT[0], 0, sizeof(struct IDT_entry) * MAX_INTERRUPTS-1);

	// register default handlers
	for (int i = 0; i < MAX_INTERRUPTS; i++)
		setvect (i, (unsigned long)default_handler);

	// fill the IDT register
	unsigned long idt_address = (unsigned long)IDT; // unsigned long is 64 bits or 8 bytes long
	idt_ptr[0] = (sizeof(struct IDT_entry) * MAX_INTERRUPTS) + ((idt_address & 0xffff) << 16); //0xffff 16 bits long
	idt_ptr[1] = idt_address >> 16;

	load_idt(idt_ptr); // assembly
	
//	load_idt:
//	mov eax, [ esp + 4 ] ;esp is 32 bits long, 4 * 32 = 2 * 64 = two long
//	lidt [ eax ]        ; load interrupt description table (IDT)
//	sti                 ; turn on interrupts
//	ret


// 1bf:	e8 fc ff ff ff       	call   1c0 <idt_init+0x7e>
// 1c4:	83 c4 10             	add    $0x10,%esp  ;0x10 = 16 bits esp = 32 bits
// 1c7:	b8 00 00 00 00       	mov    $0x0,%eax


	return 0;
}


setvect (0, (unsigned long) divide_by_zero_fault);

// sets new interrupt vector
void setvect (int intno, uint64_t vect) {

  // install interrupt handler! This overwrites prev interrupt descriptor
  install_ir (intno, INTERRUPT_GATE, KERNEL_CODE_SEGMENT_OFFSET, vect);
}

int install_ir (uint32_t i, uint16_t type_attr, uint16_t selector, uint64_t irq) {

  if (i > MAX_INTERRUPTS)
    return 0;

  if (!irq)
    return 0;

  // get base address of interrupt handler
  uint64_t base = irq;

  // store base address into idt
  IDT[i].offset_lowerbits  = (uint16_t)(base & 0xffff);
  IDT[i].offset_higherbits = (uint16_t)((base >> 16) & 0xffff);
  IDT[i].zero              = 0;
  IDT[i].type_attr         = (uint8_t)(type_attr);
  IDT[i].selector          = selector;

  return	0;
}


    
    
#2 GLOBAL DESCRIPTOR TABLE 


struct gdt_descriptor {

	// bits 0-15 of segment limit
	uint16_t		limit;

	// bits 0-23 of base address
	uint16_t		baseLo;
	uint8_t			baseMid;

	/*
	 * descriptor access flags
	 * | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	 * | P |  DPL  | S |      TYPE     |
	 * P: Present in memory or not
	 * DPL(Descriptor Privilege Level): ring 0 - ring 3
	 * S: 1 - Data/Code descriptor | 0 - gate descriptor
	 * Type: When S = 1
	 *    3: Executable
	 *    2: Consistent
	 *    1: 1 - Read+Write | 0 - Only read
	 *    0: Accessed
	 */
	uint8_t			flags; // access

	/*
	 * grand
	 * | 7 |  6  | 5 |  4  | 3 | 2 | 1 | 0 |
	 * | G | D/B | 0 | AVL |   Limit high  |
	 * G: 0 - B | 1 - 4KB
	 */
	uint8_t			grand; // limit_high, flags

	// bits 24-32 of base address
	uint8_t			baseHi;
} __attribute__((packed));

//! initialize gdt
int gdt_init () {

	//! set up gdtr
	_gdtr.m_limit = (sizeof (struct gdt_descriptor) * MAX_DESCRIPTORS)-1;
	_gdtr.m_base = (uint32_t)&_gdt[0];

	//! set null descriptor
	gdt_set_descriptor(0, 0, 0, 0, 0);


	// set default code descriptor (0x08)
	gdt_set_descriptor (1,0,0xffffffff,
		GDT_DESC_READWRITE|GDT_DESC_EXEC_CODE|GDT_DESC_CODEDATA|GDT_DESC_MEMORY,
		GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMITHI_MASK);

	// set default data descriptor (0x10)
	gdt_set_descriptor (2,0,0xffffffff,
		GDT_DESC_READWRITE|GDT_DESC_CODEDATA|GDT_DESC_MEMORY,
		GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMITHI_MASK);

	// set default user mode code descriptor (0x18)
	gdt_set_descriptor (3,0,0xffffffff,
		GDT_DESC_READWRITE|GDT_DESC_EXEC_CODE|GDT_DESC_CODEDATA|GDT_DESC_MEMORY|GDT_DESC_DPL,
		GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMITHI_MASK);

	// set default user mode data descriptor (0x20)
	gdt_set_descriptor (4,0,0xffffffff,
		GDT_DESC_READWRITE|GDT_DESC_CODEDATA|GDT_DESC_MEMORY|GDT_DESC_DPL,
		GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMITHI_MASK);

	tss_init(5);
	load_gdt(); // assembly
	tss_install(5);
	
	return 0;
}


// processor gdtr register points to base of gdt. This helps
// us set up the pointer
struct gdtr {

	// size of gdt
	uint16_t		m_limit;

	// base address of gdt
	uint32_t		m_base;
} __attribute__((packed));




// gdtr data
struct gdtr				_gdtr; // used for assembly

[ global load_gdt ]
[ extern _gdtr ]

load_gdt:
	lgdt [ _gdtr ]
	mov ax, 0x10                ; kernel data selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:load_gdt_ret
load_gdt_ret:
	ret
	
#3 PORT

inb
outb

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
static inline unsigned char port_byte_in(unsigned short port){}
static inline void port_byte_out(unsigned short port, unsigned char data){}

/* write EOI */
port_byte_out(0x20, 0x20);

status = port_byte_in(KEYBOARD_STATUS_PORT);
/* Lowest bit of status will be set if buffer is not empty */
if (status & 0x01) {
	scancode = port_byte_in(KEYBOARD_DATA_PORT);
}

Command 0x20-0x3f: Read keyboard controller RAM

    The last six bits of the command specify the RAM address to read. The read data is placed into the output buffer, and can be read by reading port 0x60. On MCA systems, type 1 controllers can access all 32 locations; type 2 controllers can only access locations 0, 0x13-0x17, 0x1d, 0x1f. 

#4 DISK

#4.0.0 asm read disk, BIOS read


;************************************************;
; Reads a series of sectors
; CX=>Number of sectors to read
; AX=>Starting sector
; ES:EBX=>Buffer to read to
;************************************************;

ReadSectors:
     .MAIN
          mov     di, 0x0005                          ; five retries for error
     .SECTORLOOP
          push    ax
          push    bx
          push    cx
          call    LBACHS                              ; convert starting sector to CHS
          mov     ah, 0x02                            ; BIOS read sector
          mov     al, 0x01                            ; read one sector
          mov     ch, BYTE [absoluteTrack]            ; track
          mov     cl, BYTE [absoluteSector]           ; sector
          mov     dh, BYTE [absoluteHead]             ; head
          mov     dl, BYTE [bsDriveNumber]            ; drive
          int     0x13                                ; invoke BIOS
          jnc     .SUCCESS                            ; test for read error
          xor     ax, ax                              ; BIOS reset disk
          int     0x13                                ; invoke BIOS
          dec     di                                  ; decrement error counter
          pop     cx
          pop     bx
          pop     ax
          jnz     .SECTORLOOP                         ; attempt to read again
          int     0x18
     .SUCCESS
          pop     cx
          pop     bx
          pop     ax
          add     bx, WORD [bpbBytesPerSector]        ; queue next buffer
          inc     ax                                  ; queue next sector
          loop    .MAIN                               ; read next sector
          ret


#4.1.0 C read disk,  read file in C


//! read a sector
extern uint8_t* flpydsk_read_sector (int sectorLBA) {

	if (_CurrentDrive >= 4)
		return 0;

	//! convert LBA sector to CHS
	int head=0, track=0, sector=1;
	flpydsk_lba_to_chs (sectorLBA, &head, &track, &sector);

	//! turn motor on and seek to track
	flpydsk_control_motor (true);
	if (flpydsk_seek (track, head) != 0)
		return 0;

	//! read sector and turn motor off
	flpydsk_read_sector_imp (head, track, sector);
	flpydsk_control_motor (false);

	//! warning: this is a bit hackish
	return (uint8_t*) DMA_BUFFER;
}


//! read a sector
extern void flpydsk_read_sector_imp (uint8_t head, uint8_t track, uint8_t sector) {

	uint32_t st0, cyl;

	//! set the DMA for read transfer
	flpydsk_dma_read ();

	//! read in a sector
	flpydsk_send_command (
				FDC_CMD_READ_SECT | FDC_CMD_EXT_MULTITRACK | FDC_CMD_EXT_SKIP | FDC_CMD_EXT_DENSITY);
	flpydsk_send_command ( head << 2 | _CurrentDrive );
	flpydsk_send_command ( track);
	flpydsk_send_command ( head);
	flpydsk_send_command ( sector);
	flpydsk_send_command ( FLPYDSK_SECTOR_DTL_512 );
	flpydsk_send_command ( ( ( sector + 1 ) >= FLPY_SECTORS_PER_TRACK ) ? FLPY_SECTORS_PER_TRACK : sector + 1 );
	flpydsk_send_command ( FLPYDSK_GAP3_LENGTH_3_5 );
	flpydsk_send_command ( 0xff );

	//! wait for irq
	flpydsk_wait_irq ();

	//! read status info
	for (int j=0; j<7; j++)
		flpydsk_read_data ();

	//! let FDC know we handled interrupt
	flpydsk_check_int (&st0,&cyl);
}

#4.1.1 C read disk through PORT,  read file in C from PORT

//! send command byte to fdc
extern void flpydsk_send_command (uint8_t cmd) {

	//! wait until data register is ready. We send commands to the data register
	for (int i = 0; i < 500; i++ )
		if ( flpydsk_read_status () & FLPYDSK_MSR_MASK_DATAREG )
			return outportb (FLPYDSK_FIFO, cmd);
}
























