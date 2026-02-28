/*********************************************************************
(c) Matt Marchant 2024
http://trederia.blogspot.com

tmxlite - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include "Texture.hpp"

#include <SDL3_image/SDL_image.h>

#include <iostream>
#include <cassert>

Texture::Texture()
    : m_texture(nullptr),
      m_size{ 0, 0 }
{

}

Texture::~Texture()
{
    if (m_texture)
    {
        SDL_DestroyTexture(m_texture);
    }
}

//public
bool Texture::loadFromFile(const std::string& path, SDL_Renderer* renderer)
{
    assert(renderer != nullptr);
    assert(!path.empty());

    if (m_texture)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
        m_size = { 0, 0 };
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        std::cerr << "Failed to load image " << path << ": " << SDL_GetError() << "\n";
        return false;
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!m_texture)
    {
        std::cerr << "Failed to create texture for " << path << ": " << SDL_GetError() << "\n";
        SDL_DestroySurface(surface);
        return false;
    }

    m_size.x = surface->w;
    m_size.y = surface->h;

    SDL_DestroySurface(surface);
    return true;
}
