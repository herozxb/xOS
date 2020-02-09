/******************************************************************************
   fat12.cpp
		-FAT12 Minidriver

   arthor\ Mike
******************************************************************************/

#include "DebugDisplay.h"
#include "fat12.h"
#include "string.h"
#include "flpydsk.h"
#include "bpb.h"
#include "ctype.h"
#include "stdio.h"

//! bytes per sector
#define SECTOR_SIZE 512

//! FAT FileSystem
FILESYSTEM _FSysFat;

//! Mount info
MOUNT_INFO _MountInfo;

//! File Allocation Table (FAT)
uint8_t FAT [SECTOR_SIZE*2];

/**
*	Helper function. Converts filename to DOS 8.3 file format
*/
void ToDosFileName (const char* filename,
            char* fname,
            unsigned int FNameLength) {

	unsigned int  i=0;

	if (FNameLength > 11)
		return;

	if (!fname || !filename)
		return;

	//! set all characters in output name to spaces
	memset (fname, ' ', FNameLength);

	//! 8.3 filename
	for (i=0; i < strlen(filename)-1 && i < FNameLength; i++) {

		if (filename[i] == '.' || i==8 )
			break;

		//! capitalize character and copy it over (we dont handle LFN format)
		fname[i] = toupper (filename[i] );
	}

	//! add extension if needed
	if (filename[i]=='.') {

		//! note: cant just copy over-extension might not be 3 chars
		for (int k=0; k<3; k++) {

			++i;
			if ( filename[i] )
				fname[8+k] = filename[i];
		}
	}

	//! extension must be uppercase (we dont handle LFNs)
	for (i = 0; i < 3; i++)
		fname[8+i] = toupper (fname[8+i]);
}

/**
*	Locates file or directory in root directory
*/
FILE fsysFatDirectory (const char* DirectoryName) {

	FILE file;
	unsigned char* buf;
	PDIRECTORY directory;

	//! get 8.3 directory name
	char DosFileName[11];
	ToDosFileName (DirectoryName, DosFileName, 11);
	DosFileName[11]=0;
	//printf("=DosFileName = %c%c%c%c%c%c%c%c%c%c%c=", DosFileName[0],DosFileName[1],DosFileName[2],DosFileName[3],DosFileName[4],DosFileName[5],DosFileName[6],DosFileName[7],DosFileName[8],DosFileName[9],DosFileName[10]);

	//! 14 sectors per directory
	for (int sector=0; sector<14; sector++) {

		//printf("=key sector is %d=",_MountInfo.rootOffset + sector + 10);
		//! read in sector of root directory
		//printf("=_MountInfo.rootOffset = %d=",_MountInfo.rootOffset);
		buf = (unsigned char*) flpydsk_read_sector (_MountInfo.rootOffset + sector +10 );

		//! get directory info
		directory = (PDIRECTORY) buf;
		//! 16 entries per sector
		for (int i=0; i<16; i++) {

			//printf("DosFileName1[0] = [%c]",DosFileName1[0]);

			//! get current filename
			char name[12];
			memcpy (name, directory->Filename, 11);
			name[11]=0;
			
			//printf("DosFileName1[0] = [%c]",DosFileName1[0]);
			//printf("=DosFileName3 =[%c%c%c%c%c%c%c%c%c%c%c]\n", DosFileName1[0],DosFileName1[1],DosFileName1[2],DosFileName1[3],DosFileName1[4],DosFileName1[5],DosFileName1[6],DosFileName1[7],DosFileName1[8],DosFileName1[9],DosFileName1[10]);
			//if(name[0]!=NULL)
			//{
			//	printf("=key name =[%c%c%c%c%c%c%c%c%c%c%c]\n", name[0],name[1],name[2],name[3],name[4],name[5],name[6],name[7],name[8],name[9],name[10]);
			//	printf("compare result is %d",strcmp (DosFileName, name));
			
			//}
			//! find a match?
			if (strcmp (DosFileName, name) == 0) {

				//! found it, set up file info
				//printf("=found it %d=",directory->FirstCluster);

				strcpy (file.name, DirectoryName);
				file.id             = 0;
				file.currentCluster = directory->FirstCluster;
				file.fileLength     = directory->FileSize;
				file.eof            = 0;
				file.fileLength     = directory->FileSize;

				//! set file type
				if (directory->Attrib == 0x10)
					file.flags = FS_DIRECTORY;
				else
					file.flags = FS_FILE;

				//! return file
				return file;
			}

			//! go to next directory
			directory++;
		}
	}

	//! unable to find file
	file.flags = FS_INVALID;
	return file;
}

