/** @file */
#ifndef R6_H_   /* Include guard */
#define R6_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

/**
   This boot struct typedef is in place in order to allow for placing a pointer to the boot structure, inside of the struct definition of the boot.
 */
typedef struct boot boot;

/**
   This dir struct typedef is in place in order to allow for placing a pointer to the dir structure, inside of the struct definition of the dir.
 */
typedef struct dir dir;

/**
	The boot structure contains the boot sector information. This block consists of the following variables:
	unsigned int bps: Bytes per Sector
				spc: Sectors per Cluster
				numRS: Number of Reserved Sectors
				numFC: Number of FAT Copies
				maxNumRD: Max Number of Root Directory Entries
				numSec: Total # of Sectors in the File System
				numSecPerFAT: Number of Sectors per FAT
				spt: Sectors per track
				numHeads: Number of heads
				numSecFAT: Total sector count for FAT32
				vID: Volume ID
	unsigned char sig: Boot Signature
	unsigned char *label: Volume Label
	unsigned char *fsType: File System Type
*/
struct boot{
  unsigned int bps, spc, numRS, numFC, maxNumRDE, numSec, numSecPerFAT, spt, numHeads, numSecFAT, vID;
  unsigned char sig;
  unsigned char label[12];
  unsigned char fsType[9];
};

/**
	The directory structure contains information about a certain directory. This block consists of the following variables:
	unsigned int attribute: shows file attributes (read, hidden, system, subdir)
				reserved: reserved
				createTime: time of file creation
				createDate: date of file creation
				lastAccess: date of last access
				lWriteTime: last write time
				lWriteDate: last write date
				firstCluster: first logical cluster location

	uint32_t size: Total directory size in bytes
	unsigned char name[9]: directory name
	unsigned char extension[4]: directory extension
*/
struct dir{
  unsigned int attribute, reserved, createTime, createDate, lastAccess, lWriteTime, lWriteDate, firstCluster, startingLoc;
  uint32_t size;
  unsigned char name[9];
  unsigned char extension[4];
};

FILE *fp;
boot bootSector;
unsigned char buffer[1500000];
//uint16_t FAT[3072];
int FAT[3072];
dir cwd;
dir rootDir;
char *fname;
dir dirListing[225];

/**
   The launchInterface function is used to begin user input/polls for user functions
*/
void launchInterface();

/**
   The getDirListing function is used to get all values in the directory
*/
void getDirListing();

/**
   The getDir function is used to populate the dir structure
*/
dir getDir(int i);

/**
   The openFileIfExists function is used to verify that the file that is being opened exists
*/
int openFileIfExists();

/**
   The printUsage function is used to print out the proper user input format
*/
void printUsage();

/**
   The printBoot function is used to print out the contents of the boot sector
*/
void printBoot();

/**
   The printRoot function is used to print out the contents of the root sector
*/
void printRoot();

/**
   The changeDir function is used to change the directory
*/
void changeDir();

/**
   The listDir function is used to list the contents of the directory
*/
void listDir();

/**
   The lsal function is used to lists all files (hidden included) in long format
*/
void lsal();

/**
   The lsl function is used to list the long file info (non hidden, starting cluster, name, attr, size)
*/
void lsl();

/**
   The lsa function is used to list all the file names (including hidden)
*/
void lsa();

/**
   The ls function is used to list all the current directory names, with the potential for a wildcard serach (e.g. .*TXT)
*/
void ls(char *arg);

/**
   The type function is used to list all the file names (including hidden)
*/
void type();

/**
   The rnm function is used to rename the file name
*/
void rnm();

/**
   The loadBootSector function is used to load in the contents of the boot sector into the boot structure
*/
void loadBootSector();

/**
   The swap16 function is used to swap the order of 2 bytes
*/
unsigned int swap16(unsigned char byte1, unsigned char byte2);

/**
   The swap24 function is used to swap the order of 3 bytes
*/
unsigned int swap24(unsigned char byte1, unsigned char byte2, unsigned char byte3);

/**
   The swap32 function is used to swap the order of 4 bytes
*/
unsigned int swap32(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);

/**
   The setRoot function is used to set the directory to ROOT
*/
void setRoot();

/**
   The pwd function is used to print the current working directory
*/
void pwd();

/**
   The readFAT function is used to read the FAT table into a separate FAT[] array
*/
void readFAT();

/**
   The clear function is used to clear the current display window
*/
void clear();

/**
   The clearDirListing function is used to clear the directory listing array
*/
void clearDirListing();

/**
   The printAttributes function is used to print the file attributes (read, hidden, system, subdir)
*/
void printAttributes(int attribute);

/**
   The getAllDirs function is used to get all of the values in a directory
*/
void getAllDirs();

/**
   The exportFile function takes the location of a directory entry in the dirListing variable, the exports the selected file to the host with the same file name.
 */
void exportFile(int inloc);

#endif // R6_H_
