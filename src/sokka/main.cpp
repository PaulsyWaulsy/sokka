#include "sokka/core/Application.hpp"
#include "sokka/core/Base.hpp"
#include "sokka/core/Logger.hpp"

int main() {
    auto app = Sokka::makeUnique<Sokka::Application>();
    app->run();

    SOKKA_INFO("Shutting Down Editor");
    return 0;
}
