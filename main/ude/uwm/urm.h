#ifndef UWM_URM_H
#define UWM_URM_H

#define MAXEXTRAS (16)
#define BACKGROUND (0)
#define LIGHTCOLOR (1)
#define SHADOWCOLOR (2)
#define STANDARDTEXT (3)
#define INACTIVETEXT (4)
#define INACTIVEBGR (5)
#define HIGHLIGHTEDTEXT (6)
#define HIGHLIGHTEDBGR (7)
#define TEXTCOLOR (8)
#define TEXTBGR (9)
#define BEVELWIDTH (10)
#define FLAGS (11)
#define STANDARDFONT (12)
#define INACTIVEFONT (13)
#define HIGHLIGHTFONT (14)
#define TEXTFONT (15)

typedef struct _UDEXrdbEntry UDEXrdbEntry;

struct _UDEXrdbEntry {
  char *name, *entry;
  int xtra;
  struct _UDEXrdbEntry *next;
};

int ReadResourceDBFromFile(char *filename);
void SetResourceDB();
char *Initurdbcppopts();

#endif
