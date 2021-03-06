#include <mbgl/renderer/render_static_data.hpp>
#include <mbgl/gfx/context.hpp>
#include <mbgl/programs/program_parameters.hpp>

namespace mbgl {

static gfx::VertexVector<gfx::Vertex<PositionOnlyLayoutAttributes>> tileVertices() {
    gfx::VertexVector<gfx::Vertex<PositionOnlyLayoutAttributes>> result;
    result.emplace_back(gfx::Vertex<PositionOnlyLayoutAttributes>({{{ 0,            0 }}}));
    result.emplace_back(gfx::Vertex<PositionOnlyLayoutAttributes>({{{ util::EXTENT, 0 }}}));
    result.emplace_back(gfx::Vertex<PositionOnlyLayoutAttributes>({{{ 0, util::EXTENT }}}));
    result.emplace_back(gfx::Vertex<PositionOnlyLayoutAttributes>({{{ util::EXTENT, util::EXTENT }}}));
    return result;
}

static gfx::IndexVector<gfx::Triangles> quadTriangleIndices() {
    gfx::IndexVector<gfx::Triangles> result;
    result.emplace_back(0, 1, 2);
    result.emplace_back(1, 2, 3);
    return result;
}

static gfx::IndexVector<gfx::LineStrip> tileLineStripIndices() {
    gfx::IndexVector<gfx::LineStrip> result;
    result.emplace_back(0);
    result.emplace_back(1);
    result.emplace_back(3);
    result.emplace_back(2);
    result.emplace_back(0);
    return result;
}

static gfx::VertexVector<RasterLayoutVertex> rasterVertices() {
    gfx::VertexVector<RasterLayoutVertex> result;
    result.emplace_back(RasterProgram::layoutVertex({ 0, 0 }, { 0, 0 }));
    result.emplace_back(RasterProgram::layoutVertex(Point<int16_t>(util::EXTENT, 0), Point<uint16_t>(util::EXTENT, 0 )));
    result.emplace_back(RasterProgram::layoutVertex(Point<int16_t>(0, util::EXTENT), Point<uint16_t>(0, util::EXTENT)));
    result.emplace_back(RasterProgram::layoutVertex(Point<int16_t>(util::EXTENT, util::EXTENT ), Point<uint16_t>(util::EXTENT, util::EXTENT)));
    return result;
}

static gfx::VertexVector<HeatmapTextureLayoutVertex> heatmapTextureVertices() {
    gfx::VertexVector<HeatmapTextureLayoutVertex> result;
    result.emplace_back(HeatmapTextureProgram::layoutVertex({ 0, 0 }));
    result.emplace_back(HeatmapTextureProgram::layoutVertex({ 1, 0 }));
    result.emplace_back(HeatmapTextureProgram::layoutVertex({ 0, 1 }));
    result.emplace_back(HeatmapTextureProgram::layoutVertex({ 1, 1 }));
    return result;
}

RenderStaticData::RenderStaticData(gfx::Context& context, float pixelRatio, const optional<std::string>& programCacheDir)
    : tileVertexBuffer(context.createVertexBuffer(tileVertices())),
      rasterVertexBuffer(context.createVertexBuffer(rasterVertices())),
      heatmapTextureVertexBuffer(context.createVertexBuffer(heatmapTextureVertices())),
      quadTriangleIndexBuffer(context.createIndexBuffer(quadTriangleIndices())),
      tileBorderIndexBuffer(context.createIndexBuffer(tileLineStripIndices())),
      programs(context, ProgramParameters { pixelRatio, false, programCacheDir })
#ifndef NDEBUG
    , overdrawPrograms(context, ProgramParameters { pixelRatio, true, programCacheDir })
#endif
{
    tileTriangleSegments.emplace_back(0, 0, 4, 6);
    tileBorderSegments.emplace_back(0, 0, 4, 5);
    rasterSegments.emplace_back(0, 0, 4, 6);
    heatmapTextureSegments.emplace_back(0, 0, 4, 6);
}

} // namespace mbgl
