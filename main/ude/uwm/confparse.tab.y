/* yacc file for generation of the ude config file parser */
/* ########################################################################

   uwm - THE ude WINDOW MANAGER

   ########################################################################

   Copyright (c) : Christian Ruppert

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   ######################################################################## */

%{
#include <stdio.h>
#include "special.h"
#include "nodes.h"
#include "settings.h"
#include "conf_func.h"
%}

/* 124 shift/reduce conflicts expected:					 *
      - 123 are caused by the empty GenericLine rule which permits the	 *
 |	user to have empty { } blocks.					 *
 |    - 1 is caused by the empty IntegerGarbage rule which is necessary	 *
 V	because cpp can also give 0 flags.				 */
%expect 124

%union {
  int intval;
#define UWM_YY_INT 0
  double floatval;
#define UWM_YY_FLOAT 1
  char *string;
#define UWM_YY_STRING 2
/*** end of config file data types ***/
#define UWM_YY_TYPENO 3
/*** what follows are internal data types ***/
  cf_func_call *function;
  NodeList *funclist;
/*  struct uwm_yy_ContextStackStruct *uwm_yy_Context;
***/
}

%token FileAtom PipeAtom
%token ItemAtom MenuAtom SubmenuAtom LineAtom WinmenuAtom
%token OptionAtom WorkspaceAtom LightOfAtom ShadowOfAtom
%token EventAtom KeystrokeAtom ButtonAtom OpenAtom
%token CloseAtom IconifyAtom DeiconifyAtom
%token WindowAtom /* MessageAtom AskAtom */ QuitAtom
%token FindAtom OrAtom AnyAtom
%token NextAtom PrevAtom NewAtom AppAtom
%token IconicAtom UniconicAtom VisibleAtom
%token /* PropertyAtom */ NoneAtom
%token DragPosAtom DragSizeAtom HexMenuAtom
%token SetFocusAtom ShowAtom RaiseAtom LowerAtom
%token MaxAtom VMaxAtom HMaxAtom DemaxAtom
%token ResizeAtom SetSizeAtom ReposAtom SetPosAtom KillAtom
%token PreprocessorAtom
%token <string> IdentifierAtom StringAtom
%token <intval> IntegerAtom
%token <floatval> FloatAtom
%right '='
%left '|'
%left '&'
%left '+' '-'
%left '*' '/'
%left NEGATION
%type <intval> Integer
%type <floatval> FloatVal
/*%type <uwm_yy_Context> FunctionBlock Menu WorkspaceSetting EventSettings*/
%type <string> String
%type <function> AnyFunction WinFunction AnyWinAction
%type <function> WinSpecifier WinLimit WinLimitBlocks
%type <function> WinFind WinFindBlocks WinFindDefault
%type <funclist> FunctionBlock WinActionBlock

%{
#define UWM_CONFPARSE_TAB_H
#include "confparse.h"
#include "uwm.h"

void uwm_yy_PushContext(int type, void *data);
void uwm_yy_PopContext();
struct uwm_yy_ContextStackStruct *uwm_yy_PeekContext();
void uwm_init_set_option(char *name, int type, YYSTYPE *value);

#define OFFSET_OF(STRUCTURE, NAME) ((long)(&(((struct STRUCTURE *)(0L))->NAME)))
/* offsets for global settings */
const uwm_init_index uwm_global_index[UWM_GLOBAL_OPTION_NR] = {
#define GO__DEFINITIONS
#include "settings_global_list.h"
};

char uwm_default_ws_name[UWM_DEFAULT_NAME_LENGTH];
uwm_init_index uwm_workspace_index[UWM_WORKSPACE_OPTION_NR] = {
#define WO__DEFINITIONS
#include "settings_workspace_list.h"
};
#undef WORKSPACE_OPTION_STRUCT_LINE

struct uwm_yy_ContextStackStruct *uwm_yy_ContextStack = NULL;

/* conversion table to convert parser datatypes to uwm datatypes
   must be updated when either parser or uwm datatypes are changed */
const ConverterFunction uwm_yy_to_setting_table[UWM_S_TYPENO][UWM_YY_TYPENO] = {
/* to		\ from	UWM_YY_INT	UWM_YY_FLOAT	UWM_YY_STRING */
/* UWM_S_INT	*/ {	uopt_int_int,	NULL,		NULL		},
/* UWM_S_FLOAT	*/ {	uopt_int_flt,	uopt_flt_flt,	NULL		},
/* UWM_S_STRING	*/ {	NULL,		NULL,		uopt_str_str	},
/* UWM_S_FONT  	*/ {	NULL,		NULL,		uopt_str_fnt	},
/* UWM_S_COLOR 	*/ {	NULL,		NULL,		uopt_str_col	},
/* UWM_S_PIXMAP	*/ {	NULL,		NULL,		uopt_str_pix	}
};
int uwm_yy_add_brace=0;
#define YYERROR_VERBOSE
%}

