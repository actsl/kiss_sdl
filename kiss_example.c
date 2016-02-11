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

  kiss_sdl version 0.6.0
*/

#include "kiss_sdl.h"
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

/* Read directory entries into the textboxes */
void dirent_read(kiss_textbox *textbox1, kiss_scrollbar *scrollbar1,
	kiss_textbox *textbox2,	kiss_scrollbar *scrollbar2,
	kiss_label *label_sel)
{
	struct dirent *ent;
	struct stat s;
	DIR *dir;
	char buf[KISS_MAX_LENGTH];
	int length;

	length = 60;
	kiss_array_free(textbox1->array);
	kiss_array_free(textbox2->array);
	kiss_array_new(textbox1->array);
	kiss_array_new(textbox2->array);
	getcwd(buf, KISS_MAX_LENGTH);
	if (buf[0] != 'C' && strcmp(buf, "/"))
		kiss_string_copy(label_sel->text, length, buf, "/");
	else if (buf[0] != 'C' && !strcmp(buf, "/"))
		kiss_string_copy(label_sel->text, length, buf, NULL);
	else if (buf[0] == 'C' && strcmp(buf, "C:\\"))
		kiss_string_copy(label_sel->text, length, buf, "\\");
	else if (buf[0] == 'C' && !strcmp(buf, "C:\\"))
		kiss_string_copy(label_sel->text, length, buf, NULL);
	dir = opendir(".");
	while ((ent = readdir(dir))) {
		stat(ent->d_name, &s);
		if (S_ISDIR(s.st_mode))
			kiss_array_appendstring(textbox1->array, 0,
				ent->d_name, "/");
		else if (S_ISREG(s.st_mode))
			kiss_array_appendstring(textbox2->array, 0,
				ent->d_name, NULL);
	}
	closedir(dir);
	scrollbar1->step = 0.;
	scrollbar2->step = 0.;
	if (textbox1->array->length) {
		qsort(textbox1->array->data, textbox1->array->length,
			sizeof(void *), kiss_string_compare);
		scrollbar1->step = 1. / (0.3 * textbox1->array->length);
	}
	if (textbox2->array->length) {
		qsort(textbox2->array->data, textbox2->array->length,
		sizeof(void *), kiss_string_compare);
		scrollbar2->step = 1. / (0.3 * textbox2->array->length);
	}
	if (textbox1->array->length <= textbox1->maxlines)
		scrollbar1->step = 0.;
	if (textbox2->array->length <= textbox2->maxlines)
		scrollbar2->step = 0.;
	textbox1->firstline = 0;
	textbox1->highlightline = -1;
	textbox2->firstline = 0;
	scrollbar1->fraction = 0.;
	scrollbar2->fraction = 0.;
}

/* The widget arguments are widgets that this widget talks with */
static void textbox1_event(kiss_textbox *textbox, SDL_Event *e,
	kiss_scrollbar *scrollbar1, kiss_textbox *textbox2,
	kiss_scrollbar *scrollbar2, kiss_label *label_sel, int *draw)
{
	int index;

	if (kiss_textbox_event(textbox, e, draw)) {
		index = textbox->firstline + textbox->selectedline;
		if (strcmp((char *) kiss_array_data(textbox->array, index),
			"")) {
			textbox->selectedline = -1;
			chdir((char *) kiss_array_data(textbox->array,
				index));
			dirent_read(textbox, scrollbar1, textbox2,
				scrollbar2, label_sel);
			*draw = 1;
		}
	}
}

static void scrollbar1_event(kiss_scrollbar *scrollbar, SDL_Event *e,
	kiss_textbox *textbox1, int *draw)
{
	int firstline;

	if (kiss_scrollbar_event(scrollbar, e, draw) &&
		textbox1->array->length - textbox1->maxlines >= 0) {
		firstline = (int) ((textbox1->array->length - 
			textbox1->maxlines) * scrollbar->fraction + 0.5);
		if (firstline >= 0) textbox1->firstline = firstline;
		*draw = 1;
	}
}

static void textbox2_event(kiss_textbox *textbox, SDL_Event *e,
	kiss_scrollbar *scrollbar2, kiss_entry *entry, int *draw)
{
	int index;

	if (kiss_textbox_event(textbox, e, draw)) {
		index = textbox->firstline + textbox->selectedline;
		if (strcmp((char *) kiss_array_data(textbox->array, index),
			"")) {
			kiss_string_copy(entry->text, entry->textlength,
				(char *) kiss_array_data(textbox->array,
				index), NULL);
			*draw = 1;
		}
	}
}

