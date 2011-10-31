#include "menu.h"
#include "stdlib.h"
#include "stdio.h"
#include "list.h"
#include "special.h"
#include "string.h"

char *MenuNodeText[MENUNODECNT]={
		"SUBMENU","ITEM","LINE","FILE","PIPE","}","DUMMY"};

list menupath,menustack;

pmenunode menunode_allocate()
{
	pmenunode t=(pmenunode) malloc(sizeof(menunode));
	if(t==NULL)
	{
		fprintf(stderr,"Error: out of memory\n");
		exit(1);
	}
	t->childlist=NULL;
	t->info1=NULL;
	t->info2=NULL;
	t->parent=NULL;
	t->type=DUMMY;
	return t;
}

void menunode_print(FILE* out, pmenunode cur, int depth)
{
	pnode iter;
	int i;
	switch(cur->type)
	{
	case DUMMY:
		/* DUMMY means that this is a root node and
		 * so it is a submenu but without header
		 * and between nodes there will be formed a gap
		 */
		iter=cur->childlist->head->next;
		while(iter->dummy==False)
		{
			if(((pmenunode)iter->data)->type!=LINE||((!iter->prev->dummy)&&(((pmenunode)iter->prev->data)->type!=LINE)))
			{
				menunode_print(out,(pmenunode)iter->data,depth+1);
				fprintf(out,"\n");
			}
			iter=iter->next;
		}
		break;
	case SUBMENU:
		for(i=0;i<depth;i++)
			fprintf(out,GAP);
		fprintf(out,"SUBMENU \"");
		string_print(out,cur->info1);
		fprintf(out,"\" {\n");
		iter=cur->childlist->head->next;
		while(iter->dummy==False)
		{
			if(((pmenunode)iter->data)->type!=LINE||((!iter->prev->dummy)&&(((pmenunode)iter->prev->data)->type!=LINE)))
				menunode_print(out,(pmenunode)iter->data,depth+1);
			iter=iter->next;
		}
		for(i=0;i<depth;i++)
			fprintf(out,GAP);
		fprintf(out,"}\n");
		break;
	case ITEM:
		for(i=0;i<depth;i++)
			fprintf(out,GAP);
		fprintf(out,"ITEM \"");
		string_print(out,cur->info1);
		fprintf(out,"\":\"");
		string_print(out,cur->info2);
		fprintf(out,"\";\n");
		break;
	case LINE:
		for(i=0;i<depth;i++)
			fprintf(out,GAP);
		fprintf(out,"LINE;\n");
		break;
	case FILE_KEY:
	case PIPE:
		for(i=0;i<depth;i++)
			fprintf(out,GAP);
		fprintf(out,"%s \"",(cur->type==FILE_KEY)?"FILE":"PIPE");
		string_print(out,cur->info1);
		fprintf(out,"\";\n");
		break;
	case PAREN_CLOSE:
		break;
	}
}

pmenunode menunode_read(FILE *in)
{
	pmenunode res,cur,iterdata,tmp;
	char s[256],*t,*u,c;
	int a;
	pnode iter;
	res=cur=menunode_allocate();
	cur->childlist=list_allocate();
	list_init(cur->childlist);
	cur->type=DUMMY;
	while (EOF != fscanf (in, "%s", s))
	{
		if((s[0] == '#')||(s[0] == '%'))
		{
			while(s[0]!='\n')
				if(EOF==fscanf(in,"%c",s))
					break;
		}
		else
		{
			for (a=0; a<MENUKEYS; a++)
			{
				if(!strncmp (s, MenuNodeText[a], strlen(MenuNodeText[a])))
				{
					switch(a)
					{
					case SUBMENU:
						if(!(t=ReadQuoted(in)))
						{
						  fprintf(stderr,"UWM: error in Menu-file -- ignored");
						  break;
						}
						for(iter=cur->childlist->head->next;iter->dummy==False;iter=iter->next)
						{
							iterdata=(pmenunode)iter->data;
							if((iterdata != NULL)&& (iterdata->type==SUBMENU) && (!strcmp(t,iterdata->info1)))
							  break;
						}
						if(iter->dummy==False)
						{
							cur=iterdata;
							free(t);
						}
						else
						{
							tmp=menunode_allocate();
							tmp->parent=cur;
							list_append(cur->childlist,tmp);
							cur=tmp;
							cur->info1=t;
							cur->type=SUBMENU;
							cur->childlist=list_allocate();
							list_init(cur->childlist);
						}

						do
							if(EOF==fscanf(in,"%c",&c))
							{
								fprintf(stderr, "UWM: error in Menu-file -- ignored");
								free(t);
								break;
							}
						while(c!='{');
						break;
					case ITEM:
						if(!(u=ReadQuoted(in)))
						{
							fprintf(stderr,"UWM: error in Menu-file -- ignored");
							break;
						}
						do
							if(EOF==fscanf(in,"%c",&c))
							{
								fprintf(stderr,"UWM: error in Menu-file -- ignored");
								free(u);
								break;
							}
						while(c!=':');
						if(!(t=ReadQuoted(in)))
						{
							fprintf(stderr,"UWM: error in Menu-file -- ignored");
							free(u);
							break;
						}
						for(iter=cur->childlist->head->next;iter->dummy==False;iter=iter->next)
						{
							iterdata=(pmenunode)iter->data;
							if((iterdata!=NULL)&&(iterdata->type==ITEM)&&(!strcmp(u,iterdata->info1)))
								break;
						}
						if(iter->dummy==True)
						{
							tmp=menunode_allocate();
							tmp->parent=cur;
							list_append(cur->childlist,tmp);
							tmp->info1=u;
							tmp->info2=t;
							tmp->type=ITEM;
						}
						else
						{
							free(u);
							free(t);
						}
						break;
					case LINE:
						for(iter = cur -> childlist -> head -> next ; iter -> dummy == False ; iter = iter -> next);
						iterdata=(pmenunode)iter->prev->data;
						if(iterdata==NULL || iterdata -> type != LINE)
						{
							tmp=menunode_allocate();
							tmp->parent=cur;
							list_append(cur->childlist,tmp);
							tmp->type=LINE;
						}
						break;
					case PAREN_CLOSE:
						if(cur->parent==NULL)
							fprintf(stderr,"UWM: error in Menu-file -- ignored.\n");
						else
						{
							cur=cur->parent;
						}
						break;
					case FILE_KEY:
					case PIPE:
						if(!(t=ReadQuoted(in)))
						{
							fprintf(stderr,"UWM: error in Menu-file -- ignored");
							break;
						}
						tmp=menunode_allocate();
						tmp->parent=cur;
						list_append(cur->childlist,tmp);
						tmp->info1=t;
						tmp->type=(a==FILE_KEY)?FILE_KEY:PIPE;
						break;
					}
				}
			}
		}
	}
	return res;
}

void menunode_destroy(pmenunode cur)
{
	pnode it;
	if(cur==NULL) return;
	switch(cur->type)
	{
	case SUBMENU:
		free(cur->info1);
	case DUMMY:
		it=cur->childlist->head->next;
		for(;!it->dummy;it=it->next)
		{
			menunode_destroy((pmenunode)it->data);
		}
		list_free(cur->childlist);
		break;
	case ITEM:
		free(cur->info2);
	case FILE_KEY:
	case PIPE:
		free(cur->info1);
		break;
	default:
		break;
	}
	free(cur);
	return;
}
