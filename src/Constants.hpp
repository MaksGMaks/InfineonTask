#pragma once

/**
 * Constant namespace for default and constants values
 */
namespace constants {
    constexpr int DEFAULT_LIST_SIZE = 200;      // default size of list to prevent stack overflow
    constexpr int DEFAULT_SLEEP_TIME = 200;      // default time for thread sleep in ms

    namespace mainWindowSize {
        constexpr int WIDTH = 800;              
        constexpr int HEIGHT = 600;
    }

    namespace buttonsSize
    {
        constexpr int WIDTH = 200;
        constexpr int HEIGHT = 50;
    }
    
}