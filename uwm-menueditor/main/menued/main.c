#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "menu.h"
#include "list.h"
#include "graph.h"
#include "special.h"
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/Xos.h"
#include "gui/xtools.h"

char defpath[]="appmenu";
char* path=defpath;
char home[256];
char udedir[256];


int main(int argc, char **argv)
{
	FILE *mf=NULL;
	char* tmp=NULL;

	tmp=getenv("HOME");
	if(tmp==NULL)
		tmp=".";
	strcpy(home,tmp);

	tmp=getenv("UDEdir");
	if(tmp==NULL)
		tmp=".";
	strcpy(udedir,tmp);


	pmenunode menuroot;
	if(argc>1)
	{
		path=MyOpen(argv[1]);
		if(path)
		{
			mf=fopen(path,"rt");
		}
		if(!mf||!path)
			fprintf(stderr,"Warning: impossible to open file \"%s\" for reading\nTrying to open \"appmenu\"",argv[1]);
	}
	if(!mf)
	{
		path=MyOpen(defpath);
		if(path)
			mf=fopen(path,"rt");
	}

	if(!mf)
	{
		fprintf(stderr,"Impossible to open file \"appmenu\"\n");
		fprintf(stderr,"Using empty menu\n");
		menuroot=menunode_allocate();
		menuroot->childlist=list_allocate();
		list_init(menuroot->childlist);
		menuroot->type=DUMMY;
	}
	else
	{
		 menuroot=menunode_read(mf);
		 fclose(mf);
	}
	x_init();
	fill_window(menuroot,path);
	x_main();
	//mf=fopen("appmenu","wt");
	//mf=NULL;
	//menunode_print(mf?mf:stdout,cur,-1);
	return 0;
}
