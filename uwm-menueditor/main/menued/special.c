#include "special.h"
#include "gui/xtools.h"
#include <sys/stat.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

extern char home[256];
extern char udedir[256];

char *ReadQuoted(FILE *f) /* allocs mem and writes string to it*/
{
	char c, text[257], *p;
	int i;

	text[0] = '\0';
	do {
		if(EOF==fscanf(f,"%c",&c)) return(NULL);
	} while(c!='"');
	for(i=0;i<256;i++) {
		if(EOF==(c = fgetc(f))) return(NULL); /* unfinished quotation */
		if(c=='\\') {
			if(EOF==(c = fgetc(f))) return(NULL); /* allow "s and \s */
		} else if(c=='"') break;
		text[i] = c;
	}
	text[i] = '\0';
	p=(char*)malloc(sizeof(char)*(strlen(text)+1));
	strcpy(p,text);
	return(p);
}

void string_print (FILE* out,char *str) /*prints string str replacing " and \ with \" and \\ */
{
	int i;
	for(i=0;str[i]!='\0';i++)
	{
		if((str[i]=='\\')||(str[i]=='"'))
			fputc('\\',out);
		fputc(str[i],out);
	}
}

void *MyCalloc(size_t n,size_t s)
{
  void *ret;
  if(!(ret=malloc(n*s)))
  {
	  fprintf(stderr,"Fatal: couldn;t allocate memory\n");
	  exit(OUT_OF_MEMORY);
  }
  return(ret);
}

char *MyOpen(char *name)
{
  char *temp = NULL;
  struct stat stats;
#define str3(T, A, B, C) if(T) free(T); T = MyCalloc(1 + strlen(A) + strlen(B) + strlen(C), sizeof(char)); sprintf(T, "%s%s%s", A, B, C)
  str3(temp, home, "/.ude/config/", name);

  if(stat(temp,&stats)){
    str3(temp, udedir, "config/", name);
    if(stat(temp,&stats)) {
      str3(temp, name, "", "");
      if(stat(temp,&stats)){
        return(NULL);
      }
    }
  }
#undef str3

  return temp;
}
