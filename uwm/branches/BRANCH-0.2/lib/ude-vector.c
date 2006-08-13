#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ude-vector.h"

static void
ude_vector_organize (UDEVector *vector)
{
  size_t c1, c2;
  UDEPointer temp;

  /* The bouble organizator */
  for (c1= 0; c1 < vector->length - 1; c1++)
    for (c2= c1 + 1; c2 < vector->length; c2++)
      if (vector->compare (vector->data[c1], vector->data[c2]) > 0)
	{
	  temp= vector->data[c1];
	  vector->data[c1]= vector->data[c2];
	  vector->data[c2]= temp;
	}

  return;
}

static void
ude_vector_organize_last (UDEVector *vector)
{
  size_t cont;
  UDEPointer vector_last= vector->data[vector->length - 1];
  int c = vector->length - 2;

  while((c>=0) && (vector->compare (vector->data[c], vector_last) > 0)) {
    vector->data[c+1] = vector->data[c];
    c--;
  }
  vector->data[c+1] = vector_last;
  return;
}
  

UDEVector*
ude_vector_create (UDEBoolean organized, UDECompareFunction compare)
{
  UDEVector *new= (UDEVector*) malloc (sizeof (UDEVector));

  if (!new)
    return NULL;

  new->data= NULL;
  new->length= 0;
  new->organized= organized;
  if (organized)
    new->compare= compare;

  return new;
}

UDEVector*
ude_vector_create_from_data (UDEPointer *data, size_t length,
			     UDEBoolean organized,
			     UDECompareFunction compare)
{
  UDEVector *new= (UDEVector*) malloc (sizeof (UDEVector));

  if (!new)
    return NULL;

  new->data= data;
  new->length= length;
  new->organized= organized;
  if (organized)
    {
      new->compare= compare;
      ude_vector_organize (new);
    }

  return new;
}

void
ude_vector_add (UDEVector *vector, UDEPointer new_element)
{
  UDEPointer *temp;
  
  if (!vector || !new_element)
    return;

  temp= (UDEPointer*) realloc (vector->data,
			       (vector->length + 1) * sizeof (UDEPointer));
  if (!temp)
    return;
  vector->data= temp;
  vector->data[vector->length]= new_element;
  ++vector->length;
  if (vector->organized)
    ude_vector_organize_last (vector);

  return;
}

UDEPointer
ude_vector_find (UDEVector *vector, UDEPointer to_find)
{
  int left, right, center, comparation;
  UDEBoolean done;

  if (!vector || !to_find)
    return NULL;
  
  left= 0; right= vector->length - 1; done= 0;
  while (left <= right && !done)
    {
      center= (left + right) >> 1;
      comparation= vector->compare (vector->data[center], to_find);
      if (comparation == 0)
	done= 1;
      else if (comparation < 0)
	left= center + 1;
      else
	right= center - 1;
    }
  return (done ? vector->data[center] : NULL);
}

void
ude_vector_delete_element (UDEVector *vector, size_t element_number,
			   UDEBoolean free_element)
{
  size_t cont;
  UDEPointer *temp;

  if (!vector || element_number < 0 || element_number >= vector->length)
    return;
  if (free_element)
    free (vector->data[element_number]);
  for (cont= element_number; cont < vector->length - 1; cont++)
    vector->data[cont]= vector->data[cont + 1];
  vector->length--;
  temp= (UDEPointer*) realloc (vector->data, vector->length * sizeof (char*));
  if (temp)
    vector->data= temp;
  return;
}

void
ude_vector_for_each (UDEVector *vector, void (*function)())
{
  size_t cont;

  if (!vector || !function)
    return;
  for (cont= 0; cont < vector->length; cont++)
    function (vector->data[cont]);
  return;
}

void
ude_vector_free (UDEVector *vector, UDEBoolean free_elements)
{
  size_t cont;

  if (!vector)
    return;

  if (vector->data)
    {
      if (free_elements)
	for (cont= 0; cont < vector->length; cont++)
	  free (vector->data[cont]);
      free (vector->data);
    }
  free (vector);
  return;
}

void
ude_vector_set_nth (UDEVector *vector, UDEPointer new_element, size_t number)
{
  UDEPointer *temp;
  size_t cont;
  
  if (!vector || !new_element)
    return;

  temp= (UDEPointer*) realloc (vector->data,
			       (vector->length + 1) * sizeof (UDEPointer));
  if (!temp)
    return;
  vector->data= temp;
  vector->organized= FALSE;
  vector->compare= NULL;
  vector->length++;
  for (cont= number; cont < vector->length - 1; cont++)
    vector->data[cont + 1]= vector->data[cont];
  vector->data[number]= new_element;
}
      
