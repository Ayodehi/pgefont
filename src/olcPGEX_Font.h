/*
	olcPGEX_Font.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                       Font 1.0                              |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	support for using TTF fonts as well as your own custom spritemap.

	It expects you to have FreeType installed and configured properly
	in your project.

	For more information visit: https://www.freetype.org/

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
*/

/*
	Example
	~~~~~~~

	#define OLC_PGEX_FONT
	#include "olcPGEX_Font.h"

	font = new olc::TTFFont("./Assets/Fonts/Roboto-Medium.ttf", 12);
	font->BuildSprite();

	font->DrawString(this, { 16, 16 }, "Hello World");
	font->DrawFormatString(this, { 16, 32 }, "%s %s", "Hello", "World");
	font->DrawVerticalString(this, { 16, 48 }, "Hello World");
	font->DrawVerticalFormatString(this, {32, 48 }, "%s %s", "Hello", "World");

	olc::vi2d string_size = font->MeasureString("Hello World");

*/


#pragma once

#include <cstdarg>
#include <string>
#include <iostream>

#define FT_CONFIG_OPTION_SUBPIXEL_RENDERING
#include <ft2build.h>
#include FT_FREETYPE_H

#include "olcPGEX_SpriteMap.h"

namespace olc
{
	class Font
	{
	public:
		Font() = default;
		Font(const std::string& font_sprite_map_path, const std::string& font_data_path)
		{
			sprite = new olc::Sprite(font_sprite_map_path);
			decal = new olc::Decal(sprite);
			LoadFontData(font_data_path);

			spritemap = new olc::SpriteMap(decal, olc::vi2d({ font_height, font_width }));
		}
		~Font()
		{
			if (sprite)
				delete sprite;
			if (decal)
				delete decal;
			if (spritemap)
				delete spritemap;
		}

		void DrawString(olc::PixelGameEngine* pge, const olc::vi2d& pos, std::string_view message, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE) const
		{
			olc::vf2d spos = { 0.0f, 0.0f };
			for (auto c : message)
			{
				if (c == '\n')
				{
					spos.x = 0;
					spos.y += font_height * scale.y;
				}
				else
				{
					spritemap->Draw(pge, c, pos + spos, scale, tint);
					spos.x += widths[c] * scale.x;
				}
			}
		}

		void DrawVerticalString(olc::PixelGameEngine* pge, const olc::vi2d& pos, std::string_view message, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE) const
		{
			olc::vf2d spos = { 0.0f, 0.0f };
			for (auto c : message)
			{
				if (c == '\n')
				{
					spos.y = 0;
					spos.x += font_width * scale.x;
				}
				else
				{
					spritemap->Draw(pge, c, pos + spos, scale, tint);
					spos.y += heights[c] * scale.y;
				}
			}
		}

		void DrawFormatString(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE, const std::string message = "", ...) const
		{
			char buffer[2048];
			va_list args;
			va_start(args, message);
			vsprintf_s(buffer, 2048, message.c_str(), args);
			va_end(args);

			DrawString(pge, pos, std::string_view(buffer), scale, tint);
		}

		void DrawVerticalFormatString(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE, const std::string message = "", ...) const
		{
			char buffer[2048];
			va_list args;
			va_start(args, message);
			vsprintf_s(buffer, 2048, message.c_str(), args);
			va_end(args);

			DrawVerticalString(pge, pos, std::string_view(buffer), scale, tint);
		}

		void DrawVerticalFormatString(olc::PixelGameEngine* pge, const olc::vi2d& pos, const std::string message = "", ...) const
		{
			char buffer[2048];
			va_list args;
			va_start(args, message);
			vsprintf_s(buffer, 2048, message.c_str(), args);
			va_end(args);

			DrawVerticalString(pge, pos, std::string_view(buffer), { 1.0f, 1.0f }, olc::WHITE);
		}

		void DrawFormatString(olc::PixelGameEngine* pge, const olc::vi2d& pos, const std::string message = "", ...) const
		{
			char buffer[2048];
			va_list args;
			va_start(args, message);
			vsprintf_s(buffer, 2048, message.c_str(), args);
			va_end(args);

			DrawString(pge, pos, std::string_view(buffer));
		}

		olc::vi2d GetFontSize()
		{
			return spritemap->sprite_size;
		}

		olc::vi2d MeasureString(std::string_view message)
		{
			int lines = 1;
			int width = 0;
			int max_height = 0;
			for (auto& x : message)
			{
				if (x == '\n')
				{
					lines++;
				}

				width += widths[(size_t)x];
				max_height = heights[(size_t)x] > max_height ? heights[(size_t)x] : max_height;
			}

			return { width, max_height };
		}

