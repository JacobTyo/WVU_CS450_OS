#include "r6.h"

int main(int argc, char *argv[])
{

  if ( argc > 3 ) {

    printf("Too many inputs\r\n\r\n");
    printUsage();

  } else if (argc == 3) {

    // dump the named file directly to standard out (the terminal)
    //or another file with NO ADDITIONAL PADDING OR FORMATTING.
    //need to see if file exists. . . oh boy
    fname = argv[1];
    int opened = openFileIfExists(argv[1]);
    if (opened == 1){
      loadBootSector();
      readFAT();
      clearDirListing();
      getAllDirs();
      //prolly make sure
      char *name = strtok(argv[2], ".");
      char *ext = strtok(NULL, ".");
      int loc;
      if (ext == NULL){
        loc = findDir(name);
      } else {
        loc = findFile(name, ext);
      }

      //findFile
      if (loc >= 0){
        //do it
        printf("Starting export\n");
        exportFile(loc);
      } else {
        printf("File cannot be found\n");
      }


    } else {
      printf("disk image could not be found\n");
    }

  } else if (argc == 2) {

    fname = argv[1];
    //Enter Interface mode if input is correct
    int opened = openFileIfExists(argv[1]);
    if (opened == 1){

      //go ahead and set root, load boot sector, read FAT, and start interface mode
      loadBootSector();
      readFAT();
      clearDirListing();
      clear();
      setRoot();
      cwd = rootDir;
      getDirListing();
      launchInterface();

    } else {

      printf("Disk Image does not exist\r\n\r\n");
      printUsage();

    }

  } else if (argc == 1) {

    printf("Not enough inputs\r\n\r\n"); //bad input
    printUsage();

  }

}

void printUsage(){

  printf("Proper Program Usage:\r\n");
  printf("To enter interface mode use:\r\n./r6 diskImage\r\n\r\n");
  printf("To get specific file from disk image, use:\r\n");
  printf("./r6 diskImage fileToGet\r\n");

}

int openFileIfExists(char *diskImg){
  int e = 0;
  unsigned char c;
  if (fp = fopen(diskImg, "r")){

    fread(buffer, 1, 1500000, fp);
    e = 1;
    fclose(fp);
    
  } else {

    fp = NULL;

  }
  
  return e;
}

void launchInterface(){

  char command [50];
  char camd[50];
  char sarg[50];
  char sarg2[50];
  char *pcom = command;
  char *cmd = camd;
  char *arg = sarg;
  char *arg2 = sarg2;
  while(1){ //command handler loop

    printf(">");

    fgets(pcom, 50, stdin);
    if (strlen(pcom) > 0 && pcom[strlen(pcom)-1] == '\n')
    pcom[strlen(pcom)-1] = '\0';

    if (((int) strlen(pcom)) > 0){
      cmd = strtok(pcom, " ");
      arg = strtok(NULL, " ");
      arg2 = strtok(NULL, " ");

      if (strcmp(cmd, "printBoot") == 0) {
        printBoot();
      } else if (strcmp(cmd, "printRoot") == 0) {
        printRoot();
      } else if (strcmp(cmd, "cd") == 0) { //change directory
        if (arg2 == NULL){
          changeDir(arg);
        } else {
          printf("You have entered too many arguments\n");
        }
      } else if (strcmp(cmd, "ls") == 0) { //list directory
        if (arg2 == NULL){
          listDir(arg);
        } else {
          printf("You have entered too many arguments\n");
        }
      } else if (strcmp(cmd, "type") == 0) {
        if (arg == NULL){
          printf("No such file or file type not supported\n");
        }
        else{
          type(arg);
        }
      } else if (strcmp(cmd, "rename") == 0) {
        if (arg2 == NULL || arg == NULL) {
          printf("This command must have two input arguments\n");
        } else {
          rnm(arg, arg2);
          clearDirListing();
          getDirListing();
        }
      } else if (strcmp(cmd, "exit") == 0) {
        break;
      } else if (strcmp(cmd, "clear") == 0) {
        clear();
      } else if (strcmp(cmd, "pwd") == 0) {
        pwd();
      } else {
        printf("Invalid Command\r\n");
      }

    }
    int k;
    for(k = 0;k < 51; k++){
      command[k] = '\0';
      camd[k] = '\0';
      sarg[k] = '\0';
      sarg2[k] = '\0';
    }
    pcom = command;

  } //end while

}


