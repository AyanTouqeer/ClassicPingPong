#include <iostream>
#include <raylib.h>
#include <raygui.h> //Add raygui header file.
#include <cmath> // For sin() function.

using namespace std;

//Defining colors.
#define LIGHT_BEIGE (Color){ 235, 215, 180, 255 }   // Light Beige
#define LIGHT_BLUE (Color){ 173, 216, 230, 255 }   // Light Blue
#define LIGHT_YELLOW (Color){ 255, 255, 224, 255 }   // Light Yellow
#define LIGHT_VIOLET (Color){ 219, 112, 255, 255 }   // Light Violet
#define ELECTRIC_BLUE (Color){ 0, 191, 255, 255 }  // Electric blue
#define BRIGHT_YELLOW (Color){ 255, 215, 0, 255 } // Bright Yellow
#define NEON_GREEN (Color){ 50, 205, 50, 255 }     // Neon Green
#define CRIMSON_RED (Color){ 220, 20, 60, 255 }    // Crimson Red

// Warm White / Light Cream
#define COLOR_WARM_WHITE   CLITERAL(Color){ 255, 241, 224, 255 } // Hex: #FFF1E0
#define COLOR_LIGHT_CREAM  CLITERAL(Color){ 245, 245, 220, 255 } // Hex: #F5F5DC

// Muted Gold / Brass
#define COLOR_MUTED_GOLD   CLITERAL(Color){ 212, 175, 55, 255 }  // Hex: #D4AF37
#define COLOR_BRASS        CLITERAL(Color){ 197, 163, 88, 255 }  // Hex: #C5A358

// Soft Green-Blue
#define COLOR_SOFT_TEAL    CLITERAL(Color){ 142, 178, 160, 255 } // Hex: #8EB2A0
#define COLOR_GREEN_BLUE   CLITERAL(Color){ 111, 175, 159, 255 } // Hex: #6FAF9F

// Desaturated Red / Brick
#define COLOR_DESAT_RED    CLITERAL(Color){ 163, 58, 58, 255 }   // Hex: #A33A3A
#define COLOR_BRICK        CLITERAL(Color){ 179, 71, 71, 255 }   // Hex: #B34747

// Light Gray / Silver
#define COLOR_LIGHT_GRAY   CLITERAL(Color){ 176, 176, 176, 255 } // Hex: #B0B0B0
#define COLOR_SILVER       CLITERAL(Color){ 204, 204, 204, 255 } // Hex: #CCCCCC



//Two variables to store the score.
int playerScore = 0;
int cpuScore = 0;

// Create a ball class.
class Ball
{
public:

    //Attribute of ball.
    float xPos, yPos, radius;
    int xSpeed, ySpeed;

    Ball()
    {
        xPos = static_cast<float>(GetScreenWidth() / 2);
        yPos = static_cast<float>(GetScreenHeight() / 2);
        radius = static_cast<float>(10);
    }

    //Methods.
    void DrawBall()
    {
        DrawCircle(xPos, yPos, radius, YELLOW);
    }

    void UpdateBallPosition()
    {
        //First increment x and y positions of the ball.
        xPos += xSpeed;
        yPos += ySpeed;

        //Restrict the position of the ball.
        if(yPos + radius >= GetScreenHeight() || yPos - radius <= 0)
        {
            //Change the direction.
            ySpeed *= -1;
        }

        if (xPos + radius >= GetScreenWidth())  //Condition for right window side. (Player side window)
        {
            cpuScore += 1;
            ResetBall();

        }

        if (xPos - radius <= 0)  //Condition for left window side. (CPU side window)
        {
            playerScore += 1;
            ResetBall();
        }


    }

    void ResetBall()
    {
        //Updating the x and y positions of the ball.
        xPos = GetScreenWidth() / 2;
        yPos = GetScreenHeight() / 2;

        //Give a new direction.
        int directionChoice[2] = {-1, 1};

        //Multiple the values by random values.
        xSpeed *= directionChoice[GetRandomValue(0, 1)];
        ySpeed *= directionChoice[GetRandomValue(0, 1)];
    }


