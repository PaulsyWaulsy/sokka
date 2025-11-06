#include "core/Application.hpp"
#include "core/Base.hpp"
#include "core/Logger.hpp"

int main() {
    auto app = Sokka::makeUnique<Sokka::Application>();
    app->run();

    SOKKA_INFO("Shutting Down Editor");
    return 0;
}