%%

TopLevel : TopLine
	 | TopLevel TopLine ;

TopLine : GenericLine
	| AnyTop ';' ;

GenericLine : /* nothing */
	    | PreProc
	    | FileLine
	    | ';'
	    | error ErrDelim { yyerrok; } ;

ErrDelim : ';' { uwm_yy_LEX_FLAG_Newline_Requested = 0; }
	 | '\n'
	 | ';' '\n' ;

AnyTop: MenuBlock
      | WorkspaceBlock
      | OptionBlock
      | EventBlock ;

String : StringAtom
       | LightOfAtom '(' String ',' FloatVal ')' {
				$$ = MultiplyColor($3, $5);
				free($3); }
       | ShadowOfAtom '(' String ',' FloatVal ')' {
				$$ = MultiplyColor($3, ((double)1.0)/$5);
				free($3); } ;

/* Deal with linenumber and filename information we get from C preprocessor */
PreProc : PreprocessorAtom Integer StringAtom IntegerGarbage '\n' {
				uwm_yyParseLineStack->linenumber = $2;
				if(uwm_yyParseLineStack->topfilename
				   != uwm_yyParseLineStack->filename) {
				  free(uwm_yyParseLineStack->filename);
				}
				uwm_yyParseLineStack->filename
				  = (strlen($3) && strcmp($3, "<stdin>"))
				    ? $3 : uwm_yyParseLineStack->topfilename; } ;

IntegerGarbage : IntegerAtom { }
	       | IntegerGarbage IntegerAtom { }
	       | /* nothing */ ;

WorkspaceBlock : WorkspaceAtom Integer
		 { uwm_yy_PushContext(UWM_YY_WORKSPACE_CONTEXT, &$2); }
		 OptionSetting { uwm_yy_PopContext(); } ;

OptionBlock : OptionAtom OptionSetting ;

OptionSetting : AnyOption
              | '{' OptionLines '}' ;

OptionLines : OptionLine
	    | OptionLines OptionLine ;

OptionLine : GenericLine
	   | AnyOption ';' ;

AnyOption: IntOption
	 | FloatOption
	 | StringOption ;

IntOption : IdentifierAtom '=' Integer {
		uwm_init_set_option($1, UWM_YY_INT, (YYSTYPE *)&($3));
	} ;

FloatOption : IdentifierAtom '=' FloatVal {
		uwm_init_set_option($1, UWM_YY_FLOAT, (YYSTYPE *)&($3));
	} ;

StringOption : IdentifierAtom '=' String {
		uwm_init_set_option($1, UWM_YY_STRING, (YYSTYPE *)&($3));
	} ;

EventBlock : EventAtom { uwm_yy_PushContext(UWM_YY_EVENT_CONTEXT, NULL); }
	     EventDescriptor ':' FunctionBlock { uwm_yy_PopContext(); } ;

EventDescriptor : KeystrokeAtom Integer ',' Integer
		| ButtonAtom Integer ',' Integer ;

FunctionBlock : { uwm_yy_PushContext(UWM_YY_FUNCTION_CONTEXT, NULL); }
		FunctionBlock_ { $$ = uwm_yy_PeekContext()->context_data;
				 uwm_yy_PopContext(); } ;

FunctionBlock_ : AnyFunction {
		 NodeAppend(uwm_yy_PeekContext()->context_data, $1); }
	       | '{' Functions '}' ;

Functions : Function
	  | Functions Function ;

Function : GenericLine
	 | AnyFunction ';' {
		 NodeAppend(uwm_yy_PeekContext()->context_data, $1); } ;

AnyFunction : WorkspaceFunction { $$ = NULL; }
	    | WinFunction { $$ = $1; }
	    | String {		/* shell command */
		$$ = MyCalloc(1, sizeof(cf_func_call));
		$$->args.shell = $1;
		$$->func = cf_Shell; }
	    | MenuAtom Integer { $$ = NULL; }
	    | QuitAtom {
		$$ = MyCalloc(1, sizeof(cf_func_call));
		$$->func = cf_Quit; }