void loadBootSector(){

  /* Read and display data */
  //this is a temporary variable to mimic the global buffer later to come

  //Bytes per Sector
  bootSector.bps = swap16(buffer[11], buffer[12]);
  //Sectors per Cluster
  bootSector.spc = buffer[13];
  //Number of Reserved Sectors
  bootSector.numRS = swap16(buffer[14], buffer[15]);
  //Number of Fat Copies
  bootSector.numFC = buffer[16];
  //Number of Root Directory Entries
  bootSector.maxNumRDE = swap16(buffer[17], buffer[18]);
  //Total # of Sectors in the File System
  bootSector.numSec = swap16(buffer[19], buffer[20]);
  //Number of Sectors per FAT
  bootSector.numSecPerFAT = swap16(buffer[22], buffer[23]);
  //Sectors per Track
  bootSector.spt = swap16(buffer[24], buffer[25]);
  //Number of Heads
  bootSector.numHeads = swap16(buffer[26], buffer[27]);
  //Total Sector Count for FAT32
  bootSector.numSecFAT = swap32(buffer[32], buffer[33], buffer[34], buffer[35]);
  //Boot Signature
  bootSector.sig = buffer[38];
  //Volume ID
  bootSector.vID = swap32(buffer[39], buffer[40], buffer[41], buffer[42]);
  //Volume Label
  bootSector.label[0] = buffer[43];
  bootSector.label[1] = buffer[44];
  bootSector.label[2] = buffer[45];
  bootSector.label[3] = buffer[46];
  bootSector.label[4] = buffer[47];
  bootSector.label[5] = buffer[48];
  bootSector.label[6] = buffer[49];
  bootSector.label[7] = buffer[50];
  bootSector.label[8] = buffer[51];
  bootSector.label[9] = buffer[52];
  bootSector.label[10] = buffer[53];
  bootSector.label[11] = '\0';
  //File System Type
  bootSector.fsType[0] = buffer[54];
  bootSector.fsType[1] = buffer[55];
  bootSector.fsType[2] = buffer[56];
  bootSector.fsType[3] = buffer[57];
  bootSector.fsType[4] = buffer[58];
  bootSector.fsType[5] = buffer[59];
  bootSector.fsType[6] = buffer[60];
  bootSector.fsType[7] = buffer[61];
  bootSector.fsType[8] = '\0';


}//end loop

//convert little endian stuffs
//based on a 16 bit long result
unsigned int swap16(unsigned char byte1, unsigned char byte2){
  //byte1: first byte in order from file (LSB)
  //byte2: second byte in order from file (MSB)
  unsigned int result = 0x0000;
  result = (result | byte2 << 8) | byte1;

  return result;
}

unsigned int swap24(unsigned char byte1, unsigned char byte2, unsigned char byte3) {

  unsigned int res;
  res = byte3;
  res << 8;
  res = res + byte2;
  res << 8;
  res = res + byte1;
  return res;

}

unsigned int swap32(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4) {
  unsigned int res1 = 0x0000;
  unsigned int res2 = 0x0000;
  unsigned int res = 0x0000;
  res1 = (res1 | byte2 << 8) | byte1;
  res2 = (res2| byte4 << 8) | byte3;
  res = (res| res2 << 16) | res1;
  return res;

}

void readFAT(){
  int n = 0;
  int fourBits = 0;
  int eightBits = 0;

  for (n = 0; n < 3072; n++){ //divided by 2 since FAT is uint_16

    if (n%2 == 0) {	//even entry
      fourBits = 1 + (3*n)/2; //this gives 8. . .
      eightBits = (3*n)/2;

      FAT[n] = ((buffer[bootSector.bps + fourBits] & 0x0f) << 8)
      + buffer[bootSector.bps + eightBits];
    } else { //odd entry
      fourBits = (3*n)/2;
      eightBits = 1 + (3*n)/2;

      FAT[n] = ((buffer[bootSector.bps + eightBits]) << 4) +
      ((buffer[bootSector.bps + fourBits])/16);
    }
    //printf("n: %x\t%x\n", n, FAT[n]);
  }

}

