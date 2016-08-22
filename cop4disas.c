/*
 * cop4disas -- Quick & dirty disassembler for the NatSemi COP4 processor.
 *
 *      Usage: cop4disas [-a address] machine-code-file
 *              The machine-code-file contains binary data, which cop4disas
 *              will interpet as COP4 machine language code.
 *
 *              By default, the starting addres is 0x000, but this can be changed
 *              with the -a option (e.g. cop4disas -a 0x80 file).
 *
 *              Author: Vassilis Prevelakis (vp080808@gmail.com)
 *                      www.series80.org
 *              Date: 22 Aug. 2016
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void abend(char *pnam, char *ers, int ern)
{
        fprintf(stderr, "%s: Error %s. (%d)\n", pnam, ers, ern);
        perror("Aborting");
        exit(ern);
}

#define XX_NOARG        0x4444

#define XX_ADDR         100
#define XX_0ARG         0
#define XX_1ARG         1
#define XX_2ARG         2

#define FIXD(D)                 (((D) == 15) ? 0 : (D)+1)

#define XX_PRT(S)               xx_prt(addr, ii, arg, (S), XX_0ARG)
#define XX_PRT2(S, A)           xx_prt(addr, ii, arg, (S), XX_1ARG, (A))
#define XX_PRT3(S, A, B)        xx_prt(addr, ii, arg, (S), XX_2ARG, (A), (B))

#define XX_PRTX(S, A, B)        xx_prt(addr, ii, arg, (S), XX_ADDR, (A), (B))

void xx_prt(int addr, unsigned int ii, unsigned int arg, char *lbl, int fmt, ...)
{
        va_list argp;
        unsigned int a;

        printf("%03X\t%02X", addr, ii);
        if (arg != XX_NOARG)
                printf(" %02X", arg);
        else
                printf("   ");

        printf("\t\t%s", lbl);
        va_start(argp, fmt);

        switch (fmt) {
        case XX_2ARG:
                a = va_arg(argp, unsigned int);
                printf(" %d,", a);
        case XX_1ARG:
                a = va_arg(argp, unsigned int);
                printf(" %d", a);
        case XX_0ARG:
                break;
        case XX_ADDR:
                a = va_arg(argp, unsigned int);
                a <<= 8;
                a += va_arg(argp, unsigned int);
                printf(" 0x%03X", a);
                break;
        default:
                abend("xx_prt", "unkown format", fmt);
        }
        va_end(argp);

        printf("\n");
}

void process_opcode(int addr, unsigned int ii, unsigned int arg)
{
        switch(ii) {

        case 0x00:      XX_PRT("CLRA"); break;
        case 0x01:      XX_PRT2("SKMBZ", 0); break;
        case 0x02:      XX_PRT("XOR"); break;
        case 0x03:      XX_PRT2("SKMBZ", 2); break;
        case 0x04:      XX_PRT2("XIS", 0); break;
        case 0x05:      XX_PRT2("LD", 0); break;
        case 0x06:      XX_PRT2("X", 0); break;
        case 0x07:      XX_PRT2("XDS", 0); break;

        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:      XX_PRT3("LBI", (ii >> 4) & 0x3, FIXD(ii & 0xF)); break;

        case 0x10:      XX_PRT("CASC"); break;
        case 0x11:      XX_PRT2("SKMBZ", 1); break;
        case 0x12:      XX_PRT("XABR"); break;
        case 0x13:      XX_PRT2("SKMBZ", 3); break;
        case 0x14:      XX_PRT2("XIS", 1); break;
        case 0x15:      XX_PRT2("LD", 1); break;
        case 0x16:      XX_PRT2("X", 1); break;
        case 0x17:      XX_PRT2("XDS", 1); break;

        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F:      XX_PRT3("LBI", (ii >> 4) & 0x3, FIXD(ii & 0xF)); break;

        case 0x20:      XX_PRT("SKC"); break;
        case 0x21:      XX_PRT("SKE"); break;
        case 0x22:      XX_PRT("SC"); break;
        case 0x23:      // 2 byte ***
                        if ((arg & 0xC0) == 0) {
                                // 01rrdddd     LDD             2 byte
                                XX_PRT3("LDD", (arg >> 4) & 0x3, arg & 0xF);
                        } else if ((arg & 0xC0) == 0x80) {
                                // 10rrdddd     XAD             2 byte
                                XX_PRT3("XAD", (arg >> 4) & 0x3, arg & 0xF);
                        } else
                                XX_PRTX("UNDEF OP 23", 0, arg);
                        break;
        case 0x24:      XX_PRT2("XIS", 2); break;
        case 0x25:      XX_PRT2("LD", 2); break;
        case 0x26:      XX_PRT2("X", 2); break;
        case 0x27:      XX_PRT2("XDS", 2); break;
        case 0x28:
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:      XX_PRT3("LBI", (ii >> 4) & 0x3, FIXD(ii & 0xF)); break;

        case 0x30:      XX_PRT("ASC"); break;
        case 0x31:      XX_PRT("ADD"); break;
        case 0x32:      XX_PRT("RC"); break;
        case 0x33:      // 2 byte ***
                        switch (arg) {
                        case 0x01:      XX_PRT2("SKGBZ", 0); break;
                        case 0x03:      XX_PRT2("SKGBZ", 2); break;
                        case 0x11:      XX_PRT2("SKGBZ", 1); break;
                        case 0x13:      XX_PRT2("SKGBZ", 3); break;
                        case 0x28:      XX_PRT("ININ"); break;
                        case 0x29:      XX_PRT("INIL"); break;
                        case 0x2A:      XX_PRT("ING"); break;
                        case 0x2C:      XX_PRT("CQMA"); break;
                        case 0x3C:      XX_PRT("CAMQ"); break;
                        case 0x2E:      XX_PRT("INL"); break;
                        case 0x3E:      XX_PRT("OBD"); break;
                        case 0x3A:      XX_PRT("OMG"); break;
                        default:
                                if (arg >= 0x50 && arg <= 0x5F) {
                                        XX_PRT2("OGI", arg & 0xF);
                                } else if (arg >= 0x60 && arg <= 0x6F) {
                                        XX_PRT2("LEI", arg & 0xF);
                                } else if (arg >= 0x80 && arg <= 0xBF) {
                                        unsigned short d = (arg & 0xF);
                                        // according to the COP400 manual
                                        // the 2nd argument ranges from 1 till 8
                                        if (d >= 1 && d <= 8)
                                                XX_PRT3("LBI", (arg >> 4) & 0x3, d);
                                        else
                                                XX_PRTX("UNDEF OP 33", 0, arg);
                                } else {
                                        XX_PRTX("UNDEF OP 33", 0, arg);
                                }
                        }
                        break;

        case 0x34:      XX_PRT2("XIS", 3); break;
        case 0x35:      XX_PRT2("LD", 3); break;
        case 0x36:      XX_PRT2("X", 3); break;
        case 0x37:      XX_PRT2("XDS", 3); break;

        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3E:
        case 0x3F:      XX_PRT3("LBI", (ii >> 4) & 0x3, FIXD(ii & 0xF)); break;

        case 0x40:      XX_PRT("COMP"); break;
        case 0x41:      XX_PRT("SKT"); break;
        case 0x42:      XX_PRT2("RMB", 2); break;
        case 0x43:      XX_PRT2("RMB", 3); break;
        case 0x44:      XX_PRT("NOP"); break;
        case 0x45:      XX_PRT2("RMB", 1); break;
        case 0x46:      XX_PRT2("SMB", 2); break;
        case 0x47:      XX_PRT2("SMB", 1); break;
        case 0x48:      XX_PRT("RET"); break;
        case 0x49:      XX_PRT("RETSK"); break;
        case 0x4A:      XX_PRT("ADT"); break;
        case 0x4B:      XX_PRT2("SMB", 3); break;
        case 0x4C:      XX_PRT2("RMB", 0); break;
        case 0x4D:      XX_PRT2("SMB", 0); break;
        case 0x4E:      XX_PRT("CBA"); break;
        case 0x4F:      XX_PRT("XAS"); break;
        case 0x50:      XX_PRT("CAB"); break;

        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F:      XX_PRT2("AISC", ii & 0xF); break;

        case 0x60:      // 2 byte ***
        case 0x61:      // 2 byte ***
        case 0x62:      // 2 byte ***
        case 0x63:      // 2 byte ***
                        XX_PRTX("JMP", (ii & 0x3), arg); break;
        case 0x68:      // 2 byte ***
        case 0x69:      // 2 byte ***
        case 0x6A:      // 2 byte ***
        case 0x6B:      // 2 byte ***
                        XX_PRTX("JSR", (ii & 0x3), arg); break;

        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F:      XX_PRT2("STII", ii & 0xF); break;

        case 0xBF:      XX_PRT("LQID"); break;
        case 0xFF:      XX_PRT("JID"); break;

        default:
                if (ii >= 0x80 && ii <= 0xFE) {
                /*
                 * the ROM address of the COP4 is 1024 bytes divided
                 * into 16 pages of 64 bytes each. So page addresses
                 * range from 0 to 15 and offsets within a page from
                 * 000000 to 111111 (0x3F). Pages 2 and 3 are special
                 * because the contain subroutines. The JP and JSRP
                 * opcodes decode differently depending on the page
                 * they are located.
                 * Within pages 2 and 3 opcodes 0x80 - 0xBF
                 *      decode to JP 0x00 to JP 0xBF
                 * While outside pages 2 and 3, opcodes 0x80 - 0xBF
                 *      decode to JSRP 0x80 - 0xBE
                 * in this case the opcode is the address, since
                 * JSRP address locations in page 2, only.
                 */
                        unsigned int pnum;

                        pnum = addr >> 6;       // current page number
                        if ((pnum == 2) || (pnum == 3))
                                XX_PRTX("JP", 0, ii);
                        else {
                                if (ii >= 0x80 && ii <= 0xBE)
                                        XX_PRTX("JSRP", 0, ii);
                                else
                                        XX_PRTX("JP", 0, (pnum << 6) + (ii & 0x3F));
                        }
                } else
                        XX_PRTX("UNDEF OP", 0, ii);
        }
}

