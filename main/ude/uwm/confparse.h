#ifndef UWM_CONFPARSE_H
#define UWM_CONFPARSE_H

#include <stdio.h>
#ifndef UWM_CONFPARSE_TAB_H
#define UWM_CONFPARSE_TAB_H
#include "confparse.tab.h"
#endif  /* UWM_CONFPARSE_TAB_H */
#include "settings.h"

#define UWM_YY_GLOBAL_CONTEXT		0
#define UWM_YY_MENU_CONTEXT		1
#define UWM_YY_WORKSPACE_CONTEXT	2
#define UWM_YY_EVENT_CONTEXT		3
#define UWM_YY_FUNCTION_CONTEXT		4

struct uwm_yy_ContextStackStruct {
  int type;
  void *context_data;
  const uwm_init_index *data_index;
  unsigned int data_index_size;
  struct uwm_yy_ContextStackStruct *stacked;
};

extern struct uwm_yy_ContextStackStruct *uwm_yy_ContextStack;

struct uwm_yyParseLineStackStruct {
  unsigned int linenumber;
  char *topfilename;
  char *filename;
  FILE *file;
  void *buffer; /* YY_BUFFER_STATE is not yet defined here,
			void * seems to be safe but ugly */
  struct uwm_yyParseLineStackStruct *stacked;
};

/* stack for filename/linenumber-support in FILE and PIPE */
extern struct uwm_yyParseLineStackStruct *uwm_yyParseLineStack;

/* flags */
extern int uwm_yy_LEX_FLAG_Newline_Requested;

/* prototypes */
void uwm_yypush_LineStack(char *file);
struct uwm_yyParseLineStackStruct *uwm_yypop_LineStack();
void uwm_yyinitscanner();

/* prototypes of confibconvertors.c */
void uopt_int_int(YYSTYPE *in, uwm_init_index *out, void *base);
void uopt_int_flt(YYSTYPE *in, uwm_init_index *out, void *base);
void uopt_flt_flt(YYSTYPE *in, uwm_init_index *out, void *base);
void uopt_str_str(YYSTYPE *in, uwm_init_index *out, void *base);

#endif
