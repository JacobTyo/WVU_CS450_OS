#include "date.h"

void date(char * args){

  char constArgs[35];
  char *pconstArgs = constArgs;//sys_alloc_mem(50);
  strcpy(pconstArgs, args);

  if (strlen(pconstArgs) == 0){

    //Read Day
    outb(0x70,0x07);
    unsigned char day = inb(0x71);

    //Read Month
    outb(0x70,0x08);
    unsigned char month = inb(0x71);

    //Read Year
    outb(0x70,0x09);
    unsigned char year = inb(0x71);

    //Read Hours
    outb(0x70,0x04);
    unsigned char hour = inb(0x71);

    //Read Minutes
    outb(0x70,0x02);
    unsigned char minute = inb(0x71);

    //Read Second
    outb(0x70,0x00);
    unsigned char second = inb(0x71);

    //convert BCD to readable date info
    unsigned char day1 = day & 0xF0;
    unsigned char day2 = (day & 0x0F) << 4;
    day1 = hex2ASCII(day1);
    day2 = hex2ASCII(day2);

    unsigned char month1 = month & 0xF0;
    unsigned char month2 = (month & 0x0F) << 4;
    month1 = hex2ASCII(month1);
    month2 = hex2ASCII(month2);

    unsigned char year1 = year & 0xF0;
    unsigned char year2 = (year & 0x0F) << 4;
    year1 = hex2ASCII(year1);
    year2 = hex2ASCII(year2);

    unsigned char hour1 = hour & 0xF0;
    unsigned char hour2 = (hour & 0x0F) << 4;
    hour1 = hex2ASCII(hour1);
    hour2 = hex2ASCII(hour2);

    unsigned char min1 = minute & 0xF0;
    unsigned char min2 = (minute & 0x0F) << 4;
    min1 = hex2ASCII(min1);
    min2 = hex2ASCII(min2);

    unsigned char sec1 = second & 0xF0;
    unsigned char sec2 = (second & 0x0F) << 4;
    sec1 = hex2ASCII(sec1);
    sec2 = hex2ASCII(sec2);


    //Using format "MON-DD-YR HH:MM:SS"
    outb(COM1, month1);
    outb(COM1, month2);
    serial_print("-");
    outb(COM1, day1);
    outb(COM1, day2);
    serial_print("-");
    outb(COM1, year1); //displays DEC value to terminal
    outb(COM1, year2);
    serial_print(" ");
    outb(COM1, hour1);
    outb(COM1, hour2);
    serial_print(":");
    outb(COM1, min1);
    outb(COM1, min2);
    serial_print(":");
    outb(COM1, sec1);
    outb(COM1, sec2);
    serial_print("    (MM-DD-YR HH:MM:SS)");

    serial_println("");
  } //end of first if (no arguements, just getDate)

  else if (strcmp(pconstArgs, NULL) != 0){
    //date --set 01-01-16 12:00:00
    unsigned char dataFinal; //BCD byte
    char * toks = ""; //toks contains command after --
    char * tokSet = "";
    char * flag = "";
    char * toksDate = "";
    char * toksTime = "";

    toks = strtok(pconstArgs, " "); //get first token (--whatever)
    flag = toks;

    toks = strtok(NULL, " "); //get second token (date)
    toksDate = toks;

    toks = strtok(NULL, " "); //get third token (time)
    toksTime = toks;

    //Check toksDate to make sure it is valid input (0-9)
    int i = 0;
    int inputFlagDate = 0; //flag for bad input, will equal 8 iff proper inputs

    for (i = 0; i < strlen(toksDate); i++ ) {
      if(*(toksDate + i) == '0'){
		  inputFlagDate++;
	  }
	  else if (*(toksDate + i) == '1'){
		  inputFlagDate++;
	  }
	  else if (*(toksDate + i) ==  '2'){
		  inputFlagDate++;
	  }
	  else if (*(toksDate + i) == '3'){
		  inputFlagDate++;
	  }
	  else if (*(toksDate + i) ==  '4'){
		  inputFlagDate++;
	  }
	  else if (*(toksDate + i) == '5'){
		  inputFlagDate++;
	  }
	  else if (*(toksDate + i) ==  '6'){
		  inputFlagDate++;
	  }
	  else if (*(toksDate + i) == '7'){
		  inputFlagDate++;
	  }
	  else if (*(toksDate + i) ==  '8'){
		  inputFlagDate++;
	  }
	  else if (*(toksDate + i) ==  '9'){
		  inputFlagDate++;
	  }
	  else if (*(toksDate + i) ==  '-'){
		  inputFlagDate++;
	  }
    }


    //Check toksTime to make sure it is valid input (0-9)
    int inputFlagTime = 0; //flag for bad input

    for (i = 0; i < strlen(toksTime); i++ ) {
      if(*(toksTime + i) == '0'){
		  inputFlagTime++;
	  }
	  else if (*(toksTime + i) == '1'){
		  inputFlagTime++;
	  }
	  else if (*(toksTime + i) == '2'){
		  inputFlagTime++;
	  }
	  else if (*(toksTime + i) == '3'){
		  inputFlagTime++;
	  }
	  else if (*(toksTime + i) == '4'){
		  inputFlagTime++;
	  }
	  else if (*(toksTime + i) == '5'){
		  inputFlagTime++;
	  }
	  else if (*(toksTime + i) == '6'){
		  inputFlagTime++;
	  }
	  else if (*(toksTime + i) == '7'){
		  inputFlagTime++;
	  }
	  else if (*(toksTime + i) == '8'){
		  inputFlagTime++;
	  }
	  else if (*(toksTime + i) == '9'){
		  inputFlagTime++;
	  }
	  else if (*(toksTime + i) == ':'){
		  inputFlagTime++;
	  }
    }

    if (strcmp(flag, "--set") == 0 && (inputFlagDate + inputFlagTime) == 16){

      //SET DATE
      //obtain each element to set to memory
      tokSet = strtok(toksDate, "-");
      //////////////////////////////////////////////////

      dataFinal = ASCII2hex(tokSet);  //convert to BCD
      //MONTH
      cli(); //disable interruputs
      outb(0x70,0x08);
      outb(0x71,dataFinal);
      sti(); //enable interrupts

      //DAY
      tokSet = strtok(NULL, "-"); //get next date element to write to mem
      dataFinal = ASCII2hex(tokSet);
      cli(); //disable interruputs
      outb(0x70,0x07);
      outb(0x71,dataFinal);
      sti(); //enable interrupts

      //YEAR
      tokSet = strtok(NULL, "-"); //get next date element to write to mem
      dataFinal = ASCII2hex(tokSet);
      cli(); //disable interruputs
      outb(0x70,0x09);
      outb(0x71,dataFinal);
      sti(); //enable interrupts

      //////////////////////////////////////////////////
      //SET TIME
      //obtain each element to set to memory
      tokSet = strtok(toksTime, ":");

      dataFinal = ASCII2hex(tokSet);  //convert to BCD
      //HOURS
      cli(); //disable interruputs
      outb(0x70,0x04);
      outb(0x71,dataFinal);
      sti(); //enable interrupts

      //MINUTES
      tokSet = strtok(NULL, "-"); //get next date element to write to mem
      dataFinal = ASCII2hex(tokSet);
      cli(); //disable interruputs
      outb(0x70,0x02);
      outb(0x71,dataFinal);
      sti(); //enable interrupts

      //SECONDS
      tokSet = strtok(NULL, "-"); //get next date element to write to mem
      dataFinal = ASCII2hex(tokSet);
      cli(); //disable interruputs
      outb(0x70,0x00);
      outb(0x71,dataFinal);
      sti(); //enable interrupts

    }
    else {
      serial_println("Inproper command (use --set MM-DD-YY HH:MM:SS to set date with 0-9 only)");
    }
  }

}

