#pragma once

#include "sokka/core/Base.hpp"
#include "sokka/ui/UIPanel.hpp"
namespace Sokka {

inline constexpr const char* SPRITE_FILE_PATH = "assets/sprites/";

class Menubar : public UIPanel {
public:
    Menubar() = default;
    ~Menubar() = default;

    void render() override;

    static Unique<Menubar> create();

private:
    void renderFile();
    void renderEdit();
    void renderView();

    void openImportDialog();
    void requestExitSDL();
};

}  // namespace Sokka
