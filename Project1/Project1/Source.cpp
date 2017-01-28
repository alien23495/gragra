#include <allegro5\allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "objects.h"
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

//GLOBALNE
int WIDTH = 1200;
int HEIGHT = 600;
int imp = 0;
int zycia = 0;

int NUM_BULLETS;
int NUM_COMETS ;

enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
bool keys[5] = { false,false,false,false,false };

int r[] = { 196,196,196,196,196,196 }, g[] = { 119,119,119,119,119,119 }, b[] = { 89,89,89,89,89,89 }; // colors of pressed buttons
int color[] = { 255, 240, 110 };

ALLEGRO_SAMPLE *song = NULL;
ALLEGRO_SAMPLE_INSTANCE *songinstance = NULL;
//Prototypy

void InitShip(SpaceShip &statek);
void DrawShip(SpaceShip &statek);
void MoveShipUp(SpaceShip &statek);
void MoveShipDown(SpaceShip &statek);
void MoveShipLeft(SpaceShip &statek);
void MoveShipRight(SpaceShip &statek);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip &statek);
void UpdateBullet(Bullet bullet[], int size);
void CollideBulet(Bullet bullet[], int Bsize, Comet comets[], int cSize, SpaceShip &statek);



void InitComet(Comet comets[], int size);
void DrawComet(Comet comets[], int size);
void StartComet(Comet comets[], int size);
void UpdateComet(Comet comets[], int size);
void CollideComet(Comet comets[], int cSize, SpaceShip &statek);
void color_press(int *r, int *g, int *b, int selection, int max);

