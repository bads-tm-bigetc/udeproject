#include <stdlib.h>
#include "special.h"
#include "main.h"
#include "graph.h"
#include "gui/xtools.h"

#define linetxt "LINE"
#define pipetxt "PIPE"
#define filetxt "FILE"
#define bcolor 0xDCDCDC
#define graycolor 0x7E7E7E

XObject *win,*diagwin, *fopen_err_win;
XObject *pathframe, *controlframe, *curdirframe, *descframe;
XObject *typelabel, *namelabel, *commlabel, *diallabel;
XObject *namebox, *commbox;
XObject *curobj;
XObject *btnmoveup,*btnmovedown,*btndelete;
extern char *MenuNodeText[MENUNODECNT];
pmenunode cur;
list cmenu,cpath,abtn,opfiles;
plist curmenu=&cmenu,curpath=&cpath,additbtn=&abtn,openfiles=&opfiles;
int curinlist=-1;
char buff[10000];
Bool *changed;

//ToDo: hide font into objects
extern XFontStruct *font;

unsigned long MenuColorGray[]      ={0x9E802C,0xA263CE,0x7E7E7E,0x5DB756,0x1D9079};
unsigned long MenuColorBackground[]={0xFFDC7C,0xDBA8FF,0xFFFFFF,0x97E091,0x42B8A0};

pathstruct* pathstruct_create(char* filepath, pmenunode cur, Bool changed)
{
	pathstruct *res=(pathstruct*) calloc(1,sizeof(pathstruct));
	if(!res)
	{
		fprintf(stderr,"Impossible to allocate structure to describe current path: Out of memory\n");
		exit(OUT_OF_MEMORY);
	}
	res->filepath=filepath;
	res->root=res->cur=cur;
	res->changed=changed;
	return res;
}


Bool quit(XObject* obj, XEvent ev, XPointer cbdata)
{
	leave_submenu(obj,ev,INTTOPOINTER(0));
	if(openfiles->count==0)
	{
		x_main_quit();
		return False;
	}
	else
		return True;
}

Bool send_response(XObject* obj, XEvent ev, XPointer cbdata)
{
	x_dialog_send_response(POINTERTOINT(cbdata));
	return False;
}

Bool quit_program(XObject* obj,XEvent ev,XPointer cbdata)
{
	if(POINTERTOINT(cbdata)!=0)
		savemenu(obj,ev,cbdata);
	x_main_quit();
	return False;
}

Bool leave_submenu(XObject* obj,XEvent ev, XPointer cbdata)
{
	curinlist=-1;
	x_frame_set_visible(descframe,False);
	int up,dres;
	XObject* top;
	ppathstruct tmp;
	up=curpath->count-POINTERTOINT(cbdata);
	while(up--)
	{
		top=(XObject*)curpath->tail->prev->data;
		if(cur->parent==NULL)
		{
			if(*changed)
			{
				sprintf(buff,"%s ?",top->button.text);
				x_label_set_text(diallabel,buff);
				x_object_move(diallabel,(diagwin->obj.width-diallabel->obj.width)/2,diallabel->label.y);
				dres=x_dialog_run(diagwin);
				if(dres==0)
					savemenu(obj,ev,cbdata);
				else if(dres==2)
					break;
			}
			free(((ppathstruct)list_get_last_data(openfiles))->filepath);
			free(list_get_last_data(openfiles));
			list_remove_last(openfiles);
			if(openfiles->count!=0)
			{
				tmp=(ppathstruct)list_get_last_data(openfiles);
				cur=tmp->cur;
				changed=&(tmp->changed);
			}
			else
				break;
		}
		else
			((ppathstruct)list_get_last_data(openfiles))->cur=cur=cur->parent;

		x_object_destroy(top);
		list_remove_last(curpath);
	}
	if(openfiles->count!=0)
		update_currentlist();
	return False;
}

