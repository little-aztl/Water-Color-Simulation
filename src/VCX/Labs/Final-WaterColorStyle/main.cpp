#include "Assets/bundled.h"
#include "Labs/Final-WaterColorStyle/App.h"

int main() {
    using namespace VCX;
    return Engine::RunApp<Labs::WaterColor_Namespace::App>(Engine::AppContextOptions {
        .Title      = "VCX Final Lab: Water Color Simulation",
        .WindowSize = {1024, 768},
        .FontSize   = 16,

        .IconFileNames = Assets::DefaultIcons,
        .FontFileNames = Assets::DefaultFonts,
    });
}