/********USER COMMANDS***********/
void printBoot(){
  printf("Bytes Per Sector: %d\r\n", bootSector.bps);
  printf("Sectors Per Cluster: %d\r\n", bootSector.spc);
  printf("Number of Reserved Clusters: %d\r\n", bootSector.numRS);
  printf("Number of FAT Copies: %d\r\n", bootSector.numFC);
  printf("Max Number of Root Directory Entries: %d\r\n", bootSector.maxNumRDE);
  printf("Total Sectors in File System: %d\r\n", bootSector.numSec);
  printf("Number of Sectors per FAT: %d\r\n", bootSector.numSecPerFAT);
  printf("Sectors per Track: %d\r\n", bootSector.spt);
  printf("Number of Heads: %d\r\n", bootSector.numHeads);
  printf("Total sector count for FAT32: %d\r\n", bootSector.numSecFAT);
  printf("Boot Signature: 0x%x\r\n", bootSector.sig);
  printf("Volume ID: %d\r\n", bootSector.vID);
  printf("Volume Label: %s\r\n", bootSector.label);
  printf("File System Type: %s\r\n", bootSector.fsType);
  printf("\r\n");
}

void printRoot(){
  dir temp = cwd;
  cwd = rootDir;
  getDirListing();
  ls(NULL);
  cwd = temp;
  getDirListing();
}

void changeDir(char *newDir){
  //first get dir starting cluster
  if (newDir == NULL){
    //change to root dir
    cwd = rootDir;
    clearDirListing();
    //get new dir listings
    getDirListing();
    //printf("CD\r\n");
  } else {

    int loc = findDir(newDir);
    if (loc >= 0){

      //set new cwd
      cwd = dirListing[loc];
      //printf("firstCluster: %d\n", cwd.firstCluster);
      //clear old dir listings
      clearDirListing();
      //get new dir listings
      getDirListing();
      //printf("CD\r\n");
    } else {
      printf("No such file or directory\n");
    }
  }
}

int findDir(char *dir){

  //look for dir in dirListing
  int i = 0;
  while (dirListing[i].name[0] != 0x00){
    if (strcmp(dirListing[i].name, dir) == 0){
      //names are the same, return loc
      return i;
    }
    i++;
  }
  //dir was not found
  return -1;

}

void listDir(char *arg1){
  //“list README.BAT” or “list *.txt”
  //can have second arg


  if (arg1 == NULL){
    //just list normal directory
    ls(NULL);

  } else if (strcmp(arg1, "-l") == 0){
    //list non hidden entries and attributes
    lsl();
  } else if (strcmp(arg1, "-a") == 0){
    //list all entries including hidden
    lsa();
  } else if (strcmp(arg1, "-la") == 0 || strcmp(arg1, "-al") == 0) {
    //list all files and attributes
    lsal();
  } else {
    //input is probably a file or wildcard nonsense
    //give the -l output for the specified file
    ls(arg1);
  }

}

void lsal(){
  //display
  int i = 0;
  while (dirListing[i].name[0] != 0x00){

    printAttributes(dirListing[i].attribute);
    printf(" %d ", dirListing[i].size);
    printf(" %d ", dirListing[i].firstCluster);
    if (strcmp(dirListing[i].extension, "") != 0){
      printf("%s.%s\n", dirListing[i].name, dirListing[i].extension);
    } else {
      printf("%s\n", dirListing[i].name);
    }

    i++;

  }

}

void lsl(){
  int i = 1;
  while (dirListing[i].name[0] != 0x00){
    if ((dirListing[i].attribute & 0x02) != 0x02){

      printAttributes(dirListing[i].attribute);
      printf(" %d ", dirListing[i].size);
      printf(" %d ", dirListing[i].firstCluster);
      if (strcmp(dirListing[i].extension, "") != 0){
        printf("%s.%s\n", dirListing[i].name, dirListing[i].extension);
      } else {
        printf("%s\n", dirListing[i].name);
      }
    }
    i++;

  }

}

void lsa(){

  int i = 0;
  while (dirListing[i].name[0] != 0x00){

    if (strcmp(dirListing[i].extension, "") != 0){
      printf("%s.%s\n", dirListing[i].name, dirListing[i].extension);
    } else {
      printf("%s\n", dirListing[i].name);
    }
    i++;

  }

}