/**
*	Reads from a file
*/
void fsysFatRead(PFILE file, unsigned char* Buffer, unsigned int Length) {

  	//printf("=====read file 2.1.0=====\n");
	if (file) {
    	//printf("=start in the fsysFatRead()=\n");
		//! starting physical sector
		unsigned int physSector = 32 + (file->currentCluster - 1);
		//printf("=before flpydsk_read_sector() physSector = %d\n",physSector);
		//! read in sector
		unsigned char* sector = (unsigned char*) flpydsk_read_sector ( physSector );

		//! copy block of memory
		memcpy (Buffer, sector, 512);
		
		/*
					  //! display sector
	  if (sector!=0) {

		  int i = 0;
		  for ( int c = 0; c < 1; c++ ) {

			  for (int j = 0; j < 128; j++)
				  printf ("0x%x ", sector[ i + j ]);
			  i += 128;

			  printf("\n\rPress any key to continue\n\r");
		  }
	  }
	  else
		  printf ("\n\r*** Error reading sector from disk");
		
		
		//*/
		
    //printf("=====read file 2.1.1.0=====\n");
		//! locate FAT sector
		unsigned int FAT_Offset = file->currentCluster + (file->currentCluster / 2); //multiply by 1.5
		unsigned int FAT_Sector = 1 + (FAT_Offset / SECTOR_SIZE);
		unsigned int entryOffset = FAT_Offset % SECTOR_SIZE;
    	//printf("=start flpydsk_read_sector()=");
		//! read 1st FAT sector
		sector = (unsigned char*) flpydsk_read_sector ( FAT_Sector );
		memcpy (FAT, sector, 512);
		
	/*
			  //! display sector
	  if (sector!=0) {

		  int i = 0;
		  for ( int c = 0; c < 1; c++ ) {

			  for (int j = 0; j < 128; j++)
				  printf ("0x%x ", sector[ i + j ]);
			  i += 128;

			  printf("\n\rPress any key to continue\n\r");
		  }
	  }
	  else
		  printf ("\n\r*** Error reading sector from disk");
//*/		
		
    //printf("=====read file 2.1.1.2=====\n");
		//! read 2nd FAT sector
		sector = (unsigned char*) flpydsk_read_sector ( FAT_Sector + 1 );
		memcpy (FAT + SECTOR_SIZE, sector, 512);
		
		/*
			  //! display sector
	  if (sector!=0) {

		  int i = 0;
		  for ( int c = 0; c < 1; c++ ) {

			  for (int j = 0; j < 128; j++)
				  printf ("0x%x ", sector[ i + j ]);
			  i += 128;

			  printf("\n\rPress any key to continue\n\r");
		  }
	  }
	  else
		  printf ("\n\r*** Error reading sector from disk");
		//*/
		
    //printf("=====read file 2.1.1.3=====\n");
		//! read entry for next cluster
		uint16_t nextCluster = *( uint16_t*) &FAT [entryOffset];

    //printf("=====read file 2.1.2=====\n");
		//! test if entry is odd or even
		if( file->currentCluster & 0x0001 )
			nextCluster >>= 4;      //grab high 12 bits
		else
			nextCluster &= 0x0FFF;   //grab low 12 bits

    	//printf("=====read file 2.1.2.0=====\n");
		//! test for end of file
		if ( nextCluster >= 0xff8) {

			file->eof = 1;
			//printf("=test for end of file=\n");
			return;
		}
    	//printf("=====read file 2.1.2.2=====\n");
		//! test for file corruption
		if ( nextCluster == 0 ) {

			file->eof = 1;
			//printf("=====read file 2.1.2.3=====\n");
			return;
		}
   		 //printf("=====read file 2.1.3=====\n");
		//! set next cluster
		file->currentCluster = nextCluster;
		//printf("=====read file 2.1.4 end=====\n");
	}
}

/**
*	Closes file
*/
void fsysFatClose (PFILE file) {

	if (file)
		file->flags = FS_INVALID;
}

