#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heap.h"

// Main function
int main() {
    EventLog system_log;
    init_log(&system_log);
    
    char command;
    bool running = true;
 
    srand(time(NULL));

    printf("== Smart Meter Gateway Firmware ==\n");
    printf("Controls: [n]ext, [p]rev, [r]un live, [h]old live, [c]lear, [x]exit\n");
    printf("NB:In this sim, pressing [ENTER] simulates a sensor reading cycle.\n");

    while (running) {
        if (system_log.live_mode || (rand() % 100 < 30)) {
            simulate_sensor_input(&system_log);
        }

        if (system_log.live_mode) {
            if (system_log.tail) {
                printf("[LIVE MONITOR] New Event Logged: ID %d | %.2fW\n", 
                       system_log.tail->data.id, system_log.tail->data.power_w);
            }
        } else {
            print_cursor(&system_log);
        }

        // User Input
        printf("\nCMD > ");
        command = getchar();
        
        // Prevents double-looping
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }

        switch (command) {
            case 'n': 
                move_next(&system_log); 
                break;
            case 'p': 
                move_prev(&system_log); 
                break;
            case 'r': 
                system_log.live_mode = true; 
                printf(">> System entered LIVE mode.\n");
                system_log.cursor = system_log.tail; 
                break;
            case 'h': 
                system_log.live_mode = false; 
                printf(">> System entered HOLD mode.\n");
                break;
            case 'c': 
                clear_log(&system_log); 
                printf(">> Memory Cleared.\n");
                break;
            case 'x': 
                printf(">> Shutting down. Saving state...\n");
                clear_log(&system_log); 
                running = false; 
                break;
            default:
                break;
        }
    }
    return 0;
}
