#include <gl2d.h>

void DrawPaddle(int pos[], int size[], int shrink)
{
  int end_x = pos[0] + size[0];
  int end_y = pos[1] + size[1];
  glBoxFilled(pos[0] * shrink, pos[1] * shrink,
    end_x * shrink, end_y * shrink, RGB15(255,255,255));
}

void DrawBall(int pos[], int size, int shrink)
{
  int end_x = pos[0] + size;
  int end_y = pos[1] + size;
  glBoxFilled(pos[0] * shrink, pos[1] * shrink,
    end_x * shrink, end_y * shrink, RGB15(255,255,255));
}