/*	    | MessageAtom { }
	    | AskAtom { } */ ;

WorkspaceSpecifier : NextAtom { }
		   | PrevAtom { }
		   | String { }
		   | NewAtom { }
		   | CloseAtom { } ;

WorkspaceFunction : WorkspaceAtom WorkspaceSpecifier { } ;

AnySpecifier : AnyAtom | { } ; /* No specifier is the same as ANY */

WinSpecifier : AnySpecifier {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_AnyWindow; }
	     | String {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->args.LimitWin.args.name = $1;
				$$->func = cf_NameWindow; }
	     | AppAtom String {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->args.LimitWin.args.application = $2;
				$$->func = cf_AppWindow; }
	     | NextAtom AnyAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_NextWindow; }
	     | PrevAtom AnyAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_PrevWindow; }
	     | NextAtom VisibleAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_NextVisibleWindow; }
	     | PrevAtom VisibleAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_PrevVisibleWindow; }
	     | NextAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_NextWSWindow; }
	     | PrevAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_PrevWSWindow; }
	     | UniconicAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_UniconicWindow; }
	     | IconicAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_IconicWindow; }
	     | VisibleAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_VisibleWindow; }
	     | NoneAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_NoneWindow; } ;

AnyWinAction : DragPosAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_DragPos; }
	     | DragSizeAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_DragSize; }
	     | HexMenuAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_HexMenu; }
	     | SetFocusAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_SetFocus; }
	     | ShowAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_Show; }
	     | RaiseAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_Raise; }
	     | LowerAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_Lower; }
	     | MaxAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_Max; }
	     | VMaxAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_VMax; }
	     | HMaxAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_HMax; }
	     | DemaxAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_Demax; }
	     | ResizeAtom '(' Integer ',' Integer ')' {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->args.Resize.x = $3;
				$$->args.Resize.y = $5;
				$$->func = cf_Resize; }
	     | SetSizeAtom '(' Integer ',' Integer ')' {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->args.SetSize.x = $3;
				$$->args.SetSize.y = $5;
				$$->func = cf_SetSize; }
	     | ReposAtom '(' Integer ',' Integer ')' {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->args.Repos.x = $3;
				$$->args.Repos.y = $5;
				$$->func = cf_Repos; }
	     | SetPosAtom '(' Integer ',' Integer ')' {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->args.SetPos.x = $3;
				$$->args.SetPos.y = $5;
				$$->func = cf_SetPos; }
	     | CloseAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_Close; }
	     | IconifyAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_Iconify; }
	     | KillAtom {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_Kill; }
	     | WorkspaceFunction {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_WorkspaceWin; } ;

WinAction : GenericLine
	  | AnyWinAction ';' {
		 NodeAppend(uwm_yy_PeekContext()->context_data, $1); } ;

WinActions : WinAction
	   | WinActions WinAction ;

WinActionBlock : { uwm_yy_PushContext(UWM_YY_FUNCTION_CONTEXT, NULL); }
		WinActionBlock_ { $$ = uwm_yy_PeekContext()->context_data;
				  uwm_yy_PopContext(); } ;

WinActionBlock_ : AnyWinAction {
		 	NodeAppend(uwm_yy_PeekContext()->context_data, $1); }
		 | '{' WinActions '}' ;

WinLimitBlocks : WinLimit { $$ = $1; }
	       | WinLimitBlocks OrAtom WinLimit {
				cf_func_call *t;
				$$ = $1;
				t = $$;
				while(t->args.LimitWin.else_function)
				  t = t->args.LimitWin.else_function;
				t->args.LimitWin.else_function = $3; } ;

WinLimit : WinSpecifier WinActionBlock {
				$$ = $1;
				$$->args.LimitWin.functions = $2;
				$$->args.LimitWin.mode = CF_MODE_WIN_LIMIT;
				$$->args.LimitWin.else_function = NULL; } ;

WinFindDefault : /* no default action */ { $$ = NULL; }
	       | OrAtom FunctionBlock {
				$$ = MyCalloc(1, sizeof(cf_func_call));
				$$->func = cf_AnyWindow;
				$$->args.LimitWin.functions = $2; } ;

WinFindBlocks : WinFind { $$ = $1; }
	      | WinFindBlocks OrAtom WinFind {
				cf_func_call *t;
				$$ = $1;
				t = $$;
				while(t->args.LimitWin.else_function)
				  t = t->args.LimitWin.else_function;
				t->args.LimitWin.else_function = $3; } ;

