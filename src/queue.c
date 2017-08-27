#include "queue.h"

#include <stdlib.h>

Queue new_queue() {
  Queue queue = malloc(sizeof(struct queue));
  queue->first = NULL;
  queue->last = NULL;
  queue->length = 0;
  return queue;
}

void free_queue(Queue *queue) {
  if (queue == NULL || *queue == NULL) return;
  struct queue_node *cursor = (*queue)->first, *to_delete = (*queue)->first;
  while (cursor != NULL) {
    to_delete = cursor;
    cursor = cursor->next;
    free(to_delete);
  }
  free(*queue);
  *queue = NULL;
}

bool is_empty_queue(Queue queue) {
  return queue->first == NULL && queue->last == NULL;
}

bool push_to_queue(Queue queue, void *data) {
  if (queue == NULL) return false;
  if (data == NULL) return false;
  struct queue_node *node = malloc(sizeof(struct queue_node));
  if (node == NULL) return false;
  node->data = data;
  node->next = NULL;
  if (queue->first == NULL) {
    queue->first = queue->last = node;
  } else {
    queue->last->next = node;
    queue->last = node;
  }
  queue->length++;
  return true;
}

void *pop_from_queue(Queue queue) {
  if (queue == NULL) return NULL;
  if (queue->first == NULL) return false;
  struct queue_node *to_delete = queue->first;
  void *data =  to_delete->data;
  if (queue->first == queue->last) {
    queue->last = queue->first = NULL;
  } else {
    queue->first = queue->first->next;
  }
  free(to_delete);
  queue->length--;
  return data;
}
