#include "GnomeEngine.h"
#include <iostream>

// Test function to verify LSP navigation
void testFunction() {
    std::cout << "Testing LSP navigation" << std::endl;
}

int main() {
    // Test LSP features:
    // 1. Place cursor on 'testFunction' below and try 'gd' to go to definition
    testFunction();
    
    // 2. Place cursor on 'GnomeEngine' below and try 'gd' to go to definition
    Gnome::GnomeEngine engine;
    
    // 3. Place cursor on 'Initialize' and try 'gd'
    engine.Initialize(800, 600, "Test");
    
    return 0;
}