void ls(char *arg){
  int i = 0;
  //chech if arg is null, then just ls
  if (arg == NULL){
    //just list the directory
    while (dirListing[i].name[0] != 0x00){
      if ((dirListing[i].attribute & 0x02) != 0x02){
        if (strcmp(dirListing[i].extension, "") != 0){
          printf("%s.%s\n", dirListing[i].name, dirListing[i].extension);
        } else {
          printf("%s\n", dirListing[i].name);
        }
      }
      i++;
    }
  } else {
    //check for wildcards
    char *name = "        ";
    char *ext = "   ";
    name = strtok(arg, ".");
    ext = strtok(NULL, ".");

    if (ext == NULL){
      ext = "";
    }

    if (strstr(name, "*") != NULL && strstr(ext, "*") != NULL){
      //name and ext contain *
      lsa();
    } else if (strstr(name, "*") != NULL){

      while (dirListing[i].name[0] != 0x00){
        if (strcmp(dirListing[i].extension, ext) == 0){
          printAttributes(dirListing[i].attribute);
          printf(" %d ", dirListing[i].size);
          printf(" %d ", dirListing[i].firstCluster);
          if (strcmp(dirListing[i].extension, "") != 0){
            printf("%s.%s\n", dirListing[i].name, dirListing[i].extension);
          } else {
            printf("%s\n", dirListing[i].name);
          }
        }
        i++;
      }

    } else if (strstr(ext, "*") != NULL){

      while (dirListing[i].name[0] != 0x00){
        if (strcmp(dirListing[i].name, name) == 0){
          printAttributes(dirListing[i].attribute);
          printf(" %d ", dirListing[i].size);
          printf(" %d ", dirListing[i].firstCluster);
          if (strcmp(dirListing[i].extension, "") != 0){
            printf("%s.%s\n", dirListing[i].name, dirListing[i].extension);
          } else {
            printf("%s\n", dirListing[i].name);
          }
        }
        i++;
      }
    } else {
      //open file that was specified if exists
      i = findFile(name, ext);
      if (i >= 0) {
        printAttributes(dirListing[i].attribute);
        printf(" %d ", dirListing[i].size);
        printf(" %d ", dirListing[i].firstCluster);
        if (strcmp(dirListing[i].extension, "") != 0){
          printf("%s.%s\n", dirListing[i].name, dirListing[i].extension);
        } else {
          printf("%s\n", dirListing[i].name);
        }
      } else {
        printf("No such file or directory\n");
      }
    }
  }
}


int findFile(char *name, char *ext){

  //look for dir in dirListing
  int i = 0;
  while (dirListing[i].name[0] != 0x00){
    if (strcmp(dirListing[i].name, name) == 0 && strcmp(dirListing[i].extension, ext) == 0){
      //names are the same, return loc
      return i;
    }
    i++;
  }
  //dir was not found
  return -1;

}


void getDirListing(){

  //get all values in directory
  //for now, just assume the root dir to figure out what is going on
  int i;
  if (strcmp(cwd.name, rootDir.name) == 0){
    i = 512 * 19;
    int j = 0;

    while (buffer[i] != 0x00){

      if (buffer[i] == 0xE5){
        i = i + 32;
        //counter = counter + 32;
        continue;
      } if (buffer[i+11] == 0xf) {//buffer[i] == 'A' && buffer[i+1] == ' '){
        //weird A file, skip maybe
        i = i + 32;
        continue;
      }
      dirListing[j++] = getDir(i);
      i = i + 32;
      //counter = counter + 32;

    }

  } else {
    i = 512 * (cwd.firstCluster + 33 - 2); //just replace this with 512 * cwd.firstCluster or whatever THINK THE PLUS 2 is needed

    int j = 0;
    int counter = 0;
    while (buffer[i] != 0x00){

      //check to see if we need to move to next sector
      if (counter >= 512){
        //move to next sector
        //basically check fat table then change i
        i = FAT[cwd.firstCluster] + 33;
        printf("new i: %d\n", i);
        counter = 0;
      }
      //check if directory is 'deleted' skip if so
      if (buffer[i] == 0xE5){
        i = i + 32;
        counter = counter + 32;
        continue;
      }
      dirListing[j++] = getDir(i);
      //printf("j: %d\n", j);
      i = i+32;
      counter = counter + 32;
    }
  }

}

