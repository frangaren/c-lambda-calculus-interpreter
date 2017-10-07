#include "variable_resolver.h"

#include <stdlib.h>
#include <wchar.h>

VariableResolver new_variable_resolver() {
  return NULL;
}

void free_variable_resolver(VariableResolver *vr) {
  if (vr == NULL || *vr == NULL) return;
  VariableResolver to_delete = *vr, cursor = *vr;
  while (cursor != NULL) {
    to_delete = cursor;
    cursor = cursor->next;
    if (to_delete->name != NULL) free(to_delete->name);
    if (to_delete->expression != NULL) free_expression(&to_delete->expression);
    free(to_delete);
  }
  *vr = NULL;
}

bool add_variable(VariableResolver *vr, wchar_t *name, Expression expression) {
  if (vr == NULL) return false;
  VariableResolver node = malloc(sizeof(struct variable_resolver));
  if (node == NULL) return false;
  node->next = *vr;
  node->name = malloc((wcslen(name) + 1) * sizeof(wchar_t));
  node->expression = expression;
  if (node->name == NULL) {
    free(node);
    return false;
  }
  wcscpy(node->name, name);
  *vr = node;
  return true;
}

bool add_variable_scope(VariableResolver *vr) {
  if (vr == NULL) return false;
  VariableResolver node = malloc(sizeof(struct variable_resolver));
  if (node == NULL) return false;
  node->next = *vr;
  node->name = NULL;
  node->expression = NULL;
  *vr = node;
  return true;
}

bool drop_variable_scope(VariableResolver *vr) {
  if (vr == NULL || *vr == NULL) return false;
  VariableResolver to_delete = *vr, cursor = *vr;
  bool should_end = false;
  while (cursor != NULL && !should_end) {
    if (cursor->name == NULL) should_end = true;
    to_delete = cursor;
    cursor = cursor->next;
    if (to_delete->name != NULL) free(to_delete->name);
    if (to_delete->expression != NULL) free_expression(&to_delete->expression);
    free(to_delete);
  }
  *vr = cursor;
  return true;
}

bool get_variable_bruijin(VariableResolver *vr, wchar_t *name,
  uint64_t *index) {
  if (vr == NULL || *vr == NULL) return false;
  VariableResolver cursor = *vr;
  *index = 0;
  while (cursor != NULL) {
    if (cursor->name != NULL) {
      if (wcscmp(name, cursor->name) == 0) {
        return true;
      }
      (*index)++;
    }
    cursor = cursor->next;
  }
  return false;
}

bool get_variable_data(VariableResolver *vr, wchar_t *name,
  Expression *expression) {
  if (vr == NULL || *vr == NULL || expression == NULL) return false;
  VariableResolver cursor = *vr;
  while (cursor != NULL) {
    if (cursor->name != NULL) {
      if (wcscmp(name, cursor->name) == 0) {
        *expression = cursor->expression;
        return true;
      }
    }
    cursor = cursor->next;
  }
  return false;
}
