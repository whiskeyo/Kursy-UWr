#ifndef _TERMINAL_H_
#define _TERMINAL_H_

int tty_open(void);
void tty_curpos(int fd, int *x, int *y);

/* https://en.wikipedia.org/wiki/ANSI_escape_code#Terminal_output_sequences */

#define ESC "\033"
#define CSI ESC "["

#define CUU(n) CSI #n "A"           /* Cursor Up */
#define CUD(n) CSI #n "B"           /* Cursor Down */
#define CUF(n) CSI #n "C"           /* Cursor Forward */
#define CUB(n) CSI #n "D"           /* Cursor Back */
#define CNL(n) CSI #n "E"           /* Cursor Next Line */
#define CPL(n) CSI #n "F"           /* Cursor Previous Line */
#define CHA(n) CSI #n "G"           /* Cursor Horizontal Absolute */
#define CUP(n, m) CSI #n ";" #m "H" /* Cursor Position */
#define ED(n) CSI #n "J"            /* Erase in Display */
#define EL(n) CSI #n "K"            /* Erase in Line */
#define SU(n) CSI #n "S"            /* Scroll Up Scroll */
#define SD(n) CSI #n "T"            /* Scroll Down Scroll */
#define CPR() CSI "6n"              /* Cursor Position Report */
#define SGR(x) CSI x "m"

#endif /* !_ANSICODES_H_ */
