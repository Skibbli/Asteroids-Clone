/*Computer Graphics for Games - Assignment 1
  Asteroids! by Duncan Hall*/

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>
#include <vector>
#include "Objects.h"
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <sstream>

using namespace std;

//===============================
//GLOBAL VARIABLES
//===============================
const float PI = 3.14159265359;
const int width = 1400;
const int height = 1000;
const int NUM_BULLETS = 200;
const int NUM_ASTEROIDS = 150;
const int NUM_EXPLOSIONS = 100;
const int NUM_POWERUPS = 4;
int numLargeAsteroids = 0;
int wave = 1;
bool getP1name = true;
const float FPS = 60;

//===============================
//FUNCTION PROTOTYPES
//===============================

void ChangeState(int &state, int newState);
void LimitCheck(SpaceShip &ship, SpaceShip &ship2, Asteroid asteroid[], Bullet bullets[], Enemy &UFO, PowerUp powerups[]);

void InitShip(SpaceShip &ship, SpaceShip &ship2, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *image2);
void DrawShip(SpaceShip &ship, SpaceShip &ship2, int ID);
void MoveShipForward(SpaceShip &ship);
void MoveShipBack(SpaceShip &ship);
void DecelerateShip(SpaceShip &ship);
void ResetShip(SpaceShip &ship, SpaceShip &ship2);

void InitBullet(Bullet bullet[]);
void DrawBullet(Bullet bullet[]);
void FireBullet(Bullet bullet[], ALLEGRO_BITMAP *image, SpaceShip &ship, SpaceShip &ship2, int ID,  Enemy &UFO, ALLEGRO_SAMPLE *sample, bool multiplayer);
void UpdateBullet(Bullet bullet[]);
void CollideBullet(Bullet bullet[], Asteroid asteroid[], SpaceShip &ship, SpaceShip &ship2, int ID, Enemy &UFO, Explosion explosions[],
	ALLEGRO_BITMAP *medAstImage, ALLEGRO_BITMAP *smallAstImage, ALLEGRO_SAMPLE *expSound);

void InitAsteroid(Asteroid asteroid[]);
bool DrawAsteroid(Asteroid asteroid[]);
void StartAsteroid(Asteroid asteroid[], int aSize, float asteroidx, float asteroidy, ALLEGRO_BITMAP *image);
void UpdateAsteroid(Asteroid asteroid[]);
void CollideAsteroid(Asteroid asteroid[], SpaceShip &ship, SpaceShip &ship2, int ID, Explosion explosions[], ALLEGRO_BITMAP *medAstImage,
	ALLEGRO_BITMAP *smallAstImage, ALLEGRO_SAMPLE *expSound);

void InitExplosions(Explosion explosions[], ALLEGRO_BITMAP *image = NULL);
void DrawExplosions(Explosion explosions[]);
void StartExplosions(Explosion explosions[], int eSize, float x, float y, ALLEGRO_SAMPLE *expSound);
void UpdateExplosions(Explosion explosions[]);

void InitUFO(Enemy &UFO, ALLEGRO_BITMAP *image = NULL);
void DrawUFO(Enemy &UFO, SpaceShip &ship, bool multiplayer, ALLEGRO_BITMAP *ufoLaserImage);
void StartUFO(Enemy &UFO);
void UpdateUFO(Enemy &UFO);
void CollideUFO(Enemy &UFO, SpaceShip &ship, SpaceShip &ship2, int ID, Explosion explosions[], ALLEGRO_SAMPLE *expSound);

void InitPowerUps(PowerUp powerups[], ALLEGRO_BITMAP *powerUp1 = NULL, ALLEGRO_BITMAP *powerUp2 = NULL,
	ALLEGRO_BITMAP *powerUp3 = NULL, ALLEGRO_BITMAP *powerUp4 = NULL);
void DrawPowerUps(PowerUp powerups[]);
void StartPowerUps(PowerUp powerups[], int type);
void UpdatePowerUps(PowerUp powerups[]);
bool CollidePowerUps(PowerUp powerups[], SpaceShip &ship, SpaceShip &ship2, int ID, ALLEGRO_SAMPLE *sample);