Bool enter_submenu(XObject*btn,XEvent ev,XPointer cbd)
{
	int x;
	XObject* obj;
	pmenunode cbdata=(pmenunode) cbd;
	if(cbdata->type==SUBMENU)
	{
		curinlist=-1;
		x_frame_set_visible(descframe,False);
		((ppathstruct)list_get_last_data(openfiles))->cur=cur=cbdata;
		int len,width;
		len=strlen(cbdata->info1);
		width=XTextWidth(font,cbdata->info1,len)+10;
		obj=(XObject*)curpath->tail->prev->data;
		x=obj->button.x+obj->obj.width;
		obj=x_button_create(pathframe,x,0,width,20,True,cbdata->info1,0,MenuColorGray[cbdata->type],0xFFFFFF,MenuColorBackground[cbdata->type],INTTOPOINTER(curpath->count+1));
		x_button_set_button_press(obj,(Callback)leave_submenu);
		list_append(curpath,(void*)obj);
		update_currentlist();
	}
	return False;
}

Bool savemenu(XObject *obj, XEvent ev, XPointer cbdata)
{
	FILE* mf=fopen(((ppathstruct)list_get_last_data(openfiles))->filepath,"wt");
	if(mf)
	{
		menunode_print(mf,((ppathstruct)list_get_last_data(openfiles))->root,-1);
		fclose(mf);
	}
	else
	{
		x_dialog_run(fopen_err_win);
	}
	*changed=False;
	return False;
}

Bool enter_file(XObject*btn,XEvent ev,XPointer cbd)
{
	int x;
	XObject* obj;
	FILE* nf;
	pmenunode cbdata=(pmenunode) cbd;
	int len,width;
	char* text, *path;
	int i;
	if(cbdata->type==FILE_KEY)
	{
		if(cbdata->info1[0]!='\0')
		{
			curinlist=-1;
			x_frame_set_visible(descframe,False);
			path=MyOpen(cbdata->info1);
			if(!path)
			{
				x_dialog_run(fopen_err_win);
				return False;
			}
			nf=fopen(path,"r");
			if(!nf)
			{
				x_dialog_run(fopen_err_win);
				return False;

			}
			cur=menunode_read(nf);
			list_append(openfiles,(void*)pathstruct_create(path,cur,False));
			changed=&(((ppathstruct)list_get_last_data(openfiles))->changed);

			text=cbdata->info1;
			len=strlen(text);
			for(i=len-1;i>=0;i--)
				if(text[i]=='/')
					break;
			i++;
			len-=i;
			width=XTextWidth(font,text+i,len)+10;
			obj=(XObject*) curpath->tail->prev->data;
			x=obj->button.x+obj->obj.width;
			obj=x_button_create(pathframe,x,0,width,20,True,text+i,0,MenuColorGray[cbdata->type],0xFFFFFF,MenuColorBackground[cbdata->type],INTTOPOINTER(curpath->count+1));
			x_button_set_button_press(obj,(Callback)leave_submenu);
			list_append(curpath,(void*) obj);
			update_currentlist();
		}
		else
		{
			x_dialog_run(fopen_err_win);
		}
	}
	return False;
}

void update_name(XObject* obj, XPointer cbdata)
{
	if(curinlist>=0)
	{
		*changed=True;
		pmenunode c=(pmenunode)list_get_nth_data(cur->childlist,curinlist);
		free(c->info1);
		c->info1=calloc(obj->textbox.buffer_size+1,sizeof(char));
		strncpy(c->info1,obj->textbox.buffer,obj->textbox.buffer_size);
		if(c->type==ITEM||c->type==SUBMENU)
			x_button_set_text(curobj,c->info1);
		else if(c->type==FILE_KEY)
		{
			int len,i;
			len=strlen(c->info1);
			for(i=len-1;i>=0;i--)
				if(c->info1[i]=='/')
					break;
			i++;
			x_button_set_text(curobj,c->info1+i);
		}
	}
}

