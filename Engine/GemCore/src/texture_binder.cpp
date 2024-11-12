/*#include <GlfwGlad.h>

namespace Gem {

	class TextureBinder {
	public:
		TextureBinder(GLenum target, GLuint texture) : target_(target) {
			GL::active_texture(GL_TEXTURE0 + texture_unit_);
			GL::bind_texture(GL_TEXTURE_2D_ARRAY, texture_array_ID_);
		}
		~TextureBinder() {
			GL::bind_texture(GL_TEXTURE_2D_ARRAY, 0);
		}
	private:
		GLenum target_;
	};

} // namespace Gem
*/