#pragma once

#include <stdbool.h>
#include <stddef.h>

struct queue_node {
  void *data;
  struct queue_node *next;
};

typedef struct queue {
  struct queue_node *first;
  struct queue_node *last;
  size_t length;
} *Queue;

Queue new_queue();
void free_queue(Queue *queue);

bool is_empty_queue(Queue queue);

bool push_to_queue(Queue queue, void *data);
void *pop_from_queue(Queue queue);
