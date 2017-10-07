#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

typedef struct variable_resolver {
  wchar_t *name;
  struct variable_resolver *next;
} *VariableResolver;

VariableResolver new_variable_resolver();
void free_variable_resolver(VariableResolver *vr);

bool add_variable(VariableResolver *vr, wchar_t *name);
bool add_variable_scope(VariableResolver *vr);
bool drop_variable_scope(VariableResolver *vr);
bool get_variable_bruijin(VariableResolver *vr, wchar_t *name, uint64_t *index);
