#pragma once

class Paddle
{
public:
  int pos[2];   // Position will be measured based on top-left
  int size[2];   // Defines Size from top-left to bottom-right
};

class Ball
{
public:
  int pos[2];
  int vel[2] = {0,0};
  int size;
  int delay_max;
  float current_delay;
  int delay;
};

class PongGame
{
public:
  int paddle_delay; /* Defines a delay before the player paddle can update
                      (usually in terms of frames)
                      NOTE: This does not apply to the enemy paddle */

  int player_score;
  int enemy_score;

  void init(int game_size[], int paddle_size[], int ball_size,
    int paddle_frames, int ball_frames)
  {
    width = game_size[0];
    height = game_size[1];
    shrink_size = ((SCREEN_WIDTH / width) + (SCREEN_HEIGHT / height)) / 2;

    paddle_delay = paddle_frames;
    pong.delay_max = ball_frames;

    player.size[0] = paddle_size[0];
    player.size[1] = paddle_size[1];
    enemy.size[0] = paddle_size[0];
    enemy.size[1] = paddle_size[1];
    pong.size = ball_size;

    player_score = 0;
    enemy_score = 0;
  }

  void reset(int pos[], int vel[], int max_delay)
  {
    player.pos[0] = (width / 2) - (player.size[0] / 2);
    player.pos[1] = 8 / shrink_size;

    enemy.pos[0] = (width / 2) - (enemy.size[0] / 2);
    enemy.pos[1] = height - (8 / shrink_size) - enemy.size[1];

    pong.pos[0] = pos[0];
    pong.pos[1] = pos[1];

    pong.vel[0] = vel[0];
    pong.vel[1] = vel[1];

    pong.delay_max = max_delay;
    pong.current_delay = pong.delay_max;
    pong.delay = pong.current_delay;
  }

  Paddle GetPlayer()
  {
    return player;
  }

  Paddle GetEnemy()
  {
    return enemy;
  }

  Ball GetBall()
  {
    return pong;
  }

  void GetGameSize(int &out_width, int &out_height)
  {
    out_width = width;
    out_height = height;
  }

  void UpdatePaddles(Paddle data, int move_delta)
  {
    enemy = data;
    if (move_delta != 0 && delay <= 0)
    {
      delay = paddle_delay;
      player.pos[0] += move_delta;
      if ((player.pos[0] + player.size[0]) > width)
        player.pos[0] = width - player.size[0];
      else if (player.pos[0] < 0)
        player.pos[0] = 0;
    }
    else if (move_delta != 0)
      delay = 0;
    else if (delay > 0)
      delay--;
  }

  bool UpdateBall()
  {
    bool scored = false;

    if (pong.delay <= 0)
    {
      pong.pos[1] += pong.vel[1];

      if (CheckTotalCollision(player) || CheckTotalCollision(enemy))
      {
        pong.vel[1] *= -1;
        pong.pos[1] += pong.vel[1] * 2;
        pong.current_delay /= 1.1f;
      }
      else if (pong.pos[1] < 0)
      {
        enemy_score++;
        scored = true;
      }
      else if ((pong.pos[1] + pong.size) > height)
      {
        player_score++;
        scored = true;
      }
      
      pong.pos[0] += pong.vel[0];
      pong.delay = pong.current_delay;
      
      if ((pong.pos[0] < 0) || ((pong.pos[0]+pong.size) > width))
      {
        pong.vel[0] *= -1;
        pong.pos[0] += pong.vel[0] * 2;
      }
      
    }
    else
      pong.delay--;

    return scored;
  }

private:
  Paddle player;
  Paddle enemy;
  Ball pong;
  int delay;
  int width;   // Stores the width of the game
  int height;  // Stores the height of the game
  int shrink_size;

  bool CheckPaddleCollision(Paddle collider, int point[])
  {
    return
    (
      (point[0] >= collider.pos[0]) &&
      (point[1] >= collider.pos[1]) &&
      (point[0] <= (collider.pos[0] + collider.size[0])) &&
      (point[1] <= (collider.pos[1] + collider.size[1]))
    );
  }

  bool CheckTotalCollision(Paddle collider)
  {
    int ball_a[2] = {pong.pos[0], pong.pos[1]};
    int ball_b[2] = {pong.pos[0] + pong.size, pong.pos[1]};
    int ball_c[2] = {pong.pos[0], pong.pos[1] + pong.size};
    int ball_d[2] = {pong.pos[0] + pong.size, pong.pos[1] + pong.size};
    return
    (
      (CheckPaddleCollision(collider, ball_a)) &&
      (CheckPaddleCollision(collider, ball_b)) &&
      (CheckPaddleCollision(collider, ball_c)) &&
      (CheckPaddleCollision(collider, ball_d))
    );
  }

};
