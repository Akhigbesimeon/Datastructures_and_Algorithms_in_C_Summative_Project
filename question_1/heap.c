#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

// Implementation
void init_log(EventLog* log) {
    log->head = NULL;
    log->tail = NULL;
    log->cursor = NULL;
    log->count = 0;
    log->live_mode = false; 
    log->next_event_id = 1;
}

// Core Logic
void log_event(EventLog* log, float power, float voltage, const char* type) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("CRITICAL ERROR: Heap Overflow\n");
        return;
    }

    // Populate Data
    new_node->data.id = log->next_event_id++;
    new_node->data.power_w = power;
    new_node->data.voltage_v = voltage;
    new_node->data.timestamp = time(NULL);
    strncpy(new_node->data.type, type, 15);
    new_node->next = NULL;
    new_node->prev = NULL;

    // Handle Capacity
    if (log->count >= MAX_EVENTS) {
        Node* victim = log->head;
        
        if (log->cursor == victim) {
            log->cursor = victim->next;
        }

        if (log->head->next) {
            log->head = log->head->next;
            log->head->prev = NULL;
        } else {
            log->head = NULL;
            log->tail = NULL;
        }

        free(victim);
        log->count--; 
    }

    // Link New Node
    if (log->tail == NULL) {
        log->head = new_node;
        log->tail = new_node;
        if (log->cursor == NULL) log->cursor = new_node; 
    } else {
        log->tail->next = new_node;
        new_node->prev = log->tail;
        log->tail = new_node;
    }

    log->count++;
    
    if (log->live_mode) {
        log->cursor = log->tail;
    }
}

// Function to delete all events in the log and frees allocated memory
void clear_log(EventLog* log) {
    Node* current = log->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    log->head = NULL;
    log->tail = NULL;
    log->cursor = NULL;
    log->count = 0;
}

// Function to move forward to a newer event
void move_next(EventLog* log) {
    if (log->cursor && log->cursor->next) {
        log->cursor = log->cursor->next;
    } else {
        printf(">> Info: At newest event.\n");
    }
}

// Function to move backwards to an older event
void move_prev(EventLog* log) {
    if (log->cursor && log->cursor->prev) {
        log->cursor = log->cursor->prev;
    } else {
        printf(">> Info: At oldest event.\n");
    }
}

// Function to display the sensor data currently pointed to
void print_cursor(EventLog* log) {
    if (!log->cursor) {
        printf("[Display] No events log.\n");
        return;
    }

    EventData* d = &log->cursor->data;
    printf("-------------------------------------------\n");
    printf(" EVENT ID: %d [%s] (Log Size: %d/%d)\n", 
           d->id, 
           (log->cursor == log->head) ? "OLDEST" : (log->cursor == log->tail ? "NEWEST" : "SAVED"), 
           log->count, MAX_EVENTS);
    printf(" Time: %s", ctime(&d->timestamp));
    printf(" Type: %-10s | Power: %6.2f W | Volt: %6.2f V\n", d->type, d->power_w, d->voltage_v);
    printf("-------------------------------------------\n");
}

// Function to generate mock data
void simulate_sensor_input(EventLog* log) {
    float p = 220.0 + ((rand() % 2000) / 100.0);
    float v = 110.0 + ((rand() % 100) / 10.0);
    
    const char* types[] = {"Normal", "Normal", "Normal", "HighLoad", "VoltSag"};
    const char* type = types[rand() % 5];
    
    log_event(log, p, v, type);
}
