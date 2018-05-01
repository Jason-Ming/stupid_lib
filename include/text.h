#ifndef __TEXT_H__
#define __TEXT_H__

#include "clinkage.h"

__BEGIN_C_DECLS
int count_words(const char* filename);
int format_words(const char* filename, const char *separator);

__END_C_DECLS

#endif