int main()
{
//===============================
//PROJECT VARIABLES
//===============================
	bool done = false;
	bool redraw = true;
	bool asteroidsLive = false;
	bool multiplayer = false;
	int frames = 0;
	int counter = 0;
	string Player1 = "";
	string Player2 = "";

	ostringstream getName;

	int state = -1;
		
//===============================
//OBJECT VARIABLES
//===============================
	SpaceShip ship;
	SpaceShip ship2;
	Bullet bullet[NUM_BULLETS];
	Asteroid asteroid[NUM_ASTEROIDS];
	Explosion explosions[NUM_EXPLOSIONS];
	Enemy UFO;
	PowerUp powerups[NUM_POWERUPS];
	vector <string> hiScores;

//===============================
//ALLEGRO VARIABLES
//===============================
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_BITMAP *shipImage = NULL;
	ALLEGRO_BITMAP *ship2Image = NULL;
	ALLEGRO_BITMAP *largeAstImage = NULL;
	ALLEGRO_BITMAP *medAstImage = NULL;
	ALLEGRO_BITMAP *smallAstImage = NULL;
	ALLEGRO_BITMAP *expImage = NULL;
	ALLEGRO_BITMAP *sLaserImage = NULL;
	ALLEGRO_BITMAP *sLaser2Image = NULL;
	ALLEGRO_BITMAP *dLaserImage = NULL;
	ALLEGRO_BITMAP *bgImage = NULL;
	ALLEGRO_BITMAP *startMenuImage = NULL;
	ALLEGRO_BITMAP *pauseMenuImage = NULL;
	ALLEGRO_BITMAP *exitMenuImage = NULL;
	ALLEGRO_BITMAP *instructionsImage = NULL;
	ALLEGRO_BITMAP *creditsImage = NULL;
	ALLEGRO_BITMAP *hiScoresImage = NULL;
	ALLEGRO_BITMAP *gameOverImage = NULL;
	ALLEGRO_BITMAP *ufoShipImage = NULL;
	ALLEGRO_BITMAP *ufoLaserImage = NULL;
	ALLEGRO_BITMAP *powerUp1 = NULL;
	ALLEGRO_BITMAP *powerUp2 = NULL;
	ALLEGRO_BITMAP *powerUp3 = NULL;
	ALLEGRO_BITMAP *powerUp4 = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font20 = NULL;
	ALLEGRO_FONT *font45 = NULL;
	ALLEGRO_FONT *font68 = NULL;
	ALLEGRO_FONT *font44 = NULL;
	ALLEGRO_FONT *font38 = NULL;
	ALLEGRO_SAMPLE *expSound = NULL;
	ALLEGRO_SAMPLE *menuMusic = NULL;
	ALLEGRO_SAMPLE *laserSound = NULL;
	ALLEGRO_SAMPLE *ufoSound = NULL;
	ALLEGRO_SAMPLE *powerUpSound = NULL;
	ALLEGRO_SAMPLE *timeWarpBegin = NULL;
	ALLEGRO_SAMPLE *timeWarpEnd = NULL;
	ALLEGRO_SAMPLE *BGmusic = NULL;
	ALLEGRO_SAMPLE_INSTANCE *menuMusicInst = NULL;
	ALLEGRO_SAMPLE_INSTANCE *UFOsoundInst = NULL;
	ALLEGRO_SAMPLE_INSTANCE *BGmusicInst = NULL;

//===============================
//ALLEGRO INIT FUNCTIONS
//===============================
	if(!al_init())									//initialize Allegro
		return -1;

	display = al_create_display(width,height);		//create display

	if(!display)									//test display object
		return -1;

//===============================
//ADDON INSTALLATION
//===============================
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();

//===============================
//PROJECT INIT
//===============================
	al_reserve_samples(30);

	expSound = al_load_sample("Explosion.wav");
	laserSound = al_load_sample("Laser.ogg");

	menuMusic = al_load_sample("Asteroids Menu Music.ogg");
	menuMusicInst = al_create_sample_instance(menuMusic);

	ufoSound = al_load_sample("UFO Noise.ogg");
	UFOsoundInst = al_create_sample_instance(ufoSound);

	BGmusic = al_load_sample("Asteroids Background Music.ogg");
	BGmusicInst = al_create_sample_instance(BGmusic);

	al_attach_sample_instance_to_mixer(BGmusicInst, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(UFOsoundInst, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(menuMusicInst, al_get_default_mixer());

	powerUpSound = al_load_sample("Power Up!.ogg");
	timeWarpBegin = al_load_sample("Timewarp Begin.ogg");
	timeWarpEnd = al_load_sample("Timewarp End.ogg");

	shipImage = al_load_bitmap("ULTIMATE SHIP.png");
	al_convert_mask_to_alpha(shipImage, al_map_rgb(255,0,255));
	ship2Image = al_load_bitmap("ULTIMATE SHIP 2.png");
	al_convert_mask_to_alpha(ship2Image, al_map_rgb(255,0,255));
	largeAstImage = al_load_bitmap("Large Asteroid.png");
	medAstImage = al_load_bitmap("Medium Asteroid.png");	
	smallAstImage = al_load_bitmap("Small Asteroid.png");
	expImage = al_load_bitmap("Explosion1.png");
	sLaserImage = al_load_bitmap("Red Laser.png");
	sLaser2Image = al_load_bitmap("Purple Laser.png");
	dLaserImage = al_load_bitmap("Green Laser.png");
	al_convert_mask_to_alpha(expImage, al_map_rgb(0,0,0));
	bgImage = al_load_bitmap("Background.jpg");
	startMenuImage = al_load_bitmap("Start Menu.png");
	exitMenuImage = al_load_bitmap("Exit Menu.png");
	pauseMenuImage = al_load_bitmap("Pause Menu.png");
	instructionsImage = al_load_bitmap("Instructions.png");
	hiScoresImage = al_load_bitmap("Hi Scores.png");
	creditsImage = al_load_bitmap("Credits.png");
	gameOverImage = al_load_bitmap("Gameover Screen.png");
	ufoShipImage = al_load_bitmap("UFO Ship.png");
	ufoLaserImage = al_load_bitmap("UFO Laser.bmp");
	al_convert_mask_to_alpha(ufoLaserImage, al_map_rgb(255,255,255));
	al_convert_mask_to_alpha(ufoShipImage, al_map_rgb(0,255,0));
	powerUp1 = al_load_bitmap("Extra Life Powerup.bmp");
	al_convert_mask_to_alpha(powerUp1, al_map_rgb(255,255,255));
	powerUp2 = al_load_bitmap("Double Laser Powerup.png");
	al_convert_mask_to_alpha(powerUp2, al_map_rgb(0,255,0));
	powerUp3 = al_load_bitmap("Crazy Laser Powerup.png");
	al_convert_mask_to_alpha(powerUp3, al_map_rgb(255,255,255));
	powerUp4 = al_load_bitmap("Timewarp Powerup.png");
	al_convert_mask_to_alpha(powerUp4, al_map_rgb(0,255,0));

	font20 = al_load_font("Are You Freakin' Serious.ttf", 20, 0);
	font45 = al_load_font("Are You Freakin' Serious.ttf", 45, 0);
	font68 = al_load_font("BAUHS93.ttf", 68, 0);
	font44 = al_load_font("BAUHS93.ttf", 44, 0);
	font38 = al_load_font("BAUHS93.ttf", 38, 0);

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	ChangeState(state, START_MENU);

//===============================
//OBJECT INITIALISATION
//===============================
	srand(time(NULL));
	InitExplosions(explosions, expImage);
	InitUFO(UFO, ufoShipImage);
	InitPowerUps(powerups, powerUp1, powerUp2, powerUp3, powerUp4);

	string input;

	ifstream infile;
	infile.open("HiScores.txt");
	if(infile.is_open())
	{
		while(getline(infile, input))
		{
			hiScores.push_back(input);	
		}	
		infile.close();
	}

//========================================
//LOADING EVENT SOURCES & START TIMER
//========================================
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);

	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		
		//=====================================
		//GAME STATE DEPENDANT EVENTS
		//=====================================

		if(state == START_MENU)
			{
				al_play_sample_instance(menuMusicInst);

				if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					switch(ev.keyboard.keycode)
					{
						case ALLEGRO_KEY_ENTER:
							{
								multiplayer = false;
								frames = 0;
								counter = 0;
								al_stop_sample_instance(menuMusicInst);

								getP1name = true;
								for(int i = 0; i < 18; i++)
								{
									keys[i] = false;
								}
								numLargeAsteroids = 0;
								InitShip(ship, ship2, shipImage, ship2Image);
								InitBullet(bullet);
								InitAsteroid(asteroid);
								InitExplosions(explosions);
								InitUFO(UFO);
								InitPowerUps(powerups);
								ChangeState(state, PLAYING);
							}
							break;
						case ALLEGRO_KEY_M:
							{
								multiplayer = true;
								frames = 0;
								counter = 0;
							//	al_stop_sample(&MENU);

								getP1name = true;
								for(int i = 0; i < 18; i++)
								{
									keys[i] = false;
								}
								numLargeAsteroids = 0;
								InitShip(ship, ship2, shipImage, ship2Image);
								InitBullet(bullet);
								InitAsteroid(asteroid);
								InitExplosions(explosions);
								InitUFO(UFO);
								InitPowerUps(powerups);
								ChangeState(state, PLAYING);
							}
							break;
						case ALLEGRO_KEY_ESCAPE: done = true; 
							break;
						case ALLEGRO_KEY_O: ChangeState(state, OPTIONS);
							break;
						case ALLEGRO_KEY_I: ChangeState(state, INSTRUCTIONS);
							break;
						case ALLEGRO_KEY_H: ChangeState(state, HI_SCORES);
							break;
						case ALLEGRO_KEY_C: ChangeState(state, CREDITS);
							break;
					}
				
				}
			}

		else if(state == CREDITS)
			{
				if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
						ChangeState(state, START_MENU);
				}
			}

		else if(state == EXIT_MENU)
			{
				if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					switch(ev.keyboard.keycode)
					{
					case ALLEGRO_KEY_Y: ChangeState(state, START_MENU);
						break;
					case ALLEGRO_KEY_N:	ChangeState(state, PAUSE_MENU);
						break;
					}
				}
			}

		else if(state == INSTRUCTIONS)
			{
				if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
						ChangeState(state, START_MENU);
				}
			}

		else if(state == HI_SCORES)
			{
				sort(hiScores.begin(), hiScores.end());

				if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
						ChangeState(state, START_MENU);
				}
			}
			
		else if(state == EXIT_MENU)
			{
				if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					switch(ev.keyboard.keycode)
					{
					case ALLEGRO_KEY_Y: ChangeState(state, START_MENU);
						break;
					case ALLEGRO_KEY_N:	ChangeState(state, PAUSE_MENU);
						break;
					}
				}
			} 

		else if(state == PAUSE_MENU)
			{
				if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					switch(ev.keyboard.keycode)
					{
					case ALLEGRO_KEY_ESCAPE: ChangeState(state, EXIT_MENU);
						break;
					case ALLEGRO_KEY_ENTER:	ChangeState(state, PLAYING);
						break;
					}
				}
			}

		else if(state == GAMEOVER)
			{
				if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					switch(ev.keyboard.keycode)
					{
					case ALLEGRO_KEY_ESCAPE: 
						{
							getName << ship.score;
							Player1 += ' ';
							Player1 += getName.str();
							getName.str("");
							hiScores.push_back(Player1);

							if(multiplayer)
							{
								getName << ship2.score;
								Player2 += ' ';
								Player2 += getName.str();
								getName.str("");
								hiScores.push_back(Player2);
							}

							Player1 = "";
							Player2 = "";

							ChangeState(state, START_MENU);
						}
						break;
					case ALLEGRO_KEY_ENTER:
						{
							multiplayer = false;
							frames = 0;
							counter = 0;

							getName << ship.score;
							Player1 += ' ';
							Player1 += getName.str();
							getName.str("");
							hiScores.push_back(Player1);

							getName << ship2.score;
							Player2 += ' ';
							Player2 += getName.str();
							getName.str("");
							hiScores.push_back(Player2);

							Player1 = "";
							Player2 = "";
							getP1name = true;
							for(int i = 0; i < 18; i++)
							{
								keys[i] = false;
							}
							numLargeAsteroids = 0;
							InitShip(ship, ship2, shipImage, ship2Image);
							InitBullet(bullet);
							InitAsteroid(asteroid);
							InitExplosions(explosions);
							InitUFO(UFO);
							InitPowerUps(powerups);
							ChangeState(state, PLAYING);
						}
						break;
					case ALLEGRO_KEY_M:
						{
								multiplayer = true;
								frames = 0;
								counter = 0;

								getName << ship.score;
								Player1 += ' ';
								Player1 += getName.str();
								getName.str("");
								hiScores.push_back(Player1);

								getName << ship2.score;
								Player2 += ' ';
								Player2 += getName.str();
								getName.str("");
								hiScores.push_back(Player2);

								Player1 = "";
								Player2 = "";
								getP1name = true;
								for(int i = 0; i < 18; i++)
								{
									keys[i] = false;
								}
								numLargeAsteroids = 0;
								InitShip(ship, ship2, shipImage, ship2Image);
								InitBullet(bullet);
								InitAsteroid(asteroid);
								InitExplosions(explosions);
								InitUFO(UFO);
								InitPowerUps(powerups);
								ChangeState(state, PLAYING);
							}
							break;
					}
				}
			}

		else if(state == GETTING_SCORE)
		{
			if(getP1name)
			{
				if(ev.type == ALLEGRO_EVENT_KEY_CHAR)
				{
					switch(ev.keyboard.keycode)
					{
						case ALLEGRO_KEY_BACKSPACE:
							{
								if (!Player1.empty())
									Player1 = Player1.substr(0, Player1.length()-1);
							}
							break;
							case ALLEGRO_KEY_ENTER: 
								{
									if (!Player1.empty())
									{
										getP1name = false;
										
										if(multiplayer == false)
											ChangeState(state, GAMEOVER);
									}									
								}
								break;
						default:
							{
								if((ev.keyboard.unichar <= 125) && (ev.keyboard.unichar >= 32))
								{
									Player1 += ev.keyboard.unichar;
								}
	
							}
							break;
					}
				}
			}

			if(multiplayer && getP1name == false)
			{
				if(ev.type == ALLEGRO_EVENT_KEY_CHAR)
				{
					switch(ev.keyboard.keycode)
					{
						case ALLEGRO_KEY_BACKSPACE:
												{
													if (!Player2.empty())
														Player2 = Player2.substr(0, Player2.length()-1);
												}
												break;
						case ALLEGRO_KEY_ENTER: 
												{
													if (!Player2.empty())
														ChangeState (state, GAMEOVER);
												}
						default:
								{
									if((ev.keyboard.unichar <= 125) && (ev.keyboard.unichar >= 32))
									{
										Player2 += ev.keyboard.unichar;
									}
								}
						break;
					}
				}
			}
		}

		else if(state == PLAYING)
		{
			al_play_sample_instance(BGmusicInst);

		//==================================
		//GAMEOVER CHECK
		//==================================
				
		if(multiplayer && ship.lives <= 0 && ship2.lives <= 0)
		{
			al_stop_sample_instance(BGmusicInst);
			ChangeState(state, GETTING_SCORE);
		}

		else if(multiplayer == false && ship.lives <= 0)
		{
			al_stop_sample_instance(BGmusicInst);	
			ChangeState(state, GETTING_SCORE);
		}

		//===============================
		//GAME KEYBOARD INPUT
		//===============================

			if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch(ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_W: keys[W] = true;
					break;
				case ALLEGRO_KEY_S: keys[S] = true;
					break;
				case ALLEGRO_KEY_A: keys[A] = true;
					break;
				case ALLEGRO_KEY_D: keys[D] = true;
					break;
				case ALLEGRO_KEY_SPACE: keys[SPACE] = true;
					if(ship.lives > 0 && !ship2.timeWarp)
					FireBullet(bullet, sLaserImage, ship, ship2, PLAYER1, UFO, laserSound, multiplayer);
					break;
				case ALLEGRO_KEY_ENTER:
					{
						if(ship.tWarp > 0)
						{
							ship.timeWarp = true;
							al_play_sample(timeWarpBegin,1.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
							ship.tWarp--;
						}
					}
					break;
				case ALLEGRO_KEY_UP: keys[UP] = true;
					break;
				case ALLEGRO_KEY_DOWN: keys[DOWN] = true;
					break;
				case ALLEGRO_KEY_LEFT: keys[LEFT] = true;
					break;
				case ALLEGRO_KEY_RIGHT: keys[RIGHT] = true;
					break;
				case ALLEGRO_KEY_PAD_ENTER: keys[ENTER2] = true;
					if(ship2.lives > 0 && !ship.timeWarp && multiplayer)
					FireBullet(bullet, sLaser2Image, ship, ship2, PLAYER2, UFO, laserSound, multiplayer);
					break;
				case ALLEGRO_KEY_PAD_PLUS:
					{
						if(ship2.tWarp > 0)
						{
							ship2.timeWarp = true;
							al_play_sample(timeWarpBegin,1.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
							ship2.tWarp--;
						}
					}
					break;
				case ALLEGRO_KEY_ESCAPE: ChangeState(state, PAUSE_MENU);
					break;
				}
			}

			else if(ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch(ev.keyboard.keycode)
				{
					case ALLEGRO_KEY_W: keys[W] = false;
						break;
					case ALLEGRO_KEY_S: keys[S] = false;
						break;
					case ALLEGRO_KEY_A: keys[A] = false;
						break;
					case ALLEGRO_KEY_D: keys[D] = false;
						break;
					case ALLEGRO_KEY_SPACE: keys[SPACE] = false;
						break;
					case ALLEGRO_KEY_UP: keys[UP] = false;
						break;
					case ALLEGRO_KEY_DOWN: keys[DOWN] = false;
						break;
					case ALLEGRO_KEY_LEFT: keys[LEFT] = false;
						break;
					case ALLEGRO_KEY_RIGHT: keys[RIGHT] = false;
						break;
					case ALLEGRO_KEY_PAD_ENTER: keys[ENTER2] = false;
						break;
				}
			}
			}

		//===============================
		//GAME UPDATE
		//===============================
		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			
			redraw = true;
			
			if(state == PLAYING)
			{
				//===================
				//UPDATING COUNTERS
				//===================
				frames++;
				counter = frames / 60;

				//=================================
				//PLAYER 1 POWERUP COUNTERS
				//=================================

				if(ship.weapon == dLASER)
				{
					ship.weapFrames++;

					if(ship.weapFrames == 840)
					{
						ship.weapon = sLASER;
						ship.weapFrames = 0;
					}
				}

				if(ship.weapon == tLASER)
				{
					ship.weapFrames++;

					if(ship.weapFrames == 660)
					{
						ship.weapon = sLASER;
						ship.weapFrames = 0;
					}
				}

				if(ship.timeWarp == true)
				{
					ship.timeWarpFrames++;

					if(ship.timeWarpFrames > 500)
					{
						ship.timeWarp = false;
						al_play_sample(timeWarpEnd,1.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
						ship.timeWarpFrames = 0;
					}
				}

				//================================
				//PLAYER 2 POWERUP COUNTERS
				//================================

				if(ship2.weapon == dLASER)
				{
					ship2.weapFrames++;

					if(ship2.weapFrames == 840)
					{
						ship2.weapon = sLASER;
						ship2.weapFrames = 0;
					}
				}

				if(ship2.weapon == tLASER)
				{
					ship2.weapFrames++;

					if(ship2.weapFrames == 660)
					{
						ship2.weapon = sLASER;
						ship2.weapFrames = 0;
					}
				}

				if(ship2.timeWarp == true)
				{
					ship2.timeWarpFrames++;

					if(ship2.timeWarpFrames > 500)
					{
						ship2.timeWarp = false;
						al_play_sample(timeWarpEnd,1.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
						ship2.timeWarpFrames = 0;
					}
				}

				//=======================================================
				//STARTS NEW WAVE IF ALL ASTEROIDS AND UFO NOT LIVE
				//=======================================================

				if(asteroidsLive == false && numLargeAsteroids > 2 && UFO.live == false)
				{
					wave++;
					InitBullet(bullet);
					InitAsteroid(asteroid);
					InitExplosions(explosions);
					InitUFO(UFO);
					frames = 0;
					counter = 0;
					numLargeAsteroids = 0;
				}

				//LIMIT CHECKING FOR ALL OBJECTS

				LimitCheck(ship, ship2, asteroid, bullet, UFO, powerups);
			
				//==========================================================
				//UPDATING GAME OBJECTS AND CHECKING FOR COLLISION
				//==========================================================

				UpdateExplosions(explosions);
				UpdateBullet(bullet);
				StartAsteroid(asteroid, 3, 0, 0, largeAstImage);

				if(ship.timeWarp == false && ship2.timeWarp == false)
				{
					UpdateAsteroid(asteroid);
					UpdateUFO(UFO);
				}

				CollideBullet(bullet, asteroid, ship, ship2, PLAYER1, UFO, explosions, medAstImage, smallAstImage, expSound);
				
				if(multiplayer)
					CollideBullet(bullet, asteroid, ship, ship2, PLAYER2, UFO, explosions, medAstImage, smallAstImage, expSound);
								
				if(ship.tempInvinc == false)
				{
					CollideAsteroid(asteroid, ship, ship2, PLAYER1, explosions, medAstImage, smallAstImage, expSound);
					CollideUFO(UFO, ship, ship2, PLAYER1, explosions, expSound);
				}
				
				if(ship2.tempInvinc == false && multiplayer)
				{
					CollideAsteroid(asteroid, ship, ship2, PLAYER2, explosions, medAstImage, smallAstImage, expSound);
					CollideUFO(UFO, ship, ship2, PLAYER2, explosions, expSound);
				}


				if(CollidePowerUps(powerups, ship, ship2, PLAYER1, powerUpSound))
					al_play_sample(powerUpSound, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

				if(multiplayer)
				{
					if(CollidePowerUps(powerups, ship, ship2, PLAYER2, powerUpSound))
						al_play_sample(powerUpSound, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);	
				}

				//============================================================
				//UPDATING INVINCIBILITY COUNTERS AFTER LOSING A LIFE
				//============================================================

				if(ship.tempInvinc == true)
				{
					ship.tempInvFrames++;
					ship.tempInvCounter = ship.tempInvFrames/60;

					if(ship.tempInvFrames > 180)
					{
						ship.tempInvFrames = 0;
						ship.tempInvCounter = 0;
						ship.tempInvinc = false;
					}
				}

				if(ship2.tempInvinc == true)
				{
					ship2.tempInvFrames++;
					ship2.tempInvCounter = ship2.tempInvFrames/60;

					if(ship2.tempInvFrames > 180)
					{
						ship2.tempInvFrames = 0;
						ship2.tempInvCounter = 0;
						ship2.tempInvinc = false;
					}
				}

				//==============================================================
				//  UFO SPAWNING AND FIRING
				//==============================================================
				if((counter % 18) - 17 == 0 && UFO.live != true)
				{
					StartUFO(UFO);
					al_play_sample_instance(UFOsoundInst);
				}


				if(UFO.live == true && !ship.tempInvinc && !ship2.tempInvinc && !ship.timeWarp && !ship2.timeWarp && frames % (int)(70 + 200/(0.3 * counter)) == 0)
					FireBullet(bullet, sLaserImage, ship, ship2, ENEMY, UFO, laserSound, multiplayer);
				else if(UFO.live == false)
					al_stop_sample_instance(UFOsoundInst);

				//==================================
				//SPAWNING POWERUPS
				//==================================

				if(multiplayer && (frames % 1050) == 0)
					StartPowerUps(powerups, (rand() % 4));
				else if((frames % 1440) == 0)
					StartPowerUps(powerups, (rand() % 4));
			
			//=================================================
			//PLAYER1 SHIP MOVEMENT AND SHOOTING
			//=================================================

			if(ship2.timeWarp == false && ship.lives > 0)
			{
				if(keys[W])
					MoveShipForward(ship);
				else if(keys[S])
					MoveShipBack(ship);
				else
					DecelerateShip(ship);

				if(keys[A])
					ship.direction -= 0.05f;
				else if(keys[D])
					ship.direction += 0.05f;
			
				if(keys[SPACE])
				{
					if(ship.weapon == sLASER)
					{
						if(frames % 10 == 0)
							FireBullet(bullet, sLaserImage, ship, ship2, PLAYER1, UFO, laserSound, multiplayer);
					}
					if(ship.weapon == dLASER)
					{
						if(frames % 5 == 0)
							FireBullet(bullet, dLaserImage, ship, ship2, PLAYER1, UFO, laserSound, multiplayer);
					}
					if(ship.weapon == tLASER)
					{
						if(frames % 3 == 0)
							FireBullet(bullet, dLaserImage, ship, ship2, PLAYER1, UFO, laserSound, multiplayer);
					}
				}
			}

			//=================================================
			//PLAYER2 SHIP MOVEMENT AND SHOOTING
			//=================================================
				
			if(ship.timeWarp == false && multiplayer && ship2.lives > 0)
			{
				if(keys[UP])
					MoveShipForward(ship2);
				else if(keys[DOWN])
					MoveShipBack(ship2);
				else
					DecelerateShip(ship2);

				if(keys[LEFT])
					ship2.direction -= 0.05f;
				else if(keys[RIGHT])
					ship2.direction += 0.05f;
			
				if(keys[ENTER2])
				{
					if(ship2.weapon == sLASER)
					{
						if(frames % 10 == 0)
							FireBullet(bullet, sLaser2Image, ship, ship2, PLAYER2, UFO, laserSound, multiplayer);
					}
					if(ship2.weapon == dLASER)
					{
						if(frames % 5 == 0)
							FireBullet(bullet, dLaserImage, ship, ship2, PLAYER2, UFO, laserSound, multiplayer);
					}
					if(ship2.weapon == tLASER)
					{
						if(frames % 3 == 0)
							FireBullet(bullet, dLaserImage, ship, ship2, PLAYER2, UFO, laserSound, multiplayer);
					}
				}
			}

			if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				done = true;
			}
			}

		//===============================
		//RENDER
		//===============================
		if(redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			switch(state)
			{	
			case PLAYING:
				{
					al_draw_scaled_bitmap(bgImage, 0, 0, 1920, 1440, 0, 0, 1400, 1000, 0);

					if(ship.tempInvinc == true && ship.lives > 0)
					{
						al_draw_textf(font45, al_map_rgb(255,255,255), width/2, height/2 - 105, ALLEGRO_ALIGN_CENTRE, "Temporary invincibility for Player 1 for %i", (3 - ship.tempInvCounter));
					}

					if(ship2.tempInvinc == true && ship2.lives > 0)
					{
						al_draw_textf(font45, al_map_rgb(255,255,255), width/2, height/2 - 185, ALLEGRO_ALIGN_CENTRE, "Temporary invincibility for Player 2 for %i", (3 - ship2.tempInvCounter));
					}

					if(ship.lives>0)
					DrawShip(ship, ship2, PLAYER1);

					if(ship2.lives>0 && multiplayer == true)
					DrawShip(ship, ship2, PLAYER2);

					DrawBullet(bullet);
					asteroidsLive = DrawAsteroid(asteroid);
					DrawExplosions(explosions);
					DrawUFO(UFO, ship, multiplayer, ufoLaserImage);
					DrawPowerUps(powerups);
					al_draw_textf(font20, al_map_rgb(255,255,255), 20, 20, 0, "Player Lives:");
					al_draw_textf(font20, al_map_rgb(255,255,255), 20, 55, 0, "Player Score: %i", ship.score);
					al_draw_textf(font20, al_map_rgb(255,255,255), 20, 90, 0, "Player Time Warps: %i", ship.tWarp);

					if(multiplayer)
					{
						al_draw_textf(font20, al_map_rgb(255,255,255), 20, height - 42, 0, "Player Lives:");
						al_draw_textf(font20, al_map_rgb(255,255,255), 20, height - 75, 0, "Player Score: %i", ship2.score);
						al_draw_textf(font20, al_map_rgb(255,255,255), 20, height - 110, 0, "Player Time Warps: %i", ship2.tWarp);
					}
				}
				break;
			case START_MENU: al_draw_bitmap(startMenuImage, 0, 0, 0);
				break;
			case CREDITS: al_draw_bitmap(creditsImage, 0, 0, 0);
				break;
			case HI_SCORES:
				{
					al_draw_bitmap(hiScoresImage, 0, 0, 0);

					for(unsigned int i = 0; i < hiScores.size() && i < 10; i++)
					{
						al_draw_textf(font68, al_map_rgb(255, 255, 255), 50, 190 + (i * 70), ALLEGRO_ALIGN_LEFT, "%s", hiScores[i].c_str(), "\n");
					}

					for(unsigned int i = 10; i < hiScores.size() && i < 20; i++)
					{
						al_draw_textf(font68, al_map_rgb(255, 255, 255), width / 2, 190 + ((i - 10) * 70), ALLEGRO_ALIGN_LEFT, "%s", hiScores[i].c_str(), "\n");
					}
				}
				break;
			case EXIT_MENU:
				{
					al_draw_scaled_bitmap(bgImage, 0, 0, 1920, 1440, 0, 0, 1400, 1000, 0);
					al_draw_bitmap(exitMenuImage, width/2 - (al_get_bitmap_width(pauseMenuImage)/2), height/2 - (al_get_bitmap_height(pauseMenuImage)/2), 0);
				}
				break;
			case PAUSE_MENU:
				{
					al_draw_scaled_bitmap(bgImage, 0, 0, 1920, 1440, 0, 0, 1400, 1000, 0);
					al_draw_bitmap(pauseMenuImage, width/2 - (al_get_bitmap_width(pauseMenuImage)/2), height/2 - (al_get_bitmap_height(pauseMenuImage)/2), 0);
				}
				break;
			case INSTRUCTIONS: al_draw_bitmap(instructionsImage, 0, 0, 0);
				break;
			case GETTING_SCORE:
				{
					al_draw_bitmap(gameOverImage, 0, 0, 0);
					al_draw_textf(font38, al_map_rgb(255,255,255), 30, 30, 0, "Please enter your name:");
					al_draw_textf(font38, al_map_rgb(255,255,255), 30, 80, 0, "Player 1: %s", Player1.c_str());

					if(getP1name == false)
						al_draw_textf(font38, al_map_rgb(255,255,255), 30, 130, 0, "Player 2: %s", Player2.c_str());
				}
				break;
			case GAMEOVER:
				{
					al_draw_bitmap(gameOverImage, 0, 0, 0);
					al_draw_textf(font38, al_map_rgb(255,255,255), width - 30, 30, ALLEGRO_ALIGN_RIGHT, "(ENTER) Start 1P Game");
					al_draw_textf(font38, al_map_rgb(255,255,255), width - 30, 80, ALLEGRO_ALIGN_RIGHT, "(M) Start 2P Game");
					al_draw_textf(font38, al_map_rgb(255,255,255), width - 30, 130, ALLEGRO_ALIGN_RIGHT, "(ESCAPE) Main Menu");
					al_draw_textf(font38, al_map_rgb(255,255,255), 30, 30, 0, "%s's score: %i", Player1.c_str(), ship.score);

					if(multiplayer)
						al_draw_textf(font38, al_map_rgb(255,255,255), 30, 80, 0, "%s'score: %i", Player2.c_str(), ship2.score);

					if(multiplayer && ship.score > ship2.score)
						al_draw_textf(font68, al_map_rgb(255,255,255), width/2, height /2 - 150, ALLEGRO_ALIGN_CENTRE, "%s won! Time for a victory lap!", Player1.c_str());
					else if (multiplayer && ship.score < ship2.score)
						al_draw_textf(font68, al_map_rgb(255,255,255), width/2, height / 2 - 150, ALLEGRO_ALIGN_CENTRE, "%s won! Time for a victory lap!", Player2.c_str());
					else
						al_draw_textf(font68, al_map_rgb(255,255,255), width/2, height / 2 - 150, ALLEGRO_ALIGN_CENTRE, "Better luck next time...");
				}
				break;
			}
			
			//===================================
			//FLIP BUFFERS
			//===================================
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}
	}

	//==================================
	//UPDATE HI SCORE FILE
	//==================================

	ofstream outfile;

	outfile.open("HiScores.txt", ios_base::out | ios_base::trunc);
	
	for(unsigned int i = 0; i < hiScores.size(); i++)
	{
		outfile << hiScores[i] << "\n";
	}

	outfile.close();
	
	//===============================
	//DESTROY PROJECT OBJECTS
	//===============================
	al_destroy_bitmap(shipImage);
	al_destroy_bitmap(ship2Image);
	al_destroy_bitmap(largeAstImage);
	al_destroy_bitmap(medAstImage);
	al_destroy_bitmap(smallAstImage);
	al_destroy_bitmap(sLaserImage);
	al_destroy_bitmap(sLaser2Image);
	al_destroy_bitmap(dLaserImage);
	al_destroy_bitmap(expImage);
	al_destroy_bitmap(startMenuImage);
	al_destroy_bitmap(pauseMenuImage);
	al_destroy_bitmap(exitMenuImage);
	al_destroy_bitmap(instructionsImage);
	al_destroy_bitmap(creditsImage);
	al_destroy_bitmap(hiScoresImage);
	al_destroy_bitmap(gameOverImage);
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(ufoShipImage);
	al_destroy_bitmap(ufoLaserImage);
	al_destroy_bitmap(powerUp1);
	al_destroy_bitmap(powerUp2);
	al_destroy_bitmap(powerUp3);
	al_destroy_bitmap(powerUp4);
	al_destroy_sample(menuMusic);
	al_destroy_sample(laserSound);
	al_destroy_sample(expSound);
	al_destroy_sample(ufoSound);
	al_destroy_sample(powerUpSound);
	al_destroy_sample(timeWarpBegin);
	al_destroy_sample(timeWarpEnd);
	al_destroy_sample(BGmusic);
	al_destroy_sample_instance(UFOsoundInst);
	al_destroy_sample_instance(BGmusicInst);
	al_destroy_sample_instance(menuMusicInst);

	//===============================
	//DESTROY SHELL OBJECTS
	//===============================
	al_destroy_font(font20);
	al_destroy_font(font44);
	al_destroy_font(font45);
	al_destroy_font(font68);
	al_destroy_font(font38);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_timer(timer);
}

//This function is for changing game state with pre and post processes, however
//I was not able to utilise them for this project
void ChangeState(int &state, int newState)
{
	state = newState;
}

//SHIP FUNCTIONS
void InitShip(SpaceShip &ship, SpaceShip &ship2, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *image2)
{
		ship.ID = PLAYER1;
		ship.x = width/2 - 200;
		ship.y = height/2;
		ship.boundx = 20;
		ship.boundy = 20;
		ship.score = 0;
		ship.lives = 4;
		ship.weapon = sLASER;
		ship.tWarp = 0;
		ship.gun = 0;
		ship.tempInvinc = false;
		ship.tempInvFrames = 0;
		ship.timeWarp = false;
		ship.timeWarpFrames = 0;
		ship.weapFrames = 0;

		ship.direction = -(PI/2);
		ship.velocityX = 0;
		ship.velocityY = 0;
		ship.maxVelocity = 4.5;
		ship.totalVelocity = 0;
		ship.acceleration = 5;
		ship.time = 1/FPS;
	
		ship.frameWidth = al_get_bitmap_width(image);
		ship.frameHeight = al_get_bitmap_height(image);

		ship.image = image;


		ship2.ID = PLAYER2;
		ship2.x = width/2 + 200;
		ship2.y = height/2;
		ship2.boundx = 20;
		ship2.boundy = 20;
		ship2.score = 0;
		ship2.lives = 4;
		ship2.weapon = sLASER;
		ship2.tWarp = 0;
		ship2.gun = 0;
		ship2.tempInvinc = false;
		ship2.tempInvFrames = 0;
		ship2.timeWarp = false;
		ship2.timeWarpFrames = 0;
		ship2.weapFrames = 0;

		ship2.direction = -(PI/2);
		ship2.velocityX = 0;
		ship2.velocityY = 0;
		ship2.maxVelocity = 4.5;
		ship2.totalVelocity = 0;
		ship2.acceleration = 5;
		ship2.time = 1/FPS;
	
		ship2.frameWidth = al_get_bitmap_width(image2);
		ship2.frameHeight = al_get_bitmap_height(image2);

		ship2.image = image2;
	
}
void DrawShip(SpaceShip &ship, SpaceShip &ship2, int ID)
{
	SpaceShip *pShip = 0;

	if(ID == PLAYER1 && ship.lives>0)
	{
		pShip = &ship;
	}
	else if(ID == PLAYER2 && ship2.lives>0)
	{
		pShip = &ship2;
	}

	if(pShip != 0)
	{
		pShip->x += pShip->velocityX;
		pShip->y += pShip->velocityY;

		al_draw_rotated_bitmap(pShip->image, pShip->frameWidth / 2, pShip->frameHeight / 2, pShip->x, pShip->y,  pShip->direction, 0);

		if((keys[W] && ID == PLAYER1) || (keys[UP] && ID == PLAYER2))
		{
			al_draw_filled_circle(((pShip->x - (cos(pShip->direction) * 29) + (rand() % 10 - 5))) , (pShip->y - (sin(pShip->direction) * 29) + (rand() % 10 - 6)), (rand() % 4 + 1), al_map_rgba(0,100,250, (rand() % 101)));
		}

		int fx;

		if(ID == PLAYER1)
			fx = 10;
		else
			fx = height - 50;

		for(int i = 0; i < pShip->lives; i++)
		{
			al_draw_tinted_scaled_rotated_bitmap_region(pShip->image, 0, 0, pShip->frameWidth, pShip->frameHeight, al_map_rgba(255,255,255,0), 0, 0, 112 + ((i * pShip->frameWidth) + 5), fx, 0.7, 0.7, 0, 0);
		}

	}
}
void MoveShipForward(SpaceShip &ship)
{
	ship.velocityX += cos(ship.direction) * ship.acceleration * ship.time;
	ship.velocityY += sin(ship.direction) * ship.acceleration * ship.time;
	
	ship.totalVelocity = sqrt((ship.velocityX * ship.velocityX) + (ship.velocityY * ship.velocityY));

	if(ship.totalVelocity > ship.maxVelocity)
	{
		ship.velocityX *= ship.maxVelocity / ship.totalVelocity;
		ship.velocityY *= ship.maxVelocity / ship.totalVelocity;
	}
}
void MoveShipBack(SpaceShip &ship)
{
	ship.velocityX += -cos(ship.direction) * ship.acceleration * ship.time;
	ship.velocityY += -sin(ship.direction) * ship.acceleration * ship.time;
	
	ship.totalVelocity = sqrt((ship.velocityX * ship.velocityX) + (ship.velocityY * ship.velocityY));

	if(ship.totalVelocity > ship.maxVelocity)
	{
		ship.velocityX *= ship.maxVelocity / ship.totalVelocity;
		ship.velocityY *= ship.maxVelocity / ship.totalVelocity;
	}
}
void DecelerateShip(SpaceShip &ship)
{
	if(ship.velocityX > 1.10 || ship.velocityX < -1.10)
	ship.velocityX *= 0.975;
	if(ship.velocityY > 1.10 || ship.velocityY < -1.10)
	ship.velocityY *= 0.975;
}

//BULLET FUNCTIONS
void InitBullet(Bullet bullet[])
{
	for(int i = 0; i < NUM_BULLETS; i++)
	{
		bullet[i].velocity = 10;
		bullet[i].vectorX = cos(bullet[i].angle);
		bullet[i].vectorY = sin(bullet[i].angle);
		bullet[i].distTravelled = 0;
		bullet[i].live = false;
	}
}
void DrawBullet(Bullet bullet[])
{
	for(int i = 0; i < NUM_BULLETS; i++)
	{
		if(bullet[i].live)
		{
			bullet[i].x += bullet[i].vectorX * bullet[i].velocity;
			bullet[i].y += bullet[i].vectorY * bullet[i].velocity;

			al_draw_rotated_bitmap(bullet[i].image, 5, 3, bullet[i].x, bullet[i].y, bullet[i].angle, 0);
		}
	}
}
void FireBullet(Bullet bullet[], ALLEGRO_BITMAP *image, SpaceShip &ship, SpaceShip &ship2, int ID, Enemy &UFO, ALLEGRO_SAMPLE *sample, bool multiplayer)
{
	for(int i = 0; i < NUM_BULLETS; i++)
	{
		if(!bullet[i].live)
		{
			if(ID == PLAYER1)
				{
				bullet[i].ID = ID;
				bullet[i].live = true;
				bullet[i].distTravelled = 0;
				al_play_sample(sample, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, (NULL));

				if(ship.weapon == sLASER)
				{
					bullet[i].image = image;
					bullet[i].x = ship.x + (cos(ship.direction) * 10);
					bullet[i].y = ship.y + (sin(ship.direction) * 10);			
					bullet[i].angle = ship.direction;
					bullet[i].vectorX = cos(bullet[i].angle);
					bullet[i].vectorY = sin(bullet[i].angle);
					break;
				}

				if(ship.weapon == dLASER)
				{					
					if(ship.gun == 0)
					{
					bullet[i].x = ship.x + (cos(ship.direction + 0.935) * 11);
					bullet[i].y = ship.y - (sin(ship.direction + 0.935) * 11);
					ship.gun = 1;
					}
					else
					{
						bullet[i].x = ship.x + (cos(ship.direction - 0.935) * 11);
						bullet[i].y = ship.y - (sin(ship.direction - 0.935) * 11);
						ship.gun = 0;
					}
					
					
					bullet[i].angle = ship.direction;
					bullet[i].distTravelled = 0;
					bullet[i].vectorX = cos(bullet[i].angle);
					bullet[i].vectorY = sin(bullet[i].angle);
					bullet[i].image = image;
					break;
				}

				if(ship.weapon == tLASER)
				{
					if(ship.gun == 0)
					{
						bullet[i].x = ship.x + (cos(ship.direction + 0.935) * 11);
						bullet[i].y = ship.y - (sin(ship.direction + 0.935) * 11);
						bullet[i].angle = rand() % 6 - PI;
						ship.gun = 1;
					}

					else if( ship.gun == 1)
					{
						bullet[i].x = ship.x + (cos(ship.direction) * 10);
						bullet[i].y = ship.y + (sin(ship.direction) * 10);
						ship.gun = 2;
					}

					else if (ship.gun == 2)
					{
						bullet[i].x = ship.x + (cos(ship.direction - 0.935) * 11);
						bullet[i].y = ship.y - (sin(ship.direction - 0.935) * 11);
						bullet[i].angle = rand() % 6 - PI;
						ship.gun = 3;
					}

					else if (ship.gun == 3)
					{
						bullet[i].x = ship.x - (cos(ship.direction) * 10);
						bullet[i].y = ship.y - (sin(ship.direction) * 10);
						bullet[i].angle = rand() % 6 - PI;
						ship.gun = 0;
					}
					
					bullet[i].vectorX = cos(bullet[i].angle);
					bullet[i].vectorY = sin(bullet[i].angle);
					bullet[i].image = image;
					bullet[i].ID = PLAYER1;
					break;
				}
			}
			
			else if(ID == PLAYER2)
			{
				bullet[i].ID = ID;
				bullet[i].live = true;
				bullet[i].distTravelled = 0;
				al_play_sample(sample, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, (NULL));
				
				if(ship2.weapon == sLASER)
				{
					bullet[i].x = ship2.x + (cos(ship2.direction) * 10);
					bullet[i].y = ship2.y + (sin(ship2.direction) * 10);
					bullet[i].angle = ship2.direction;
					bullet[i].vectorX = cos(bullet[i].angle);
					bullet[i].vectorY = sin(bullet[i].angle);
					bullet[i].image = image;
					break;
				}

				if(ship2.weapon == dLASER)
				{
					bullet[i].ID = PLAYER2;
					
					if(ship2.gun == 0)
					{
					bullet[i].x = ship2.x + (cos(ship2.direction + 0.935) * 11);
					bullet[i].y = ship2.y - (sin(ship2.direction + 0.935) * 11);
					ship2.gun = 1;
					}
					else
					{
						bullet[i].x = ship2.x + (cos(ship2.direction - 0.935) * 11);
						bullet[i].y = ship2.y - (sin(ship2.direction - 0.935) * 11);
						ship2.gun = 0;
					}
					
					bullet[i].angle = ship2.direction;
					bullet[i].vectorX = cos(bullet[i].angle);
					bullet[i].vectorY = sin(bullet[i].angle);
					bullet[i].image = image;
					break;
				}

				if(ship2.weapon == tLASER)
				{
					bullet[i].ID = PLAYER2;

					if(ship2.gun == 0)
					{
						bullet[i].x = ship2.x + (cos(ship2.direction + 0.935) * 11);
						bullet[i].y = ship2.y - (sin(ship2.direction + 0.935) * 11);
						bullet[i].angle = rand() % 6 - PI;
						ship2.gun = 1;
					}

					else if( ship2.gun == 1)
					{
						bullet[i].x = ship2.x + (cos(ship2.direction) * 10);
						bullet[i].y = ship2.y + (sin(ship2.direction) * 10);
						ship2.gun = 2;
					}

					else if (ship2.gun == 2)
					{
						bullet[i].x = ship2.x + (cos(ship2.direction - 0.935) * 11);
						bullet[i].y = ship2.y - (sin(ship2.direction - 0.935) * 11);
						bullet[i].angle = rand() % 6 - PI;
						ship2.gun = 3;
					}

					else if (ship2.gun == 3)
					{
						bullet[i].x = ship2.x - (cos(ship2.direction) * 10);
						bullet[i].y = ship2.y - (sin(ship2.direction) * 10);
						bullet[i].angle = rand() % 6 - PI;
						ship2.gun = 0;
					}
					
					bullet[i].vectorX = cos(bullet[i].angle);
					bullet[i].vectorY = sin(bullet[i].angle);
					bullet[i].image = image;
					break;
				}
				}
		
			if(ID == ENEMY)
			{
				bullet[i].ID = ENEMY;
				bullet[i].x = UFO.x;
				bullet[i].y = UFO.y;
				bullet[i].live = true;
				bullet[i].distTravelled = 0;

				if((rand() % 2) && ship.lives>0)
					bullet[i].angle = atan2(ship.y - UFO.y, ship.x - UFO.x);
				else if ((rand() % 2) != 0 && ship2.lives>0 && multiplayer)
					bullet[i].angle = atan2(ship2.y - UFO.y, ship2.x - UFO.x);

				bullet[i].vectorX = cos(bullet[i].angle);
				bullet[i].vectorY = sin(bullet[i].angle);
				bullet[i].velocity = 5.0;
				bullet[i].image = image;
				break;
			}
		}
	}
}
void UpdateBullet(Bullet bullet[])
{
	for(int i = 0; i < NUM_BULLETS; i++)
	{
		if(bullet[i].live)
		{
			bullet[i].x += bullet[i].vectorX * bullet[i].velocity;
			bullet[i].y += bullet[i].vectorY * bullet[i].velocity;
			bullet[i].distTravelled += sqrt((pow(bullet[i].vectorX, 2)) * (pow(bullet[i].velocity, 2)) + (pow(bullet[i].vectorY, 2)) * (pow(bullet[i].velocity, 2)));

			if(bullet[i].distTravelled > 400)
				bullet[i].live = false;
		}
	}
}
void CollideBullet(Bullet bullet[], Asteroid asteroid[], SpaceShip &ship, SpaceShip &ship2, int ID, Enemy &UFO, Explosion explosions[], ALLEGRO_BITMAP *medAstImage, ALLEGRO_BITMAP *smallAstImage, ALLEGRO_SAMPLE *expSound)
{
	SpaceShip *pShip = 0;

	if(ID == PLAYER1 && ship.lives>0)
	{
		pShip = &ship;
	}
	else if(ID == PLAYER2 && ship2.lives>0)
	{
		pShip = &ship2;
	}

	if(pShip != 0)
	{
		for(int i = 0; i < NUM_BULLETS; i++)
		{
			if(bullet[i].live && bullet[i].ID != ENEMY)
			{
				for(int j = 0; j < NUM_ASTEROIDS; j++)
				{
					if(asteroid[j].live)
					{
						if(bullet[i].x > (asteroid[j].x - asteroid[j].boundx) &&
						   bullet[i].x < (asteroid[j].x + asteroid[j].boundx) &&
						   bullet[i].y > (asteroid[j].y - asteroid[j].boundy) &&
						   bullet[i].y < (asteroid[j].y + asteroid[j].boundy))
						{
							bullet[i].live = false;
							asteroid[j].health--;

							if(asteroid[j].health == 0)
							{
								if(asteroid[j].size == 3)
								{
									StartExplosions(explosions, 3, bullet[i].x, bullet[i].y, expSound);
									StartAsteroid(asteroid, 2, asteroid[j].x, asteroid[j].y, medAstImage);
									asteroid[j].live = false;
								}
						
								else if(asteroid[j].size == 2)
								{
									StartExplosions(explosions, 2, bullet[i].x, bullet[i].y, expSound);
									StartAsteroid(asteroid, 1, asteroid[j].x, asteroid[j].y, smallAstImage);
									asteroid[j].live = false;
								}

								else if(asteroid[j].size == 1)
								{
									StartExplosions(explosions, 1, bullet[i].x, bullet[i].y, expSound);
									asteroid[j].live = false;
								}

									if(bullet[i].ID == PLAYER1)
										ship.score += 50;
									else
										ship2.score += 50;
							}
						}
					}
				}

				if(bullet[i].x > (UFO.x - UFO.boundx) &&
					bullet[i].x < (UFO.x + UFO.boundx) &&
					bullet[i].y > (UFO.y - UFO.boundy) &&
					bullet[i].y < (UFO.y + UFO.boundy) && UFO.live == true)
				{
					bullet[i].live = false;

					if(bullet[i].ID == PLAYER1)
						ship.score += 250;
					else
						ship2.score += 250;

					UFO.live = false;
					StartExplosions(explosions, 2, UFO.x, UFO.y, expSound);
				}
			}

			if(bullet[i].live && bullet[i].ID == ENEMY)
			{
				if(bullet[i].x > (pShip->x - pShip->boundx) &&
					bullet[i].x < (pShip->x + pShip->boundx) &&
					bullet[i].y > (pShip->y - pShip->boundy) &&
					bullet[i].y < (pShip->y + pShip->boundy))
				{
					bullet[i].live = false;
					pShip->lives--;
					pShip->tempInvinc = true;
					pShip->direction = -(PI/2);
					pShip->velocityX = 0;
					pShip->velocityY = 0;
					pShip->y = height/2;

					if(ID == PLAYER1)
						pShip->x = width/2 - 200;
					else
						pShip->x = width/2 + 200;	

					StartExplosions(explosions, 1, bullet[i].x, bullet[i].y, expSound);
				}
			}
		}
	}
}

//ASTEROID FUNCTIONS
void InitAsteroid(Asteroid asteroid[])
{
	for(int i=0; i < NUM_ASTEROIDS; i++)
	{
		asteroid[i].ID = ENEMY;
		asteroid[i].live = false;
		asteroid[i].velocity = 1.65;
		asteroid[i].boundx = 37;
		asteroid[i].boundy = 37;
		asteroid[i].size = 3;

		asteroid[i].maxFrame = 64;
		asteroid[i].curFrame = 0;
		asteroid[i].frameCount = 0;
		asteroid[i].frameDelay = 4;
		asteroid[i].frameWidth = 128;
		asteroid[i].frameHeight = 128;
		asteroid[i].animationColumn = 8;
		asteroid[i].animationDirection = -1;

		if(rand() % 2)
			asteroid[i].animationDirection = 1;
		else
			asteroid[i].animationDirection = -1;
	}
}
bool DrawAsteroid(Asteroid asteroid[])
{
	bool liveAsteroids = false;
	
	for(int i = 0; i < NUM_ASTEROIDS; i++)
	{
		if(asteroid[i].live)
		{
			int fx = (float)(asteroid[i].curFrame % asteroid[i].animationColumn) * asteroid[i].frameWidth;
			int fy = (asteroid[i].curFrame / asteroid[i].animationColumn) * asteroid[i].frameHeight;

			if(asteroid[i].size == 3)
			{
			al_draw_bitmap_region(asteroid[i].image, fx, fy, asteroid[i].frameWidth, asteroid[i].frameHeight, asteroid[i].x - asteroid[i].frameWidth / 2, asteroid[i].y - asteroid[i].frameHeight / 2, 0);
			liveAsteroids = true;
			}
			else if(asteroid[i].size == 2)
			{
			al_draw_bitmap_region(asteroid[i].image, fx, fy, asteroid[i].frameWidth, asteroid[i].frameHeight, asteroid[i].x - asteroid[i].frameWidth / 2, asteroid[i].y - asteroid[i].frameHeight / 2, 0);
			liveAsteroids = true;
			}
			else if(asteroid[i].size == 1)
			{
			al_draw_bitmap_region(asteroid[i].image, fx, fy, asteroid[i].frameWidth, asteroid[i].frameHeight, asteroid[i].x - asteroid[i].frameWidth / 2, asteroid[i].y - asteroid[i].frameHeight / 2, 0);
			liveAsteroids = true;
			}
		}
	}

	return liveAsteroids;
}
void StartAsteroid(Asteroid asteroid[], int aSize, float asteroidx, float asteroidy, ALLEGRO_BITMAP *image)
{
	int numAsteroidsSpawned = 0;

	for(int i = 0; i < NUM_ASTEROIDS; i++)
	{
		if(!asteroid[i].live)
		{
			if(aSize == 2 && numAsteroidsSpawned < 3)
			{
				asteroid[i].live = true;
				asteroid[i].x = asteroidx;
				asteroid[i].y = asteroidy;
				asteroid[i].size = 2;
				asteroid[i].angle = rand() % 6 - PI;
				asteroid[i].vectorX = cos(asteroid[i].angle);
				asteroid[i].vectorY = sin(asteroid[i].angle);
				asteroid[i].velocity = (rand() % 2) + 1.2;
				asteroid[i].frameDelay = 4;
				asteroid[i].boundx = 25;
				asteroid[i].boundy = 25;
				asteroid[i].frameWidth = 84.5;
				asteroid[i].frameHeight = 84.5;
				asteroid[i].health = 2;

				asteroid[i].image = image;

				numAsteroidsSpawned++;
			}

			if(aSize == 1 && numAsteroidsSpawned < 3)
			{
				asteroid[i].live = true;
				asteroid[i].x = asteroidx;
				asteroid[i].y = asteroidy;
				asteroid[i].size = 1;
				asteroid[i].angle = rand() % 6 - PI;
				asteroid[i].vectorX = cos(asteroid[i].angle);
				asteroid[i].vectorY = sin(asteroid[i].angle);
				asteroid[i].velocity = (rand() % 2) + 1.5;
				asteroid[i].frameDelay = 3;
				asteroid[i].boundx = 13;
				asteroid[i].boundy = 13;
				asteroid[i].frameWidth = 44.75;
				asteroid[i].frameHeight = 44.75;
				asteroid[i].health = 1;

				asteroid[i].image = image;

				numAsteroidsSpawned++;
			}		
			
			else
				{
					if(rand() % 15000 == 0 && numLargeAsteroids < 4 + (wave * 2))
					{
						asteroid[i].live = true;
						asteroid[i].x = width + asteroid[i].frameWidth;
						asteroid[i].y = 30 + rand() % (height - 60);
						asteroid[i].angle = rand() % 6 - PI;
						asteroid[i].vectorX = cos(asteroid[i].angle);
						asteroid[i].vectorY = sin(asteroid[i].angle);
						asteroid[i].velocity = (rand() % 2) + 1;
						asteroid[i].health = 3;
						asteroid[i].frameWidth = 128;
						asteroid[i].frameHeight = 128;
						asteroid[i].boundx = 37;
						asteroid[i].boundy = 37;

						asteroid[i].image = image;

						numLargeAsteroids++;
					}
				}
		}
	}
}
void UpdateAsteroid(Asteroid asteroid[])
{
	for(int i = 0; i < NUM_ASTEROIDS; i++)
	{
		if(asteroid[i].live)
		{
			if(++asteroid[i].frameCount >= asteroid[i].frameDelay)
			{
				asteroid[i].curFrame += asteroid[i].animationDirection;
				if (asteroid[i].curFrame >= asteroid[i].maxFrame)
					asteroid[i].curFrame = 0;
				else if(asteroid[i].curFrame <= 0)
					asteroid[i].curFrame = asteroid[i].maxFrame - 1;

				asteroid[i].frameCount = 0;
			}
		}
			asteroid[i].x += asteroid[i].vectorX * asteroid[i].velocity;
			asteroid[i].y += asteroid[i].vectorY * asteroid[i].velocity;	
	}
}
void CollideAsteroid(Asteroid asteroid[], SpaceShip &ship, SpaceShip &ship2, int ID, Explosion explosions[], ALLEGRO_BITMAP *medAstImage, ALLEGRO_BITMAP *smallAstImage, ALLEGRO_SAMPLE *expSound)
{
	for (int i = 0; i < NUM_ASTEROIDS; i++)
	{
		if(asteroid[i].live)
		{
			SpaceShip *pShip = 0;

			if(ID == PLAYER1 && ship.lives>0)
			{
				pShip = &ship;
			}
			else if(ID == PLAYER2 && ship2.lives>0)
			{
				pShip = &ship2;
			}

			if(pShip != 0)
			{
				if(asteroid[i].x - asteroid[i].boundx < pShip->x + pShip->boundx &&
					asteroid[i].x + asteroid[i].boundx > pShip->x - pShip->boundx &&
					asteroid[i].y - asteroid[i].boundy < pShip->y + pShip->boundy &&
					asteroid[i].y + asteroid[i].boundy > pShip->y - pShip->boundy)
				{

					if(asteroid[i].size == 3)
					{
						StartExplosions(explosions, 3, asteroid[i].x, asteroid[i].y, expSound);
						StartExplosions(explosions, 2, pShip->x, pShip->y, expSound);
						StartAsteroid(asteroid, 2, asteroid[i].x, asteroid[i].y, medAstImage);
					}
						else if(asteroid[i].size == 2)
					{
						StartExplosions(explosions, 2, asteroid[i].x, asteroid[i].y, expSound);
						StartExplosions(explosions, 2, pShip->x, pShip->y, expSound);
						StartAsteroid(asteroid, 1, asteroid[i].x, asteroid[i].y, smallAstImage);
					}

					else if(asteroid[i].size == 1)
					{
						StartExplosions(explosions, 1, pShip->x, pShip->y, expSound);
						StartExplosions(explosions, 2, pShip->x, pShip->y, expSound);
					}

					asteroid[i].live = false;
					pShip->lives--;
					pShip->tempInvinc = true;
					pShip->score += 50;
					pShip->direction = -(PI/2);
					pShip->velocityX = 0;
					pShip->velocityY = 0;
					pShip->y = height/2;

					if(ID == PLAYER1)
						pShip->x = width/2 - 200;
					else
						pShip->x = width/2 + 200;			
				}
			}
		}
	}
}

//EXPLOSION FUNCTIONS
void InitExplosions(Explosion explosion[], ALLEGRO_BITMAP *image)
{
	for(int i = 0; i < NUM_EXPLOSIONS; i++)
	{
		explosion[i].live = false;

		explosion[i].maxFrame = 48;
		explosion[i].curFrame = 0;
		explosion[i].frameCount = 0;
		explosion[i].frameDelay = 1;
		explosion[i].frameWidth = 256;
		explosion[i].frameHeight = 256;
		explosion[i].animationColumn = 8;
		explosion[i].animationDirection = 1;

		if(image!=NULL)
			explosion[i].image = image;
		
	}
}
void DrawExplosions(Explosion explosion[])
{
	for(int i = 0; i < NUM_EXPLOSIONS; i++)
	{
		if(explosion[i].live)
		{
			int fx = (explosion[i].curFrame % explosion[i].animationColumn) * explosion[i].frameWidth;
			int fy = (explosion[i].curFrame / explosion[i].animationColumn) * explosion[i].frameHeight;

			if(explosion[i].size == 3)
				al_draw_tinted_scaled_rotated_bitmap_region(explosion[i].image, fx, fy, explosion[i].frameWidth, explosion[i].frameHeight, al_map_rgba(255,255,255,100), 0, 0, explosion[i].x - explosion[i].frameWidth / 2, explosion[i].y - explosion[i].frameHeight / 2, explosion[i].size / 5, explosion[i].size / 5, 0, 0);

			if(explosion[i].size == 2)
				al_draw_tinted_scaled_rotated_bitmap_region(explosion[i].image, fx, fy, explosion[i].frameWidth, explosion[i].frameHeight, al_map_rgba(255,255,255,100), 0, 0, explosion[i].x - explosion[i].frameWidth / 3.4, explosion[i].y - explosion[i].frameHeight / 3.2, explosion[i].size / 4, explosion[i].size / 4, 0, 0);

			if(explosion[i].size == 1)
				al_draw_tinted_scaled_rotated_bitmap_region(explosion[i].image, fx, fy, explosion[i].frameWidth, explosion[i].frameHeight, al_map_rgba(255,255,255,100), 0, 0, explosion[i].x - explosion[i].frameWidth / 6, explosion[i].y - explosion[i].frameHeight / 4.80, explosion[i].size / 4, explosion[i].size / 4, 0, 0);
		}
	}
}
void StartExplosions(Explosion explosion[], int eSize, float x, float y, ALLEGRO_SAMPLE *expSound)
{
	for(int i = 0; i < NUM_EXPLOSIONS; i++)
	{
		if(!explosion[i].live)
		{
			explosion[i].live = true;
			explosion[i].x = x;
			explosion[i].y = y;
			explosion[i].size = eSize;
			break;
		}
	}

	switch(eSize)
	{
		case 1: al_play_sample(expSound, 0.6, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			break;
		case 2: al_play_sample(expSound, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			break;
		case 3: al_play_sample(expSound, 0.8, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			break;
	}
}
void UpdateExplosions(Explosion explosion[])
{
	for(int i = 0; i < NUM_EXPLOSIONS; i++)
	{
		if(explosion[i].live)
		{
			if(++explosion[i].frameCount >= explosion[i].frameDelay)
			{
				explosion[i].curFrame += explosion[i].animationDirection;

				if (explosion[i].curFrame >= explosion[i].maxFrame)
				{
					explosion[i].curFrame = 0;
					explosion[i].live = false;
				}

				explosion[i].frameCount = 0;
			}
		}
	}				
}

//UFO FUNCTIONS
void InitUFO(Enemy &UFO, ALLEGRO_BITMAP *image)
{
	UFO.ID = ENEMY;
	UFO.x = width + 100;
	UFO.y = 30 + rand() % (height - 60);
	UFO.boundx = 28;
	UFO.boundy = 11;
	UFO.vectorX = 0;
	UFO.vectorY = 0;
	UFO.angle = -PI;
	UFO.live = false;
	UFO.velocity = 2.25;
	UFO.frameWidth = 64;
	UFO.frameHeight = 25;

	if(image!=NULL)
		UFO.image = image;
}
void DrawUFO(Enemy &UFO, SpaceShip &ship, bool multiplayer, ALLEGRO_BITMAP *ufoLaserImage)
{
	if(UFO.live == true)
	{
		al_draw_bitmap(UFO.image, UFO.x - UFO.frameWidth/2, UFO.y - UFO.frameHeight/2, 0);

		if(multiplayer == false)
			al_draw_rotated_bitmap(ufoLaserImage, 1, 5, UFO.x, UFO.y, atan2(ship.y - UFO.y, ship.x - UFO.x), 0);
	}
}
void UpdateUFO(Enemy &UFO)
{
	UFO.vectorX = cos(UFO.angle);
	UFO.vectorY = sin(UFO.angle);
		
	UFO.x += UFO.vectorX * UFO.velocity;
	UFO.y += UFO.vectorY * UFO.velocity;
	UFO.distTravelled += sqrt((pow(UFO.vectorX, 2)) * (pow(UFO.velocity, 2)) + (pow(UFO.vectorY, 2)) * (pow(UFO.velocity, 2)));

	if(UFO.distTravelled > 325)
	{
		if(rand() % 2 == 0)
			UFO.angle += 0.425;
		else
			UFO.angle -= 0.425;

		UFO.distTravelled = 0;
	}
}
void StartUFO(Enemy &UFO)
{
	UFO.x = width + 100;
	UFO.y = 30 + rand() % (height - 60);
	UFO.distTravelled = 0;
	UFO.live = true;

	if(rand() % 2)
		UFO.angle = 0;
	else
		UFO.angle = PI;

	UFO.vectorX = cos(UFO.angle);
	UFO.vectorY = sin(UFO.angle);
}
void CollideUFO(Enemy &UFO, SpaceShip &ship, SpaceShip &ship2, int ID, Explosion explosions[], ALLEGRO_SAMPLE *expSound)
{
	SpaceShip *pShip = 0;

	if(ID == PLAYER1 && ship.lives>0)
	{
		pShip = &ship;
	}
	else if(ID == PLAYER2 && ship2.lives>0)
	{
		pShip = &ship2;
	}

	if(pShip != 0)
	{
		if(UFO.x - UFO.boundx < pShip->x + pShip->boundx &&
			UFO.x + UFO.boundx > pShip->x - pShip->boundx &&
			UFO.y - UFO.boundy < pShip->y + pShip->boundy &&
			UFO.y + UFO.boundy > pShip->y - pShip->boundy && UFO.live == true)
		{
			UFO.live = false;
			pShip->lives--;
			pShip->tempInvinc = true;
			pShip->score += 250;
			pShip->direction = -(PI/2);
			pShip->velocityX = 0;
			pShip->velocityY = 0;
			pShip->y = height/2;

			if(ID == PLAYER1)
				pShip->x = width/2 - 200;
			else
				pShip->x = width/2 + 200;	

			StartExplosions(explosions, 2, pShip->x, pShip->y, expSound);
			StartExplosions(explosions, 2, UFO.x, UFO.y, expSound);
		}
	}
}

//POWERUP FUNCTIONS
void InitPowerUps(PowerUp powerups[], ALLEGRO_BITMAP *powerUp1, ALLEGRO_BITMAP *powerUp2, ALLEGRO_BITMAP *powerUp3, ALLEGRO_BITMAP *powerUp4)
{
		powerups[0].ID = POWERUP;
		powerups[0].type = LIFE;
		powerups[0].x = width + 100;
		powerups[0].y = 30 + rand() % (height - 60);
		powerups[0].boundx = 20;
		powerups[0].boundy = 20;
		powerups[0].frameWidth = 40;
		powerups[0].frameHeight = 40;
		powerups[0].velocity = 2;
		powerups[0].live = false;
		powerups[0].angle = 0;
		powerups[0].distTravelled = 0;

		if(powerUp1 != NULL)
		powerups[0].image = powerUp1;

		powerups[1].ID = POWERUP;
		powerups[1].type = DLASER;
		powerups[1].x = width + 100;
		powerups[1].y = 30 + rand() % (height - 60);
		powerups[1].boundx = 18;
		powerups[1].boundy = 18;
		powerups[1].frameWidth = 40;
		powerups[1].frameHeight = 40;
		powerups[1].velocity = 2;
		powerups[1].live = false;
		powerups[1].angle = 0;
		powerups[1].distTravelled = 0;

		if(powerUp2 != NULL)
		powerups[1].image = powerUp2;

		powerups[2].ID = POWERUP;
		powerups[2].type = TLASER;
		powerups[2].x = width + 100;
		powerups[2].y = 30 + rand() % (height - 60);
		powerups[2].boundx = 18;
		powerups[2].boundy = 18;
		powerups[2].frameWidth = 40;
		powerups[2].frameHeight = 40;
		powerups[2].velocity = 2;
		powerups[2].live = false;
		powerups[2].angle = 0;
		powerups[2].distTravelled = 0;

		if(powerUp3 != NULL)
		powerups[2].image = powerUp3;

		powerups[3].ID = POWERUP;
		powerups[3].type = TIMEWARP;
		powerups[3].x = width + 100;
		powerups[3].y = 30 + rand() % (height - 60);
		powerups[3].boundx = 18;
		powerups[3].boundy = 18;
		powerups[3].frameWidth = 40;
		powerups[3].frameHeight = 40;
		powerups[3].velocity = 2;
		powerups[3].live = false;
		powerups[3].angle = 0;
		powerups[3].distTravelled = 0;

		if(powerUp3 != NULL)
		powerups[3].image = powerUp4;
	
}
void DrawPowerUps(PowerUp powerups[])
{
	for(int i = 0; i < NUM_POWERUPS; i++)
	{
		if(powerups[i].live)
		{
			powerups[i].vectorX = cos(powerups[i].angle);
			powerups[i].vectorY = sin(powerups[i].angle);
		
			powerups[i].x += powerups[i].vectorX * powerups[i].velocity;
			powerups[i].y += powerups[i].vectorY * powerups[i].velocity;
			powerups[i].distTravelled += sqrt((pow(powerups[i].vectorX, 2)) * (pow(powerups[i].velocity, 2)) + (pow(powerups[i].vectorY, 2)) * (pow(powerups[i].velocity, 2)));

			if(powerups[i].distTravelled > 325)
			{
				if(rand() % 2 == 0)
					powerups[i].angle += 0.3425;
				else
					powerups[i].angle -= 0.3425;
			
				powerups[i].distTravelled = 0;

			}

			al_draw_bitmap(powerups[i].image, powerups[i].x - powerups[i].frameWidth / 1.65, powerups[i].y - powerups[i].frameHeight / 1.7, 0);
		}
	}
}
void StartPowerUps(PowerUp powerups[], int type)
{
	powerups[type].x = width + 100;
	powerups[type].y = 30 + rand() % (height - 60);
	powerups[type].live = true;

	if(rand() % 2)
		powerups[type].angle = 0;
	else
		powerups[type].angle = PI;

	powerups[type].vectorX = cos(powerups[type].angle);
	powerups[type].vectorY = sin(powerups[type].angle);
}
bool CollidePowerUps(PowerUp powerups[], SpaceShip &ship, SpaceShip &ship2, int ID, ALLEGRO_SAMPLE *sample)
{
	bool powerUpHit = false;

	SpaceShip *pShip = 0;

	if(ID == PLAYER1 && ship.lives>0)
	{
		pShip = &ship;
	}
	else if(ID == PLAYER2 && ship2.lives>0)
	{
		pShip = &ship2;
	}

	if(pShip != 0)
	{
		for(int i = 0; i < 4; i++)
		{
			if(powerups[i].live == true)
			{
				if(powerups[i].x - powerups[i].boundx < pShip->x + pShip->boundx &&
					powerups[i].x + powerups[i].boundx > pShip->x - pShip->boundx &&
					powerups[i].y - powerups[i].boundy < pShip->y + pShip->boundy &&
					powerups[i].y + powerups[i].boundy > pShip->y - pShip->boundy && pShip->lives>0)
				{
			
					if(powerups[i].type == LIFE)
					{
						powerups[i].live = false;
						pShip->lives++;
					}
			
					if(powerups[i].type == DLASER)
					{
						powerups[i].live = false;
						pShip->weapon = dLASER;
						pShip->weapFrames = 0;
					}

					if(powerups[i].type == TLASER)
					{
						powerups[i].live = false;
						pShip->weapon = tLASER;
						pShip->weapFrames = 0;
					}

					if(powerups[i].type == TIMEWARP)
					{
						powerups[i].live = false;
						pShip->tWarp++;
					}

					powerUpHit = true;
				}
			}
		}
	}

	return powerUpHit;
}

void LimitCheck(SpaceShip &ship, SpaceShip &ship2, Asteroid asteroid[], Bullet bullet[], Enemy &UFO, PowerUp powerups[])
{
	if(ship.y < 0)
		ship.y = height;
	if(ship.y > height)
		ship.y = 0;
	if(ship.x > width)
		ship.x = 0;
	if(ship.x < 0)
		ship.x = width;

	if(ship2.y < 0)
		ship2.y = height;
	if(ship2.y > height)
		ship2.y = 0;
	if(ship2.x > width)
		ship2.x = 0;
	if(ship2.x < 0)
		ship2.x = width;

	for(int i = 0; i < NUM_POWERUPS; i++)
	{
		if(powerups[i].live)
		{
			if(powerups[i].y < 0 - powerups[i].frameHeight - 50)
				powerups[i].y = height + powerups[i].frameHeight + 10;
			if(powerups[i].y > height + powerups[i].frameHeight + 50)
				powerups[i].y = 0 - powerups[i].frameHeight - 10;
			if(powerups[i].x > width + powerups[i].frameWidth + 50)
				powerups[i].x = 0 - powerups[i].frameWidth - 10;
			if(powerups[i].x < 0 - powerups[i].frameWidth - 50)
				powerups[i].x = width + powerups[i].frameWidth + 10;
		}
	}

	if(UFO.y < 0 - UFO.frameHeight - 10)
		UFO.y = height + UFO.frameHeight + 10;
	if(UFO.y > height + UFO.frameHeight + 10)
		UFO.y = 0 - UFO.frameHeight - 10;
	if(UFO.x > width + UFO.frameWidth + 10)
		UFO.x = 0 - UFO.frameWidth - 10;
	if(UFO.x < 0 - UFO.frameWidth - 10)
			UFO.x = width + UFO.frameWidth + 10;

	for(int i = 0; i < NUM_ASTEROIDS; i++)
	{
		if(asteroid[i].y < 0 - asteroid[i].frameHeight - 10)
			asteroid[i].y = height + asteroid[i].frameHeight + 10;
		if(asteroid[i].y > height + asteroid[i].frameHeight + 10)
			asteroid[i].y = 0 - asteroid[i].frameHeight - 10;
		if(asteroid[i].x > width + asteroid[i].frameWidth + 10)
			asteroid[i].x = 0 - asteroid[i].frameWidth - 10;
		if(asteroid[i].x < 0 - asteroid[i].frameWidth - 10)
			asteroid[i].x = width + asteroid[i].frameWidth + 10;
	}

	for(int i = 0; i < NUM_BULLETS; i++)
	{
		if(bullet[i].y < 0)
			bullet[i].y = height;
		if(bullet[i].y > height)
			bullet[i].y = 0;
		if(bullet[i].x > width)
			bullet[i].x = 0;
		if(bullet[i].x < 0)
			bullet[i].x = width;
	}
}