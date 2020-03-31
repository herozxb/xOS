#include "stdio.h"
#include "string.h"
#include "DebugDisplay.h"
#include "terminal.h"
#include "Hal.h"
#include "fat12a.h"
#include "fat12.h"
#include "user.h"


#define MAX_TERMINAL 4
#define VIDEO_SIZE 80 * 32 * 2

extern const char* HELLO_INFO ="\
           _____  _____\n\
          / __ \\/ ___/\n\
         / / / /\\__ \\ Happy Search Operating System, xOS \n\
     \\/ / /_/ /___/ / -------------------------------\n\
     /\\/\\____//____/  \n\n";
extern const char* HELP_INFO =
"xOS Shell version 1.0\n\
These shell commands are defined internally. Type 'help' to see this list.\n\
\n\
 help             -- Show this list\n\
 ls               -- Show existing files in current directory\n\
 cd [dir]         -- Change directory\n\
 cp [src] [dst]   -- Copy files\n\
 rm [file]        -- Delete files\n\
 tick             -- Show current execution time\n\
 exec [prg] [...] -- Execute the user program (support multiple programs)\n\
 clr              -- Clear the screen\n\
 exit             -- Exit OS\n";

terminal_t terminal_list[MAX_TERMINAL];

terminal_t *curr_terminal, *tmp_terminal;

int get_current_terminal()
{
	return curr_terminal->num;
}

void set_tmp_terminal(int i)
{
	tmp_terminal = &terminal_list[i];
}

void set_at_first_term()
{
	curr_terminal = tmp_terminal = &terminal_list[0];
	curr_terminal->num = 0;
	curr_terminal->cursor = 0;
}

uintptr_t get_terminal_buf_addr()
{
	if (curr_terminal->num == tmp_terminal->num)
		return VIDEO_ADDRESS;
	else
		return (uintptr_t)&tmp_terminal->buf;
}

void set_cursor(int offset){
	if (curr_terminal->num == tmp_terminal->num){
		offset /= 2; // Convert from cell offset to char offset
		// This is similar to get_cursor, only now we write
		// bytes to those internal device registers
		port_byte_out(REG_SCREEN_CTRL, 14);
		port_byte_out(REG_SCREEN_DATA, (unsigned char)((offset >> 8) & 0x00FF));
		port_byte_out(REG_SCREEN_CTRL, 15);
		port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0x00FF));
	} else
		tmp_terminal->cursor = offset;
}

int get_cursor() {
	if (curr_terminal->num == tmp_terminal->num){
		// The device uses its control register as an index
		// to select its internal registers, of which we are
		// interested in:
		// reg 14: which is the high byte of the cursor's offset
		// reg 15: which is the low byte of the cursor's offset
		// Once the internal register has been selected, we may read or
		// write a byte on the data register
		port_byte_out(REG_SCREEN_CTRL, 14);
		int offset = port_byte_in(REG_SCREEN_DATA) << 8;
		port_byte_out(REG_SCREEN_CTRL, 15);
		offset += port_byte_in(REG_SCREEN_DATA);
		// Since the cursor offset reported by the VGA hardware is the
		// number of characters, we multiply by two to convert it to
		// a character cell offset
		return offset * 2;
	} else
		return tmp_terminal->cursor;
}

void command_not_found(char* str)
{
	if (strcmp(str,"\n") == 0 || strlen(str) == 0)
		return;
	printf("%s: command not found\n",str);
}

void put_prompt()
{
	set_color(GREEN,BLACK);
	printf("root@xOS:");
	set_color(LIGHT_BLUE,BLACK);
	//printf("%s",curr_path);
	set_color(WHITE,BLACK);
	printf("+");
}