void usage(char *pnam, int ern)
{
        fprintf(stderr, "Usage(%d): %s [-a addr] filename\n", ern, pnam);
        exit(-1);
}

int main(int argc, char **argv)
{
        char *pnam;
        char *fname;
        FILE *fd;
        unsigned int    ii, arg;
        int addr = 0;

        pnam = argv[0];

        argv++;
        argc--;
        if (argc > 0)   /* process parameters */
        {
                while (argc > 0 && **argv == '-')
                {
                        switch (*(*argv+1)) {
                        case 'a':
                                addr = strtol(*++argv, NULL, 0);
                                argv++;
                                argc -= 2;
                                break;
                        default:
                                usage(pnam, 2);
                        }
                }
                if (argc > 0)
                        fname = *argv;
                else
                        usage(pnam, 3);
        } else
                usage(pnam, 4);

        if ((fd = fopen(fname, "r")) == NULL)
                abend(pnam, fname, -2);

        while ((ii = fgetc(fd)) != EOF)
        {       
                arg = XX_NOARG;
                switch (ii) {
                case 0x23:      // 2 byte ***
                case 0x33:      // 2 byte ***
                case 0x60:      // 2 byte ***
                case 0x61:      // 2 byte ***
                case 0x62:      // 2 byte ***
                case 0x63:      // 2 byte ***
                case 0x68:      // 2 byte ***
                case 0x69:      // 2 byte ***
                case 0x6A:      // 2 byte ***
                case 0x6B:      // 2 byte ***
                        // two byte instructions, fetch operant
                        if ((arg = fgetc(fd)) == EOF) 
                                abend(pnam, "EOF in the middle of two byte instr.", -3);
                }
                process_opcode(addr, ii, arg);

                addr++;
                if (arg != XX_NOARG)
                        addr++;
        }
}

