// Copyright(c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

#ifndef IO_H
#define IO_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

static inline unsigned char port_byte_in(unsigned short port){
	unsigned char result;
	__asm__ volatile (
	          ".intel_syntax noprefix \n\t" 
	          "in al, dx\n\t"
	          ".att_syntax"
					 :"=a"(result)
					 :"d"(port)
					 );
	return result;
}

static inline void port_byte_out(unsigned short port, unsigned char data){
	__asm__ volatile (
	          ".intel_syntax noprefix \n\t" 
	          "out dx, al \n\t"
	          ".att_syntax"
					 :
					 :"a"(data), "d"(port)
					 );
}

static inline unsigned short port_word_in(unsigned short port){
	unsigned short result;
	__asm__ volatile (
	          ".intel_syntax noprefix \n\t" 
	          "in ax, dx\n\t"
	          ".att_syntax"
					 :"=a"(result)
					 :"d"(port)
					 );
	return result;
}

static inline void port_word_out(unsigned short port, unsigned short data){
	__asm__ volatile (
	          ".intel_syntax noprefix \n\t" 
	          "out dx, ax\n\t"
	          ".att_syntax"
					 :
					 :"a"(data), "d"(port)
					 );
}

#endif // IO_H
