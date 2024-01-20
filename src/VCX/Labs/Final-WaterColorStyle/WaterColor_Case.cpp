#include "Labs/Final-WaterColorStyle/WaterColor_Case.h"



namespace VCX::Labs::WaterColor_Namespace {
    
    WaterColor_Case::WaterColor_Case() : 
        _viewer (
            Engine::GL::SamplerOptions{
                .MinFilter = Engine::GL::FilterMode::Linear,
                .MagFilter             = Engine::GL::FilterMode::Nearest 
        }),
        _pigmentMix(false),
        _canvas(c_Size.first, c_Size.second),
        _radius(0.1),
        _msaa(false),
        _ratio(0.5),
        _color(0, 0, 0)
         {
        _canvas.Fill(glm::vec3(1));
        _temp_color[0] = _temp_color[1] = _temp_color[2] = 0;
    }

    
    void WaterColor_Case::OnSetupPropsUI() {
        if (ImGui::Checkbox("Anti-Aliasing", &_msaa)) {
            _canvas = Common::ImageRGB(c_Size.first * (_msaa ? 2 : 1), c_Size.second * (_msaa ? 2 : 1));
            _canvas.Fill(glm::vec3(1));
        }
        ImGui::Checkbox("Pigment Mixing Simulation", &_pigmentMix);
        ImGui::SliderFloat("Radius", &_radius, 0, 0.5);
        ImGui::SliderFloat("Blend Ratio", &_ratio, 0, 1);
        if (ImGui::ColorPicker3("Color", _temp_color)) {
            _color = glm::vec3(_temp_color[0], _temp_color[1], _temp_color[2]);
        }
    }

    
    void WaterColor_Case::PreviewCircle(Common::ImageRGB& result) {
        result = _canvas;
        if (_proxy.IsMouseDown()) return;
        DrawCircle(result, Colori2f(194, 195, 201, 1), _proxy.MousePos(), _radius, 3);
    }

    void WaterColor_Case::DrawWaterColorBlob() {
        if (!_proxy.IsClicking()) return;
        if (! _proxy.IsHovering()) return;
        Common::ImageRGB     layer(_canvas.GetSize());
        layer.Fill(glm::vec3(1));
        std::list<glm::vec2> vertices;
        RegularPolygen(vertices, 10, _proxy.MousePos(), _radius);
        DeformPolygenMultipleTimes(vertices, 5, _radius / 3, 2);
        for (int lyer = 1; lyer <= 30; ++lyer) {
            auto cur_vert = vertices;
            DeformPolygenMultipleTimes(cur_vert, 4, _radius / 10, 4);
            DrawFilledPolygen(cur_vert, layer, glm::vec4(_color, 0.04));
        }
        Merge(layer);
    }

    bool WaterColor_Case::Check_White(glm::vec3 c) {
        return c.r > 1 - eps && c.b > 1 - eps && c.g > 1 - eps;
    }

    void WaterColor_Case::Merge(const Common::ImageRGB& layer) {
        
        for (int x = 0; x < _canvas.GetSizeX(); ++x) {
            for (int y = 0; y < _canvas.GetSizeY(); ++y) {
                auto && proxy = _canvas.At(x, y);
                glm::vec3 cur_color = layer.At(x, y), ori_color = _canvas.At(x, y);
                if (Check_White(cur_color)) {
                    continue;
                }
                if (Check_White(ori_color)) {
                    proxy = cur_color;
                    continue;
                }
                if (!_pigmentMix) {
                    proxy = _ratio * cur_color + (1 - _ratio) * ori_color;
                } else {
                    float r, g, b;
                    mixbox_lerp_float(
                        cur_color[0], cur_color[1], cur_color[2], 
                        ori_color[0], ori_color[1], ori_color[2], 
                        _ratio, 
                        &r, &g, &b
                    );
                    proxy = glm::vec3(r, g, b);
                }
            }
        }
        
    }

    
    Common::CaseRenderResult WaterColor_Case::OnRender(
        std::pair<std::uint32_t, std::uint32_t> const desiredSize
    ) {
        Common::ImageRGB temp;
        DrawWaterColorBlob();
        PreviewCircle(temp);
        _viewer.Update(temp);
        return Common::CaseRenderResult {
            .Fixed     = true,
            .Image     = _viewer,
            .ImageSize = c_Size
        };
    }

    
    void WaterColor_Case::OnProcessInput(ImVec2 const& pos) {
        auto         window  = ImGui::GetCurrentWindow();
        bool         hovered = false;
        bool         anyHeld = false;
        auto &       io      = ImGui::GetIO();
        ImVec2 const delta   = io.MouseDelta;
        ImGui::ButtonBehavior(window->Rect(), window->GetID("##io"), &hovered, &anyHeld);
        _proxy.Update(
            ImVec2(c_Size.first, c_Size.second),
            pos,
            delta,
            hovered,
            ImGui::IsMouseDown(ImGuiMouseButton_Left),
            ImGui::IsMouseDown(ImGuiMouseButton_Right)
        );
    }
}
