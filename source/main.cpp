/*-----------------------------------------------------------------
MIT License

Copyright (c) 2019 antoine62
Copyright (c) 2019-2025 R-YaTian

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------*/

#include <nds.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <fat.h>
#include <fstream>

// File management part
std::string themename(int themenum)
{
	std::string themepath = "/TTMenu/themes/" + std::to_string(themenum);

	if ((access((themepath + "/Color.bin").c_str(), F_OK) == 0))
	{
		if ((access((themepath + "/name.txt").c_str(), F_OK) == 0))
		{
			std::ifstream namefile(themepath + "/name.txt");

			std::string sLine;
			std::getline(namefile, sLine);
			std::string nameofthetheme = sLine;
			namefile.close();
			return sLine;
		}
		else
		{
			return "Theme found, please add a name.txt file.";
		}
	}
	else
	{
		return "Theme not found";
	}
}

// NDS related
unsigned long int selected = 0;

void resetscreen()
{
	int i = 0;
	while (i < 21)
	{
		printf("\x1b[%i;0H                                  ", i);
		i++;
	}
	printf("\x1b[0m]");
	printf("\x1b[37;1m]");
	printf("\x1b[10;1HA - Install theme\n Y - Restore default\n Left/Right - Select themes\n YSThemeR V2.1 BY R-YaTian\n github.com/R-YaTian/YSThemeR");
	printf("\x1b[35;1m");
	printf("\x1b[1;1HTheme %lu - %s \n", selected, themename(selected).c_str());
	printf("\x1b[0m");
	printf("\x1b[33;1m");
	printf("\x1b[20;1HOriginal author: antoine62");
	printf("\x1b[0m");
	printf("\x1b[34;1m");
	printf("\x1b[21;1Hgithub.com/antoine62/YSTheme");
	printf("\x1b[0m");
}

void stop(void)
{
	while (pmMainLoop())
	{
		swiWaitForVBlank();
	}
}

// NDS Code
int main(int argc, char **argv)
{
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);
	vramSetBankH(VRAM_H_SUB_BG);
	consoleInit(NULL, 1, BgType_Text4bpp, BgSize_T_256x256, 15, 0, false, true);

	if (!fatInitDefault())
	{
		printf("fatInitDefault failed!\n");
		stop();
	}

	std::string themesname = themename(selected);
	resetscreen();
	while (pmMainLoop())
	{
		themesname = themename(selected);
		//Scan nds KEY
		scanKeys();
		int pressed = keysDown();
		if (pressed & KEY_RIGHT)
		{
			if (selected != 5)
			{
				selected += 1;
				resetscreen();
			}
			else
			{
			selected = 0;
			resetscreen();
			}
		}
		if (pressed & KEY_LEFT)
		{
			if (selected != 0)
			{
				selected -= 1;
				resetscreen();
			}
			else
			{
			selected = 5;
			resetscreen();
			}
		}
		if (pressed & KEY_Y)
		{
			std::string themepath = "/TTMenu/themes/0";
			std::ifstream source((themepath + "/Color.bin").c_str(), std::ios::binary);
			std::ofstream dest("/TTmenu/Color.bin", std::ios::binary);
			dest << source.rdbuf();
			source.close();
			dest.close();

			std::ifstream source1((themepath + "/YSmenu1.bmp").c_str(), std::ios::binary);
			std::ofstream dest1("/TTmenu/YSmenu1.bmp", std::ios::binary);
			dest1 << source1.rdbuf();
			source1.close();
			dest1.close();

			std::ifstream source2((themepath + "/YSmenu2.bmp").c_str(), std::ios::binary);
			std::ofstream dest2("/TTmenu/YSMenu2.bmp", std::ios::binary);
			dest2 << source2.rdbuf();
			source2.close();
			dest2.close();
			printf("\x1b[32;1m");
			printf("\x1b[8;1HDone!Please reboot your console\x1b[0m");
			stop();
		}
		if (pressed & KEY_A)
		{
			if (themesname != "Theme not found")
			{
				std::string themepath = "/TTMenu/themes/" + std::to_string(selected);
				std::ifstream source((themepath + "/Color.bin").c_str(), std::ios::binary);
				std::ofstream dest("/TTmenu/Color.bin", std::ios::binary);
				dest << source.rdbuf();
				source.close();
				dest.close();

				std::ifstream source1((themepath + "/YSmenu1.bmp").c_str(), std::ios::binary);
				std::ofstream dest1("/TTmenu/YSmenu1.bmp", std::ios::binary);
				dest1 << source1.rdbuf();
				source1.close();
				dest1.close();

				std::ifstream source2((themepath + "/YSmenu2.bmp").c_str(), std::ios::binary);
				std::ofstream dest2("/TTmenu/YSMenu2.bmp", std::ios::binary);
				dest2 << source2.rdbuf();
				source2.close();
				dest2.close();
				printf("\x1b[32;1m");
				printf("\x1b[8;1HDone!Please reboot your console\x1b[0m");
				stop();
			}
		}
	}
	return 0;
}
