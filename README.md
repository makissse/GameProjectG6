Hi! In this README file, I decided to give you an overview of how the inner workings of the game function. Of course, there are some drawbacks in certain parts of the code—for example, the section with the AI. That's why I’d really appreciate any feedback you might have. I made this file mainly to make it easier to adapt to the code I’ve written.

```
Pong Game – Logic Explanation
if (mode == MODE_MENU)
{
    if (IsKeyPressed(KEY_DOWN)) menuOption = (menuOption + 1) % 2;
    if (IsKeyPressed(KEY_UP)) menuOption = (menuOption + 1) % 2;
    if (IsKeyPressed(KEY_W)) menuOption = (menuOption + 1) % 2;
    if (IsKeyPressed(KEY_S)) menuOption = (menuOption + 1) % 2;

    if (IsKeyPressed(KEY_ENTER))
    {
        mode = (menuOption == 0) ? MODE_PVP : MODE_PVAI;            

        scoreLeft = 0;
        scoreRight = 0;
        ballPos = { screenWidth / 2.0f, screenHeight / 2.0f };
        ballVel = { 360.0f, 240.0f };

        leftPaddle = { 30.0f, screenHeight / 2.0f - paddleHeight / 2.0f, paddleWidth, paddleHeight };
        rightPaddle = { screenWidth - 40.0f, screenHeight / 2.0f - paddleHeight / 2.0f, paddleWidth, paddleHeight };
        gameRunning = true;
    }
}
```

Logic:

Handles the main menu, allows the player to navigate and select a game mode.

Resets scores, ball position, ball velocity, and paddles when a mode is selected.

Switches the game to active state (gameRunning = true).

```
// --- Player 1 movement ---
if (IsKeyDown(KEY_W) && leftPaddle.y > 0) leftPaddle.y -= playerSpeed * delta;
if (IsKeyDown(KEY_S) && leftPaddle.y + leftPaddle.height < screenHeight) leftPaddle.y += playerSpeed * delta;

// --- Player 2 / AI movement ---
if (mode == MODE_PVP)
{
    if (IsKeyDown(KEY_UP) && rightPaddle.y > 0) rightPaddle.y -= playerSpeed * delta;
    if (IsKeyDown(KEY_DOWN) && rightPaddle.y + rightPaddle.height < screenHeight) rightPaddle.y += playerSpeed * delta;
}
else if (mode == MODE_PVAI)
{
    float paddleCenter = rightPaddle.y + rightPaddle.height / 2.0f;
    if (ballPos.y < paddleCenter && rightPaddle.y > 0) rightPaddle.y -= aiSpeed * delta;
    if (ballPos.y > paddleCenter && rightPaddle.y + rightPaddle.height < screenHeight) rightPaddle.y += aiSpeed * delta;
}
```

Logic:

Updates player and AI paddle positions each frame.

Player 1 uses W/S keys.

Player 2:

PvP: uses arrow keys.

PvAI: simple AI follows the ball’s Y position, moving paddle toward the ball center.

Delta time ensures movement is smooth regardless of FPS.

```
// --- Ball movement ---
ballPos.x += ballVel.x * delta;
ballPos.y += ballVel.y * delta;

// --- Bounce off top/bottom ---
if (ballPos.y - ballRadius <= 0) { ballPos.y = ballRadius; ballVel.y *= -1; }
if (ballPos.y + ballRadius >= screenHeight) { ballPos.y = screenHeight - ballRadius; ballVel.y *= -1; }
```

Logic:

Updates ball position each frame.

Checks top and bottom collisions and reflects the ball by inverting Y velocity.

Keeps ball inside the screen.

```
// --- Paddle collisions ---
if (CheckCollisionCircleRec(ballPos, ballRadius, leftPaddle))
{
    ballPos.x = leftPaddle.x + leftPaddle.width + ballRadius;
    ballVel.x *= -1.05f;
}
if (CheckCollisionCircleRec(ballPos, ballRadius, rightPaddle))
{
    ballPos.x = rightPaddle.x - ballRadius;
    ballVel.x *= -1.05f;
}
```

Logic:

Detects collision between the ball and each paddle.

Reflects the ball horizontally (X velocity) and slightly increases speed for progressive difficulty.

Ensures ball doesn’t stick inside the paddle.

```
// --- Scoring ---
if (ballPos.x - ballRadius <= 0 || ballPos.x + ballRadius >= screenWidth)
{
    if (ballPos.x - ballRadius <= 0) scoreRight++;
    else scoreLeft++;

    ballPos = { screenWidth / 2.0f, screenHeight / 2.0f };

    int angleDeg = GetRandomValue(-45, 45);
    float rad = angleDeg * (3.14159265f / 180.0f);

    float dirX = (ballPos.x < screenWidth / 2) ? 1.0f : -1.0f;
    float speed = 360.0f;

    ballVel.x = dirX * speed * cosf(rad);
    ballVel.y = speed * sinf(rad);
}
```

Logic:

Checks if a goal is scored.

Updates score for the correct player.

Resets the ball to the center.

Generates a random angle for the next ball launch to avoid repetitive straight shots.

Ball moves toward the player who lost the point, with a vertical component determined by the random angle.

```
// --- Drawing Game ---
DrawRectangleRec(leftPaddle, RAYWHITE);
DrawRectangleRec(rightPaddle, RAYWHITE);
DrawCircleV(ballPos, ballRadius, RAYWHITE);

DrawText(TextFormat("%i", scoreLeft), screenWidth / 4 - 20, 20, 40, RAYWHITE);
DrawText(TextFormat("%i", scoreRight), 3 * screenWidth / 4 - 20, 20, 40, RAYWHITE);

DrawFPS(10, 10);
```

Logic:

Draws paddles, ball, and scores on the screen.

Displays current FPS in the corner for performance monitoring.