dir getDir(int i){
  int j;
  dir newDir;
  //set name
  newDir.startingLoc = i;
  for (j = 0; j < 8; j++){
    if (buffer[i+j] != ' '){
      newDir.name[j] = buffer[i+j];
    } else {
      newDir.name[j] = '\0';
      i = i + (8 - j);
      break;
    }
  }
  newDir.name[j] = '\0';
  i = i + j;

  //set extension name
  int z = 0;
  for (j = 0; j < 3; j++){
    if (buffer[i+j] != ' '){
      newDir.extension[z++] = buffer[i+j];
    }
  }
  newDir.extension[z] = '\0';
  i = i + j;

  newDir.attribute = buffer[i++];
  newDir.reserved = swap16(buffer[i], buffer[i+1]);
  i = i + 2;
  newDir.createTime = swap16(buffer[i], buffer[i+1]);
  i = i + 2;
  newDir.createDate = swap16(buffer[i], buffer[i+1]);
  i = i + 2;
  newDir.lastAccess = swap16(buffer[i], buffer[i+1]);
  i = i + 4;
  newDir.lWriteTime = swap16(buffer[i], buffer[i+1]);
  i = i + 2;
  newDir.lWriteDate = swap16(buffer[i], buffer[i+1]);
  i = i + 2;
  newDir.firstCluster = swap16(buffer[i], buffer[i+1]);
  i = i + 2;
  newDir.size = swap32(buffer[i], buffer[i+1], buffer[i+2], buffer[i+3]);
  i = i + 4;
  return newDir;
}

void pwd(){
  //print the current working directory
  //pwd = root;
  printf("Current working directory: %s\n", cwd.name);
}

void setRoot(){
  //this aint right
  int i = 512 * 19;
  rootDir = getDir(i); //this is to just get it formatted right
  rootDir.name[0] = 'R';
  rootDir.name[1] = 'O';
  rootDir.name[2] = 'O';
  rootDir.name[3] = 'T';
  rootDir.name[4] = '\0';
  rootDir.firstCluster = 19;
}

void type(char * typeDir){
  dir tmpDir;
  int i; //index of first byte of file
  int loc = -1;
  int curCluster;
  int tmpCluster;
  unsigned int byteLocation;
  char * name = strtok(typeDir, ".");
  char * ext = strtok(NULL, ".");

  if (ext == NULL){
    //loc = findDir(name);
    printf("File must have extension type .TXT, .BAT, .C\n");
  }
  else{
    loc = findFile(name,ext);
  }

  char c;

  if (loc >= 0 && (strcmp(ext, "TXT") == 0 || strcmp(ext, "BAT") == 0 || strcmp(ext, "C") == 0 )){
    //set new dir
    tmpDir = dirListing[loc];
  } else {
    printf("No such file or file type not supported\n");
  }

  tmpCluster = tmpDir.firstCluster; //sets intial cluster number
  byteLocation = 512 * (tmpCluster + 33 - 2); //gets to first3 byte of file

  for (i = 0; i < tmpDir.size; i++){

    if (i % 512 == 0 && tmpDir.size > 512 && FAT[tmpCluster] != 0xfff && i != 0){ //file spans multiple clusters
      tmpCluster = FAT[tmpCluster];
      byteLocation = 512 * (tmpCluster + 33 - 2);
    }

    printf("%c",buffer[(i % 512) + byteLocation]); //works for first and middle clusters


    if (i%500 == 0 && i != 0){
      printf("\n\n<ENTER to Continue... q -> ENTER to quit>\n\n");
      c = getchar();
      if (c == 'q'){ break;}

    }
  }


}//finished reading file



void rnm(char *arg, char *arg1){ //rename
  int written = 0;
  if (strlen(arg) > 0 && strlen(arg1) > 0){
    char *name = strtok(arg, ".");
    char *ext = strtok(NULL, ".");
    int loc;
    if (ext == NULL){
      loc = findDir(arg);
    } else {
      loc = findFile(name, ext);
    }
    loc = dirListing[loc].startingLoc;
    if (loc >= 0){
      //modify buffer at loc and write
      int i;
      for (i = 0; i < 11; i++){
        buffer[loc+i] = ' ';
      }
      //get chars from new name
      char *newname = strtok(arg1, ".");
      char *newext = strtok(NULL, ".");
      int z;
      if (newext != NULL){
        if (strlen(arg1) < 13){
          //change name
          for (z = 0; z < 8; z++){
            buffer[loc+z] = (char) newname[z];
          }
          loc = loc + z;
          for (z = 0; z < 4; z++){
            buffer[loc+z] = (char) newext[z];
          }
          written = 1;
        } else {
          //name is too long
          printf("The name you have entered is too long\n");
        }
      } else {
        //newext is null, only change name
        if (strlen(arg1) < 9){
          for (z = 0; z < 8; z++){
            buffer[loc+z] = (char) newname[z];
          }
          written = 1;
        } else {
          printf("The name you have enetered is too long\n");
        }
      }
    } else {
      printf("The file does not exist\n");
    }
  }
  if (written == 1){
    //name has been changed and file need written
    FILE *fp = fopen(fname, "w");
    int k;
    for (k = 0; k < 1500000; k++){
      fputc(buffer[k], fp);
    }
    fclose(fp);
  }
}