	protected:
		void LoadFontData(const std::string& path)
		{
			
			std::ifstream in(path, std::ifstream::binary);
			int fontsheet_width = 0;
			int fontsheet_height = 0;
			
			char char_offset = 0;
			in.read((char*)&fontsheet_width, sizeof(int));
			in.read((char*)&fontsheet_height, sizeof(int));
			in.read((char*)&font_width, sizeof(int));
			in.read((char*)&font_height, sizeof(int));
			in.read((char*)&char_offset, sizeof(char));
			in.seekg(char_offset, std::ios_base::cur);

			widths = std::vector<char>(fontsheet_width / font_width - (int)char_offset);
			in.read((char*)widths.data(), sizeof(char) * fontsheet_width / font_width - char_offset);
		}

		olc::Sprite* sprite = nullptr;
		olc::Decal* decal = nullptr;
		olc::SpriteMap* spritemap = nullptr;
		int font_height = 8;
		int font_width = 8;
		std::vector<char> widths;
		std::vector<char> heights;
	};

	class MonospaceFont : public Font
	{
	public:
		MonospaceFont(const std::string& path, const olc::vi2d& sprite_size)
		{
			sprite = new olc::Sprite(path);
			decal = new olc::Decal(sprite);
			spritemap = new olc::SpriteMap(decal, sprite_size);

			this->font_height = sprite_size.y;
			this->font_width = sprite_size.x;

			widths = std::vector<char>(spritemap->decal->sprite->width / sprite_size.x, sprite_size.x);
		}		
	};

	class TTFFont : public Font
	{
	public:
		TTFFont(const std::string& path, int font_height = 12)
			: font_path{ path }, font_size{ font_height }
		{
			auto tmp = (int)floor(font_height / 0.75);
			sprite_size = { tmp, tmp };
		}

		bool BuildSprite()
		{
			auto error = FT_Init_FreeType(&library);
			if (error)
			{
#ifdef _DEBUG
				std::cerr << "Could not initialize library" << std::endl;
#endif
				return false;
			}

			error = FT_New_Face(library, font_path.c_str(), 0, &face);
			if (error) // FT_Err_Unknown_File_Format
			{
#ifdef _DEBUG
				std::cerr << "Could not load font" << std::endl;
#endif
				return false;
			}

			error = FT_Set_Char_Size(face, 0, font_size * 64, 0, 0);
			if (error)
			{
#ifdef _DEBUG
				std::cerr << "could not set char size" << std::endl;
#endif
				return false;
			}

			widths = std::vector<char>(256);
			heights = std::vector<char>(256);
			
			sprite = new olc::Sprite(sprite_size.x * 256, sprite_size.y);

			for (int x = 0; x < sprite->width; ++x)
				for (int y = 0; y < sprite->height; ++y)
					sprite->SetPixel({ x, y }, olc::Pixel(0, 0, 0, 0)); // Set all the pixels transparent
			
			for (int c = 0; c < 256; ++c)
			{
				error = FT_Load_Char(face, (char)c, FT_LOAD_RENDER);
				
				if (error)
				{
#ifdef _DEBUG
					std::cerr << "Could not load glyph for " << (char)c << std::endl;
#endif
					continue;
				}

				widths[c] = face->glyph->metrics.horiAdvance / 64;
				heights[c] = face->glyph->metrics.vertAdvance / 64;

				int offset = c * sprite_size.x;

				// Draw the glyph into the sprite map
				for (int row = 0; row < face->glyph->bitmap.rows; ++row)
				{
					for (int col = 0; col < face->glyph->bitmap.width; ++col)
					{
						int pixel = (int)face->glyph->bitmap.buffer[row * face->glyph->bitmap.width + col];

						// TODO: Figure out what the correct vertical offset should be dynamically based on font size and sprite size.
						sprite->SetPixel(offset + face->glyph->bitmap_left + col, sprite_size.y - font_size / 2 - face->glyph->bitmap_top + row, olc::Pixel(255, 255, 255, pixel));
					}
				}
			}
			
			decal = new olc::Decal(sprite);
			spritemap = new olc::SpriteMap(decal, sprite_size);
			font_height = sprite_size.x;
			font_width = sprite_size.y;
		}

		void TestDraw(olc::PixelGameEngine* pge, const olc::vi2d& pos) const
		{
			pge->DrawDecal(pos, decal);
		}

	private:
		std::string font_path = "";

		int font_size = 12;
		olc::vi2d sprite_size{ 16, 16 };

		FT_Library library = nullptr;
		FT_Face face = nullptr;
	};
}