WinFind : FindAtom WinSpecifier WinActionBlock {
				$$ = $2;
				$$->args.LimitWin.functions = $3;
				$$->args.LimitWin.mode = CF_MODE_WIN_FIND;
				$$->args.LimitWin.else_function = NULL; } ;

WinFunction : WindowAtom WinLimitBlocks { $$ = $2; }
	    | WindowAtom WinFindBlocks WinFindDefault {
				cf_func_call *t;
				$$ = $2;
				t = $$;
				while(t->args.LimitWin.else_function)
				  t = t->args.LimitWin.else_function;
				t->args.LimitWin.else_function = $3; } ;

MenuBlock : MenuAtom Integer Menu ;

SubmenuBlock : SubmenuAtom String Menu ;

Menu : { uwm_yy_PushContext(UWM_YY_MENU_CONTEXT, NULL); }
       Menu_ { uwm_yy_PopContext(); } ;

Menu_ : '{' MenuLines '}'
      | WinmenuAtom ;

MenuLines : MenuLine
	  | MenuLines MenuLine ;

MenuLine : GenericLine
	 | AnyMenu ';' ;

AnyMenu : DrawLine
	| MenuItem
	| SubmenuBlock ;

DrawLine : LineAtom { } ;

MenuItem : ItemAtom String ':' FunctionBlock ;

FileLine : FileAtom String ';' { uwm_yypush_LineStack($2, 0); }
         | PipeAtom String ';' { uwm_yypush_LineStack($2, 1); } ;

/*** more or less standard yacc stuff for arithmetics ***/
/*
Integer: IntegerAtom { $$ = $1 ; } ;
FloatVal : FloatAtom { $$ = $1 ; } ;
*/
Integer : '(' Integer ')' { $$ = $2 ; }
	| Integer '+' Integer { $$ = $1 + $3 ; }
	| Integer '-' Integer { $$ = $1 - $3 ; }
	| Integer '*' Integer { $$ = $1 * $3 ; }
	| Integer '/' Integer { if($3 != 0) {
				  $$ = $1 / $3 ;
				} else {
				  yyerror("Division by Zero");
				  YYERROR ;
				}
			      }
	| Integer '|' Integer { $$ = $1 | $3 ; }
	| Integer '&' Integer { $$ = $1 & $3 ; }
	| '-' Integer %prec NEGATION { $$ = - $2 ; }
	| IntegerAtom { $$ = $1 ; } ;

FloatVal : '(' FloatVal ')' { $$ = $2 ; }
	 | FloatVal '+' FloatVal { $$ = $1 + $3 ; }
	 | Integer '+' FloatVal { $$ = $1 + $3 ; }
	 | FloatVal '+' Integer { $$ = $1 + $3 ; }
	 | FloatVal '-' FloatVal { $$ = $1 - $3 ; }
	 | Integer '-' FloatVal { $$ = $1 - $3 ; }
	 | FloatVal '-' Integer { $$ = $1 - $3 ; }
	 | FloatVal '*' FloatVal { $$ = $1 * $3 ; }
	 | Integer '*' FloatVal { $$ = $1 * $3 ; }
	 | FloatVal '*' Integer { $$ = $1 * $3 ; }
	 | FloatVal '/' FloatVal { if($3 != 0) {
				  $$ = $1 / $3 ;
				} else {
				  yyerror("Division by Zero");
				  YYERROR ;
				}
			      }
	 | Integer '/' FloatVal { if($3 != 0) {
				  $$ = $1 / $3 ;
				} else {
				  yyerror("Division by Zero");
				  YYERROR ;
				}
			      }
	 | FloatVal '/' Integer { if($3 != 0) {
				  $$ = $1 / $3 ;
				} else {
				  yyerror("Division by Zero");
				  YYERROR ;
				}
			      }
	 | '-' FloatVal %prec NEGATION { $$ = - $2 ; }
	 | FloatAtom { $$ = $1 ; } ;

%%

int yyerror(char *s)
{
  uwm_yy_LEX_FLAG_Newline_Requested = 1;
  if(uwm_yyParseLineStack) {
    fprintf(stderr, "UWM: error on line %d of file %s: %s\n",
	    uwm_yyParseLineStack->linenumber,
	    uwm_yyParseLineStack->filename, s);
  } else {
    fprintf(stderr, "UWM: error on end of configuration files: %s\n", s);
  }
}

