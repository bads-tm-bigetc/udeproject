#include <stdio.h>

struct teststruct {
  int a;
  void *b;
  double c;
  int d;
  union {
    int i;
    double j;
    void *k;
  } e;
  struct teststruct *f;
};

struct idx {
  void *offset;
  int type;
  char *name;
};

#define OFFSET_OF(NAME) ((&(((struct teststruct *)(0))->NAME)))

const struct idx ehlo[] = {{OFFSET_OF(a), 0, "a"},
			   {OFFSET_OF(b), 1, "b"},
			   {OFFSET_OF(c), 2, "c"},
			   {OFFSET_OF(d), 3, "d"},
			   {OFFSET_OF(e), 4, "e"},
			   {OFFSET_OF(f), 1, "f"}};
#undef OFFSET_OF

#define deref(TYPE, ADR, OFFS) (*((TYPE *)(((void *)ADR) + ((unsigned long)OFFS))))
int main()
{
  struct teststruct t;
  int a;
  printf("%d\n", deref(int, &t, ehlo[0].offset) = 1);
  printf("%X\n", deref(void *, &t, ehlo[1].offset) = &t);
  printf("%f\n", deref(double, &t, ehlo[2].offset) = 3.345);
  printf("%d\n", deref(int, &t, ehlo[3].offset) = 3);
  printf("%f\n", deref(double, &t, ehlo[4].offset) = t.c);
  printf("%X\n", deref(struct teststruct *, &t, ehlo[5].offset) = &t);
  printf("---\n");
  printf("%d\n", t.a);
  printf("%X\n", t.b);
  printf("%f\n", t.c);
  printf("%d\n", t.d);
  printf("%f\n", t.e.j);
  printf("%X\n", t.f);
  printf("---\n");
  for(a = 0; a < 6; a++) {
    printf("%X\n", ehlo[a].offset);
  }
  printf("---\n");
  printf("%X\n", ((void *)&(t.a)) - (void *) &t);
  printf("%X\n", ((void *)&(t.b)) - (void *) &t);
  printf("%X\n", ((void *)&(t.c)) - (void *) &t);
  printf("%X\n", ((void *)&(t.d)) - (void *) &t);
  printf("%X\n", ((void *)&(t.e)) - (void *) &t);
  printf("%X\n", ((void *)&(t.f)) - (void *) &t);
  return 0;
}
