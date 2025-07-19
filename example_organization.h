#ifndef EXAMPLE_ORGANIZATION_H
#define EXAMPLE_ORGANIZATION_H

// ============================================================================
// FORWARD DECLARATIONS & INCLUDES
// ============================================================================
#include <iostream>
#include <string>

namespace Gnome {

// ============================================================================
// CLASS DECLARATIONS (Interface)
// ============================================================================

class GnomeEngine {
public:
    // Constructor and destructor
    GnomeEngine();
    ~GnomeEngine();
    
    // Core engine methods  
    bool Initialize(int width = 800, int height = 600, const std::string& title = "GnomeEngine");
    void Run();
    void Shutdown();
    
    // Getters
    bool IsRunning() const;
    
private:
    // Member variables
    bool m_running;
    std::string m_windowTitle;
    
    // Private methods
    bool InitializeWindow(int width, int height, const std::string& title);
};

// ============================================================================
// INLINE IMPLEMENTATIONS
// ============================================================================

// Constructor (IMPLEMENTATION)
inline GnomeEngine::GnomeEngine() 
    : m_running(false)
    , m_windowTitle("GnomeEngine")
{
    std::cout << "GnomeEngine constructor called" << std::endl;
}

// Destructor (IMPLEMENTATION) 
inline GnomeEngine::~GnomeEngine() {
    Shutdown();
}

// Initialize method (IMPLEMENTATION)
inline bool GnomeEngine::Initialize(int width, int height, const std::string& title) {
    m_windowTitle = title;
    m_running = true;
    std::cout << "Initializing " << title << " with size " << width << "x" << height << std::endl;
    return true;
}

// Run method (IMPLEMENTATION)
inline void GnomeEngine::Run() {
    if (!m_running) {
        std::cerr << "Engine not initialized!" << std::endl;
        return;
    }
    std::cout << "Running " << m_windowTitle << std::endl;
}

// Getter (IMPLEMENTATION)
inline bool GnomeEngine::IsRunning() const {
    return m_running;
}

// Private method (IMPLEMENTATION)
inline bool GnomeEngine::InitializeWindow(int width, int height, const std::string& title) {
    std::cout << "Initializing window: " << title << std::endl;
    return true;
}

} // namespace Gnome

#endif // EXAMPLE_ORGANIZATION_H
