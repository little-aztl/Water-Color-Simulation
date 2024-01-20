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

        // ���ò������
        virtual void OnSetupPropsUI() override;

        /**
         * ��ҳ�����ʾ��
         *
         * \param desiredSize ��ʾ�����С
         * \return һ��CaseRenderResult��
         */
        virtual Common::CaseRenderResult OnRender(
            std::pair<std::uint32_t, std::uint32_t> const desiredSize
        ) override;
        
        /**
         * ���û������Ĵ���
         *
         * \param pos ��ǰ����λ��
         */
        virtual void OnProcessInput(ImVec2 const & pos) override;

        /**
         * Ԥ��ˮ����Ⱦ�Ĵ��·�Χ��
         *
         * \param result ��ԲȦ����canvas�ϵĽ��
         */
        void PreviewCircle(Common::ImageRGB &_result);


        // �ڻ�����ģ��һСƬˮ�ʵ���ȾЧ����
        void DrawWaterColorBlob();

        /**
         * ����_pigmentMix����һ���µ�ͼ��ϲ��������ϡ�
         * 
         * \param layer �µ�ͼ��
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