#include <Gem/Core/texture_binder.h>

namespace Gem {

	namespace Core {

		// Constructor
		TextureBinder::TextureBinder() {
			// No initialization required
		}

		// Destructor
		TextureBinder::~TextureBinder() {
			unbind_all();
		}

		void TextureBinder::bind_texture(const Gem::Graphics::Texture* texture, GLuint texture_unit) {
			if (texture) {
				texture->bind(texture_unit);
				bound_textures_.push_back(texture);
			}
			else {
				std::cerr << "ERROR::TextureBinder::bind_texture: Texture pointer is null." << std::endl;
			}
		}

		void TextureBinder::unbind_all() {
			for (const Gem::Graphics::Texture* texture : bound_textures_) {
				if (texture) {
					texture->unbind();
				}
			}
			bound_textures_.clear();
		}

	} // namespace Graphics

} // namespace Gem
