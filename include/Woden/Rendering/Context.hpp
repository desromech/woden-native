#ifndef WODEN_RENDERING_CONTEXT_HPP
#define WODEN_RENDERING_CONTEXT_HPP

#include "AGPU/agpu.hpp"
#include <memory>

namespace Woden
{
namespace Rendering
{

typedef std::shared_ptr<class RenderingContext> RenderingContextPtr;
/**
 * I hold a specific rendering context
 */
class RenderingContext
{
public:
    static RenderingContextPtr getMainContext();
    static RenderingContextPtr getOrCreateMainContext(int argc, const char *argv[]);
    static void releaseMainContext();

    bool initialize(int argc, const char *argv[]);

    bool vsyncDisabled = false;
    agpu_device_ref device;
    agpu_command_queue_ref defaultCommandQueue;
private:
    static RenderingContextPtr mainContext;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_RENDERING_CONTEXT_HPP