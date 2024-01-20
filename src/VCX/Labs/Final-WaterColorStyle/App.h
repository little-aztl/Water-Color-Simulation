#pragma once

#include <vector>

#include "Engine/app.h"
#include "Labs/Common/UI.h"
#include "Labs/Final-WaterColorStyle/WaterColor_Case.h"

namespace VCX::Labs::WaterColor_Namespace {
    class App : public Engine::IApp {
    private:
        Common::UI    _ui;
        std::size_t   _caseId = 0;
        WaterColor_Case _waterColorCase;

        std::vector<std::reference_wrapper<Common::ICase>> _cases = {
            _waterColorCase
        };

    public:
        App();

        void OnFrame() override;
    };
}
