#pragma once

#include <vector>

#include "Canvas.hpp"
#include "EditorTool.hpp"
#include "Menubar.hpp"
#include "TilePallete.hpp"
#include "imgui.h"

class GUI {
   public:
    GUI();
    ~GUI() = default;

    void render();
    EditorTool getCurrentTool() const { return currentTool_; }

   private:
    void renderMenuBar();
    void renderDockspace();
    void renderToolbar();
    void renderCanvas();
    void renderTilePallete();

    TilePallete tilePallete_;
    Canvas canvas_;
    Menubar menubar_;

    EditorTool currentTool_ = EditorTool::Brush;
};
