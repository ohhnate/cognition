#include "core/cognition.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    CognitionEngine engine;
   
    if (!cognition_init(&engine, "Cognition Engine Test", 800, 600)) {
        printf("Failed to initialize Cognition Engine\n");
        return 1;
    }
   
    cognition_run(&engine);
   
    cognition_shutdown(&engine);
   
    printf("Program finished. Waiting for 5 seconds before closing...\n");
    SDL_Delay(5000);  // Wait for 5 seconds
   
    return 0;
}