    //Check ball collision with the paddle.
    void CheckBallCollisionWithPaddle(float ballXPos, float ballYPos, float ballRadius, float paddleXPosRight, float paddleYPosRight, float paddleXPosLeft, float paddleYPosLeft, float width, float height)
    {
        if(CheckCollisionCircleRec(Vector2{ballXPos, ballYPos}, ballRadius, Rectangle{paddleXPosRight, paddleYPosRight, width, height}))
        {
            xSpeed *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ballXPos, ballYPos}, ballRadius, Rectangle{paddleXPosLeft, paddleYPosLeft, width, height}))
        {
            xSpeed *= -1;
        }
    }


};

class Paddles
{
protected:
    void RestrictThePaddle(float &yPosUniversal, int paddleHeight)
    {
        if (yPosUniversal + paddleHeight >= GetScreenHeight())  //To restrict top window side.
        {
            yPosUniversal = GetScreenHeight() - paddleHeight;
        }

        if (yPosUniversal <= 0)  //To restrict bottom window side.
        {
            yPosUniversal = 0;
        }
    }

public:
    //Attributes.
    float xPosRight, xPosLeft, yPosRight, yPosLeft, xSpeed, ySpeed;
    float width, height;

    Paddles()
    {
        xPosRight = static_cast<float>(GetScreenWidth() - 35);
        xPosLeft = static_cast<float>(GetScreenHeight() / 2 - 60);
        yPosRight = static_cast<float>(10);
        yPosLeft = static_cast<float>(GetScreenHeight() / 2 - 60);
        width = static_cast<float>(25);
        height = static_cast<float>(120);
    }
    //Methods.

    //Note that rectangle in raylib is drawn from top left corner.
    void DrawRightPaddle()
    {
        DrawRectangleRounded(Rectangle{xPosRight, yPosRight, width, height}, 0.8, 0, WHITE); // Right paddle.
    }

    void DrawLeftPaddle()
    {
        DrawRectangleRounded(Rectangle{xPosLeft, yPosLeft, width, height}, 0.8, 0, WHITE); // Left paddle.
    }

    // Move paddles.
    void MoveRightPaddle()
    {
        //Move Paddle up and down.
        if(IsKeyDown(KEY_UP) )
        {
            yPosRight -= ySpeed; //Moves the paddle vertically up.
        }

        if(IsKeyDown(KEY_DOWN))
        {
            yPosRight += ySpeed; //Moves the paddle vertically down.
        }

        //Restrict the movement.
        RestrictThePaddle(yPosRight, height);
    }

    void MoveLeftPaddle(float ballYPos)
    {
        //Move the paddle vertically up or down.
        if ( yPosLeft >= ballYPos)
        {
            yPosLeft -= ySpeed; //Moves the paddle vertically up.
        }

        if ( yPosLeft <= ballYPos)
        {
            yPosLeft += ySpeed; //Moves the paddle vertically down.
        }

        //Restrict the movement.
        RestrictThePaddle(yPosLeft, height);
    }


};

// Button class to handle main menu buttons.
class Button {
public:
    Rectangle bounds;
    Color baseColor;
    Color hoverColor;
    Color activeColor;
    std::string text;

    // Paddle button-specific attributes
    bool isPaddleButton;   // To differentiate between paddle and text buttons
    Vector2 paddlePosition;
    int paddleRadius;
    Color paddleColor;

    // Constructor for text-based button
    Button(float x, float y, float width, float height, Color base, Color hover, Color active, const std::string &label)
        : bounds{ x, y, width, height }, baseColor(base), hoverColor(hover), activeColor(active), text(label),
          isPaddleButton(false), paddlePosition{0, 0}, paddleRadius(0), paddleColor{WHITE} {}

    // Constructor for paddle button
    Button(Vector2 position, int radius, Color base, Color hover, Color active)
        : bounds{ position.x - radius, position.y - radius, static_cast<float> (radius * 2), static_cast<float>(radius * 2) }, 
          baseColor(base), hoverColor(hover), activeColor(active), text(""),
          isPaddleButton(true), paddlePosition(position), paddleRadius(radius), paddleColor(base) {}

    // Checks if mouse is over the button
    bool IsHovered() const {
        return isPaddleButton ? CheckCollisionPointCircle(GetMousePosition(), paddlePosition, paddleRadius) 
                              : CheckCollisionPointRec(GetMousePosition(), bounds);
    }

    // Draws the button (text or paddle icon based on type)
    void Draw() const {
        Color currentColor = baseColor;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && IsHovered()) {
            currentColor = activeColor;  // Active color when clicked
        } else if (IsHovered()) {
            currentColor = hoverColor;   // Hover color
        }