void update_command(XObject* obj, XPointer cbdata)
{
	if(curinlist>=0)
	{
		*changed=True;
		pmenunode c=(pmenunode)list_get_nth_data(cur->childlist,curinlist);
		free(c->info2);
		c->info2=calloc(obj->textbox.buffer_size+1,sizeof(char));
		strncpy(c->info2,obj->textbox.buffer,obj->textbox.buffer_size);
	}
}
extern XContext xctxt;
extern Display *dis;
void clearmenulist(plist l)
{
	XObject *button;
	while(l->count!=0)
	{
		button=(XObject*)l->head->next->data;
		x_object_destroy(button);
		list_remove_nth(l,0);
	}
}

void update_currentlist()
{
	x_frame_set_visible(curdirframe,False);
	clearmenulist(curmenu);
	clearmenulist(additbtn);
	appendsubmenutolist(curmenu,additbtn,cur->childlist);
	x_frame_set_visible(curdirframe,True);
}

Bool create_object(XObject* obj,XEvent ev,XPointer cbdata)
{
	pmenunode newobj=menunode_allocate();
	newobj->parent=cur;
	newobj->type=POINTERTOINT(cbdata);
	switch(newobj->type)
	{
	case SUBMENU:
		newobj->info1=calloc(1,sizeof(char));
		newobj->childlist=list_allocate();
		list_init(newobj->childlist);
		break;
	case ITEM:
		newobj->info1=calloc(1,sizeof(char));
		newobj->info2=calloc(1,sizeof(char));
		break;
	case LINE:
		break;
	case FILE_KEY:
	case PIPE:
		newobj->info1=calloc(1,sizeof(char));
		break;
	default:
		break;
	}
	list_append(cur->childlist,(void*) newobj);
	appendnodetolist(curmenu,additbtn,cur->childlist->tail->prev);
	XObject* btn=(XObject*)(curmenu->tail->prev->data);
	item_press(btn,ev,btn->obj.cbdata);
	*changed=True;
	return False;
}

Bool item_press(XObject* obj,XEvent ev,XPointer cbd)
{
	int id;
	pnode it=cur->childlist->head->next;
	for(id=0;!it->dummy;it=it->next,id++)
		if(it->data==cbd) break;
	if(it->dummy)
		return False;
	curinlist=id;
	curobj=obj;
	x_frame_set_visible(descframe,True);
	pmenunode cbdata=(pmenunode) cbd;
	switch(cbdata->type)
	{
	case SUBMENU:
		x_label_set_text(typelabel,MenuNodeText[cbdata->type]);
		x_textbox_set_text(namebox,cbdata->info1);
		x_label_set_text(namelabel,"Name:");
		x_label_set_visible(namelabel,True);
		x_label_set_visible(commlabel,False);
		x_textbox_set_visible(namebox,True);
		x_textbox_set_visible(commbox,False);
		x_object_set_focused(namebox);
		break;
	case ITEM:
		x_label_set_text(typelabel,MenuNodeText[cbdata->type]);
		x_textbox_set_text(namebox,cbdata->info1);
		x_label_set_text(namelabel,"Name:");
		x_textbox_set_text(commbox,cbdata->info2);
		x_label_set_text(commlabel,"Command:");
		x_label_set_visible(namelabel,True);
		x_label_set_visible(commlabel,True);
		x_textbox_set_visible(namebox,True);
		x_textbox_set_visible(commbox,True);
		x_object_set_focused(namebox);
		break;
	case LINE:
		x_label_set_text(typelabel,MenuNodeText[cbdata->type]);
		x_label_set_visible(namelabel,False);
		x_label_set_visible(commlabel,False);
		x_textbox_set_visible(namebox,False);
		x_textbox_set_visible(commbox,False);
		break;
	case FILE_KEY:
		x_label_set_text(typelabel,MenuNodeText[cbdata->type]);
		x_textbox_set_text(namebox,cbdata->info1);
		x_label_set_text(namelabel,"File:");
		x_label_set_visible(namelabel,True);
		x_label_set_visible(commlabel,False);
		x_textbox_set_visible(namebox,True);
		x_textbox_set_visible(commbox,False);
		x_object_set_focused(namebox);
		break;
	case PIPE:
		x_label_set_text(typelabel,MenuNodeText[cbdata->type]);
		x_textbox_set_text(namebox,cbdata->info1);
		x_label_set_text(namelabel,"Command:");
		x_label_set_visible(namelabel,True);
		x_label_set_visible(commlabel,False);
		x_textbox_set_visible(namebox,True);
		x_textbox_set_visible(commbox,False);
		x_object_set_focused(namebox);
		break;
	default:
		break;
	}
	return False;
}

