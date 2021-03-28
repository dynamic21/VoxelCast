#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define friction 0.98
#define playerSpeed 80
#define PixelSize 8

using olc::vf2d;
using olc::vi2d;
using olc::BLACK;
using olc::WHITE;
using std::vector;
using std::to_string;
using olc::Key;

unsigned int state = (unsigned int)time(NULL);

unsigned int xorshift32()
{
	state ^= state << 13;
	state ^= state >> 17;
	state ^= state << 5;
	return state;
}

class Example : public olc::PixelGameEngine
{
private:
	vf2d playerPos;
	vf2d playerVel;
	vector<bool> map;
	vi2d screen;

public:

	Example()
	{
		map.resize(ScreenWidth() * ScreenHeight());
		sAppName = "DDA";
	}

	bool OnUserCreate() override
	{
		int x = ScreenWidth() / PixelSize;
		int y = ScreenHeight() / PixelSize;
		screen = { x , y };
		playerPos = { (float)x / 2, (float)y / 2 };
		playerVel = { 0, 0 };
		map.resize(x * y);
		for (int i = 0; i < map.size(); i++)
			map[i] = xorshift32() & 1;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(Key::UP).bHeld || GetKey(Key::W).bHeld) playerVel.y -= playerSpeed * fElapsedTime;
		if (GetKey(Key::DOWN).bHeld || GetKey(Key::S).bHeld) playerVel.y += playerSpeed * fElapsedTime;
		if (GetKey(Key::LEFT).bHeld || GetKey(Key::A).bHeld) playerVel.x -= playerSpeed * fElapsedTime;
		if (GetKey(Key::RIGHT).bHeld || GetKey(Key::D).bHeld) playerVel.x += playerSpeed * fElapsedTime;

		playerVel *= friction;
		playerPos += playerVel * fElapsedTime;

		Clear(BLACK);

		for (int x = 0; x < screen.x; x++)
			for (int y = 0; y < screen.y; y++)
				if (map[y * screen.x + x])
					FillRect(vi2d(x, y) * PixelSize, { PixelSize, PixelSize }, WHITE);

		FillCircle(playerPos * PixelSize, PixelSize / 2, olc::GREEN);
		//DrawString(0, 0, to_string(0), olc::GREEN);
		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(512, 480, 1, 1))
		demo.Start();
	return 0;
}