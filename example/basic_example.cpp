#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>

#include "olcPGEX_Font.h"

class FontDemo : public olc::PixelGameEngine
{
public:
	FontDemo() = default;
	~FontDemo() = default;

	bool OnUserCreate() override
	{
		for (int i = 12; i < 72; i += 6)
		{
			fonts[i] = new olc::TTFFont("./Roboto-Medium.ttf", i);
			fonts[i]->BuildSprite();
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		int draw_offset_y = 10;
		for (int i = 12; i < 72; i += 6)
		{
			fonts[i]->DrawString(this, { 16, draw_offset_y }, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
			draw_offset_y += i + 2;
		}

		return true;
	}

	bool OnUserDestroy() override
	{
		return true;
	}

private:
	std::map<int, olc::TTFFont*> fonts;
};

int main(int argc, char** argv)
{
	FontDemo d;
	if (d.Construct(1900, 480, 1, 1, false, false))
		d.Start();
}