extern void *YY_CURRENT_BUFFER;

void uwm_yy_PushContext(int type, void *data)
{
  struct uwm_yy_ContextStackStruct *s;
  int wsno;

  s = MyCalloc(1, sizeof(struct uwm_yy_ContextStackStruct));

  s->type = type;
  s->stacked = uwm_yy_ContextStack;
  switch(type) {
    case UWM_YY_GLOBAL_CONTEXT:
         s->context_data = settings.global_settings;
	 s->data_index = uwm_global_index;
	 s->data_index_size = UWM_GLOBAL_OPTION_NR;
         break;
    case UWM_YY_WORKSPACE_CONTEXT:
	 wsno = *((int *)data);
	 if(wsno >= settings.workspace_settings_count) {
	   int a;
	   settings.workspace_settings = realloc(settings.workspace_settings,
			(wsno + 1) * sizeof(*(settings.workspace_settings)));
	   if(!settings.workspace_settings) {
	     fprintf(TheScreen.errout, "Out of memory for workspaces.\n");
	     exit(-1);
	   }
	   for(a = settings.workspace_settings_count; a <= wsno; a++) {
	     settings.workspace_settings[a] = NULL;
	   }
	   settings.workspace_settings_count = wsno + 1;
	 }
	 if(!settings.workspace_settings[wsno]) {
	   settings.workspace_settings[wsno]
				  = MyCalloc(1, sizeof(uwm_workspace_settings));
	   memset(settings.workspace_settings[wsno], 0,
		  sizeof(uwm_workspace_settings));
	 }
	 s->context_data = settings.workspace_settings[wsno];
	 s->data_index = uwm_workspace_index;
	 s->data_index_size = UWM_WORKSPACE_OPTION_NR;
	 break;
    case UWM_YY_FUNCTION_CONTEXT:
	 s->context_data = NodeListCreate(); /* list of functions to call */
	 s->data_index = NULL;
	 s->data_index_size = 0;
	 break;
    case UWM_YY_EVENT_CONTEXT:
    case UWM_YY_MENU_CONTEXT:
         s->context_data = NULL;
	 s->data_index = NULL;
	 s->data_index_size = 0;
         break;
  }
  uwm_yy_ContextStack = s;
}

void uwm_yy_PopContext()
{
  struct uwm_yy_ContextStackStruct *s;

  if(s = uwm_yy_ContextStack) {
    uwm_yy_ContextStack = s->stacked;
    free(s);
  }
}

struct uwm_yy_ContextStackStruct *uwm_yy_PeekContext()
{
  return(uwm_yy_ContextStack);
}

int uwm_yyparse_wrapper(char *initialfilename)
{
  int retval;
  uwm_yyinitscanner();
  if(uwm_yypush_LineStack(initialfilename, 0)) return(-1);
  uwm_yy_PushContext(UWM_YY_GLOBAL_CONTEXT, NULL);
  return(yyparse() || uwm_yy_ContextStack);
}

void uwm_init_set_option(char *name, int type, YYSTYPE *value)
{
  int a;
  struct uwm_yy_ContextStackStruct *ctxt;
  const uwm_init_index *r;
  ConverterFunction converter;

  r = NULL;
  ctxt = uwm_yy_PeekContext();
  for(a = 0; a < ctxt->data_index_size; a++) {
    if(!strcmp(name, ctxt->data_index[a].name)) {
      r = &(ctxt->data_index[a]);
      break;
    }
  }
  if(!r) {
    fprintf(TheScreen.errout,
	    "UWM: Invalid configuration option on line %d of file %s: %s\n",
            uwm_yyParseLineStack->linenumber,
	    uwm_yyParseLineStack->filename, name);
  } else if(converter = uwm_yy_to_setting_table[r->type][type]) {
    char *error;
    if(error = converter(value, (uwm_init_index *)r, ctxt->context_data)) {
      fprintf(TheScreen.errout,
              "UWM: Error in config file %s on line %d: %s",
              uwm_yyParseLineStack->filename,
              uwm_yyParseLineStack->linenumber, error);
    }
  } else {
    fprintf(TheScreen.errout,
	    "UWM: Invalid datatype for option \"%s\" on line %d of file %s.\n",
            name, uwm_yyParseLineStack->linenumber,
	    uwm_yyParseLineStack->filename);
  }
  free(name);
}
