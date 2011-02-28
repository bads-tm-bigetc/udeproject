#ifndef UWM_URM_H
#define UWM_URM_H

#define MAXNUMS (11)
#define MAXEXTRAS (15)
#define BACKGROUND (0)
#define LIGHTCOLOR (1)
#define SHADOWCOLOR (2)
#define STANDARDTEXT (3)
#define INACTIVETEXT (4)
#define HIGHLIGHTEDTEXT (5)
#define HIGHLIGHTEDBGR (6)
#define TEXTCOLOR (7)
#define TEXTBGR (8)
#define BEVELWIDTH (9)
#define FLAGS (10)
#define STANDARDFONT (11)
#define INACTIVEFONT (12)
#define HIGHLIGHTFONT (13)
#define TEXTFONT (14)

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
