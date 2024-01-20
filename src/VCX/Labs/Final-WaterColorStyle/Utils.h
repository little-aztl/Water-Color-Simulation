#include <vector>
#include "Labs/Common/ImageRGB.h"
#include "Labs/Common/ImGuiHelper.h"
#include <set>
#include "mixbox.h"
#include <random>
#include <cmath>

#define eps 1e-6

namespace VCX::Labs::WaterColor_Namespace {
    extern const double PI;
    

    class InteractProxy {
    public:
        glm::vec2 MousePos() const;
        glm::vec2 MouseDeltaPos() const;
        bool      IsHovering() const;
        bool      IsClicking(bool left = true) const;
        bool      IsDragging(bool left = true) const;
        bool      IsMouseDown(bool left = true) const;
        glm::vec2 DraggingStartPoint(bool left = true) const;
        void      Update(ImVec2 const & size, ImVec2 const & pos, ImVec2 const & delta, bool hover, bool lHeld, bool rHeld);

    private:
        glm::ivec2 _size      = glm::ivec2(0);
        glm::vec2  _pos       = glm::vec2(0);
        glm::vec2  _dPos      = glm::vec2(0);
        glm::vec2  _lStart    = glm::vec2(0);
        glm::vec2  _rStart    = glm::vec2(0);
        bool       _hover     = false;
        bool       _move      = false;
        bool       _lClick    = false;
        bool       _rClick    = false;
        bool       _lDrag     = false;
        bool       _rDrag     = false;
        bool       _lLastDrag = false;
        bool       _rLastDrag = false;
    };
    /**
     * 画圆圈
     *
     * \param canvas 传入画布
     * \param color glm::vec4 圆圈的颜色
     * \param origin glm::vec2 圆圈的中心
     * \param radius float 圆圈的半径
     * \param width float 画笔的宽度
     */
    void DrawCircle(Common::ImageRGB & canvas, glm::vec4 color, glm::vec2 origin, float radius, float width);

    /**
     * 0~256颜色转0~1颜色
     * 
     * \param r int
     * \param g int
     * \param b int
     * \param a float 
     * \return glm::vec4
     */
    glm::vec4 Colori2f(int r, int g, int b, float a);

    /**
     * 生成正多边形。
     *
     * \param result 顶点链表的引用
     * \param n 正多边形的边数
     * \param origin 正多边形的中心
     * \param radius 半径
     * \return list<glm::vec2> 顶点
     */
    void RegularPolygen(std::list<glm::vec2> & result, int n, glm::vec2 origin, float radius);

    inline glm::ivec2 Rasterize(glm::vec2 p, int sx, int sy);

    inline glm::ivec2 Rasterize(float x, float y, int sx, int sy);

    inline int RasterizeX(float x, int sx);

    inline int RasterizeY(float y, int sy);

    glm::ivec2 MakeIn(glm::ivec2 p, int sx, int sy);

    /**
     * 按照透明度设置某个像素的RGBA颜色
     * 
     * \param canvas 画布
     * \param x 横坐标
     * \param y 纵坐标
     * \param color RGBA颜色
     */
    void SetRGBA(Common::ImageRGB & canvas, int x, int y, glm::vec4 color, bool pigment_mixing);

    struct Edge {
        float x, dx;
        int   ymax;
        bool operator<(const Edge & cur) const;
    };

    /**
     * 将多边形的顶点转化成边表
     * 
     * \param vertices std::list<glm::vec2> 多边形的顶点
     * \param edges std::multiset<Edge> 将要生成的边表
     * \param canvas 画布
     */
    void EdgesList(
        std::list<glm::vec2> & vertices, 
        std::vector<std::vector<Edge> >&edges, 
        int sx,
        int sy
    );

    void DrawFilledPolygen(std::list<glm::vec2> & vertices, Common::ImageRGB & canvas, glm::vec4 color, bool pigment_mixing);

    glm::vec2 RandomlyModifyVertex(glm::vec2 p, std::uniform_real_distribution<float> & uni_distr, std::normal_distribution<float> & norm_distr);

    void DeformPolygenOnce(std::list<glm::vec2> & vertices, float var);

    /**
     * 改变规则多边形的外形，迭代多次
     * 
     * \param vertices 原多边形的顶点
     * \param iter 迭代次数
     * \param init_var 初始方差
     */
    void DeformPolygenMultipleTimes(std::list<glm::vec2> & vertices, int iter, float init_var, float vdiv);
} // namespace VCX::Labs::WaterColor_Namespace




