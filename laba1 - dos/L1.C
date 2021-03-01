#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#define PORT1 0x3F8
#define PORT2 0x2F8

void whatChrctr(unsigned char* ch){
	if(*ch == 13){
	       *ch = '\n';
	       }
	else if(*ch == 8){
		*ch = '\b';
		}
}

void main(int argc, char** argv) {
    unsigned char c = 0;
    unsigned char chrctr = 0;
    int i=0;
    int flag=0;
    outportb(PORT1 + 3, 0x80); /* Set DLAB ON */
    outportb(PORT1 + 0, 0x0C); /* Set the baud rate to 9600 */
    outportb(PORT1 + 1, 0x00); /* Set Baud - Divisor latch HIGH */
    outportb(PORT1 + 3, 0x03); /* 8 bits, no parity, 1 stop */
    outportb(PORT1 + 2, 0xC7); /* FIFO Control Register */
    outportb(PORT1 + 4, 0x0B); /* Turn on DTR, RTS, and OUT2) */

    outportb(PORT2 + 3, 0x80); /* Set DLAB ON */
    outportb(PORT2 + 0, 0x0C); /* Set the baud rate to 9600 */
    outportb(PORT2 + 1, 0x00); /* Set Baud - Divisor latch HIGH */
    outportb(PORT2 + 3, 0x03); /* 8 bits, no parity, 1 stop */
    outportb(PORT2 + 2, 0xC7); /* FIFO Control Register */
    outportb(PORT2 + 4, 0x0B); /* Turn on DTR, RTS, and OUT2) */

    for(i=0;i<24;i++)
	putchar('\n');

    if(argc != 2){
	return ;
	}
    else{
	flag = (strcmp("COM1", argv[1]));
	if(flag==0){
		printf("Working with COM1\n");
		}
	else {
		printf("Working with COM2\n");
		}
	}
    while (chrctr != 27) {  //'escape'keyboard stroke
	c = inportb(PORT1 + 5);
	if (c & 0x01) {
	    chrctr = inportb(PORT1);
	    whatChrctr(&chrctr);
	    if(chrctr == '\b'){
		putchar(chrctr);
		putchar(' ');
		putchar(chrctr);
		}
	    else{
		putchar(chrctr);
		}
	}
	if (kbhit()) {
	    chrctr = getch();
	    whatChrctr(&chrctr);
	    if(chrctr == '\b'){
		putchar(chrctr);
		putchar(' ');
		putchar(chrctr);
		}
	    else{
		putchar(chrctr);
		}
	    outportb(PORT2, chrctr);
	}
    }
}