        if (isPaddleButton) {
            // Draw paddle icon button
            DrawCircleV(paddlePosition, paddleRadius, currentColor);  // Paddle head
            DrawRectangle(paddlePosition.x - 5, paddlePosition.y + paddleRadius, 10, 30, WHITE); // Paddle handle
        } else {
            // Draw text-based button
            DrawRectangleRec({bounds.x + 3, bounds.y + 3, bounds.width, bounds.height}, Fade(BLACK, 0.3f)); // Shadow
            DrawRectangleRounded(bounds, 0.3, 4, currentColor); // Rounded rectangle button
            DrawText(text.c_str(), bounds.x + bounds.width / 2 - MeasureText(text.c_str(), 20) / 2, bounds.y + bounds.height / 2 - 10, 20, WHITE);
        }
    }

    // Checks if the button is clicked
    bool IsClicked() const {
        return IsHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }
};


// MainMenu class to handle menu logic
class MainMenu
{
private:
    bool gameStarted;
    bool showSettings;
    Texture2D background;  //define a background variable.
    float waveAngle; //Add angle for wave animation.

public:
    MainMenu(Texture2D bg) : gameStarted(false), showSettings(false), background(bg) {}

    float titleSize = 50.0f;          // Initial font size
    float titleScaleSpeed = 0.5f;     // Speed of scaling
    bool increasing = true;           // Track scaling direction
    float backgroundOffset = 0.0f;

    // Define source and destination rectangles for drawing
    Rectangle srcRect = { 0.0f, 0.0f, static_cast<float>(background.width), static_cast<float>(background.height) };
    Rectangle destRect = { 0.0f, 0.0f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };

    // Adjust origin if needed, or use (0, 0) for top-left alignment
    Vector2 origin = { 0.0f, 0.0f };

    // Title text with a neon glow effect
    void DrawNeonText(const char* title, int posX, int posY, int fontSize, Color mainColor) {
        DrawText(title, posX - 2, posY - 2, fontSize + 4, Fade(mainColor, 0.3f)); // Outer glow layer
        DrawText(title, posX + 2, posY + 2, fontSize + 2, Fade(mainColor, 0.5f)); // Inner glow layer
        DrawText(title, posX, posY, fontSize, mainColor); // Main title layer
    }


    void AnimateTitlePulsate()
    {
        if (increasing)
            titleSize += titleScaleSpeed;
        else
            titleSize -= titleScaleSpeed;

        if (titleSize >= 55.0f) increasing = false;
        if (titleSize <= 50.0f) increasing = true;

        int posX = GetScreenWidth() / 2 - MeasureText("PING PONG", titleSize) / 2;
        int posY = GetScreenHeight() / 4;

        // Use neon glow effect with a color you like for the title
        DrawNeonText("PING PONG", posX, posY, titleSize, COLOR_SILVER);
    }

    // Enhanced menu drawing function in MainMenu class
    void DrawMenu(float screenWidth, float screenHeight, Font customFont)
    {
        // Draw background with animation
        DrawTexturePro(background, srcRect, destRect, origin, 0.0f, WHITE);

        // Define buttons with improved colors and positioning
        Button startButton(screenWidth / 2 - 50, screenHeight / 2 - 40, 100, 30, COLOR_SOFT_TEAL, COLOR_GREEN_BLUE, COLOR_MUTED_GOLD, "Start Game");
        Button settingsButton(screenWidth / 2 - 50, screenHeight / 2 + 10, 100, 30, COLOR_SOFT_TEAL, COLOR_GREEN_BLUE, COLOR_MUTED_GOLD, "Settings");
        Button exitButton(screenWidth / 2 - 50, screenHeight / 2 + 60, 100, 30, COLOR_SOFT_TEAL, COLOR_GREEN_BLUE, COLOR_MUTED_GOLD, "Exit");

        // Draw the buttons and check for interactions
        startButton.Draw();
        if (startButton.IsClicked()) {
            gameStarted = true;
        }

        settingsButton.Draw();
        if (settingsButton.IsClicked()) {
            showSettings = !showSettings;
        }

        exitButton.Draw();
        if (exitButton.IsClicked()) {
            CloseWindow();
        }
    }

