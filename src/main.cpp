#include "EditorApp.hpp"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    EditorApp app("Sokka Editor", 1280, 720);
    app.run();
    return 0;
}