void appendnodetolist(plist l, plist ab, pnode it)
{
	XObject *button;
	pmenunode iterdata=it->data;
	int cnt=l->count;
	if(iterdata)
	{
		if(iterdata->type==LINE)
		{
			button=x_button_create(curdirframe,0,20*cnt,150,20,True,linetxt,0,MenuColorGray[iterdata->type],0xFFFFFF,MenuColorBackground[iterdata->type],it->data);
			x_button_set_button_press(button,(Callback)item_press);
			list_append(l,(void*)button);
		}
		else if(iterdata->type==PIPE)
		{
			button=x_button_create(curdirframe,0,20*cnt,150,20,True,pipetxt,0,MenuColorGray[iterdata->type],0xFFFFFF,MenuColorBackground[iterdata->type],it->data);
			x_button_set_button_press(button,(Callback)item_press);
			list_append(l,(void*)button);
		}
		else if(iterdata->type==FILE_KEY)
		{
			int i;
			int len=strlen(iterdata->info1);
			for(i=len-1;i>=0;i--)
				if(iterdata->info1[i]=='/')
					break;
			i++;
			button=x_button_create(curdirframe,0,20*cnt,150,20,True,iterdata->info1+i,0,MenuColorGray[iterdata->type],0xFFFFFF,MenuColorBackground[iterdata->type],it->data);
			x_button_set_button_press(button,(Callback)item_press);
			list_append(l,(void*)button);
			button=x_button_create(curdirframe,130,20*cnt,20,20,True,">",0,MenuColorGray[iterdata->type],0xFFFFFF,MenuColorBackground[iterdata->type],it->data);
			x_button_set_button_press(button,(Callback)enter_file);
			list_append(ab,(void*)button);
		}
		else if(iterdata->type==ITEM)
		{
			button=x_button_create(curdirframe,0,20*cnt,150,20,True,iterdata->info1,0,MenuColorGray[iterdata->type],0xFFFFFF,MenuColorBackground[iterdata->type],it->data);
			x_button_set_button_press(button,(Callback)item_press);
			list_append(l,(void*)button);
		}
		else if(iterdata->type==SUBMENU)
		{
			button=x_button_create(curdirframe,0,20*cnt,130,20,True,iterdata->info1,0,MenuColorGray[iterdata->type],0xFFFFFF,MenuColorBackground[iterdata->type],it->data);
			x_button_set_button_press(button,(Callback)item_press);
			list_append(l,(void*)button);
			button=x_button_create(curdirframe,130,20*cnt,20,20,True,">",0,MenuColorGray[iterdata->type],0xFFFFFF,MenuColorBackground[iterdata->type],it->data);
			x_button_set_button_press(button,(Callback)enter_submenu);
			list_append(ab,(void*)button);
		}
	}
}

void appendsubmenutolist(plist l,plist ab, plist menu)
{

	pnode it=menu->head->next;
	for(;!it->dummy;it=it->next)
	{
		appendnodetolist(l,ab,it);
	}
}

Bool delete_cur_item(XObject* btn,XEvent ev,void* cbdata )
{
	if(curinlist!=-1)
	{
		*changed=True;
		menunode_destroy(list_get_nth_data(cur->childlist,curinlist));
		list_remove_nth(cur->childlist,curinlist);
		update_currentlist();
		curinlist=-1;
		x_frame_set_visible(descframe,False);
	}
	return False;
}

