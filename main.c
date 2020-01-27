#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char * argv[])
{
	if (argc != 3)
		return EXIT_FAILURE;

	const int WIDTH = atoi(argv[1]);
	const int HEIGHT = atoi(argv[2]);

	if(!HEIGHT || !WIDTH || !al_init() || !al_install_keyboard() || !al_install_mouse() || !al_init_image_addon() || !al_init_font_addon() || !al_init_ttf_addon())
    		return EXIT_FAILURE;
	
	ALLEGRO_FONT *font = al_load_font("arial.ttf", 18, 0);
	ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	if(!font || !display || !event_queue)
	{
		al_destroy_font (font);	
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);		
		return EXIT_FAILURE;
	}
		
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	bool wantQuit = false;
	int n_given_points = 0;
	int points_x[5];
	int points_y[5];

	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_text(font, al_map_rgb(0, 0, 0), 0, HEIGHT-20, 0, "Press any key for reset. Choose points using mouse.");
	al_flip_display();
	
	while (!wantQuit)
	{
		while (n_given_points < 5 && !wantQuit)
		{
			ALLEGRO_EVENT ev;
      			al_wait_for_event(event_queue, &ev);
			if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				++n_given_points;
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				n_given_points = 0;
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(font, al_map_rgb(0, 0, 0), 0, HEIGHT-20, 0, "Press any key for reset. Choose points using mouse.");
				al_flip_display();	
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				wantQuit = true;	
		}
		if (!wantQuit)
		{
			al_save_bitmap("display.bmp", al_get_backbuffer(display)); 
			char buf[100000];	
			FILE *bitmap = fopen("display.bmp", "rb");
			if (feof(bitmap) || ferror(bitmap))
				return EXIT_FAILURE;
			fgets(buf, 100000, bitmap);
			fclose(bitmap);
			//draw_bezier.s
			ALLEGRO_BITMAP *bmp = al_load_bitmap ("display.bmp");
			if (!bmp)
				return EXIT_FAILURE;
			al_draw_bitmap(bmp, 0, 0, 0);
			al_destroy_bitmap (bmp);
			al_flip_display();
			ALLEGRO_EVENT ev;
      			al_wait_for_event(event_queue, &ev);
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				n_given_points = 0;
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_text(font, al_map_rgb(0, 0, 0), 0, HEIGHT-20, 0, "Press any key for reset. Choose points using mouse.");
				al_flip_display();				
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				wantQuit = true;
		}
	}
	al_destroy_font (font);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);	//MAX_SIZE_BUF, ALLEGRO_COLOR, MESSAGE, NAME_OF_FILE, skalowanie czcionki, co oprócz EXIT_FAILURE
	return 0;
}
