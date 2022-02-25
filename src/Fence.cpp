#include <gsdf/Common.h>
#include <gsdf/Fence.h>
#include <numeric>

namespace GFX
{

  std::optional<Fence> Fence::Create()
  {
    return Fence{};
  }

  Fence::Fence()
  {
  }

  Fence::~Fence()
  {
    glDeleteSync(reinterpret_cast<GLsync>(sync_));
    sync_ = nullptr;
  }

  Fence::Fence(Fence&& old) noexcept
  {
    sync_ = std::exchange(old.sync_, nullptr);
  }

  Fence& Fence::operator=(Fence&& old) noexcept
  {
    if (this == &old) return *this;
    this->~Fence();
    sync_ = std::exchange(old.sync_, nullptr);
    return *this;
  }

  void Fence::Signal()
  {
    sync_ = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
  }

  uint64_t Fence::Wait()
  {
    GSDF_ASSERT(sync_ != nullptr);
    GLuint id;
    glGenQueries(1, &id);
    glBeginQuery(GL_TIME_ELAPSED, id);
    GLenum result = glClientWaitSync(reinterpret_cast<GLsync>(sync_), GL_SYNC_FLUSH_COMMANDS_BIT, std::numeric_limits<GLuint64>::max());
    GSDF_ASSERT(result == GL_CONDITION_SATISFIED);
    glEndQuery(GL_TIME_ELAPSED);
    uint64_t elapsed;
    glGetQueryObjectui64v(id, GL_QUERY_RESULT, &elapsed);
    glDeleteQueries(1, &id);
    this->~Fence();
    return elapsed;
  }
}