int main(void)
{
	//prymitywne zmienne;
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	bool isGameOver = false;
	bool menu_done = false;

	//zmienne obiektowe
	SpaceShip statek;
	Bullet *bullets = new Bullet[100000];
	Comet *comets = new Comet[100000];
	//zmienne allegro
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18 = NULL;

	//inicjalizacja funkcji
	al_init();
	al_init_image_addon();
	if (!al_init())			//inicjalizacja allegro
	{
		return -1;
	}

	display = al_create_display(WIDTH, HEIGHT);
	if (!display)
	{
		return -1;
	}

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	srand(time(NULL));

	al_reserve_samples(10);
	song = al_load_sample("song.ogg");

	songinstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songinstance, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(songinstance, al_get_default_mixer());
	
	
	font18 = al_load_font("arial.ttf", 18, 0);

	
	/////////////////

main_menu:

	InitShip(statek);
	InitBullet(bullets, 100000);
	InitComet(comets, 100000);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_start_timer(timer);
	for(int i=0;i<5;i++)
	keys[i] = false;
	menu_done = false;
	ALLEGRO_BITMAP *cursor = al_load_bitmap("grafika/Cursor_small.png");
	ALLEGRO_FONT *menu_item = al_load_ttf_font("arial.ttf", 18, 0);
	//ALLEGRO_BITMAP *dup = al_load_bitmap("dup.png");
	ALLEGRO_BITMAP * dup = al_load_bitmap("background.jpg");



	int cord_cursor_x = (WIDTH / 2) - 120;
	int cord_cursor_y = ((HEIGHT / 2) - 4) + 30;

	while (!menu_done)
	{
		ALLEGRO_EVENT menu_ev;
		al_wait_for_event(event_queue, &menu_ev);
		//al_draw_bitmap(dup, 0, 0, 0);

		if (menu_ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (menu_ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:

				if (cord_cursor_y > ((HEIGHT / 2) - 4) -90)
					cord_cursor_y -= 30;
				if (cord_cursor_y == ((HEIGHT / 2) - 4) + 0) cord_cursor_y= ((HEIGHT / 2) - 4) -30;
				break;
			case ALLEGRO_KEY_DOWN:

				if (cord_cursor_y < ((HEIGHT / 2) - 4) + 60)
					cord_cursor_y += 30;
				if (cord_cursor_y == ((HEIGHT / 2) - 4) + 0) cord_cursor_y= ((HEIGHT / 2) - 4)+ 30;
				break;
			case ALLEGRO_KEY_ENTER:

				

				if (cord_cursor_y == ((HEIGHT / 2) - 4) + 30)
				{
					// start game
					menu_done = true;
					done = false;
					isGameOver = false;		
					NUM_BULLETS = 1;
					NUM_COMETS = 4;
				}


				else if (cord_cursor_y == ((HEIGHT / 2) - 4) + 60)
				{
					menu_done = true;
					done = true;

				}

				else if (cord_cursor_y == ((HEIGHT / 2) - 4) -30)
				{
					if (HEIGHT != 600)
					{
						WIDTH = 1200;
						HEIGHT = 600;
						al_destroy_display(display);
						display = al_create_display(WIDTH, HEIGHT);
						if (!display)
						{
							return -1;
						}
						cord_cursor_y = ((HEIGHT / 2) - 4) - 30;
					}

				}
				else if (cord_cursor_y == ((HEIGHT / 2) - 4) - 60)
				{
					if (HEIGHT != 450)
					{
						WIDTH = 1200;
						HEIGHT = 450;
						al_destroy_display(display);
						display = al_create_display(WIDTH, HEIGHT);
						if (!display)
						{
							return -1;
						}
						cord_cursor_y = ((HEIGHT / 2) - 4) - 60;
					}

				}

				else if (cord_cursor_y == ((HEIGHT / 2) - 4) - 90)
				{
					if (HEIGHT != 300)
					{
						WIDTH = 1200;
						HEIGHT = 300;
						al_destroy_display(display);
						display = al_create_display(WIDTH, HEIGHT);
						if (!display)
						{
							return -1;
						}
						cord_cursor_y = ((HEIGHT / 2) - 4) - 90;
					}
				}







				break;
			case ALLEGRO_KEY_ESCAPE:
			{
				menu_done = true;
				done = true;
				break;

			}
			}
		}
		else if (menu_ev.type == ALLEGRO_EVENT_TIMER)
		{

			if (cord_cursor_y == ((HEIGHT / 2) - 4) - 90)
				color_press(r, g, b, 2 , 5);
			if (cord_cursor_y == ((HEIGHT / 2) - 4) - 60)
				color_press(r, g, b, 3, 5);
			if (cord_cursor_y == ((HEIGHT / 2) - 4) - 30)
				color_press(r, g, b, 4, 5);
			if (cord_cursor_y == ((HEIGHT / 2) - 4) + 30)
				color_press(r, g, b, 0, 5);
			if (cord_cursor_y == ((HEIGHT / 2) - 4) + 60)
				color_press(r, g, b, 1, 5);
			
			// rendering
			al_draw_text(menu_item, al_map_rgb(r[2], g[2], b[2]), WIDTH / 2, (HEIGHT / 2) - 90, ALLEGRO_ALIGN_CENTRE, "1200x300");
			al_draw_text(menu_item, al_map_rgb(r[3], g[3], b[3]), WIDTH / 2, (HEIGHT / 2) - 60, ALLEGRO_ALIGN_CENTRE, "1200x450");
			al_draw_text(menu_item, al_map_rgb(r[4], g[4], b[4]), WIDTH / 2, (HEIGHT / 2) - 30, ALLEGRO_ALIGN_CENTRE, "1200x600");


			al_draw_text(menu_item, al_map_rgb(r[0], g[0], b[0]), WIDTH / 2, (HEIGHT / 2) + 30, ALLEGRO_ALIGN_CENTRE, "START GAME");
			al_draw_text(menu_item, al_map_rgb(r[1], g[1], b[1]), WIDTH / 2, (HEIGHT / 2) + 60, ALLEGRO_ALIGN_CENTRE, "EXIT");
			al_draw_bitmap(cursor, cord_cursor_x, cord_cursor_y, 0);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}
	// destroying objects
	al_destroy_font(menu_item);

	al_destroy_bitmap(cursor);

	//	al_destroy_event_queue(event_queue);


		///////////////////////////////////////////



	while (!done)
	{
		al_play_sample_instance(songinstance);

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			if (keys[UP]) MoveShipUp(statek);
			if (keys[DOWN]) MoveShipDown(statek);
			if (keys[LEFT]) MoveShipLeft(statek);
			if (keys[RIGHT]) MoveShipRight(statek);

			if (!isGameOver)
			{
				al_play_sample_instance(songinstance);

				UpdateBullet(bullets, NUM_BULLETS);
				StartComet(comets, NUM_COMETS);
				UpdateComet(comets, NUM_COMETS);
				CollideBulet(bullets, NUM_BULLETS, comets, NUM_COMETS, statek);
				CollideComet(comets, NUM_COMETS, statek);

				if (statek.lives <= 0)
					isGameOver = true;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
			{
				done = true;
				break;
			}

			case ALLEGRO_KEY_UP:
			{
				keys[UP] = true;
				break;
			}

			case ALLEGRO_KEY_DOWN:
			{
				keys[DOWN] = true;
				break;
			}

			case ALLEGRO_KEY_LEFT:
			{
				keys[LEFT] = true;
				break;
			}

			case ALLEGRO_KEY_RIGHT:
			{
				keys[RIGHT] = true;
				break;
			}

			case ALLEGRO_KEY_SPACE:
			{
				keys[SPACE] = true;
				FireBullet(bullets, NUM_BULLETS, statek);
				break;
			}

			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
			{
				done = false;
				break;
			}

			case ALLEGRO_KEY_UP:
			{
				keys[UP] = false;
				break;
			}

			case ALLEGRO_KEY_DOWN:
			{
				keys[DOWN] = false;
				break;
			}

			case ALLEGRO_KEY_LEFT:
			{
				keys[LEFT] = false;
				break;
			}

			case ALLEGRO_KEY_RIGHT:
			{
				keys[RIGHT] = false;
				break;
			}

			case ALLEGRO_KEY_SPACE:
			{
				keys[SPACE] = false;
				break;
			}

			}
		}

		
		if (redraw&& al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			if (!isGameOver)
			{
				DrawShip(statek);
				DrawComet(comets, NUM_COMETS);
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(dup, 0, 0, 0);

				DrawBullet(bullets, NUM_BULLETS);;
				al_draw_textf(font18, al_map_rgb(255, 0, 0), 5, 5, 0, "Lives:%i     Score:%i", statek.lives, statek.score);

			}
			else
			{
				al_draw_bitmap(dup, 0, 0, 0);
				al_draw_textf(font18, al_map_rgb(0, 255, 0), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "GAME OVER SCORE: %i", statek.score);
				al_flip_display();
				al_rest(3);
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_stop_sample_instance(songinstance);
				isGameOver = false;
				goto main_menu;
			}
		}

	}
	//al_draw_bitmap(dup, 50, 50, 0);
	al_destroy_display(display);   //usuwa okienko
	al_destroy_sample(song);
	al_destroy_sample_instance(songinstance);

	al_destroy_bitmap(dup);
	delete[] bullets;
	delete[] comets;
}


