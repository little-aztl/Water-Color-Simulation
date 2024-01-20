#include "Labs/Final-WaterColorStyle/Utils.h"


namespace VCX::Labs::WaterColor_Namespace {
    std::default_random_engine gen;
    extern const double        PI = acos(-1);

    glm::vec2 InteractProxy::MousePos() const {
        return _pos;
    }

    glm::vec2 InteractProxy::MouseDeltaPos() const {
        return _dPos;
    }

    bool InteractProxy::IsHovering() const {
        return _hover;
    }

    bool InteractProxy::IsClicking(bool left) const {
        return left ? _lClick : _rClick;
    }

    bool InteractProxy::IsDragging(bool left) const {
        return (left ? _lDrag : _rDrag) && _move;
    }

    glm::vec2 InteractProxy::DraggingStartPoint(bool left) const {
        return left ? _lStart : _rStart;
    }

    void InteractProxy::Update(ImVec2 const & size, ImVec2 const & pos, ImVec2 const & delta, bool hover, bool lHeld, bool rHeld) {
        if (! _lLastDrag && _lDrag) _lStart = glm::vec2(pos.x / size.x, pos.y / size.y);
        if (! _rLastDrag && _rDrag) _rStart = glm::vec2(pos.x / size.x, pos.y / size.y);
        _pos       = glm::vec2(pos.x / size.x, pos.y / size.y);
        _dPos      = glm::vec2(delta.x / size.x, delta.y / size.y);
        _hover     = hover && _pos.x <= 1.f && _pos.y <= 1.f && _pos.x >= 0.f && _pos.y >= 0.f;
        _move      = delta.x != 0 || delta.y != 0;
        _lClick    = _lLastDrag && ! _lDrag && _lStart == _pos;
        _rClick    = _rLastDrag && ! _rDrag && _rStart == _pos;
        _lLastDrag = _lDrag;
        _rLastDrag = _rDrag;
        _lDrag     = lHeld;
        _rDrag     = rHeld;
    }

    bool InteractProxy::IsMouseDown(bool left) const {
        if (left) return _lDrag;
        return _rDrag;
    }

    
    void DrawCircle(Common::ImageRGB & canvas, glm::vec4 color, glm::vec2 origin, float radius, float width) {
        //printf("%.2lf %.2lf\n", origin.x, origin.y);
        width  = std::max(1.0f, width * canvas.GetSizeX() * 0.001f);
        int y0 = static_cast<int>(std::floor(origin.y * (canvas.GetSizeY() - 1)));
        int x0 = static_cast<int>(std::floor(origin.x * (canvas.GetSizeX() - 1)));
        int r1 = static_cast<int>(std::round(radius * (canvas.GetSizeX() - 1) - width * 0.5f));
        int y1 = r1;
        int D1 = 5 - 4 * r1;
        int r2 = static_cast<int>(std::round(radius * (canvas.GetSizeX() - 1) + width * 0.5f));
        int x2 = 0;
        int y2 = r2;
        int D2 = 5 - 4 * r2;
        for (int y = y1; y <= y2; ++y) {
            for (auto yy : std::array<int, 2>({ y0 + y, y0 - y })) {
                if (static_cast<std::size_t>(yy) < canvas.GetSizeY() - 1 && static_cast<std::size_t>(x0) < canvas.GetSizeX() - 1) {
                    auto && proxy = canvas.At(x0, yy);
                    proxy         = glm::vec3(color) * color.a + static_cast<glm::vec3>(proxy) * (1.0f - color.a);
                }
                if (x2 != y && static_cast<std::size_t>(y0) < canvas.GetSizeY() - 1 && static_cast<std::size_t>(yy - y0 + x0) < canvas.GetSizeX() - 1) {
                    auto && proxy = canvas.At(yy - y0 + x0, y0);
                    proxy         = glm::vec3(color) * color.a + static_cast<glm::vec3>(proxy) * (1.0f - color.a);
                }
                if (y == 0) break;
            }
        }
        while (y2 > x2) {
            x2++;
            if (D2 > 0) {
                y2--;
                D2 += 8 * (x2 - y2) + 4;
            } else D2 += 8 * x2 + 4;
            if (y1 < x2) {
                y1 = x2;
            } else if (D1 > 0) {
                y1--;
                D1 += 8 * (x2 - y1) + 4;
            } else D1 += 8 * x2 + 4;
            for (auto xx : std::array<int, 2>({ x0 + x2, x0 - x2 })) {
                for (int y = y1; y <= y2; ++y) {
                    for (auto yy : std::array<int, 2>({ y0 + y, y0 - y })) {
                        if (static_cast<std::size_t>(yy) < canvas.GetSizeY() - 1 && static_cast<std::size_t>(xx) < canvas.GetSizeX() - 1) {
                            auto && proxy = canvas.At(xx, yy);
                            proxy         = glm::vec3(color) * color.a + static_cast<glm::vec3>(proxy) * (1.0f - color.a);
                        }
                        if (x2 != y && static_cast<std::size_t>(xx - x0 + y0) < canvas.GetSizeY() - 1 && static_cast<std::size_t>(yy - y0 + x0) < canvas.GetSizeX() - 1) {
                            auto && proxy = canvas.At(yy - y0 + x0, xx - x0 + y0);
                            proxy         = glm::vec3(color) * color.a + static_cast<glm::vec3>(proxy) * (1.0f - color.a);
                        }
                        if (y == 0) break;
                    }
                }
                if (x2 == 0) break;
            }
        }
    }