/**
*	Locates a file or folder in subdirectory
*/
FILE fsysFatOpenSubDir (FILE kFile,
						const char* filename) {

	FILE file;

	//! get 8.3 directory name
	char DosFileName[11];
	ToDosFileName (filename, DosFileName, 11);
	DosFileName[11]=0;

	//! read directory
	while (! kFile.eof ) {

		//! read directory
		unsigned char buf[512];
		fsysFatRead (&file, buf, 512);

		//! set directort
		PDIRECTORY pkDir = (PDIRECTORY) buf;

		//! 16 entries in buffer
		for (unsigned int i = 0; i < 16; i++) {

			//! get current filename
			char name[11];
			memcpy (name, pkDir->Filename, 11);
			name[11]=0;

			//! match?
			if (strcmp (name, DosFileName) == 0) {

				//! found it, set up file info
				strcpy (file.name, filename);
				file.id             = 0;
				file.currentCluster = pkDir->FirstCluster;
				file.fileLength     = pkDir->FileSize;
				file.eof            = 0;
				file.fileLength     = pkDir->FileSize;

				//! set file type
				if (pkDir->Attrib == 0x10)
					file.flags = FS_DIRECTORY;
				else
					file.flags = FS_FILE;

				//! return file
				return file;
			}

			//! go to next entry
			pkDir++;
		}
	}

	//! unable to find file
	file.flags = FS_INVALID;
	return file;
}

/**
*	Opens a file
*/
FILE fsysFatOpen (const char* FileName) {

	FILE curDirectory;
	char* p = 0;
	bool rootDir=true;
	char* path = (char*) FileName;
	//printf("=in the fsysFatOpen()=");
	//! any '\'s in path?
	p = strchr (path, '\\');

	if (!p) {

		//! nope, must be in root directory, search it
		curDirectory = fsysFatDirectory (path);
		//printf("=in the curDirectory=");
		//printf("=curDirectory.flags %d=",curDirectory.flags);
		//printf("=curDirectory.deviceID %c=",curDirectory.deviceID);
		//! found file?
		if (curDirectory.flags == FS_FILE)
		{
			//printf("=curDirectory.name = %c=",curDirectory.name[5]);
			//printf("=curDirectory.fileLength = %d=",curDirectory.fileLength);
			//printf("=curDirectory.currentCluster = %d=",curDirectory.currentCluster);
			return curDirectory;
		}

			

		//! unable to find
		FILE ret;
		ret.flags = FS_INVALID;
		return ret;
	}

	//! go to next character after first '\'
	p++;

	while ( p ) {

		//! get pathname
		char pathname[16];
		int i=0;
		for (i=0; i<16; i++) {

			//! if another '\' or end of line is reached, we are done
			if (p[i]=='\\' || p[i]=='\0')
				break;

			//! copy character
			pathname[i]=p[i];
		}
		pathname[i]=0; //null terminate

		//! open subdirectory or file
		if (rootDir) {

			//! search root directory - open pathname
			curDirectory = fsysFatDirectory (pathname);
			rootDir=false;
		}
		else {

			//! search a subdirectory instead for pathname
			curDirectory = fsysFatOpenSubDir (curDirectory, pathname);
		}

		//! found directory or file?
		if (curDirectory.flags == FS_INVALID)
			break;

		//! found file?
		if (curDirectory.flags == FS_FILE)
			return curDirectory;

		//! find next '\'
		p=strchr (p+1, '\\');
		if (p)
			p++;
	}

	//! unable to find
	FILE ret;
	ret.flags = FS_INVALID;
	return ret;
}

/**
*	Mounts the filesystem
*/
void fsysFatMount () {

	//! Boot sector info
	PBOOTSECTOR bootsector;

	//! read boot sector
	bootsector = (PBOOTSECTOR) flpydsk_read_sector (0);

	//! store mount info
	_MountInfo.numSectors     = bootsector->Bpb.NumSectors;
	//printf("bootsector->Bpb.NumSectors = %d\n",bootsector->Bpb.NumSectors);
	_MountInfo.fatOffset      = 1;
	_MountInfo.fatSize        = bootsector->Bpb.SectorsPerFat;
	//printf("bootsector->Bpb.SectorsPerFat = %d\n",bootsector->Bpb.SectorsPerFat);
	_MountInfo.fatEntrySize   = 8;
	_MountInfo.numRootEntries = bootsector->Bpb.NumDirEntries;
	_MountInfo.rootOffset     = (bootsector->Bpb.NumberOfFats * bootsector->Bpb.SectorsPerFat) + 1;
	_MountInfo.rootSize       = ( bootsector->Bpb.NumDirEntries * 32 ) / bootsector->Bpb.BytesPerSector;
}

/**
*	Initialize filesystem
*/
void fsysFatInitialize () {

	//! initialize filesystem struct
	strcpy (_FSysFat.Name, "FAT12");
	_FSysFat.Directory = fsysFatDirectory;
	_FSysFat.Mount     = fsysFatMount;
	_FSysFat.Open      = fsysFatOpen;
	_FSysFat.Read      = fsysFatRead;
	_FSysFat.Close     = fsysFatClose;

	//! register ourself to volume manager
	volRegisterFileSystem ( &_FSysFat, 0 );

	//! mounr filesystem
	fsysFatMount ();
	//printf("File system init\n");
}
