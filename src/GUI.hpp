#pragma once

#include <vector>

#include "Canvas.hpp"
#include "EditorTool.hpp"
#include "Menubar.hpp"
#include "Texture.hpp"
#include "Tileset.hpp"
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
    void renderTileset();

    Canvas canvas_;
    Tileset tileset_;
    Menubar menubar_;

    EditorTool currentTool_ = EditorTool::Brush;
};
