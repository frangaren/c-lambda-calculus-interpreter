#include "variable_resolver.h"

#include <stdlib.h>
#include <string.h>

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
    free(to_delete);
  }
  *vr = NULL;
}

bool add_variable(VariableResolver *vr, char *name) {
  if (vr == NULL) return false;
  VariableResolver node = malloc(sizeof(struct variable_resolver));
  if (node == NULL) return false;
  node->next = *vr;
  node->name = malloc((strlen(name) + 1) * sizeof(char));
  if (node->name == NULL) {
    free(node);
    return false;
  }
  strcpy(node->name, name);
  *vr = node;
  return true;
}

bool add_variable_scope(VariableResolver *vr) {
  if (vr == NULL) return false;
  VariableResolver node = malloc(sizeof(struct variable_resolver));
  if (node == NULL) return false;
  node->next = *vr;
  node->name = NULL;
  *vr = node;
  return true;
}

bool drop_variable_scope(VariableResolver *vr) {
  if (vr == NULL || *vr == NULL) return false;
  VariableResolver to_delete = *vr, cursor = *vr;
  while (cursor != NULL) {
    if (cursor->name == NULL) break;
    to_delete = cursor;
    cursor = cursor->next;
    if (to_delete->name != NULL) free(to_delete->name);
    free(to_delete);
  }
  *vr = cursor;
  return true;
}

bool get_variable_bruijin(VariableResolver *vr, char *name, uint64_t *index) {
  if (vr == NULL || *vr == NULL) return false;
  VariableResolver cursor = *vr;
  *index = 0;
  while (cursor != NULL) {
    if (cursor->name != NULL) {
      if (strcmp(name, cursor->name) == 0) {
        return true;
      }
      (*index)++;
    }
    cursor = cursor->next;
  }
  return false;
}
