#include "GnomeEngine.h"

int main() {
    // Test navigation: Place cursor on each symbol and try gd
    
    // 1. This should jump to line 48 (constructor declaration)
    Gnome::GnomeEngine engine;
    
    // 2. This should jump to line 56 (Initialize declaration) 
    engine.Initialize(800, 600, "Test");
    
    // 3. This should jump to line 57 (Run declaration)
    engine.Run();
    
    return 0;
}
