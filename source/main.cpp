#include <cstdio>
#include <nds.h>
#include "draw.h"
#include "game.h"

using namespace std;

volatile int frame = 0;

PongGame game;

void Vblank()
{
	frame++;
}

void ResetGame()
{
	int random_vel[2] = {-1,-1};
	int width;
	int height;
	game.GetGameSize(width, height);
	int random_pos[2] = {width / 2, height / 2};
	int low = game.GetPlayer().pos[1] + 1;
	int high = game.GetEnemy().pos[1] - 1;
	int random_delay = rand() % (30 - 10 + 1) + 10;

	random_pos[0] = rand() % width;
	random_pos[1] = rand() % (high - low + 1) + low;

	random_vel[0] = ((rand() % 2) * 2) - 1;
	if (random_pos[1] > (height / 2))
		random_vel[1] = -1;
	else
		random_vel[1] = 1;

	game.reset(random_pos, random_vel, random_delay);
}

Paddle WiFi_Communicate()
{
	Paddle enemy;
	int width;
	int height;

	enemy = game.GetEnemy();
	game.GetGameSize(width, height);

	if ((frame % 2) == 0)
	{
		if ((enemy.pos[0] + (enemy.size[0] / 2)) < game.GetBall().pos[0])
			enemy.pos[0]++;
		else if ((enemy.pos[0] + (enemy.size[0] / 2)) > game.GetBall().pos[0])
			enemy.pos[0]--;

		if ((enemy.pos[0] + enemy.size[0]) > width)
			enemy.pos[0] = width - enemy.size[0];
		else if (enemy.pos[0] < 0)
			enemy.pos[0] = 0;
	}



	return enemy;
}

int main(int argc, char **argv)
{
	int pad_size[2] = {7, 1};
	int move_factor = 0;
	int shrink = 8;

	int game_size[2] = {SCREEN_WIDTH / shrink, SCREEN_HEIGHT / shrink};

	consoleDemoInit();
	consoleClear();

	irqSet(IRQ_VBLANK,Vblank);

	iprintf("Initializing game . . . \n");
	game.init(game_size, pad_size, 1, 2, 10);

	iprintf("Initializing gl . . . \n");

	videoSetMode(MODE_5_3D);
	glScreen2D();

	iprintf("Setting up initial game . . . \n");
	ResetGame();

	consoleClear();

	while (true)
	{
		scanKeys();
		int keys = keysHeld();
		if (keys & KEY_START) break;

		// print at using ansi escape sequence \x1b[line;columnH
		iprintf("\x1b[10;0HFrame = %d\n\n",frame);
		iprintf("Player Score = %d\n",game.player_score);
		iprintf("Enemy Score = %d\n",game.enemy_score);

		if (keys & KEY_LEFT)
			move_factor = -1;
		else if (keys & KEY_RIGHT)
			move_factor = 1;
		else
			move_factor = 0;

		Paddle stream = WiFi_Communicate();

		game.UpdatePaddles(stream, move_factor);
		if (game.UpdateBall())
			ResetGame();

		glBegin2D();

		DrawPaddle(game.GetPlayer().pos, game.GetPlayer().size, shrink);
		DrawPaddle(game.GetEnemy().pos, game.GetEnemy().size, shrink);
		DrawBall(game.GetBall().pos, game.GetBall().size, shrink);

		glEnd2D();

		glFlush(0);
		swiWaitForVBlank();

	}

	return 0;
}
