#ifndef _UDE_TYPES_H_
#define _UDE_TYPES_H_

typedef void* UDEPointer;
typedef unsigned char UDEBoolean;
#undef FALSE
#define FALSE (0)
#undef TRUE
#define TRUE (!FALSE)

/* standard return vlaues: */
#define UDE_SUCCESS (0)           /* everything's fine */
#define UDE_FAIL    (-1)          /* operation failed */
#define UDE_BADVAL  (1)           /* bad values */
/* more to come... */

#endif /* !_UDE_TYPES_H_ */
