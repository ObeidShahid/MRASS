#ifndef vector_h
#define vector_h

struct vector;
typedef struct vector vector;

vector *init_vector();

void add_char(vector *vec, char c);

char *get_str(vector *vec);

void clear_vector(vector *vec);

void free_vector(vector *vec);

#endif
