/*
  kiss_sdl widget toolkit
  Copyright (c) 2016 Tarvo Korrovits <tkorrovi@mail.com>

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would
     be appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not
     be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source
     distribution.

  kiss_sdl version 1.2.0
*/

#include "kiss_sdl.h"

static struct {
	char name[KISS_MAX_LENGTH];
	int population;
	int area;
} cities[] = {
	{"Atlanta", 5268860, 8376},
	{"Baltimore", 2710489, 2710},
	{"Boston", 4552402, 1422},
	{"Charlotte", 2217012, 3198},
	{"Chicago", 9461105, 10856},
	{"Cincinnati", 2130151, 4808},
	{"Cleveland", 2077240, 82},
	{"Dallas", 6426214, 9286},
	{"Denver", 2543482, 155},
	{"Detroit", 4290060, 3888},
	{"Houston", 5920416, 10062},
	{"Kansas City", 2035334, 7952},
	{"Los Angeles", 12828837, 4850},
	{"Miami", 5564635, 6137},
	{"Minneapolis", 3279833, 8120},
	{"New York", 19069796, 13318},
	{"Orlando", 2134411, 4012},
	{"Philadelphia", 5965343, 5118},
	{"Phoenix", 4192887, 14598},
	{"Pittsburgh", 2356285, 5706},
	{"Portland", 2226009, 6684},
	{"Sacramento", 2149127, 21429},
	{"San Antonio", 2142508, 7387},
	{"San Diego", 3095313, 372},
	{"San Francisco", 4335391, 6984},
	{"Seattle", 4039809, 5872},
	{"St. Louis", 2812896, 8458},
	{"Tampa", 2783243, 2554},
	{"Washington", 5582170, 5564},
	{"", 0, 0}
};

static void select1_event(kiss_selectbutton *select1, SDL_Event *e,
	kiss_selectbutton *select2, int *draw)
{
	if (kiss_selectbutton_event(select1, e, draw))
		select2->selected ^= 1;
}

static void select2_event(kiss_selectbutton *select2, SDL_Event *e,
	kiss_selectbutton *select1, int *draw)
{
	if (kiss_selectbutton_event(select2, e, draw))
		select1->selected ^= 1;
}

static void combobox_event(kiss_combobox *combobox, SDL_Event *e,
	char *stext, kiss_entry *entry, kiss_selectbutton *select1,
	kiss_selectbutton *select2, kiss_hscrollbar *hscrollbar, int *draw)
{
	void **p, *s;
	int i;

	s = combobox->entry.text;
	if (kiss_combobox_event(combobox, e, draw)) {
		if ((p = (void **) bsearch(&s, combobox->textbox.array->data,
			combobox->textbox.array->length, sizeof(void *),
			kiss_string_compare))) {
			i = p - combobox->textbox.array->data;
			if (select1->selected)
				sprintf(stext, "The population of the "
					"metropolitan area of %s is %d.",
					cities[i].name,
					cities[i].population);
			else
				sprintf(stext, "The metropolitan area of "
					"%s is %d square miles.",
					cities[i].name,
					cities[i].area);
		
		} else
			strcpy(stext, "Data not found");
		strcpy(entry->text, stext);
		hscrollbar->fraction = 0.;
		hscrollbar->step = 0.;
		if (strlen(stext) - entry->textwidth /
			kiss_textfont.advance > 0)
			hscrollbar->step = 1. / (strlen(stext) -
				entry->textwidth / kiss_textfont.advance);
		entry->text[entry->textwidth / kiss_textfont.advance] = 0;
		*draw = 1;
	}
}

/* This is to show the hscrollbar, only works with ASCII characters */
static void hscrollbar_event(kiss_hscrollbar *hscrollbar, SDL_Event *e,
	char *stext, int *first, kiss_entry *entry, int *draw)
{
	char *p;

	p = stext;
	if (kiss_hscrollbar_event(hscrollbar, e, draw) && strlen(stext) -
		entry->textwidth / kiss_textfont.advance > 0) {
		*first = (int) ((strlen(stext) - entry->textwidth /
		kiss_textfont.advance) * hscrollbar->fraction + 0.5);
		if (*first >= 0) {
			strcpy(entry->text, p + *first);
			entry->text[entry->textwidth /
				kiss_textfont.advance] = 0;
			*draw = 1;
		}
	}
}

static void button_ok_event(kiss_button *button_ok, SDL_Event *e,
	int *quit, int *draw)
{
	if (kiss_button_event(button_ok, e, draw)) *quit = 1;
}

