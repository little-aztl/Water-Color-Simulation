#pragma once

#include "Engine/Async.hpp"
#include "Labs/Common/ICase.h"
#include "Labs/Common/ImageRGB.h"
#include "Labs/Final-WaterColorStyle/Utils.h"
#include "Labs/Common/ImGuiHelper.h"

namespace VCX::Labs::WaterColor_Namespace {
    static constexpr auto c_Size = std::pair(900U, 900U);
    
    class WaterColor_Case : public Common::ICase {
    public:
        
        WaterColor_Case();
        virtual std::string_view const GetName() override {
            return "Water Color Simulatation";
        }

        // 配置侧边栏。
        virtual void OnSetupPropsUI() override;

        /**
         * 主页面的显示。
         *
         * \param desiredSize 显示所需大小
         * \return 一个CaseRenderResult类
         */
        virtual Common::CaseRenderResult OnRender(
            std::pair<std::uint32_t, std::uint32_t> const desiredSize
        ) override;
        
        /**
         * 与用户交互的处理。
         *
         * \param pos 当前鼠标的位置
         */
        virtual void OnProcessInput(ImVec2 const & pos) override;

        /**
         * 预览水彩晕染的大致范围。
         *
         * \param result 将圆圈画在canvas上的结果
         */
        void PreviewCircle(Common::ImageRGB &_result);


        // 在画布上模拟一小片水彩的晕染效果。
        void DrawWaterColorBlob();

        /**
         * 根据_pigmentMix，将一层新的图层合并到画布上。
         * 
         * \param layer 新的图层
         */
        void Merge(const Common::ImageRGB & layer);


        bool Check_White(glm::vec3 c);
        
    private:
        Engine::GL::UniqueTexture2D _viewer;
        bool                        _pigmentMix;
        float                         _radius, _ratio;
        Common::ImageRGB            _canvas;
        InteractProxy               _proxy;
        bool                          _msaa;
        glm::vec3                     _color;
        float                         _temp_color[3];
    };
}