static void scrollbar2_event(kiss_scrollbar *scrollbar, SDL_Event *e,
	kiss_textbox *textbox2, int *draw)
{
	int firstline;

	if (kiss_scrollbar_event(scrollbar, e, draw) &&
		textbox2->array->length) {
		firstline = (int) ((textbox2->array->length - 
			textbox2->maxlines) * scrollbar->fraction + 0.5);
		if (firstline >= 0) textbox2->firstline = firstline;
		*draw = 1;
	}
}

static void button_ok1_event(kiss_button *button, SDL_Event *e,
	kiss_window *window1, kiss_window *window2, kiss_label *label_sel,
	kiss_entry *entry, kiss_label *label_res,
	kiss_progressbar *progressbar, int *draw)
{
	char buf[KISS_MAX_LENGTH];

	if (kiss_button_event(button, e, draw)) {
		kiss_string_copy(buf, (window2->rect.w -
			2 * kiss_slider_width) / kiss_text_advance,
			label_sel->text, entry->text);
		kiss_string_copy(label_res->text, KISS_MAX_LABEL, 
			"The following path was selected:\n", buf);
		window2->visible = 1;
		window2->focus = 1;
		window1->focus = 0;
		button->prelight = 0;
		progressbar->fraction = 0.;
		progressbar->run = 1;
		*draw = 1;
	}
}

static void button_cancel_event(kiss_button *button, SDL_Event *e,
	int *quit, int *draw)
{
	if (kiss_button_event(button, e, draw)) *quit = 1;
}

static void button_ok2_event(kiss_button *button, SDL_Event *e,
	kiss_window *window1, kiss_window *window2,
	kiss_progressbar *progressbar, int *draw)
{
	if (kiss_button_event(button, e, draw)) {
		window2->visible = 0;
		window1->focus = 1;
		button->prelight = 0;
		progressbar->fraction = 0.;
		progressbar->run = 0;
		*draw = 1;
	}
}