void terminal_loop()
{
	bool flag = true;
	while (1) {
		if (flag == true)
			flag = false;
		else
			put_prompt();
		char str[MAX_BUF_LEN];
		getline(str);
		char* rest = str;
		strsep(&rest," ");
		if (strcmp(str,"help") == 0)
			print(HELP_INFO);
		else if (strcmp(str,"exit") == 0)
			break;
		else if (strcmp(str,"clr") == 0){
		  printf("clear_screen\n");
			clear_screen();
		}
		else if (strcmp(str,"ls") == 0)
		{
		  printf("ls\n");
			//fat12_ls();
			const char* DirectoryName = "ls";
			fat12a_ls(DirectoryName);
		}
		else if (strcmp(str,"cd") == 0){
		  	printf("cd\n");
			fat12_cd(rest);
		} 
		else if (strcmp(str,"rm") == 0)
		{
		  printf("rm\n");
			fat12_rm(rest);
		} 
		else if (strcmp(str,"write") == 0)
		{
		  printf("write\n");
			char buf[1000];
			strcpy(buf,"This is a test message!");
			fat12_create_file((uintptr_t)buf,strlen(buf),"test.txt");
		} 
		else if (strcmp(str,"cp") == 0){
		  printf("cp\n");
			char* src = rest;
			strsep(&rest," ");
			fat12_cp(src,rest);
		} else if (strcmp(str,"tick") == 0)
			printf("%d\n",get_tick_count());
		else if (strcmp(str,"read") == 0)
		{
		  	//printf("read\n");

					//! get pathname
			//fsysFatInitialize();
			char path[32];//="file.txt";
			printf ("\n\rInput example: \"file.txt\", \"a:\\file.txt\", \"a:\\folder\\file.txt\"\n\rFilename> ");
			getline(path);
			
			
      printf("\n%s\n",path);
			//! open file
			FILE file = volOpenFile (path);
			//FILE curDirectory = fsysFatDirectory (path);
			
      //printf("=file=\n");
      //printf("=file.name=%s;\n",file.name);
      //printf("=file.flags=%d;\n",file.flags);
      //printf("=file.fileLength=%d;\n",file.fileLength);
      //printf("=file.id=%d;\n",file.id);
      //printf("=file.eof=%d;\n",file.eof);
      //printf("=file.position=%d;\n",file.position);
      //printf("=file.currentCluster=%d;\n",file.currentCluster);
      //printf("=file.deviceID=%d;\n",file.deviceID);
      //printf("=file.buffer=%s;\n",file.buffer);

			
			
			//printf("=====read file 1.0=====\n");
			//! test for invalid file
			if (file.flags == FS_INVALID) {  //FS_INVALID =2

				printf ("\n\rUnable to open file\n");
				//printf("%x",'E');
				//'F' 0x46 'I' 0x49 'L' 0x4C 'E' 0x45
				continue;
				//return;
			}
			//printf("=====read file 1.1=====\n");
			//! cant display directories
			if (( file.flags & FS_DIRECTORY ) == FS_DIRECTORY) {

				printf ("\n\rUnable to display contents of directory.");
				return;
			}
			//printf("=====read file 1.2=====\n");
			//! top line
			//printf ("-------[%s]-------\n", file.name);
			//printf ("file.currentCluster number is : %d \n", file.currentCluster);
			//printf ("file.eof number is : %d \n", file.eof);


			//! display file contents
			while (file.eof != 1) {
				//printf("=====read file 2.0=====\n");
				//! read cluster
				unsigned char buf[512];
				//printf("=start volReadFile()=\n");
				volReadFile ( &file, buf,file.fileLength);
				//fsysFatRead( &file, buf, 512);
				//printf("=before display file=\n");
			
				//! display file
				for (int i=0; i<file.fileLength; i++)
				{
					//printf("#,");
					printf("%c",buf[i]);
					//DebugPutc (buf[i]);
				}
				//printf("=file.eof=%d;\n",file.eof);
				printf ("------[continue]------\n");

				//! wait for input to continue if not EOF
				if (file.eof != 1) {
				  //printf("=file.eof=%d;\n",file.eof);
					printf ("------[file is not end]------\n");
					//getch ();
					//printf ("\r"); //clear last line
				}
			}

			//! done :)
			//printf ("\n--------[EOF]--------");
			
			//read_disk_test();

		}
		else if (strcmp(str,"read2") == 0)
		{
			read_disk_test();
		}
		else if (strcmp(str,"user") == 0)
		{
			go_user();
		}
		else if (strcmp(str,"exec") == 0){
		  printf("exec\n");
			exec_user_prg(rest);
		} else
			command_not_found(str);
	}
}

void new_terminal()
{
	set_color(CYAN,BLACK);
	print(HELLO_INFO);
	set_color(WHITE,BLACK);
	put_prompt();
}

void change_terminal(int new_ter)
{
	disable();
	memcpy((void*)curr_terminal->buf,(void*)VIDEO_ADDRESS,VIDEO_SIZE);
	curr_terminal->cursor = get_cursor();
	//get_curr_path(curr_terminal->path);
	//curr_terminal->dir = get_curr_dir();
	//if (get_proc() == NULL)
		curr_terminal = tmp_terminal = &terminal_list[new_ter];
	//else
	//	curr_terminal = &terminal_list[new_ter];
	clear_screen();
	memcpy((void*)VIDEO_ADDRESS,(void*)curr_terminal->buf,VIDEO_SIZE);
	set_cursor(curr_terminal->cursor);
	//set_curr_path(curr_terminal->path);
	//set_curr_dir(curr_terminal->dir);
	enable();
}

void terminal_init()
{
	disable();
	for (int i = 0; i < MAX_TERMINAL; ++i){
		curr_terminal = tmp_terminal = &terminal_list[i];
		curr_terminal->num = i;
		curr_terminal->cursor = 0;
		//curr_terminal->dir = get_curr_dir();
		strcpy(curr_terminal->path,"/");
		memset(curr_terminal->buf,0,sizeof(curr_terminal->buf));
	}
	for (int i = 0; i < MAX_TERMINAL; ++i){
		curr_terminal = tmp_terminal = &terminal_list[i];
		new_terminal();
		memcpy((void*)curr_terminal->buf,(void*)VIDEO_ADDRESS,VIDEO_SIZE);
		curr_terminal->cursor = get_cursor();
		clear_screen();
	}
	curr_terminal = tmp_terminal = &terminal_list[0];
	memcpy((void*)VIDEO_ADDRESS,(void*)curr_terminal->buf,VIDEO_SIZE);
	set_cursor(curr_terminal->cursor);
	enable();
}

