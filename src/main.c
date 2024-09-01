#include "cognition.h"

int main(int argc, char* argv[]) {
    CognitionEngine engine;
    
    if (!cognition_init(&engine, "Cognition Engine Test", 800, 600)) {
        return 1;
    }
    
    cognition_run(&engine);
    
    cognition_shutdown(&engine);
    
    return 0;
}