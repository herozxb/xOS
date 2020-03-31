#define SECTSIZE 512
#define TMP_USER_ADDR 0x1000

#include "string.h"
#include "ide.h"
#include "stdio.h"
#include "flpydsk.h"


static inline void enable () {
	__asm__ volatile ("sti\n\r");
}

// disable all hardware interrupts
static inline void disable () {
	__asm__ volatile ("cli\n\r":::"memory");
}



void insw(uint32_t port, uintptr_t addr, int cnt) {
    __asm__ volatile (
    	".intel_syntax noprefix \n\t"
    	"mov di,ax\n\t" // destination
    	"rep insw\n\t" // repeat input string
      ".att_syntax"
    	:
    	:"a"(addr),"d"(port),"c"(cnt)
    	);
}

void outsw(uint32_t port, uintptr_t addr, int cnt) {
    __asm__ volatile (
    	".intel_syntax noprefix \n\t"
    	"mov esi, eax\n\t" // source
    	"rep outsw\n\t" // repeat output string
    	".att_syntax"
    	:
    	:"a"(addr),"d"(port),"c"(cnt)
    	);
}

/* waitdisk - wait for disk ready */
void waitdisk(void) {
	//while ((port_byte_in(0x1F7) & 0xC0) != 0x40)
        /* do nothing */;
}

/* readsect - read a single sector at @secno into @dst */
/*
void readsect(uintptr_t dst, uint32_t secno) {
  //printf("===========6.2.2.0============");

	disable(); // IMPORTANT!!!
  //printf("===========6.2.2.1============");
    port_byte_out(0x1F2, 1);                         // count = 1
    port_byte_out(0x1F3, secno & 0xFF);
    port_byte_out(0x1F4, (secno >> 8) & 0xFF);
    port_byte_out(0x1F5, (secno >> 16) & 0xFF);
    port_byte_out(0x1F6, ((secno >> 24) & 0xF) | 0xE0);
    port_byte_out(0x1F7, 0x20);                      // cmd 0x20 - read sectors
    // wait for disk to be ready
      //printf("===========6.2.2.2============");
    waitdisk();
  //printf("===========6.2.2.3============");
    // read a sector
    insw(0x1F0, dst, SECTSIZE / 2);
      //printf("===========6.2.2.4============");
}
*/
//            uintptr_t dst is the desternation address
void readsect(uintptr_t dst, uint32_t secno) {
	//printf("===========readsect()============");
	dst =(uintptr_t) flpydsk_read_sector(secno);
}


// read [@startsec,@startsec+@cnt] sectors to @addr
void read_sectors(uintptr_t addr, uint32_t startsec, uint32_t cnt)
{
	//printf("===========read_sectors()============");
	for (int i = 0; i < cnt; i++){
		//printf("===========6.2.1============");
		readsect(TMP_USER_ADDR + i * SECTSIZE, startsec + i);
		//printf("%d\n",i);
		memcpy((void*)addr, (const void*)TMP_USER_ADDR, cnt * SECTSIZE);
	}
		//printf("===========6.2.2============");
}

void writesect(uintptr_t src, uint32_t secno) {

	disable(); // IMPORTANT!!!
	waitdisk();

    port_byte_out(0x1F2, 1);                         // count = 1
    port_byte_out(0x1F3, secno & 0xFF);
    port_byte_out(0x1F4, (secno >> 8) & 0xFF);
    port_byte_out(0x1F5, (secno >> 16) & 0xFF);
    port_byte_out(0x1F6, ((secno >> 24) & 0xF) | 0xE0);
    port_byte_out(0x1F7, 0x30);                      // cmd 0x30 - write sectors
    // wait for disk to be ready
    waitdisk();

    // write a sector
    outsw(0x1F0, src, SECTSIZE / 2);
}

// write @addr to [@startsec,@startsec+@cnt]
void write_sectors(uintptr_t addr, uint32_t startsec, uint32_t cnt)
{
	for (int i = 0; i < cnt; i++)
		writesect(addr + i * SECTSIZE, startsec + i);
	enable();
}

void show_one_sector(uintptr_t addr)
{
	printf("Addr: %xh\n", addr);
	uint8_t* sector = (uint8_t*) addr;
	int i = 0;
	for (int c = 0; c < 4; c++ ) {
		for (int j = 0; j < 128; j++){
			printf ("%c", sector[ i + j ]);
			if (j % 2 == 1)
				printf(" ");
		}
		i += 128;
	}
	printf("\n");
}

void show_hex(uintptr_t addr, size_t cnt)
{
	printf("Addr: %xh\n", addr);
	uint8_t* sector = (uint8_t*) addr;
	for (int j = 0; j < cnt; j++){
		printf ("%x", sector[j]);
		if (j % 2 == 1)
			printf(" ");
	}
	printf("\n");
}

void read_disk_test(){
	printf("Please enter the sector number: ");
	uint8_t* sector = (uint8_t*)0x1000;
	int num = 0;
	scanf("%d",&num);
	printf("Reading sector %d...\n", num);
	read_sectors((uintptr_t)sector, num, 1);
	// display sector
	printf("Address: %x\n", (uintptr_t)sector);
	show_one_sector(0x1000);
}
