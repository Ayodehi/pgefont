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

	font->DrawStringW(this, { 16, 16 }, L"Hello World");

	olc::bbox string_size = font->MeasureString("Hello World");

*/


#pragma once

#include <cstdarg>
#include <string>
#include <iostream>

#define FT_CONFIG_OPTION_SUBPIXEL_RENDERING
#include <ft2build.h>
#include FT_FREETYPE_H

namespace olc
{
	template <typename T>
	struct bbox
	{
		T x;
		T y;
		T h;
		T w;
	};

	struct FontDetails
	{
		int horizontalBearingX;
		int horizontalBearingY;
		int horizontalAdvance;
		int verticalBearingX;
		int verticalBearingY;
		int verticalAdvance;
		int spritemapIndex;
		int spritemapOffsetX;
		int spritemapOffsetY;
		int height;
		int width;
	};

	class Font
	{
	public:
		Font() = default;
		~Font()
		{
			for (auto d : decals)
				delete d;
		}


		void DrawString(olc::PixelGameEngine* pge, const olc::vi2d& origin, std::string_view message, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE) const
		{
			olc::vf2d spos = { 0.0f, 0.0f };
			for (auto c : message)
			{
				if (c == '\n')
				{
					spos.x = 0;
					spos.y += fontDetails[c].verticalAdvance * scale.y;
				}
				else
				{
					pge->DrawPartialDecal(
						{ origin.x + spos.x + fontDetails[c].horizontalBearingX, origin.y + spos.y - fontDetails[c].horizontalBearingY },
						decals[fontDetails[c].spritemapIndex],
						{ (float)fontDetails[c].spritemapOffsetX, (float)fontDetails[c].spritemapOffsetY },
						{ (float)fontDetails[c].width, (float)fontDetails[c].height },
						scale,
						tint
					);

					spos.x += fontDetails[c].horizontalAdvance * scale.x;
				}
			}
		}

		void DrawStringW(olc::PixelGameEngine* pge, const olc::vi2d& origin, std::wstring_view message, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE) const
		{
			olc::vf2d spos = { 0.0f, 0.0f };
			for (auto c : message)
			{
				if (c == '\n')
				{
					spos.x = 0;
					spos.y += fontDetails[c].verticalAdvance * scale.y;
				}
				else
				{
					pge->DrawPartialDecal(
						{ origin.x + spos.x + fontDetails[c].horizontalBearingX, origin.y + spos.y - fontDetails[c].horizontalBearingY },
						decals[fontDetails[c].spritemapIndex],
						{ (float)fontDetails[c].spritemapOffsetX, (float)fontDetails[c].spritemapOffsetY },
						{ (float)fontDetails[c].width, (float)fontDetails[c].height },
						scale,
						tint
					);
					
					spos.x += fontDetails[c].horizontalAdvance * scale.x;
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
					spos.x += fontDetails[c].horizontalAdvance * scale.x;
				}
				else
				{
					pge->DrawPartialDecal(
						{ pos.x + spos.x + fontDetails[c].verticalBearingX, pos.y + spos.y - fontDetails[c].verticalBearingY },
						decals[fontDetails[c].spritemapIndex],
						{ (float)fontDetails[c].spritemapOffsetX, (float)fontDetails[c].spritemapOffsetY },
						{ (float)fontDetails[c].width, (float)fontDetails[c].height },
						scale,
						tint
					);

					spos.y += fontDetails[c].verticalAdvance * scale.y;
				}
			}
		}

		void DrawVerticalStringW(olc::PixelGameEngine* pge, const olc::vi2d& pos, std::wstring_view message, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE) const
		{
			olc::vf2d spos = { 0.0f, 0.0f };
			for (auto c : message)
			{
				if (c == '\n')
				{
					spos.y = 0;
					spos.x += fontDetails[c].horizontalAdvance * scale.x;
				}
				else
				{
					pge->DrawPartialDecal(
						{ pos.x + spos.x + fontDetails[c].verticalBearingX, pos.y + spos.y - fontDetails[c].verticalBearingY },
						decals[fontDetails[c].spritemapIndex],
						{ (float)fontDetails[c].spritemapOffsetX, (float)fontDetails[c].spritemapOffsetY },
						{ (float)fontDetails[c].width, (float)fontDetails[c].height },
						scale,
						tint
					);

					spos.y += fontDetails[c].verticalAdvance * scale.y;
				}
			}
		}

