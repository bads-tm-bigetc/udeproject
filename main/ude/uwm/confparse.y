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

%union {
  int intval;
#define UWM_YY_INT 0
  double floatval;
#define UWM_YY_FLOAT 1
  char *string;
#define UWM_YY_STRING 2
  struct uwm_yy_ContextStackStruct *uwm_yy_Context;
/***/
#define UWM_YY_TYPENO 3
}

%token FileAtom PipeAtom
%token ItemAtom SubmenuAtom LineAtom WinmenuAtom
%token WorkspaceAtom EventAtom MenuAtom OptionAtom
%token KeystrokeAtom
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
%type <string> Identifier
%type <uwm_yy_Context> FunctionBlock Menu WorkspaceSetting EventSettings

%{
#include <stdio.h>
#include "special.h"

#define UWM_CONFPARSE_TAB_H
#include "confparse.h"

#define OFFSET_OF(STRUCTURE, NAME) ((&(((struct STRUCTURE *)(0L))->NAME)))

/* offsets for global settings */
#define UWM_GLOBAL_OPTION_NR 26
#define GLOBAL_OPTION_STRUCT_LINE(NAME, TYPE) \
        {#NAME,    OFFSET_OF(_uwm_global_settings, NAME),    TYPE}
const uwm_init_index uwm_global_index[UWM_GLOBAL_OPTION_NR] = {
  GLOBAL_OPTION_STRUCT_LINE(BorderWidth, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(TransientBorderWidth, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(TitleHeight, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(FrameBevelWidth, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(BevelWidth, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(MenuXOffset, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(MenuYOffset, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(FrameFlags, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(TitleFont, UWM_S_FONT),
  GLOBAL_OPTION_STRUCT_LINE(Font, UWM_S_FONT),
  GLOBAL_OPTION_STRUCT_LINE(MonoFont, UWM_S_FONT),
  GLOBAL_OPTION_STRUCT_LINE(HighlightFont, UWM_S_FONT),
  GLOBAL_OPTION_STRUCT_LINE(InactiveFont, UWM_S_FONT),
  GLOBAL_OPTION_STRUCT_LINE(StartScript, UWM_S_STRING),
  GLOBAL_OPTION_STRUCT_LINE(StopScript, UWM_S_STRING),
  GLOBAL_OPTION_STRUCT_LINE(ResourceFile, UWM_S_STRING),
  GLOBAL_OPTION_STRUCT_LINE(HexPath, UWM_S_STRING),
  GLOBAL_OPTION_STRUCT_LINE(PlacementStratety, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(PlacementThreshold, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(OpaqueMoveSize, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(MaxWinWidth, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(MaxWinHeight, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(WarpPointerToNewWinH, UWM_S_FLOAT),
  GLOBAL_OPTION_STRUCT_LINE(WarpPointerToNewWinV, UWM_S_FLOAT),
  GLOBAL_OPTION_STRUCT_LINE(SnapDistance, UWM_S_INT),
  GLOBAL_OPTION_STRUCT_LINE(BehaviourFlags, UWM_S_INT)
};

struct uwm_yy_ContextStackStruct *uwm_yy_ContextStack = NULL;
uwm_global_settings global_settings;

typedef void (*ConverterFunction)(YYSTYPE *in, uwm_init_index *out, void *base);
/* conversion table to convert parser datatypes to uwm datatypes
   must be updated when either parser or uwm datatypes are changed */
const ConverterFunction uwm_yy_to_setting_table[UWM_S_TYPENO][UWM_YY_TYPENO] = {
/* to		\ from	UWM_YY_INT	UWM_YY_FLOAT	UWM_YY_STRING */
/* UWM_S_INT	*/ {	uopt_int_int,	NULL,		NULL		},
/* UWM_S_FLOAT	*/ {	uopt_int_flt,	uopt_flt_flt,	NULL		},
/* UWM_S_STRING	*/ {	NULL,		NULL,		uopt_str_str	},
/* UWM_S_FONT  	*/ {	NULL,		NULL,		NULL		}
};
%}

%%

TopLevel : TopLine
         | TopLevel TopLine
	 | error ErrDelim { yyerrok; }

TopLine : AnyLine
        | MenuBlock
	| WorkspaceBlock
	| OptionBlock 
	| EventBlock
	| FileLine ;

AnyLine : PreProc
	| ';' ;

ErrDelim : ';' { uwm_yy_LEX_FLAG_Newline_Requested = 0; }
	 | '\n' 
	 | ';' '\n' ;

/* Deal with linenumber and filename information we get from C preprocessor */
PreProc : PreprocessorAtom Integer StringAtom IntegerGarbage '\n' {
				  uwm_yyParseLineStack->linenumber = $2;
				  if(uwm_yyParseLineStack->topfilename
				     != uwm_yyParseLineStack->filename) {
				    free(uwm_yyParseLineStack->filename);
				  }
				  uwm_yyParseLineStack->filename
				       = strlen($3) 
				         ?$3:uwm_yyParseLineStack->topfilename;
				}
IntegerGarbage : Integer
	       | IntegerGarbage Integer
	       | ;

/* Workspace related settings */

WorkspaceBlock : WorkspaceAtom Integer WorkspaceSetting ;

WorkspaceSetting : {uwm_yy_PushContext(UWM_YY_WORKSPACE_CONTEXT);}
	           OptionSetting {$$ = uwm_yy_PopContext();}

OptionBlock : OptionAtom OptionSetting ;

OptionSetting : OptionLine
              | '{' OptionLines '}' ;

OptionLines : OptionLine
            | OptionLines OptionLine
	    | error ErrDelim { yyerrok; }

OptionLine : AnyLine
	   | IntOption
           | FloatOption
	   | StringOption
	   | FileLine ;

IntOption : Identifier '=' Integer ';' {
		uwm_init_set_option($1, UWM_YY_INT, (YYSTYPE *)&($3));
	}

FloatOption : Identifier '=' FloatVal ';' {
		uwm_init_set_option($1, UWM_YY_FLOAT, (YYSTYPE *)&($3));
	}

StringOption : Identifier '=' StringAtom ';' {
		uwm_init_set_option($1, UWM_YY_STRING, (YYSTYPE *)&($3));
	}

Identifier : IdentifierAtom {$$ = $1 ; }

EventBlock : EventAtom EventSettings ;

EventSettings : {uwm_yy_PushContext(UWM_YY_EVENT_CONTEXT);}
		EventSetting {$$ = uwm_yy_PopContext();}

EventSetting : EventLine
	     | '{' EventLines '}' ;

EventLines : EventLine
	   | EventLines EventLine
	   | error ErrDelim { yyerrok; }

EventLine : AnyLine
	  | KeystrokeAtom Integer ',' Integer ':' FunctionBlock
	  | FileLine ;

FunctionBlock : {uwm_yy_PushContext(UWM_YY_FUNCTION_CONTEXT);}
		FunctionBlock_ {$$ = uwm_yy_PopContext();}

FunctionBlock_ : Function
	       | '{' Functions '}' ;

Functions : Function
	  | Functions Function ;

Function : AnyLine
	 | Identifier '(' FunctionArgs ')' ';'
	 | Identifier '(' ')' ';'
	 | StringAtom ';';

FunctionArgs : FunctionArg
	     | FunctionArgs ',' FunctionArg ;

FunctionArg : StringAtom
	    | Integer
	    | FloatVal ;

MenuBlock : MenuAtom Integer Menu ;

SubmenuBlock : SubmenuAtom StringAtom Menu ;

Menu : {uwm_yy_PushContext(UWM_YY_MENU_CONTEXT);}
       Menu_ {$$ = uwm_yy_PopContext();}

Menu_ : '{' MenuLines '}'
      | WinmenuAtom ';'
      | FileLine ;

MenuLines : MenuLine
          | MenuLines MenuLine
	  | error ErrDelim { yyerrok; }

MenuLine : AnyLine
	 | DrawLine
         | MenuItem
	 | SubmenuBlock
	 | FileLine ;

DrawLine : LineAtom ';' { printf("line\n"); }

MenuItem : ItemAtom StringAtom ':' FunctionBlock ;

FileLine : FileAtom StringAtom ';'
         | PipeAtom StringAtom ';' { printf("FileAction: %s\n", $2); }

/*** more or less standard yacc stuff for arithmetics ***/

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
	| IntegerAtom { $$ = $1 ; }

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
	 | FloatAtom { $$ = $1 ; }

%%

int yyerror(char *s)
{
  uwm_yy_LEX_FLAG_Newline_Requested = 1;
  fprintf(stderr, "UWM: error on line %d of file <%s>: %s\n",
          uwm_yyParseLineStack->linenumber, uwm_yyParseLineStack->filename, s);
}

extern void *YY_CURRENT_BUFFER;

char *types[] = {"Global", "Menu", "Workspace", "Event", "Function"};
void uwm_yy_PushContext(int type)
{
  struct uwm_yy_ContextStackStruct *s;

  s = MyCalloc(1, sizeof(struct uwm_yy_ContextStackStruct));

  s->type = type;
  s->stacked = uwm_yy_ContextStack;
  switch(type) {
    case UWM_YY_GLOBAL_CONTEXT:
         s->context_data = &global_settings;
	 s->data_index = uwm_global_index;
	 s->data_index_size = UWM_GLOBAL_OPTION_NR;
         break;
    case UWM_YY_MENU_CONTEXT:
    case UWM_YY_WORKSPACE_CONTEXT:
    case UWM_YY_EVENT_CONTEXT:
    case UWM_YY_FUNCTION_CONTEXT:
         s->context_data = NULL;
	 s->data_index = NULL;
	 s->data_index_size = 0;
         break;
  }
  uwm_yy_ContextStack = s;
}

struct uwm_yy_ContextStackStruct *uwm_yy_PopContext()
{
  struct uwm_yy_ContextStackStruct *s;

  if(s = uwm_yy_ContextStack) {
    uwm_yy_ContextStack = s->stacked;

    if(uwm_yy_ContextStack) {
    }
  }
  return(s);
}

struct uwm_yy_ContextStackStruct *uwm_yy_PeekContext()
{
  return(uwm_yy_ContextStack);
}

int uwm_yyparse_wrapper(char *initialfilename)
{
  uwm_yyinitscanner();
  uwm_yypush_LineStack(initialfilename);
  uwm_yy_PushContext(UWM_YY_GLOBAL_CONTEXT);
  return yyparse();
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
    fprintf(stderr, "UWM: Invalid configuration option on line %d of %s: %s\n",
            uwm_yyParseLineStack->linenumber,
	    uwm_yyParseLineStack->filename, name);
  } else if(converter = uwm_yy_to_setting_table[r->type][type]) {
    converter(value, (uwm_init_index *)r, ctxt->context_data);
  } else {
    fprintf(stderr,
	    "UWM: Invalid datatype for option \"%s\" on line %d of %s.\n",
            name, uwm_yyParseLineStack->linenumber,
	    uwm_yyParseLineStack->filename, name);
  }
}
