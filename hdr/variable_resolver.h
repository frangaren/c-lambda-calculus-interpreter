#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct variable_resolver {
  char *name;
  struct variable_resolver *next;
} *VariableResolver;

VariableResolver new_variable_resolver();
void free_variable_resolver(VariableResolver *vr);

bool add_variable(VariableResolver *vr, char *name);
bool add_variable_scope(VariableResolver *vr);
bool drop_variable_scope(VariableResolver *vr);
bool get_variable_bruijin(VariableResolver *vr, char *name, uint64_t *index);
