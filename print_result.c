#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 

#include "api.h"

#define LINESIZE 50
 
/******************************************** 
 *  Output a binary result 
 * 
 * 
 *  Parameters: 
 *    - fp   File Pointer of output file 
 *    - yb   Address of APIBINARY buffer 
 *    - yn   Number of bytes to be output 
 ********************************************/ 
void printBinary(FILE *fp, APIBINARY *yb, APIWORD yn) 
{ 
   /* line format:                           */ 
   /* line   0    25                         */ 
   /* XXX:  XX XX XX XX XX XX XX XX XXXXXXXX */ 
   static char line[LINESIZE+1]; 
   static char addr[4]; 
   unsigned n=0, z, hexz;
   
	while (n<yn) { 
		for (z=0; z<LINESIZE; z++) line[z]=' '; 

		line[LINESIZE]=addr[0]='\0';   
		z=hexz=0;
		
		while (n < yn && z < 8) { 
			if (z == 0)
				sprintf(addr,"%03X:", n);   
			else   
				hexz++;

			sprintf(&line[hexz], "%02X", yb[n]);   
			hexz += 2;   
			line[hexz] = ' ';   
			line[LINESIZE-8+z] = isprint((int)yb[n]) ? yb[n] : '.';   
			n++;   
			z++;   
		}   
		
		          /*1234567890123456789012345*/ 
		fprintf(fp,"\n                         %s%s",addr,line); 
	}   
} 

/******************************************** 
 *  Output the result field 
 * 
 *  If the displayed File Pointer shows the value NULL 
 *  all results are polled but not output 
 * 
 *  Parameters: 
 *    - fp 
 *      File Pointer of output file 
 * 
 *  Return is APITRUE if correctly output, 
 *  otherwise APIFALSE 
 * 
 ********************************************/ 
void printResultField(FILE *fp) 
{ 
   static APITEXT t[APIMAXTEXT]; 
   static APITEXT text[APIMAXNAME]; 
   static APIBINARY yb[APIMAXBINARY]; 
   APIWORD sets, set, results, result;   
   APIWORD yn, w;   
   APICHAR c;   
   APIBYTE b;   
   APIINTEGER  n;   
   APILONG l;   
   APIDWORD d;   
   APIREAL r;   
   APIRESULTFORMAT format;   

    if (fp != NULL) fprintf(fp, "RESULTFIELD:\n\n");   

    /* Identify the number of results */ 
    if (apiResultSets (&sets)) { 

		/* For all result sets */ 
		for (set=1; (set <= sets) && (apiErrorCode() == EDIABAS_ERR_NONE); set++) { 

			if (fp != NULL) fprintf(fp, "SET %3u:", set);   

			/* Number of results in current result set */ 
			if (apiResultNumber(&results,set)) {   

				/* For all results */ 
				for(result=1; (result <= results) && (apiErrorCode() == EDIABAS_ERR_NONE); result++) {   

					/* Identify result name */ 
					if (apiResultName(text, result, set)) {   

						if (fp != NULL) fprintf(fp,"  %sRESULT %3u: ", (result==1) ? "" : "        ", result); 

						/* Identify result format */   
						if (apiResultFormat(&format, text, set)) {   

							/*Download results by format and output */ 
							switch (format) {   
								case APIFORMAT_CHAR:   
									if (apiResultChar(&c, text, set))   
									   if (fp != NULL) fprintf(fp, "[ CHAR    ]   %s   =   %+d", text,c);   
								break;   
								case APIFORMAT_BYTE:   
									if (apiResultByte(&b, text, set))   
									   if (fp != NULL) fprintf(fp, "[ BYTE    ]   %s   =   %u", text,b);   
								break;
								case APIFORMAT_INTEGER:   
									if (apiResultInt(&n, text, set))
									   if (fp != NULL) fprintf(fp, "[ INTEGER ]   %s   =   %+d", text,n);   
								break;   
								case APIFORMAT_WORD:   
									if (apiResultWord(&w, text, set))   
									   if (fp != NULL) fprintf(fp, "[ WORD    ]   %s   =   %u", text,w);   
								break;   
								case APIFORMAT_LONG:   
									if (apiResultLong(&l, text, set))   
									   if (fp != NULL) fprintf(fp, "[ LONG    ]   %s   =   %ld", text,l);   
								break;
								case APIFORMAT_DWORD:   
									if (apiResultDWord(&d, text, set))   
									   if (fp != NULL) fprintf(fp, "[ DWORD   ]   %s   =   %lu", text,d);   
								break;   
								case APIFORMAT_REAL:   
									if (apiResultReal(&r, text, set))   
									   if (fp != NULL) fprintf(fp, "[ REAL    ]   %s   =   %E", text,r);   
								break;   
								case APIFORMAT_TEXT:   
									if (apiResultText(t,text,set,""))   
									   if (fp != NULL) fprintf(fp, "[ TEXT    ]   %s   =   %c%s%c", text,'"',t,'"');   
								break;   
								case APIFORMAT_BINARY:   
									if (apiResultBinary(yb,&yn, text, set)) {   
									   if (fp != NULL) fprintf(fp, "[ BINARY  ]   %s   =   %u   bytes,", text,yn);   
									   printBinary(fp, yb, yn); 
									} 
								break;
							}

							if (fp != NULL) fprintf(fp, "\n");   
						} 
					} 
				}
			}
		}   
	}   
}

