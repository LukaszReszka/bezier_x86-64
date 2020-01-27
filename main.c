#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "draw_bezier_curve.h"

int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Error - two arguments required (size of display).\n");		
		return EXIT_FAILURE;
	}

	int points_x[6];
	int points_y[6];
	points_x[0] = atoi(argv[1]);
	points_y[0] = atoi(argv[2]);
	const int WIDTH = points_x[0];
	const int HEIGHT = points_y[0];

	if(!HEIGHT || !WIDTH)
	{
		fprintf(stderr, "Incorrect arguments format (only natural numbers).\n");		
		return EXIT_FAILURE;		
	}

	const char MESSAGE[] = "Press any key for reset. Choose points using mouse."; 
	const char FILE_NAME[] = "display.bmp";
	const int POINTS_R = 5;

	
	if(!al_init() || !al_install_keyboard() || !al_install_mouse() || !al_init_image_addon() || !al_init_font_addon() || !al_init_ttf_addon() || !al_init_primitives_addon())
	{
		fprintf(stderr, "Failed to initialize Allegro5 library.\n");	
		return EXIT_FAILURE;
	}

	ALLEGRO_COLOR background_color = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR text_color = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR points_color = al_map_rgb(255, 0, 0);
	ALLEGRO_FONT *font = al_load_font("arial.ttf", 18, 0);
	ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	if(!font || !display || !event_queue)
	{
		al_destroy_font (font);	
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
		fprintf(stderr, "Failed to initialize Allegro5 variable.\n");	
		return EXIT_FAILURE;
	}
	
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	const size_t BUF_SIZE = ((((WIDTH*3)+((WIDTH*3)%4))*HEIGHT)+54)+1; 	
	bool wantQuit = false, printResult = false, shouldWait = false;
	int n_given_points = 1;

	al_clear_to_color(background_color);
	al_draw_text(font, text_color, 0, HEIGHT-20, 0, MESSAGE);
	al_flip_display();
	
	while (!wantQuit)
	{
		while (n_given_points < 6 && !wantQuit)
		{
			ALLEGRO_EVENT ev;
      			al_wait_for_event(event_queue, &ev);
			if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				points_x[n_given_points] = ev.mouse.x;
				points_y[n_given_points] = ev.mouse.y;
				al_draw_filled_circle(ev.mouse.x, ev.mouse.y, POINTS_R, points_color);
				if (++n_given_points == 6)
				{
					al_save_bitmap(FILE_NAME, al_get_backbuffer(display));
					printResult = true;
				}
				al_flip_display();
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				n_given_points = 1;
				al_clear_to_color(background_color);
				al_draw_text(font, text_color, 0, HEIGHT-20, 0, MESSAGE);
				al_flip_display();	
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				wantQuit = true;	
		}
		if (!wantQuit)
		{
			if (printResult)
			{
				char buf[BUF_SIZE];	
				FILE *bitmap = fopen(FILE_NAME, "rb");
				if (feof(bitmap) || ferror(bitmap))
				{
					fprintf(stderr, "Failed to open bitmap.\n");				
					return EXIT_FAILURE;
				}
				fread(buf, 1, BUF_SIZE, bitmap);
				fclose(bitmap);
				draw_bezier_curve(buf, points_x, points_y);		//wywołanie funkcji asemblerowej
				bitmap = fopen(FILE_NAME, "wb");
				if (feof(bitmap) || ferror(bitmap))
				{
					fprintf(stderr, "Failed to save bitmap.\n");				
					return EXIT_FAILURE;
				}
				fwrite(buf, 1, BUF_SIZE, bitmap);
				fclose(bitmap);
				ALLEGRO_BITMAP *bmp = al_load_bitmap(FILE_NAME);
				if (!bmp)
				{
					fprintf(stderr, "Failed to load bitmap.\n");				
					return EXIT_FAILURE;
				}
				al_draw_bitmap(bmp, 0, 0, 0);
				al_destroy_bitmap (bmp);
				al_flip_display();
				printResult = false;
				shouldWait = true;			
			}
			if(shouldWait)
			{
				ALLEGRO_EVENT ev;
      				al_wait_for_event(event_queue, &ev);
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					n_given_points = 1;
					al_clear_to_color(background_color);
					al_draw_text(font, text_color, 0, HEIGHT-20, 0, MESSAGE);
					al_flip_display();
					shouldWait = false;				
				}
				else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
					wantQuit = true;
			}
		}
	}
	al_destroy_font (font);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
}
