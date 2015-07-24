//Object IDS

enum IDS {PLAYER1, PLAYER2, ENEMY, BULLET, POWERUP, MENU};
enum POWERUPS {LIFE, DLASER, TLASER, TIMEWARP};
enum WEAPONS {sLASER, dLASER, tLASER};
enum state {START_MENU, OPTIONS, CREDITS, HI_SCORES, INSTRUCTIONS, PLAYING, PAUSE_MENU, GAMEOVER, GETTING_SCORE, EXIT_MENU};
enum KEYS {UP, DOWN, LEFT, RIGHT, SPACE, ESCAPE, ENTER, P, Y, N, I, H, W, A, S, D, ENTER2, NUMPLUS};
bool keys[19] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false, false,false,false};

//=========================
//DEFINING STRUCTS
//=========================

struct SpaceShip
{
	int ID;
	float x;
	float y;
	float boundx;
	float boundy;
	int score;
	int lives;
    int weapon;
	int tWarp;
	int gun;
	bool tempInvinc;
	bool timeWarp;
	int tempInvFrames;
	int tempInvCounter;
	int timeWarpFrames;
	int weapFrames;

	float direction;
	float velocityX;
	float velocityY;
	float totalVelocity;
	float maxVelocity;
	float acceleration;
	float time;

	int frameWidth;
	int frameHeight;

	ALLEGRO_BITMAP *image;
};

struct Bullet
{
	int ID;
	float x;
	float y;
	float vectorX;
	float vectorY;
	float angle;
	bool live;
	float velocity;
	float distTravelled;

	ALLEGRO_BITMAP *image;
};

struct Asteroid
{
	int ID;
	int type;
	float x;
	float y;
	bool live;
	float size;
	int health;

	float velocity;
	float boundx;
	float boundy;
	float vectorX;
	float vectorY;
	float angle;


	float maxFrame;
	int curFrame;
	float frameCount;
	float frameWidth;
	float frameHeight;
	int frameDelay;
	int animationColumn;
	int animationDirection;

	ALLEGRO_BITMAP *image;
};

struct Explosion
{
	float x;
	float y;
	float size;
	bool live;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumn;
	int animationDirection;

	ALLEGRO_BITMAP *image;
	
};

struct Enemy
{
	int ID;
	float x;
	float y;
	float boundx;
	float boundy;
	bool live;
	
	float angle;
	float vectorX;
	float vectorY;
	float velocity;
	float distTravelled;

	int frameWidth;
	int frameHeight;

	ALLEGRO_BITMAP *image;
};

struct PowerUp
{
	int ID;
	int type;
	float x;
	float y;
	float boundx;
	float boundy;
	bool live;
	
	float angle;
	float vectorX;
	float vectorY;
	float velocity;
	float distTravelled;

	float frameWidth;
	float frameHeight;

	ALLEGRO_BITMAP *image;
};