/* lex file for generation of the ude config file lexical analyser */
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

%option noyywrap
%Start Quoted Unquoted
%array
%{
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "special.h"
#include "confparse.h"
#include "uwm.h"

extern UDEScreen TheScreen;

void uwm_yydrop(unsigned int n);

#define KEYWORDS 45
const struct ude_configfile_keys_struct {
  char *keyword;
  int   token;
} ude_configfile_keywords[KEYWORDS] = {
/* File I/O */
	{"FILE", FileAtom},		/* Include a file */
	{"PIPE", PipeAtom},		/* Read more from a pipe */
/* Menu Definition */
	{"ITEM", ItemAtom},		/* Define selectable menu item */
	{"MENU", MenuAtom},		/* Define a Top level menu */
	{"SUBMENU", SubmenuAtom},	/* Define a Submenu */
	{"LINE", LineAtom},		/* Seperation line between items */
	{"WINMENU", WinmenuAtom},	/* Include windows menu as submenu */
/* Global and Workspace Options */
	{"OPTION", OptionAtom},		/* Global setting */
	{"WORKSPACE", WorkspaceAtom},	/* Workspace specific setting */
	{"LIGHT_OF", LightOfAtom},	/* Calculate color of light bevel */
	{"SHADOW_OF", ShadowOfAtom},	/* Calculate color of shadow bevel */
/* Event definitions */
	{"EVENT", EventAtom},		/* Define event */
	{"KEYSTROKE", KeystrokeAtom},	/* Keyboard */
	{"BUTTON", ButtonAtom},		/* Mouse button */
	{"OPEN", OpenAtom},		/* Window Withdrawn -> Any */
	{"CLOSE", CloseAtom},		/* Window Any -> Withdrawn */
	{"ICONIFY", IconifyAtom},	/* Window Any -> Iconic */
	{"DEICONIFY", DeiconifyAtom},	/* Window Iconic -> Normal */
/* Top level actions */
	{"WINDOW", WindowAtom},		/* Specify window */
	{"FIND", FindAtom},		/* Window command grammar */
	{"OR", OrAtom},			/* Window command grammar */
#if 0
   the following atom is used in this context but defined above:
	{"WORKSPACE", WorkspaceAtom},	/* Go to workspace */
#endif
#if 0
	{"MESSAGE", MessageAtom},	/* Display Message */
	{"ASK", AskAtom},		/* Display Requester */
#endif
	{"QUIT", QuitAtom},		/* Exit gracefully */
/* Window and workspace specifications */
	{"ANY", AnyAtom},		/* Any window, whatever is selected */
	{"NEXT", NextAtom},		/* Next window or None */
	{"PREVIOUS", PrevAtom},		/* Previous window or None */
	{"NEW", NewAtom},		/* Previous window or None */
	{"APPLICATION", AppAtom},	/* WM_CLASS */
	{"STATE", StateAtom},		/* WM_STATE */
#if 0
	{"PROPERTY", PropertyAtom},	/* Compare property value */
#endif
	{"NONE", NoneAtom},		/* no window/deselect if selected */
/* Interactive window actions */
	{"DRAG_POSITION", DragPosAtom},	/* Use mouse to reposition window */
	{"DRAG_SIZE", DragSizeAtom},	/* Use mouse to resize window */
	{"HEX_MENU", HexMenuAtom},	/* Display HexMenu */
/* Noninteractive window actions */
	{"SET_FOCUS", SetFocusAtom},	/* Give Window the focus */
	{"SHOW", ShowAtom},		/* Deiconify, go to ws and raise win */
	{"RAISE", RaiseAtom},		/* Raise window */
	{"LOWER", LowerAtom},		/* Lower window */
	{"MAXIMIZE", MaxAtom},		/* Maximize window in X and Y */
	{"VERTICAL_MAXIMIZE", VMaxAtom},/* Maximize window in Y only */
	{"HORIZONTAL_MAXIMIZE", HMaxAtom},/* Maximize window in X only */
	{"DEMAXIMIZE", DemaxAtom},	/* Restore original size */
	{"RESIZE", ResizeAtom},		/* Add or subtract from window size */
	{"SET_SIZE", SetSizeAtom},	/* Set absolute window size */
	{"REPOSITION", ReposAtom},	/* Add or subtract from position */
	{"SET_POSITION", SetPosAtom},	/* Set absolute window position */
#if 0
   the following atoms are used in this context but defined above:
	{"CLOSE", CloseAtom},		/* Close window */
	{"ICONIFY", IconifyAtom},	/* Iconify window */
#endif
	{"KILL", KillAtom},		/* XKill window */
};

struct uwm_yyParseLineStackStruct *uwm_yyParseLineStack = NULL;

/* flags */
int uwm_yy_LEX_FLAG_Newline_Requested = 0;
%}
%%
%{
BEGIN Unquoted;
%} 

\$\{[^$}]*\}		{
			char *s, *t;
			int a;
			uwm_yydrop(1);
			s = strrchr(yytext, '$');
			t = getenv(&s[2]);
			uwm_yydrop(yyleng - ((s - yytext) / sizeof(char)));
			if(t) {
			  for(a = (strlen(t) - 1); a >= 0; a--) {
			      unput(t[a]);
			  }
			}
			yymore();
			}
<Unquoted>["]		{          /* text value */
			BEGIN Quoted;
			}
<Quoted>[$]		{
			yymore();
			}
