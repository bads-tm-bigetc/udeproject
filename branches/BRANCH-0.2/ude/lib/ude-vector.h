#ifndef _UDE_VECTOR_H_
#define _UDE_VECTOR_H_

#include <stdlib.h>
#include "ude-types.h"

typedef int (*UDECompareFunction) (UDEPointer, UDEPointer);

typedef struct _UDEVector UDEVector;

struct _UDEVector {
  UDEPointer *data;
  size_t length;
  UDEBoolean organized;
  UDECompareFunction compare;
};

/* Create a new vector with no data. if ORGANIZED is true then COMPARE will be
   used to determine how the elements must be organized */
UDEVector *ude_vector_create (UDEBoolean organized,
			      UDECompareFunction compare);
/* Create a vector and asign the data passed. You must also specify the vector
   lenght. Data must be an malloc allocated piece of memory */
UDEVector *ude_vector_create_from_data (UDEPointer *data, size_t length,
					UDEBoolean organized,
					UDECompareFunction compare);
/* Add a new element to the vector */
void ude_vector_add (UDEVector *vector, UDEPointer new_element);
/* Find an element in the vector. If the vector is not an organized one the it
   will allways return NULL */
UDEPointer ude_vector_find (UDEVector *vector, UDEPointer to_find);
/* Remove the element number ELEMENT_NUMBER from the vector. If FREE_ELEMENT
   is true free will be called with the element as argument */ 
void ude_vector_delete_element (UDEVector *vector, size_t element_number,
				UDEBoolean free_element);
/* Get the ELEMENT_NUMBERnth element from the vector. USE THE MACRO INSTEAD
   OF ACCESING THE DATA DIRECTLY, THIS COULD TURN IN TO A FUNCTION IN THE
   FUTURE */
#define ude_vector_get_nth(vector, element_number) \
          ((vector && vector->data && vector->length > element_number \
            && element_number >= 0)? \
	   vector->data[element_number] : NULL)

/* This one will put the pased element at position NUMBER, if the vector is
   an organized one this will automatically turn in to an unorganized vector */
void ude_vector_set_nth (UDEVector *vector, UDEPointer new_element,
			 size_t number);

void ude_vector_for_each (UDEVector *vector, void (*function)());
void ude_vector_free (UDEVector *vector, UDEBoolean free_elements);
	   
#endif /* !_UDE_VECTOR_H_ */
