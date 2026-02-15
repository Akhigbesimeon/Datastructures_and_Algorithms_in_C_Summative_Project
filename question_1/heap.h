#ifndef HEAP_H
#define HEAP_H

#include <time.h>
#include <stdbool.h>

#define MAX_EVENTS 20

// Data Structures
typedef struct {
    int id;
    float power_w;
    float voltage_v;
    char type[16]; 
    time_t timestamp;
} EventData;

// Doubly Linked List Node
typedef struct Node {
    EventData data;
    struct Node* next; 
    struct Node* prev; 
} Node;

// Manager for the Log System
typedef struct {
    Node* head;      
    Node* tail;      
    Node* cursor;    
    int count;       
    bool live_mode;   
    int next_event_id;
} EventLog;

// Function Prototypes

// Initialization and Cleanup
void init_log(EventLog* log);
void clear_log(EventLog* log);

// Core Operations
void log_event(EventLog* log, float power, float voltage, const char* type);
void move_next(EventLog* log);
void move_prev(EventLog* log);

// Display & Simulation
void print_cursor(EventLog* log);
void simulate_sensor_input(EventLog* log);

#endif 
