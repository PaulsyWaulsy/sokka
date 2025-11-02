#pragma once

class GUI {
   public:
    GUI();
    ~GUI() = default;

    void render();

   private:
    void renderMenuBar();
    void renderDockspace();
    void renderPanels();
};