		void DrawFormatStringW(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE, const std::wstring message = L"", ...) const
		{
			wchar_t buffer[2048];
			va_list args;
			va_start(args, message);
			vswprintf_s(buffer, 2048, message.c_str(), args);
			va_end(args);

			DrawStringW(pge, pos, std::wstring_view(buffer), scale, tint);
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

		void DrawVerticalFormatStringW(olc::PixelGameEngine* pge, const olc::vi2d& pos, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE, const std::wstring message = L"", ...) const
		{
			wchar_t buffer[2048];
			va_list args;
			va_start(args, message);
			vswprintf_s(buffer, 2048, message.c_str(), args);
			va_end(args);

			DrawVerticalStringW(pge, pos, std::wstring_view(buffer), scale, tint);
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

		void DrawVerticalFormatStringW(olc::PixelGameEngine* pge, const olc::vi2d& pos, const std::wstring message = L"", ...) const
		{
			wchar_t buffer[2048];
			va_list args;
			va_start(args, message);
			vswprintf_s(buffer, 2048, message.c_str(), args);
			va_end(args);

			DrawVerticalStringW(pge, pos, std::wstring_view(buffer), { 1.0f, 1.0f }, olc::WHITE);
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

		void DrawFormatStringW(olc::PixelGameEngine* pge, const olc::vi2d& pos, const std::wstring message = L"", ...) const
		{
			wchar_t buffer[2048];
			va_list args;
			va_start(args, message);
			vswprintf_s(buffer, 2048, message.c_str(), args);
			va_end(args);

			DrawStringW(pge, pos, std::wstring_view(buffer));
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

		olc::bbox<int> MeasureStringW(std::wstring_view message)
		{
			int max_width = 0;
			int max_height = 0;
			int min_y = 65535;
			int max_y = 0;
			int line_width = 0;
			int above_baseline = 0;
			for (auto& x : message)
			{
				if (x == '\n')
				{
					max_width = line_width > max_width ? line_width : max_width;
					max_height += abs(max_y - min_y);

					min_y = 0;
					max_y = 65535;
					line_width = 0;
				}

				int tmp_min_y = -fontDetails[x].horizontalBearingY;
				int tmp_max_y = tmp_min_y + fontDetails[x].height;

				min_y = tmp_min_y < min_y ? tmp_min_y : min_y;
				max_y = tmp_max_y > max_y ? tmp_max_y : max_y;

				line_width += fontDetails[x].horizontalAdvance;
			}

			max_width = line_width > max_width ? line_width : max_width;
			max_height += abs(max_y - min_y);

			return { 0, min_y, max_height, max_width };
		}

		olc::bbox<int> MeasureString(std::string_view message)
		{
			int max_width = 0;
			int max_height = 0;
			int min_y = 65535;
			int max_y = 0;
			int line_width = 0;
			int above_baseline = 0;
			for (auto& x : message)
			{
				if (x == '\n')
				{
					max_width = line_width > max_width ? line_width : max_width;
					max_height += abs(max_y - min_y);
					
					min_y = 0;
					max_y = 65535;
					line_width = 0;
				}

				int tmp_min_y = -fontDetails[x].horizontalBearingY;
				int tmp_max_y = tmp_min_y + fontDetails[x].height;

				min_y = tmp_min_y < min_y ? tmp_min_y : min_y;
				max_y = tmp_max_y > max_y ? tmp_max_y : max_y;

				line_width += fontDetails[x].horizontalAdvance;
			}

			max_width = line_width > max_width ? line_width : max_width;
			max_height += abs(max_y - min_y);

			return { 0, min_y, max_height, max_width };
		}

	protected:
		
		std::vector<olc::Sprite*> sprites;
		std::vector<olc::Decal*> decals;
		olc::vi2d sprite_map_size{ 2048, 2048 };
		olc::vi2d sprite_tile_size{ 128, 128 };
		std::vector<FontDetails> fontDetails;
	};

	class TTFFont : public Font
	{
	public:
		TTFFont(const std::string& path, int font_height = 12)
			: font_path{ path }, font_size{ font_height }
		{
			auto tmp = (int)floor(font_height / 0.75);
			sprite_size = { tmp, tmp };
			sprite_tile_size = { sprite_map_size.x / tmp, sprite_map_size.y / tmp };
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

			fontDetails = std::vector<FontDetails>(face->num_glyphs);

			int number_of_spritemaps_required = (int)ceil((float)face->num_glyphs / (sprite_tile_size.x * sprite_tile_size.y));

			std::vector<olc::Sprite*> sprites(number_of_spritemaps_required);

			for (int i = 0; i < number_of_spritemaps_required; ++i)
			{
				auto sprite = new olc::Sprite(sprite_map_size.x, sprite_map_size.y);

				for (int x = 0; x < sprite->width; ++x)
					for (int y = 0; y < sprite->height; ++y)
						sprite->SetPixel({ x, y }, olc::Pixel(0, 0, 0, 0)); // Set all the pixels transparent

				sprites[i] = sprite;
			}
			
			for (FT_ULong c = 0; c < face->num_glyphs; ++c)
			{
				auto index = FT_Get_Char_Index(face, c);
				error = FT_Load_Glyph(face, index, FT_LOAD_DEFAULT);
				error = FT_Render_Glyph(face->glyph, FT_Render_Mode::FT_RENDER_MODE_NORMAL);
				
				if (error)
				{
#ifdef _DEBUG
					std::cerr << "Could not load glyph for " << c << std::endl;
#endif
					continue;
				}

				fontDetails[c].horizontalBearingX = face->glyph->metrics.horiBearingX / 64;
				fontDetails[c].horizontalBearingY = face->glyph->metrics.horiBearingY / 64;
				fontDetails[c].horizontalAdvance = face->glyph->metrics.horiAdvance / 64;
				fontDetails[c].verticalBearingX = face->glyph->metrics.vertBearingX / 64;
				fontDetails[c].verticalBearingY = face->glyph->metrics.vertBearingY / 64;
				fontDetails[c].verticalAdvance = face->glyph->metrics.vertAdvance / 64;
				fontDetails[c].height = face->glyph->metrics.height / 64;
				fontDetails[c].width = face->glyph->metrics.width / 64;

				int sprite_index = (int)floor((float)c / (sprite_tile_size.x * sprite_tile_size.y));
				int xoffset = ((c - (sprite_index * (sprite_tile_size.x * sprite_tile_size.y))) % sprite_tile_size.x) * sprite_size.x;
				int yoffset = ((c - (sprite_index * (sprite_tile_size.x * sprite_tile_size.y))) / sprite_tile_size.y) * sprite_size.y;
 				int baseline = sprite_size.y - font_size / 2;

				fontDetails[c].spritemapIndex = sprite_index;
				fontDetails[c].spritemapOffsetX = xoffset;
				fontDetails[c].spritemapOffsetY = yoffset;

				// Draw the glyph into the sprite map
				for (int row = 0; row < face->glyph->bitmap.rows; ++row)
				{
					for (int col = 0; col < face->glyph->bitmap.width; ++col)
					{
						int pixel = (int)face->glyph->bitmap.buffer[row * face->glyph->bitmap.width + col];

						// Just draw the bitmap into the sprite slot and worry about positioning on render
						sprites[sprite_index]->SetPixel(
							xoffset + col,
							yoffset + row,
							olc::Pixel(255, 255, 255, pixel)
						);
					}
				}
			}
			
			for (int i = 0; i < number_of_spritemaps_required; ++i)
				decals.push_back(new olc::Decal(sprites[i]));

			return true;
		}

		void TestDraw(olc::PixelGameEngine* pge, int idx, const olc::vi2d& pos) const
		{
			pge->DrawDecal(pos, decals[idx]);
		}

	private:
		std::string font_path = "";

		int font_size = 12;
		olc::vi2d sprite_size{ 16, 16 };

		FT_Library library = nullptr;
		FT_Face face = nullptr;
	};
}