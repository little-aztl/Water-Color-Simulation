#include "Labs/Final-WaterColorStyle/App.h"

namespace VCX::Labs::WaterColor_Namespace {

    App::App() :
        _ui(Labs::Common::UIOptions { }) {
    }

    void App::OnFrame() {
        _ui.Setup(_cases, _caseId);
    }
}
