#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "engine/graphics/texture.hpp"

using namespace engine::graphics;

Texture::Texture(const char* path, const TextureParams& params) : path(path) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        if (params.minFilter == GL_LINEAR_MIPMAP_LINEAR || params.minFilter == GL_NEAREST_MIPMAP_NEAREST ||
            params.minFilter == GL_LINEAR_MIPMAP_NEAREST || params.minFilter == GL_NEAREST_MIPMAP_LINEAR) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else {
        std::cerr << "ERROR::TEXTURE: Failed to load texture: " << std::endl;
        std::cerr << stbi_failure_reason() << std::endl;
    }

    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
}

void Texture::bind(GLenum textureUint) const {
    glActiveTexture(textureUint);
    glBindTexture(GL_TEXTURE_2D, ID);
}