unsigned char hex2ASCII(unsigned char dec){

  //passed in value of ####0000 (only care about first nibble)
  if (dec == 0x00){
    return 0x30;
  }
  else if (dec == 0x10){
    return 0x31;
  }
  else if (dec == 0x20){
    return 0x32;
  }
  else if (dec == 0x30){
    return 0x33;
  }
  else if (dec == 0x40){
    return 0x34;
  }
  else if (dec == 0x50){
    return 0x35;
  }
  else if (dec == 0x60){
    return 0x36;
  }
  else if (dec == 0x70){
    return 0x37;
  }
  else if (dec == 0x80){
    return 0x38;
  }
  else if (dec == 0x90){
    return 0x39;
  }
  else{
    return 0x00;
  }

} //end hex2ASCII

unsigned char ASCII2hex(char *ASCII){
  //input: 2 digit char pointer for each element of date/time in ASCII

  unsigned char temp;
  unsigned char temp2;

  if (*ASCII == 0x30){
    temp = 0x00;
  }
  else if (*ASCII == 0x31){
    temp = 0x10;
  }
  else if (*ASCII== 0x32){
    temp = 0x20;
  }
  else if (*ASCII == 0x33){
    temp = 0x30;
  }
  else if (*ASCII == 0x34){
    temp = 0x40;
  }
  else if (*ASCII == 0x35){
    temp = 0x50;
  }
  else if (*ASCII == 0x36){
    temp = 0x60;
  }
  else if (*ASCII == 0x37){
    temp = 0x70;
  }
  else if (*ASCII == 0x38){
    temp = 0x80;
  }
  else if (*ASCII == 0x39){
    temp = 0x90;
  }
  else{
    return 0x00;
  }

  //take care of second digit
  if (*(ASCII+1) == 0x30){
    temp2 = temp | 0x00;
  }
  else if (*(ASCII+1) == 0x31){
    temp2 = temp | 0x01;
  }
  else if (*(ASCII+1)== 0x32){
    temp2 = temp | 0x02;
  }
  else if (*(ASCII+1) == 0x33){
    temp2 = temp | 0x03;
  }
  else if (*(ASCII+1) == 0x34){
    temp2 = temp | 0x04;
  }
  else if (*(ASCII+1) == 0x35){
    temp2 = temp | 0x05;
  }
  else if (*(ASCII+1) == 0x36){
    temp2 = temp | 0x06;
  }
  else if (*(ASCII+1) == 0x37){
    temp2 = temp | 0x07;
  }
  else if (*(ASCII+1) == 0x38){
    temp2 = temp | 0x08;
  }
  else if (*(ASCII+1) == 0x39){
    temp2 = temp | 0x09;
  }
  else{
    return 0x00;
  }

  return temp2;

} //end ASCII2hex