int main(int argc, char **argv)
{
	SDL_Renderer *renderer;
	SDL_Event e;
	kiss_array objects, a1, a2;
	kiss_window window1, window2;
	kiss_label label1, label2, label_sel, label_res;
	kiss_button button_ok1, button_ok2, button_cancel;
	kiss_textbox textbox1, textbox2;
	kiss_scrollbar scrollbar1, scrollbar2;
	kiss_progressbar progressbar;
	kiss_entry entry;
	int textbox_width, textbox_height, window2_width,
		window2_height, draw, quit;

	quit = 0;
	textbox_width = 400;
	textbox_height = 400;
	window2_width = 640;
	window2_height = 270;
	renderer = kiss_init("kiss_sdl example", &objects);
	kiss_array_new(&a1);
	kiss_array_append(&objects, ARRAY_TYPE, &a1);
	kiss_array_new(&a2);
	kiss_array_append(&objects, ARRAY_TYPE, &a2);

	/* Arrange the widgets nicely relative to each other */
	kiss_window_new(&window1, 0, 0, kiss_screen_width,
		kiss_screen_height);
	kiss_textbox_new(&textbox1, &a1, kiss_screen_width / 2 -
		(2 * textbox_width + 2 * kiss_slider_width -
		KISS_FRAME_EDGE) / 2, 3 * kiss_button_height,
		textbox_width, textbox_height);
	kiss_scrollbar_new(&scrollbar1, textbox1.rect.x + textbox_width,
		textbox1.rect.y + KISS_FRAME_EDGE, textbox_height -
		2 * KISS_FRAME_EDGE);
	kiss_textbox_new(&textbox2, &a2,
		scrollbar1.rect.x + kiss_slider_width,
		textbox1.rect.y, textbox_width, textbox_height);
	kiss_scrollbar_new(&scrollbar2, textbox2.rect.x + textbox_width,
		scrollbar1.rect.y, scrollbar1.rect.h);
	kiss_label_new(&label1, "Folders", textbox1.rect.x + KISS_FRAME_EDGE,
		textbox1.rect.y - kiss_text_lineheight);
	kiss_label_new(&label2, "Files", textbox2.rect.x + KISS_FRAME_EDGE,
		textbox1.rect.y - kiss_text_lineheight);
	kiss_label_new(&label_sel, "", textbox1.rect.x + KISS_FRAME_EDGE,
		textbox1.rect.y + textbox_height + kiss_button_height);
	kiss_entry_new(&entry, "kiss", textbox1.rect.x,
		label_sel.rect.y + kiss_text_lineheight, 2 * textbox_width +
		2 * kiss_slider_width + KISS_FRAME_EDGE);
	kiss_button_new(&button_cancel, "Cancel",
		entry.rect.x + entry.rect.w - KISS_FRAME_EDGE -
		kiss_button_width, entry.rect.y + entry.rect.h +
		kiss_button_height);
	kiss_button_new(&button_ok1, "OK", button_cancel.rect.x -
		2 * kiss_button_width, button_cancel.rect.y);
	kiss_window_new(&window2, kiss_screen_width / 2 - window2_width / 2,
		kiss_screen_height / 2 - window2_height / 2, window2_width,
		window2_height);
	kiss_label_new(&label_res, "", window2.rect.x + kiss_slider_width,
		window2.rect.y + kiss_slider_height);
		label_res.textcolor = kiss_blue;
	kiss_progressbar_new(&progressbar, window2.rect.x +
		kiss_slider_width - KISS_FRAME_EDGE, window2.rect.y +
		window2.rect.h / 2 - kiss_bar_height / 2 - KISS_BORDER,
		window2.rect.w - 2 * kiss_slider_width +
		2 * KISS_FRAME_EDGE);
	kiss_button_new(&button_ok2, "OK", window2.rect.x +
		window2.rect.w / 2 - kiss_button_width / 2,
		progressbar.rect.y + progressbar.rect.h +
		2 * kiss_slider_height);

	dirent_read(&textbox1, &scrollbar1, &textbox2, &scrollbar2,
		&label_sel);
	/* Do that, and all widgets associated with the window will show */
	window1.visible = 1;
	draw = 1;

	while (!quit) {

		/* Some code may be written here */

		SDL_Delay(10);
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = 1;

			kiss_window_event(&window2, &e, &draw);
			kiss_window_event(&window1, &e, &draw);
			textbox1_event(&textbox1, &e, &scrollbar1,
				&textbox2, &scrollbar2, &label_sel, &draw);
			scrollbar1_event(&scrollbar1, &e, &textbox1, &draw);
			textbox2_event(&textbox2, &e, &scrollbar2, &entry,
				&draw);
			scrollbar2_event(&scrollbar2, &e, &textbox2, &draw);
			kiss_entry_event(&entry, &e, &draw);
			button_ok1_event(&button_ok1, &e, &window1, &window2,
				&label_sel, &entry, &label_res,
				&progressbar, &draw);
			button_cancel_event(&button_cancel, &e, &quit,
				&draw);
			button_ok2_event(&button_ok2, &e, &window1, &window2,
				&progressbar, &draw);

		}

		scrollbar1_event(&scrollbar1, NULL, &textbox1, &draw);
		scrollbar2_event(&scrollbar2, NULL, &textbox2, &draw);
		kiss_progressbar_event(&progressbar, NULL, &draw);

		if (!draw) continue;
		SDL_RenderClear(renderer);

		kiss_window_draw(&window1, renderer, NULL);
		kiss_label_draw(&label1, renderer, &window1);
		kiss_label_draw(&label2, renderer, &window1);
		kiss_textbox_draw(&textbox1, renderer, &window1);
		kiss_scrollbar_draw(&scrollbar1, renderer, &window1);
		kiss_textbox_draw(&textbox2, renderer, &window1);
		kiss_scrollbar_draw(&scrollbar2, renderer, &window1);
		kiss_label_draw(&label_sel, renderer, &window1);
		kiss_entry_draw(&entry, renderer, &window1);
		kiss_button_draw(&button_ok1, renderer, &window1);
		kiss_button_draw(&button_cancel, renderer, &window1);
		kiss_window_draw(&window2, renderer, NULL);
		kiss_label_draw(&label_res, renderer, &window2);
		kiss_pogressbar_draw(&progressbar, renderer, &window2);
		kiss_button_draw(&button_ok2, renderer, &window2);

		SDL_RenderPresent(renderer);
		draw = 0;
	}
	kiss_clean(&objects);
	return 0;
}

