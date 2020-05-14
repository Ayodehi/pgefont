/*
	olcPGEX_Font.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                     SpriteMap 1.0                           |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	basic spritemap functionality.

	License (The Unlicense)
	~~~~~~~~~~~~~~~

	This is free and unencumbered software released into the public domain.

	Anyone is free to copy, modify, publish, use, compile, sell, or
	distribute this software, either in source code form or as a compiled
	binary, for any purpose, commercial or non-commercial, and by any
	means.

	In jurisdictions that recognize copyright laws, the author or authors
	of this software dedicate any and all copyright interest in the
	software to the public domain. We make this dedication for the benefit
	of the public at large and to the detriment of our heirs and
	successors. We intend this dedication to be an overt act of
	relinquishment in perpetuity of all present and future rights to this
	software under copyright law.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.

	For more information, please refer to <https://unlicense.org>

  Author & Contributors
  ~~~~~~~~~~~~~~~

  Ayodehi
*/

/*
	Example
	~~~~~~~

	#define OLC_PGEX_SPRITEMAP
	#include "olcPGEX_SpriteMap.h"

	auto sprite = new olc::Sprite("./Assets/SpriteMaps/spritemap.png");
	auto decal = new olc::Decal(sprite);
	auto spritemap = new olc::SpriteMap(decal, { 16, 16 });

	spritemap->Draw(this, 0, { 16, 16 });
	spritemap->DrawRotated(this, 0, { 16, 32 }, 45.0f, { 0.0f, 0.0f });

	spritemap->DrawSpriteMap(this, { 32, 16 });

*/

#pragma once

namespace olc
{
	class SpriteMap
	{
	public:
		SpriteMap() = default;
		SpriteMap(olc::Decal* decal, const olc::vi2d& size)
			: decal{ decal }, sprite_size{ size }
		{
			width = decal->sprite->width / size.x;
			height = decal->sprite->height / size.y;
		}
		~SpriteMap() = default;

		void Draw(olc::PixelGameEngine* pge, int idx, olc::vf2d position, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE)
		{
			int row = idx / width;
			int col = idx % width;
			olc::vi2d drawoffset = { col * sprite_size.x, row * sprite_size.y };
			
			pge->DrawPartialDecal(position, decal, drawoffset, sprite_size, scale, tint);
		}

		void DrawRotated(olc::PixelGameEngine* pge, int idx, olc::vi2d position, float angle, const olc::vf2d& center_of_rotation, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE)
		{
			int row = idx / width;
			int col = idx % width;
			olc::vi2d drawoffset = { col * sprite_size.x, row * sprite_size.y };

			pge->DrawPartialRotatedDecal(position, decal, angle, center_of_rotation, drawoffset, sprite_size, scale, tint);
		}

		void DrawSpriteMap(olc::PixelGameEngine* pge, const olc::vi2d& position, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE)
		{
			pge->DrawDecal(position, decal, scale, tint);
		}

		olc::vi2d sprite_size;
		int32_t width = 0;
		int32_t height = 0;
		olc::Decal* decal = nullptr;
	};
}