Bool moveup(XObject* btn,XEvent ev,void* cbdata )
{
	if(curinlist>0)
	{
		*changed=True;
		void* data=list_get_nth_data(cur->childlist,curinlist);
		list_remove_nth(cur->childlist,curinlist);
		list_insert_before_nth(cur->childlist,curinlist-1,data);
		update_currentlist();

		curinlist--;
		curobj=list_get_nth_data(curmenu,curinlist);
	}
	return False;
}

Bool movedown(XObject* btn,XEvent ev,void* cbdata )
{
	if(curinlist+1<cur->childlist->count)
	{
		*changed=True;
		void* data=list_get_nth_data(cur->childlist,curinlist+1);
		list_remove_nth(cur->childlist,curinlist+1);
		list_insert_before_nth(cur->childlist,curinlist,data);
		update_currentlist();
		curinlist++;
		curobj=list_get_nth_data(curmenu,curinlist);
	}
	return False;
}

Bool resize(XObject* obj,XEvent e, void* cbdata)
{
	int w,h;
	x_object_move(controlframe,win->obj.width-controlframe->obj.width-5,win->obj.height-controlframe->obj.height-10);
	x_object_resize(pathframe,win->obj.width-pathframe->frame.x-10,pathframe->obj.height);
	w=win->obj.width-descframe->frame.x-10;
	h=controlframe->frame.y-descframe->frame.y-5;
	if(w<200) w=200;
	if(h<100) h=100;
	x_object_resize(descframe,w,h);
	x_object_resize(curdirframe,curdirframe->obj.width,h);
	x_object_move(btndelete,w-btndelete->obj.width-10,h-btndelete->obj.height-10);
	x_object_move(btnmovedown,w-btnmovedown->obj.width-10,btndelete->button.y-btnmovedown->obj.height-2);
	x_object_move(btnmoveup,w-btnmoveup->obj.width-10,btnmovedown->button.y-btnmoveup->obj.height-2);
	x_object_resize(namebox,w-namebox->textbox.x-10,namebox->obj.height);
	x_object_resize(commbox,w-commbox->textbox.x-10,commbox->obj.height);
	return False;
}

