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
      //fonts[i] = new olc::TTFFont("./NotoSansJP-Regular.otf", i); // Note, due to the sheer number of glpyhs, using this font will require 5GB+ of RAM!
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
      auto bounds = fonts[i]->MeasureString("Computed Bounding Box");

      //fonts[i]->DrawStringW(this, { 16, draw_offset_y }, L"こんいちは");
      //auto bounds = fonts[i]->MeasureStringW(L"こんいちは");

			DrawRect(16 + bounds.x, draw_offset_y + bounds.y, bounds.w, bounds.h, olc::RED);
			draw_offset_y += i + 16;
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
	if (d.Construct(750, 512, 1, 1, false, false))
		d.Start();
}