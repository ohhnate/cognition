#include "core/cognition.h"
#include "core/utils/cog_error.h"
#include <stdio.h>

#include <signal.h>

void signal_handler(int signal) {
    cog_log_error("Caught signal %d", signal);
    exit(1);
}

int main(int argc, char* argv[]) {
    // Set up signal handling
    signal(SIGSEGV, signal_handler);
    signal(SIGABRT, signal_handler);

    CognitionEngine engine;
   
    if (!cognition_init(&engine, "Cognition Engine Test", 800, 600)) {
        cog_log_error("Failed to initialize Cognition Engine");
        cognition_shutdown(&engine);
        return 1;
    }
   
    cognition_run(&engine);
   
    cognition_shutdown(&engine);
   
    return 0;
}