int main(int argc, char **argv)
{
	SDL_Renderer *renderer;
	SDL_Event e;
	kiss_array objects, a;
	kiss_window window;
	kiss_label label1 = {0}, label2 = {0};
	kiss_button button_ok = {0};
	kiss_hscrollbar hscrollbar = {0};
	kiss_entry entry = {0};
	kiss_combobox combobox = {0};
	kiss_selectbutton select1 = {0}, select2 = {0};
	char stext[KISS_MAX_LENGTH];
	int combobox_width, combobox_height, entry_width;
	int first, draw, quit, i;

	quit = 0;
	draw = 1;
	first = 0;
	strcpy(stext, "");
	/* Combobox textbox width and height */
	combobox_width = 150;
	combobox_height = 66;
	entry_width = 250;
	renderer = kiss_init("kiss_sdl example 2", &objects, 640, 480);
	if (!renderer) return 1;
	kiss_array_new(&a);
	for (i = 0; cities[i].population; i++)
		kiss_array_appendstring(&a, 0, cities[i].name, NULL);
	kiss_array_append(&objects, ARRAY_TYPE, &a);

	/* Arrange the widgets nicely relative to each other */
	kiss_window_new(&window, NULL, 1, 0, 0, kiss_screen_width,
		kiss_screen_height);
	kiss_label_new(&label1, &window, "Population",
		kiss_screen_width / 2 - (combobox_width +
		kiss_up.w - kiss_edge) / 2 + kiss_edge,
		6 * kiss_textfont.lineheight);
	kiss_selectbutton_new(&select1, &window,
		label1.rect.x + combobox_width + kiss_up.w -
		kiss_edge - kiss_selected.w, label1.rect.y +
		kiss_textfont.ascent - kiss_selected.h);
	kiss_label_new(&label2, &window, "Area", label1.rect.x,
		label1.rect.y + 2 * kiss_textfont.lineheight);
	kiss_selectbutton_new(&select2, &window, select1.rect.x,
		label2.rect.y + kiss_textfont.ascent - kiss_selected.h);
	kiss_combobox_new(&combobox, &window, "none", &a, label1.rect.x -
		kiss_edge, label2.rect.y + 2 * kiss_textfont.lineheight,
		combobox_width, combobox_height);
	kiss_entry_new(&entry, &window, 1, "",
		kiss_screen_width / 2 - entry_width / 2 + kiss_edge,
		combobox.entry.rect.y + combobox.entry.rect.h +
		2 * kiss_textfont.lineheight + kiss_border, entry_width);
	kiss_hscrollbar_new(&hscrollbar, &window, entry.rect.x,
		entry.rect.y + entry.rect.h, entry.rect.w);
	kiss_button_new(&button_ok, &window, "OK",
		entry.rect.x + entry.rect.w - kiss_edge - kiss_normal.w,
		entry.rect.y + entry.rect.h + kiss_left.h +
		2 * kiss_normal.h);

	select1.selected = 1;
	hscrollbar.step = 0.;
	/* Do that, and all widgets associated with the window will show */
	window.visible = 1;

	while (!quit) {

		/* Some code may be written here */

		SDL_Delay(10);
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = 1;

			kiss_window_event(&window, &e, &draw);
			select1_event(&select1, &e, &select2, &draw);
			select2_event(&select2, &e, &select1, &draw);
			combobox_event(&combobox, &e, stext, &entry,
				&select1, &select2, &hscrollbar, &draw);
			hscrollbar_event(&hscrollbar, &e, stext, &first,
				&entry, &draw);
			button_ok_event(&button_ok, &e, &quit, &draw);
		}

		kiss_combobox_event(&combobox, NULL, &draw);
		hscrollbar_event(&hscrollbar, NULL, stext, &first, &entry,
			&draw);

		if (!draw) continue;
		SDL_RenderClear(renderer);

		kiss_window_draw(&window, renderer);
		kiss_button_draw(&button_ok, renderer);
		kiss_hscrollbar_draw(&hscrollbar, renderer);
		kiss_entry_draw(&entry, renderer);
		kiss_combobox_draw(&combobox, renderer);
		kiss_selectbutton_draw(&select2, renderer);
		kiss_label_draw(&label2, renderer);
		kiss_selectbutton_draw(&select1, renderer);
		kiss_label_draw(&label1, renderer);

		SDL_RenderPresent(renderer);
		draw = 0;
	}
	kiss_clean(&objects);
	return 0;
}