void InitShip(SpaceShip &statek)
{
	statek.x = 20;
	statek.y = HEIGHT / 2;
	statek.ID = PLAYER;
	statek.lives = 3;
	statek.speed = 7;
	statek.boundx = 14;
	statek.boundy = 14;
	statek.score = 0;
}
void DrawShip(SpaceShip &statek)
{
	ALLEGRO_BITMAP *statekimg = al_load_bitmap("grafika/statek.png");

	//al_draw_filled_triangle(statek.x - 12, statek.y - 17, statek.x + 12, statek.y, statek.x - 12, statek.y + 17, al_map_rgb(0, 255, 0));
	al_draw_bitmap(statekimg, statek.x - 17, statek.y - 17, 0);


}

void MoveShipUp(SpaceShip &statek)
{
	statek.y -= statek.speed;
	if (statek.y < 0) statek.y = 0;
}
void MoveShipDown(SpaceShip &statek)
{
	statek.y += statek.speed;
	if (statek.y > HEIGHT) statek.y = HEIGHT;
}
void MoveShipLeft(SpaceShip &statek)
{
	statek.x -= statek.speed;
	if (statek.x < 0)	statek.x = 0;
}
void MoveShipRight(SpaceShip &statek)
{
	statek.x += statek.speed;
	if (statek.x > 300)	statek.x = 300;
}


void InitBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 10;
		bullet[i].live = false;
	}
}
void DrawBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)
			al_draw_filled_circle(bullet[i].x, bullet[i].y, 2, al_map_rgb(255, 255, 255));
	}
}
void FireBullet(Bullet bullet[], int size, SpaceShip &statek)
{
	for (int i = 0; i < size; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x = statek.x + 23;
			bullet[i].y = statek.y+6;
			bullet[i].live = true;
			break;
		}
	}
}
void UpdateBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)
		{
			bullet[i].x += bullet[i].speed;
			if (bullet[i].x > WIDTH)
				bullet[i].live = false;
		}
	}
}

void CollideBulet(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip &statek)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < cSize; j++)
			{
				if (comets[j].live)
				{
					if (bullet[i].x > (comets[j].x - comets[j].boundx+25) &&
						bullet[i].x < (comets[j].x + comets[j].boundx) &&
						bullet[i].y>(comets[j].y - comets[j].boundy) &&
						bullet[i].y < (comets[j].y + comets[j].boundy))

					{
						
						bullet[i].live = false;
						
					
						comets[j].live = false;
						statek.score++;
						imp++;
						//zycia++;
						if (statek.score % 10 == 0)
						{
							//zycia = 0;
							statek.lives++;
						}
						if (imp == 3)
						{
							NUM_BULLETS = NUM_BULLETS + 1;
							NUM_COMETS = NUM_COMETS + 3;
							imp = 0;
						//	statek.lives++;
							//Bullet *bullets = new Bullet[NUM_BULLETS];
							//Comet *comets = new Comet[NUM_COMETS];
							//InitBullet(bullets, NUM_BULLETS);
							//InitComet(comets, NUM_COMETS);
							
						}
					}
				}
			}
		}
	}
}



void InitComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		comets[i].ID = ENEMY;
		comets[i].live = false;
		comets[i].speed = 5;
		comets[i].boundx = 23;
		comets[i].boundy = 28;

	}
}
void DrawComet(Comet comets[], int size)
{
	ALLEGRO_BITMAP * kometa1 = al_load_bitmap("grafika/kometa1.png");
//	ALLEGRO_BITMAP * kometa2 = al_load_bitmap("grafika/kometa2.png");
//	ALLEGRO_BITMAP * kometa3 = al_load_bitmap("grafika/kometa3.png");

	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			/*if(rand() % 3 ==0)	al_draw_bitmap(kometa1, comets[i].x, comets[i].y-23, 0);
			else if (rand() % 3 == 1)	al_draw_bitmap(kometa2, comets[i].x, comets[i].y - 23, 0);
			else if (rand() % 3 == 2)	al_draw_bitmap(kometa3, comets[i].x, comets[i].y - 23, 0);*/
			al_draw_bitmap(kometa1, comets[i].x, comets[i].y - 23, 0);
			//al_draw_filled_circle(comets[i].x, comets[i].y, 20, al_map_rgb(255, 0, 0));
		}
	}
}
void StartComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!comets[i].live)
		{
			if (rand() % 500 == 0)
			{
				comets[i].live = true;
				comets[i].x = WIDTH;
				comets[i].y = 30 + rand() % (HEIGHT - 60);
				break;
			}
		}
	}
}
void UpdateComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			comets[i].x -= comets[i].speed;
			//if (comets[i].x < 0) comets[i].live = false;
		}
	}
}
void CollideComet(Comet comets[], int cSize, SpaceShip &statek)
{
	for (int i = 0; i < cSize; i++)
	{
		if (comets[i].live)
		{
			if (comets[i].x - comets[i].boundx < statek.x + statek.boundx - 30 &&
				comets[i].x + comets[i].boundx > statek.x - statek.boundx &&
				comets[i].y - comets[i].boundy < statek.y + statek.boundy &&
				comets[i].y + comets[i].boundy > statek.y + statek.boundy)
			{
				statek.lives--;
				comets[i].live = false;
			}
			else if (comets[i].x < 0)
			{
				statek.lives--;
				comets[i].live = false;
			}
		}
	}
}

void color_press(int *r, int *g, int *b, int selection, int max)
{
	for (int i = 0; i < max; ++i)
	{
		r[i] = color[0]; g[i] = color[1]; b[i] = color[2];
	}
	r[selection] = 255; g[selection] = 120; b[selection] = 0;
}
