#include <iostream>
#include "things.h"
#include "minecart.h"

namespace minecart
{
    namespace testing
    {
        void printSomething() {
            minecart::logging::log_info << "[Testing] "<< someString  << std::endl;
        }
    }
}