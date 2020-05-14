#pragma once

#include "olcPGEX_SpriteMap.h"

namespace olc
{
	enum class TileAttributes
	{
		NONE = 0x0000,
		WALK = 0x0001,
		REQUIRES_CANOE = 0x0002,
		REQUIRES_BOAT = 0x0004,
		REQUIRES_AIRSHIP = 0x0008,
		PARTIAL_HIDE_PLAYER = 0x0010
	};

	inline TileAttributes operator|(TileAttributes a, TileAttributes b) {
		return static_cast<TileAttributes>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline TileAttributes operator&(TileAttributes a, TileAttributes b) {
		return static_cast<TileAttributes>(static_cast<int>(a) & static_cast<int>(b));
	}

	inline TileAttributes operator~(TileAttributes a) {
		return static_cast<TileAttributes>(~static_cast<int>(a));
	}

	inline TileAttributes& operator|=(TileAttributes& a, TileAttributes b) {
		a = static_cast<TileAttributes>(static_cast<int>(a) | static_cast<int>(b));
		return a;
	}

	inline TileAttributes& operator&=(TileAttributes& a, TileAttributes b) {
		a = static_cast<TileAttributes>(static_cast<int>(a) & static_cast<int>(b));
		return a;
	}

	class Tile
	{
	public:
		Tile() = default;
		Tile(SpriteMap* spritemap, int id, std::vector<unsigned char> sprites, int stride, const vi2d& size, TileAttributes attributes)
		{
			this->spritemap = spritemap;
			this->id = id;
			this->sprites = sprites;
			this->stride = stride;
			this->height = (int)(sprites.size() / stride);
			this->size = size;
			this->attributes = attributes;
		}

		void Draw(PixelGameEngine* pge, vf2d position, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE) const
		{
			int idx = 0;
			for (int row = 0; row < height; ++row)
			{
				for (int col = 0; col < stride; ++col)
				{
					vf2d offset = { position.x + col, position.y + row };
					spritemap->Draw(pge, sprites[idx++], offset, scale, tint); // * spritemap->sprite_size
				}
			}
		}

		bool canWalk() const
		{
			return (attributes & (TileAttributes::REQUIRES_AIRSHIP | TileAttributes::REQUIRES_CANOE | TileAttributes::REQUIRES_BOAT)) == TileAttributes::NONE;
		}

		bool hasAttribute(TileAttributes attrib)
		{
			return (attrib & attributes) != TileAttributes::NONE;
		}

		void setAttribute(TileAttributes attrib)
		{
			attributes |= attrib;
		}

		void unsetAttribute(TileAttributes attrib)
		{
			attributes &= ~attrib;
		}

		bool toggleAttribute(TileAttributes attrib)
		{
			if (hasAttribute(attrib))
			{
				unsetAttribute(attrib);
				return false;
			}
			else
			{
				setAttribute(attrib);
				return true;
			}
		}

		int id = 0;
		std::vector<unsigned char> sprites;
		TileAttributes attributes = TileAttributes::NONE;

	private:
		SpriteMap* spritemap = nullptr;
		int stride = 0;
		int height = 0;
		vi2d size;
	};
}