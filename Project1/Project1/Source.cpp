#include <allegro5\allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include "objects.h"

//GLOBALNE
const int WIDTH = 1200;
const int HEIGHT = 600;
const int NUM_BULLETS = 5;
const int NUM_COMETS = 10;

enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
bool keys[5] = { false,false,false,false,false };


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

void InitComet(Comet comets[], int size);
void DrawComet(Comet comets[], int size);
void StartComet(Comet comets[], int size);
void UpdateComet(Comet comets[], int size);
int main(void)
{
	//prymitywne zmienne;
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	//zmienne obiektowe
	SpaceShip statek;
	Bullet bullets[NUM_BULLETS];
	Comet comets[NUM_COMETS];
	//zmienne allegro
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

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

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	InitShip(statek);
	InitBullet(bullets, NUM_BULLETS);
	InitComet(comets, NUM_COMETS);
	srand(time(NULL));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_start_timer(timer);
	ALLEGRO_BITMAP * dup = al_load_bitmap("background.jpg");
	while (!done)
	{

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			if (keys[UP]) MoveShipUp(statek);
			if (keys[DOWN]) MoveShipDown(statek);
			if (keys[LEFT]) MoveShipLeft(statek);
			if (keys[RIGHT]) MoveShipRight(statek);
			UpdateBullet(bullets, NUM_BULLETS);
			StartComet(comets, NUM_COMETS);
			UpdateComet(comets, NUM_COMETS);
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
			DrawShip(statek);

			DrawComet(comets, NUM_COMETS);;
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(dup, 0, 0, 0);
			DrawBullet(bullets, NUM_BULLETS);;
		}

	}
	//al_draw_bitmap(dup, 50, 50, 0);
	al_destroy_display(display);   //usuwa okienko
	al_destroy_bitmap(dup);


}


void InitShip(SpaceShip &statek)
{
	statek.x = 20;
	statek.y = HEIGHT / 2;
	statek.ID = PLAYER;
	statek.lives = 3;
	statek.speed = 7;
	statek.boundx = 6;
	statek.boundy = 7;
}
void DrawShip(SpaceShip &statek)
{
	al_draw_filled_triangle(statek.x - 12, statek.y - 17, statek.x + 12, statek.y, statek.x - 12, statek.y + 17, al_map_rgb(0, 255, 0));

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
			bullet[i].x = statek.x + 17;
			bullet[i].y = statek.y;
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

void InitComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		comets[i].ID = ENEMY;
		comets[i].live = false;
		comets[i].speed = 5;
		comets[i].boundx = 18;
		comets[i].boundy = 18;

	}
}
void DrawComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			al_draw_filled_circle(comets[i].x, comets[i].y, 20, al_map_rgb(255, 0, 0));
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
			if (comets[i].x < 0) comets[i].live = false;
		}
	}
}