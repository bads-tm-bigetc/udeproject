#ifndef UWM_CONFPARSE_H
#define UWM_CONFPARSE_H

#include <stdio.h>
#ifndef UWM_CONFPARSE_TAB_H
#define UWM_CONFPARSE_TAB_H
#include "confparse.tab.h"
#endif  /* UWM_CONFPARSE_TAB_H */
#include "settings.h"

/* configuration datatypes */
/* ATTENTION: ONLY CHANGE TOGETHER WITH CONVERSION TABLE IN confparse.y */
#define UWM_S_INT 0
#define UWM_S_FLOAT 1
#define UWM_S_STRING 2
#define UWM_S_FONT 3
#define UWM_S_COLOR 4
#define UWM_S_PIXMAP 5

#define UWM_S_TYPENO 6

typedef struct _uwm_init_index {
  char *name;
  long offset;
  int type;
  char *default_val_string;
} uwm_init_index;

#define UWM_DEFAULT_NAME_LENGTH 100
extern char uwm_default_ws_name[UWM_DEFAULT_NAME_LENGTH];
#define UWM_GLOBAL_OPTION_NR 27
extern const uwm_init_index uwm_global_index[UWM_GLOBAL_OPTION_NR];
#define UWM_WORKSPACE_OPTION_NR 21
extern uwm_init_index uwm_workspace_index[UWM_WORKSPACE_OPTION_NR];

/* convertors */
typedef char*(*ConverterFunction)(YYSTYPE *in, const uwm_init_index *out,
				  void *base);
extern const ConverterFunction
             uwm_yy_to_setting_table[UWM_S_TYPENO][UWM_YY_TYPENO];

/* prototypes for configconvertors.c */
char *uopt_int_int(YYSTYPE *in, const uwm_init_index *out, void *base);
char *uopt_int_flt(YYSTYPE *in, const uwm_init_index *out, void *base);
char *uopt_flt_flt(YYSTYPE *in, const uwm_init_index *out, void *base);
char *uopt_str_str(YYSTYPE *in, const uwm_init_index *out, void *base);
char *uopt_str_fnt(YYSTYPE *in, const uwm_init_index *out, void *base);
char *uopt_str_col(YYSTYPE *in, const uwm_init_index *out, void *base);
char *uopt_str_pix(YYSTYPE *in, const uwm_init_index *out, void *base);
char *MultiplyColor(char *color, double factor);

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

#endif