void fill_window(pmenunode top, char *path)
{
	XObject* btn, *lbl;
	int width;
	int i;
	int offset;
	cur=top;
	list_init(curmenu);
	list_init(curpath);
	list_init(additbtn);
	list_init(openfiles);
	win=x_window_create(640,480,True,0,0,bcolor,"UDE Menu Editor",None);
	x_window_set_destroy_event(win,(Callback)quit);
	x_window_set_configure(win,(Callback)resize);

	curdirframe=x_frame_create(win,10,40,150,400,True,0,0,bcolor,None);
	appendsubmenutolist(curmenu,additbtn,top->childlist);

	descframe=x_frame_create(win,165,40,630-165,400,False,1,0,bcolor,None);
	lbl=x_label_create(descframe,20,20,"Type:",True,None);
	namelabel=x_label_create(descframe,20,lbl->label.y+lbl->obj.height+5,"Name:",True,None);
	commlabel=x_label_create(descframe,20,namelabel->label.y+namelabel->obj.height+5,"Command:",True,None);
	offset=commlabel->label.x+commlabel->obj.width+10;
	typelabel=x_label_create(descframe,offset,lbl->label.y,"",True,None);
	namebox=x_textbox_create(descframe,offset,namelabel->label.y,350,namelabel->obj.height,True,None);
	commbox=x_textbox_create(descframe,offset,commlabel->label.y,350,commlabel->obj.height,True,None);
	x_textbox_set_change(namebox,(CallbackLite) update_name);
	x_textbox_set_change(commbox,(CallbackLite) update_command);

	btndelete=x_button_create(descframe,630-165-90-10,400-25-2,90,25,True,"Delete",0,graycolor,0xFFFFFF,bcolor,None);
	x_button_set_button_press(btndelete,(Callback)delete_cur_item);

	btnmovedown=x_button_create(descframe,630-165-75-10,400-50-4,75,25,True,"Move down",0,graycolor,0xFFFFFF,bcolor,None);
	x_button_set_button_press(btnmovedown,(Callback)movedown);

	btnmoveup=x_button_create(descframe,630-165-75-10,400-75-6,75,25,True,"Move up",0,graycolor,0xFFFFFF,bcolor,None);
	x_button_set_button_press(btnmoveup,(Callback)moveup);

	pathframe=x_frame_create(win,10,10,620,20,True,0,0,bcolor,None);

	width=XTextWidth(font,"/",1)+10;
	btn=x_button_create(pathframe,0,0,width,20,True,"/",0,MenuColorGray[FILE_KEY],0xFFFFFF,MenuColorBackground[FILE_KEY],INTTOPOINTER(1));
	x_button_set_button_press(btn,(Callback)leave_submenu);
	list_append(curpath,(void*) btn);

	list_append(openfiles,(void*) pathstruct_create(path,top,False));
	changed=&(((ppathstruct)list_get_last_data(openfiles))->changed);


	controlframe=x_frame_create(win,80,450,630,20,True,0,0,bcolor,None);
	btn=x_button_create(controlframe,630-90,0,90,20,True,"Exit",0,graycolor,0xFFFFFF,bcolor,None);
	x_button_set_button_press(btn,(Callback)quit);

	btn=x_button_create(controlframe,630-180,0,90,20,True,"Save",0,graycolor,0xFFFFFF,bcolor,(XPointer) top);
	x_button_set_button_press(btn,(Callback)savemenu);

	for(i=0;i<5;i++)
	{
		sprintf(buff,"New %s",MenuNodeText[i]);
		btn=x_button_create(controlframe,630-90*(i+3),0,90,20,True,buff,0,MenuColorGray[i],0xFFFFFF,MenuColorBackground[i],INTTOPOINTER(i));
		x_button_set_button_press(btn,(Callback)create_object);
	}
	XRaiseWindow(controlframe->obj.dis,controlframe->obj.win);


	diagwin=x_window_create_transient(win,200,75,False,0,0,bcolor,"Confirm saving",None);
	lbl=x_label_create(diagwin,0,0,"Save changes to",True,None);
	x_object_move(lbl,(diagwin->obj.width-lbl->obj.width)/2,5);
	diallabel=x_label_create(diagwin,0,lbl->label.y+lbl->obj.height+5,"",True,None);

	btn=x_button_create(diagwin,15,75-20-5,50,20,True,"Yes",0,graycolor,0xFFFFFF,bcolor,INTTOPOINTER(0));
	x_button_set_button_press(btn,(Callback)send_response);

	btn=x_button_create(diagwin,15+60,75-20-5,50,20,True,"No",0,graycolor,0xFFFFFF,bcolor,INTTOPOINTER(1));
	x_button_set_button_press(btn,(Callback)send_response);

	btn=x_button_create(diagwin,15+120,75-20-5,50,20,True,"Cancel",0,graycolor,0xFFFFFF,bcolor,INTTOPOINTER(2));
	x_button_set_button_press(btn,(Callback)send_response);

	fopen_err_win=x_window_create_transient(win,200,75,False,0,0,bcolor,"Open failed",None);
	lbl=x_label_create(fopen_err_win,0,0,"Failed to open file",True,None);
	x_object_move(lbl,(fopen_err_win->obj.width-lbl->obj.width)/2,(fopen_err_win->obj.height-lbl->obj.height-25)/2);

	btn=x_button_create(fopen_err_win,(fopen_err_win->obj.width-50)/2,fopen_err_win->obj.height-25,50,20,True,"OK",0,graycolor,0xFFFFFF,bcolor,INTTOPOINTER(0));
	x_button_set_button_press(btn,(Callback)send_response);
}