    void DrawMovingBackground(Texture2D background)
    {
        // Increase the offset each frame to create motion
        backgroundOffset += 2.0f; // Adjust speed as needed
        if (backgroundOffset > background.width) {
            backgroundOffset = 0; // Reset offset to loop
        }

        // Draw the texture twice to create a seamless scrolling effect
        DrawTextureEx(background, (Vector2){-backgroundOffset, 0}, 0.0f, 1.0f, WHITE);
        DrawTextureEx(background, (Vector2){background.width - backgroundOffset, 0}, 0.0f, 1.0f, WHITE);
    }

    // Method to handle the game state
    bool IsGameStarted() const
    {
        return gameStarted;
    }

    void ResetGame()
    {
        gameStarted = false;
    }
};

//Create ball object.
Ball ball;

//Create paddle object.
Paddles paddle;

int main()
{
    cout << "Starting the game." << endl;

    //Screen size variables.
    const int screenWidth = 800;
    const int screenHeight = 600;

    //Initialize the window of game.
    InitWindow(screenWidth, screenHeight, "PING PONG");

    // Load the font at the beginning of your main function
    Font customFont = LoadFontEx("Fonts/YourFont.ttf", 50, 0, 0);

    // Load the background image.
    Texture2D background = LoadTexture("D:/OneDrive/Desktop/ClassicPingPong/Images/ClassicBackgroundMainMenuPNG.png");

    //Debug statement.
    cout << "Attempting to load texture from: " << "D:/Images/ClassicBackground.png" << endl;

    // Verify that the texture loaded correctly
    if (background.width == 0 || background.height == 0) {
        cout << "Error: Failed to load background texture!" << endl;
        return -1;
    }
    else
    {
        cout << "Successfully loaded texture with width: " << background.width << " and height: " << background.height << endl;
    }

    // Create main menu object with background texture
    MainMenu mainMenu(background);

    // Menu option variables
    bool startGame = false;
    bool showSettings = false;

    //Declare the frame rate.
    SetTargetFPS(60); //The game will run with same Frame rate on every computer.

   //Initialize paddle attributes.
    paddle.xPosRight = screenWidth - 35;
    paddle.yPosRight = screenHeight / 2 - 60;
    paddle.xPosLeft = 10;
    paddle.yPosLeft = screenHeight / 2 - 60;
    paddle.width = 25;
    paddle.height = 120;
    paddle.xSpeed = 7;
    paddle.ySpeed = 7;

    //Initialize ball attributes.
    ball.xPos = screenWidth / 2;
    ball.yPos = screenHeight / 2;
    ball.radius = 10;
    ball.xSpeed = 7;
    ball.ySpeed = 7;

    //Start the game loop.
    while(!WindowShouldClose())
    {
        BeginDrawing();

        //Clear background.
        ClearBackground(LIGHT_BEIGE);

        //Draw background.
        DrawRectangle(0, 0, screenWidth / 2, GetScreenHeight(), BEIGE);


         // Check if the game has started or if we're in the menu
        if (!mainMenu.IsGameStarted())
        {
            // Draw the main menu.
            mainMenu.DrawMenu(screenWidth, screenHeight, customFont);
            // Draw the animated title.
            mainMenu.AnimateTitlePulsate();
        }
        else
        {
            // Game is running, display game elements
            ball.DrawBall();
            ball.UpdateBallPosition();
            paddle.DrawLeftPaddle();
            paddle.DrawRightPaddle();
            paddle.MoveRightPaddle();
            paddle.MoveLeftPaddle(ball.yPos);
            ball.CheckBallCollisionWithPaddle(ball.xPos, ball.yPos, ball.radius, paddle.xPosRight, paddle.yPosRight, paddle.xPosLeft, paddle.yPosLeft, paddle.width, paddle.height);

            // Draw middle line and score
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
            DrawCircleLines(screenWidth / 2, screenHeight / 2, 150, LIGHTGRAY);
            DrawText(TextFormat("%d", cpuScore), screenWidth / 4, 20, 60, LIGHT_BLUE);
            DrawText(TextFormat("%d", playerScore), screenWidth / 1.4, 20, 60, LIGHT_BLUE);

            // Add a back-to-menu button
            if (GuiButton((Rectangle){ screenWidth - 110, screenHeight - 40, 100, 30 }, "Main Menu"))
            {
                mainMenu.ResetGame();  // Go back to the main menu
            }
        }

        EndDrawing();
    }

    // Unload the background texture before closing the window.
    UnloadTexture(background);

    // Unload the font before closing the program
    UnloadFont(customFont);

    CloseWindow();
    return 0;
}