void clear(){
  int i;
  for(i = 0; i < 38; i++){
    printf("\n");
  }
}

void clearDirListing(){
  int i;
  for (i = 0; i < 225; i++){
    dirListing[i].name[0] = 0x00;
    dirListing[i].extension[0] = 0x00;
  }
}

void printAttributes(int attribute) {
  int i = 8;
  int bits[8];
  while (attribute > 0){
    bits[i] = attribute % 2;
    i--;
    attribute = attribute / 2;
  }

  if (bits[4]==1) {
    printf("d");
  } else {
    printf("-");
  }
  if (bits[8]==1) {
    printf("r");
  } else {
    printf("-");
  }
  if (bits[7]==1) {
    printf("h");
  } else {
    printf("-");
  }
  if (bits[6]==1) {
    printf("s ");
  } else {
    printf("- ");
  }
}

void getAllDirs(){

  //get all values in directory
  //for now, just assume the root dir to figure out what is going on
  int i;
  int t = 0;
  int subdirs[500];
  for (i = 0; i < 500; i++){
    subdirs[i] = -1;
  }
  i = 512 * 19;
  int j = 0;

  while (buffer[i] != 0x00){

    if (buffer[i] == 0xE5){
      i = i + 32;
      //counter = counter + 32;
      continue;
    } if (buffer[i+11] == 0xf) {//buffer[i] == 'A' && buffer[i+1] == ' '){
      //weird A file, skip maybe
      i = i + 32;
      continue;
    }
    dirListing[j] = getDir(i);
    if ((dirListing[j].attribute & 0x10) == 0x10){
      //this is a sub directory, mark for later
      subdirs[t++] = j;
    }
    i = i + 32;
    //counter = counter + 32;
    j++;
  }

  //filter subdirs

  //now on to sub directories
  int counter = 0;
  t = 0;
  int finished = 0;
  while (subdirs[t] != -1){

    //add all the contents to dirListing;
    while (strcmp(dirListing[subdirs[t]].name, ".") == 0 || strcmp(dirListing[subdirs[t]].name, "..") == 0 || strcmp(dirListing[subdirs[t]].extension, ".") == 0
    || strcmp(dirListing[subdirs[t]].extension, "..") == 0){
      t++;
      if (subdirs[t] == -1){
        finished = 1;
      }
    }

    if (finished == 1){
      break;
    }

    i = 512 * (dirListing[subdirs[t]].firstCluster + 33 - 2);
    while (buffer[i] != 0x00){

      //check to see if we need to move to next sector
      if (counter >= 512){
        //move to next sector
        //basically check fat table then change i
        i = FAT[dirListing[subdirs[t]].firstCluster] + 33;
        counter = 0;
      }
      //check if directory is 'deleted' skip if so
      if (buffer[i] == 0xE5){
        i = i + 32;
        counter = counter + 32;
        continue;
      }
      dirListing[j++] = getDir(i);
      //printf("j: %d\n", j);
      i = i+32;
      counter = counter + 32;
    }
    t++;
  }
}

void exportFile(int inloc){

  dir tmpDir = dirListing[inloc];
  unsigned char output[tmpDir.size];
  int m = 0;
  int i; //index of first byte of file
  int tmpCluster;
  unsigned int byteLocation;
  char c;

  tmpCluster = tmpDir.firstCluster; //sets intial cluster number
  byteLocation = 512 * (tmpCluster + 33 - 2); //gets to first3 byte of file

  printf("loading file into buffer: %x\n", byteLocation);

  for (i = 0; i < tmpDir.size; i++){

    if (i % 512 == 0 && tmpDir.size > 512 && FAT[tmpCluster] != 0xfff && i != 0){
      tmpCluster = FAT[tmpCluster];
      byteLocation = 512 * (tmpCluster + 33 - 2);

    }
    output[i] = buffer[(i % 512) + byteLocation];

  }

  printf("Exporting file\n");
  //now export output
  FILE *fp = fopen(strcat(strcat(tmpDir.name, "."), tmpDir.extension), "w");
  int k;
  for (k = 0; k < tmpDir.size; k++){
    fputc(output[k], fp);
  }
  fclose(fp);

  printf("%s successfully exported\n", strcat(strcat(tmpDir.name, "."), tmpDir.extension));

}