<Quoted>[^\\\n$"]*[\\\n$"] 	{          /* text value */
			switch(yytext[yyleng - 1]) {
			case '\\' :
			  yytext[yyleng - 1] = input();
			  yymore();
			  break;
			case '"' :
			  uwm_yydrop(1);
			  BEGIN Unquoted;
			  yylval.string = MyStrdup(yytext);
			  return(StringAtom);
			case '\n' :
			  uwm_yyParseLineStack->linenumber++;
			  yymore();
			  break;
			case '$' :
			  yyless(yyleng - 1);
			  yymore();
			  break;
			}
			}
<Unquoted>0[xX][0-9a-fA-F]+[^0-9a-fA-F.pP] |
<Unquoted>[1-9][0-9]*[^0-9.eEpP] |
<Unquoted>0[0-7]*[^0-9.eEpP] {  /* integer value */
			yyless(yyleng - 1);
			yylval.intval = atoi(yytext);
			return(IntegerAtom);
			}
<Unquoted>0[Xx][0-9a-fA-F]+[pP][-+]?[0-9]+[^0-9] |
<Unquoted>[0-9]+(\.[0-9]+)?([Ee][-+]?[0-9]+)?[^0-9] { /* float value */
			yyless(yyleng - 1);
			yylval.floatval = strtod(yytext, NULL);
			return(FloatAtom);
			}
<Unquoted>[_a-zA-Z][_0-9a-zA-Z]*[^_0-9a-zA-Z] {   /* Atoms */
			int a;

			if(yytext[yyleng - 1] == '$') {
			  yyless(yyleng - 1);
			  yymore();
			} else {
			  yyless(yyleng - 1);
			  for(a=0; a < KEYWORDS; a++) {
			    if(!strcmp(ude_configfile_keywords[a].keyword,
				       yytext))
			      return(ude_configfile_keywords[a].token);
			  }
			  if(a == KEYWORDS) {
			    yylval.string = MyStrdup(yytext);
			    return(IdentifierAtom);
			  }
			}
			}
<Unquoted>[:=;{}(),*/|&+-] { /* seperators, operators, braces, linecount */
			return(yytext[0]);
			}
<Unquoted>\n		{
			  uwm_yyParseLineStack->linenumber++;
			  if(uwm_yy_LEX_FLAG_Newline_Requested) {
			    uwm_yy_LEX_FLAG_Newline_Requested = 0;
			    return('\n');
			  }
			}

<Unquoted>^#[ \t]+[0-9]+[ \t]+["][^\n"]*["].*$ {
			uwm_yy_LEX_FLAG_Newline_Requested = 1;
			yyless(1);
			return(PreprocessorAtom);
			}
<Unquoted>[ \t] ;
<Unquoted>[#%].*$ ;		/* delete comments */
<Unquoted>.		{
			fprintf(TheScreen.errout,
			        "UWM: error on line %d of file %s: >%s< does not parse\n",
			        uwm_yyParseLineStack->linenumber,
			        uwm_yyParseLineStack->filename, yytext);
			if(yytext[0] == '\\') input();
			}
<<EOF>>		{
		  if(!uwm_yypop_LineStack()) {
		    yyterminate();
		  }
		}
%%

void uwm_yydrop(unsigned int n)
{
/*  int a;			this would be the proper solution in my eyes...
  yyless(yyleng - n);		doesn't work for some reason...
  for(a = 0; a < n; a++) input(); */
  yyleng -= n;
  yytext[yyleng] = '\0'; 
}

void uwm_yypush_LineStack(char *file, int ispipe)
{
  struct uwm_yyParseLineStackStruct *s;

  s = MyCalloc(1, sizeof(struct uwm_yyParseLineStackStruct));

  if(file) {
    if(ispipe) {
      int pid;
      int files[2];
      if(pipe(files)) {
        free(s);
        return;
      }
      if(!(pid = fork())) {
	close(files[0]);
	fclose(stdout);
	if(STDOUT_FILENO != fcntl(files[1], F_DUPFD, STDOUT_FILENO)) exit(-1);
	close(files[1]);
	if(-1 == fcntl(STDOUT_FILENO, F_SETFD, 0)) exit(-1);
	execl("/bin/sh", "/bin/sh", "-c" , file, NULL);
	fprintf(TheScreen.errout, "UWM: couldn't start /bin/sh\n");
	exit(-1);
      }
      close(files[1]);
      if(!(s->file = fdopen(files[0], "r")) || (pid < 0)) {
	close(files[0]);
	free(s);
	return;
      }
    } else {
      s->file = MyOpen(file, "");
      if(! s->file) {
        free(s);
        return;
      }
    }
    s->filename = s->topfilename = file;
  } else {
    s->file = stdin;
    s->filename = "stdin";
  }
  s->buffer = yy_create_buffer(s->file, YY_BUF_SIZE);
  yy_switch_to_buffer(s->buffer);
  s->linenumber = 1;
  s->stacked = uwm_yyParseLineStack;
  uwm_yyParseLineStack = s;
}

struct uwm_yyParseLineStackStruct *uwm_yypop_LineStack()
{
  struct uwm_yyParseLineStackStruct *s;

  s = uwm_yyParseLineStack;

  if(uwm_yyParseLineStack = s->stacked) {
    yy_switch_to_buffer(uwm_yyParseLineStack->buffer);
  }
  yy_delete_buffer(s->buffer);
  free(s);

  return(uwm_yyParseLineStack);
}

void uwm_yyinitscanner()
{
  yy_delete_buffer(YY_CURRENT_BUFFER);  /* we don't read from stdin */
}