    glm::vec4 Colori2f(int r, int g, int b, float a) {
        return glm::vec4(1.0F * r / 256, 1.0F * g / 256, 1.0F * b / 256, a);
    }

    inline glm::ivec2 Rasterize(glm::vec2 p, int sx, int sy) {
        int x = roundf(p.x * sx), y = roundf(p.y * sy);
        return glm::uvec2(x, y);
    }

    inline glm::ivec2 Rasterize(float x, float y, int sx, int sy) {
        return Rasterize(glm::vec2(x, y), sx, sy);
    }

    inline int RasterizeX(float x, int sx) {
        return roundf(x * sx);
    }

    inline int RasterizeY(float y, int sy) {
        return roundf(y * sy);
    }

    bool Edge::operator<(const Edge& cur) const{
        int x1 = roundf(x), x2 = roundf(cur.x);
        if (x1 != x2) return x1 < x2;
        return dx < cur.dx;
    }

    glm::ivec2 MakeIn(glm::ivec2 p, int sx, int sy) {
        if (p.x < 0) p.x = 0;
        if (p.x >= sx) p.x = sx - 1;
        if (p.y < 0) p.y = 0;
        if (p.y >= sy) p.y = sy - 1;
        return p;
    }

    void RegularPolygen(std::list<glm::vec2>&result, int n, glm::vec2 origin, float radius) {
        result.clear();
        double               angle = 0, delta = 2 * PI / n;
        for (int i = 0; i < n; ++i) {
            glm::vec2 cur(glm::cos(angle) * radius, glm::sin(angle) * radius);
            cur += origin;
            result.push_back(cur);
            angle += delta;
        }
    }

    void EdgesList(
        std::list<glm::vec2> & vertices, 
        std::vector<std::vector<Edge> > &edges, 
        int sx, 
        int sy
    ) {
        for (std::list<glm::vec2>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
            auto nxt = it;
            ++nxt;
            if (nxt == vertices.end()) nxt = vertices.begin();
            glm::ivec2 p1 = Rasterize(*it, sx, sy);
            p1            = MakeIn(p1, sx, sy);
            glm::ivec2 p2 = Rasterize(*nxt, sx, sy);
            p2            = MakeIn(p2, sx, sy);
            if (p1.y == p2.y) continue;
            if (p1.y > p2.y) std::swap(p1, p2);
            Edge cur;
            cur.x    = p1.x;
            cur.ymax = p2.y;
            cur.dx   = (float) (p1.x - p2.x) / (p1.y - p2.y);
            edges[p1.y].push_back(cur);
        }
    }

    glm::vec2 RandomlyModifyVertex(glm::vec2 p, std::uniform_real_distribution<float>& uni_distr, std::normal_distribution<float>& norm_distr) {
        float angle = uni_distr(gen);
        float len   = norm_distr(gen);
        p.x += glm::cos(angle) * len;
        p.y += glm::sin(angle) * len;
        return p;
    }

    void DeformPolygenOnce(std::list<glm::vec2>& vertices, float var) {
        std::normal_distribution<float> norm_distr(0, var);
        std::uniform_real_distribution<float> uni_distr(0, 2 * PI);
        for (std::list<glm::vec2>::iterator it = vertices.begin(); it != vertices.end();) {
            auto nxt = it;
            ++nxt;
            if (nxt == vertices.end()) nxt = vertices.begin();

            auto mid_point = RandomlyModifyVertex(((*it) + (*nxt)) / 2.0F, uni_distr, norm_distr);
            if (nxt != vertices.begin())
                vertices.insert(nxt, mid_point);
            else vertices.insert(vertices.end(), mid_point);
            ++ ++it;
        }
    }

    void DeformPolygenMultipleTimes(std::list<glm::vec2>& vertices, int iter, float init_var, float vdiv) {
        for (int step = 1; step <= iter; ++step) {
            DeformPolygenOnce(vertices, init_var);
            init_var /= vdiv;
        }
    }

    void SetRGBA(Common::ImageRGB&canvas, int x, int y, glm::vec4 color) {
        auto && proxy = canvas.At(x, y);
        proxy         = glm::vec3(color) * color.a + static_cast<glm::vec3>(proxy) * (1.0f - color.a);
    }

    void DrawFilledPolygen(std::list<glm::vec2> & vertices, Common::ImageRGB & canvas, glm::vec4 color) {
        std::vector<std::vector<Edge> > edges_list;
        edges_list.resize(canvas.GetSizeY() + 1);
        EdgesList(vertices, edges_list, canvas.GetSizeX(), canvas.GetSizeY());
        for (int y = 0; y < canvas.GetSizeY(); ++y) {
            for (int i = 0; i < (int)edges_list[y].size(); ) {
                if (edges_list[y][i].ymax == y) {
                    edges_list[y].erase(edges_list[y].begin() + i);
                } else ++i;
            }
            if (edges_list[y].size() % 2 == 1) {
                throw "the length of edge_list is odd!";
            }
            if (edges_list[y].empty()) continue;
            std::sort(edges_list[y].begin(), edges_list[y].end());
            for (int i = 0; i < (int)edges_list[y].size(); i += 2) {
                int j = i + 1;
                int x1 = roundf(edges_list[y][i].x), x2 = roundf(edges_list[y][j].x);
                for (int x = x1; x < x2; ++x) {
                    SetRGBA(canvas, x, y, color);
                }

                Edge new_edge = edges_list[y][i];
                new_edge.x += new_edge.dx;
                edges_list[y + 1].push_back(new_edge);

                new_edge = edges_list[y][j];
                new_edge.x += new_edge.dx;
                edges_list[y + 1].push_back(new_edge);
            }
        }
    }
}
