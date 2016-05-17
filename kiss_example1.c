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

  kiss_sdl version 0.10.4
*/

#include "kiss_sdl.h"

static void text_reset(kiss_textbox *textbox, kiss_vscrollbar *vscrollbar)
{
	qsort(textbox->array->data, textbox->array->length, sizeof(void *),
		kiss_string_compare);
	vscrollbar->step = 0.;
	if (textbox->array->length - textbox->maxlines > 0)
		vscrollbar->step = 1. / (textbox->array->length -
			textbox->maxlines);
	textbox->firstline = 0;
	textbox->highlightline = -1;
	vscrollbar->fraction = 0.;
}

/* Read directory entries into the textboxes */
static void dirent_read(kiss_textbox *textbox1, kiss_vscrollbar *vscrollbar1,
	kiss_textbox *textbox2,	kiss_vscrollbar *vscrollbar2,
	kiss_label *label_sel)
{
	kiss_dirent *ent;
	kiss_stat s;
	kiss_dir *dir;
	char buf[KISS_MAX_LENGTH], ending[2];

	kiss_array_free(textbox1->array);
	kiss_array_free(textbox2->array);
	kiss_array_new(textbox1->array);
	kiss_array_new(textbox2->array);
	kiss_getcwd(buf, KISS_MAX_LENGTH);
	strcpy(ending, "/");
	if (buf[0] == 'C') strcpy(ending, "\\");
	if (!strcmp(buf, "/") || !strcmp(buf, "C:\\")) strcpy(ending, "");
	kiss_string_copy(label_sel->text, (2 * textbox1->rect.w +
		2 * kiss_up.w) / kiss_textfont.advance, buf, ending);
#ifdef _MSC_VER
	dir = kiss_opendir("*");
#else
	dir = kiss_opendir(".");
#endif
	while ((ent = kiss_readdir(dir))) {
		if (!ent->d_name) continue;
		kiss_getstat(ent->d_name, &s);
		if (kiss_isdir(s))
			kiss_array_appendstring(textbox1->array, 0,
				ent->d_name, "/");
		else if (kiss_isreg(s))
			kiss_array_appendstring(textbox2->array, 0,
				ent->d_name, NULL);
	}
	kiss_closedir(dir);
	text_reset(textbox1, vscrollbar1);
	text_reset(textbox2, vscrollbar2);
}

/* The widget arguments are widgets that this widget talks with */
static void textbox1_event(kiss_textbox *textbox, SDL_Event *e,
	kiss_vscrollbar *vscrollbar1, kiss_textbox *textbox2,
	kiss_vscrollbar *vscrollbar2, kiss_label *label_sel, int *draw)
{
	int index;

	if (kiss_textbox_event(textbox, e, draw)) {
		index = textbox->firstline + textbox->selectedline;
		if (strcmp((char *) kiss_array_data(textbox->array, index),
			"")) {
			textbox->selectedline = -1;
			kiss_chdir((char *) kiss_array_data(textbox->array,
				index));
			dirent_read(textbox, vscrollbar1, textbox2,
				vscrollbar2, label_sel);
			*draw = 1;
		}
	}
}

static void vscrollbar1_event(kiss_vscrollbar *vscrollbar, SDL_Event *e,
	kiss_textbox *textbox1, int *draw)
{
	int firstline;

	if (kiss_vscrollbar_event(vscrollbar, e, draw) &&
		textbox1->array->length - textbox1->maxlines > 0) {
		firstline = (int) ((textbox1->array->length - 
			textbox1->maxlines) * vscrollbar->fraction + 0.5);
		if (firstline >= 0) textbox1->firstline = firstline;
		*draw = 1;
	}
}

static void textbox2_event(kiss_textbox *textbox, SDL_Event *e,
	kiss_vscrollbar *vscrollbar2, kiss_entry *entry, int *draw)
{
	int index;

	if (kiss_textbox_event(textbox, e, draw)) {
		index = textbox->firstline + textbox->selectedline;
		if (strcmp((char *) kiss_array_data(textbox->array, index),
			"")) {
			kiss_string_copy(entry->text,
				entry->textwidth / kiss_textfont.advance,
				(char *) kiss_array_data(textbox->array,
				index), NULL);
			*draw = 1;
		}
	}
}

static void vscrollbar2_event(kiss_vscrollbar *vscrollbar, SDL_Event *e,
	kiss_textbox *textbox2, int *draw)
{
	int firstline;

	if (kiss_vscrollbar_event(vscrollbar, e, draw) &&
		textbox2->array->length) {
		firstline = (int) ((textbox2->array->length - 
			textbox2->maxlines) * vscrollbar->fraction + 0.5);
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
		kiss_string_copy(buf, kiss_maxlength(kiss_textfont,
			window2->rect.w - 2 * kiss_vslider.w,
			label_sel->text, entry->text),
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
	kiss_vscrollbar vscrollbar1, vscrollbar2;
	kiss_progressbar progressbar;
	kiss_entry entry;
	int textbox_width, textbox_height, window2_width, window2_height,
		draw, quit;

	quit = 0;
	draw = 1;
	textbox_width = 250;
	textbox_height = 250;
	window2_width = 400;
	window2_height = 168;
	renderer = kiss_init("kiss_sdl example 1", &objects, 640, 480);
	kiss_array_new(&a1);
	kiss_array_append(&objects, ARRAY_TYPE, &a1);
	kiss_array_new(&a2);
	kiss_array_append(&objects, ARRAY_TYPE, &a2);

	/* Arrange the widgets nicely relative to each other */
	kiss_window_new(&window1, NULL, 1, 0, 0, kiss_screen_width,
		kiss_screen_height);
	kiss_textbox_new(&textbox1, &window1, 1, &a1, kiss_screen_width / 2 -
		(2 * textbox_width + 2 * kiss_up.w - kiss_edge) / 2,
		3 * kiss_normal.h, textbox_width, textbox_height);
	kiss_vscrollbar_new(&vscrollbar1, &window1, textbox1.rect.x +
		textbox_width, textbox1.rect.y, textbox_height);
	kiss_textbox_new(&textbox2, &window1, 1, &a2,
		vscrollbar1.uprect.x + kiss_up.w, textbox1.rect.y,
		textbox_width, textbox_height);
	kiss_vscrollbar_new(&vscrollbar2, &window1, textbox2.rect.x +
		textbox_width, vscrollbar1.uprect.y, textbox_height);
	kiss_label_new(&label1, &window1, "Folders", textbox1.rect.x +
		kiss_edge, textbox1.rect.y - kiss_textfont.lineheight);
	kiss_label_new(&label2, &window1, "Files", textbox2.rect.x +
		kiss_edge, textbox1.rect.y - kiss_textfont.lineheight);
	kiss_label_new(&label_sel, &window1, "", textbox1.rect.x +
		kiss_edge, textbox1.rect.y + textbox_height +
		kiss_normal.h);
	kiss_entry_new(&entry, &window1, 1, "kiss", textbox1.rect.x,
		label_sel.rect.y + kiss_textfont.lineheight,
		2 * textbox_width + 2 * kiss_up.w + kiss_edge);
	kiss_button_new(&button_cancel, &window1, "Cancel",
		entry.rect.x + entry.rect.w - kiss_edge - kiss_normal.w,
		entry.rect.y + entry.rect.h + kiss_normal.h);
	kiss_button_new(&button_ok1, &window1, "OK", button_cancel.rect.x -
		2 * kiss_normal.w, button_cancel.rect.y);
	kiss_window_new(&window2, NULL, 1, kiss_screen_width / 2 -
		window2_width / 2, kiss_screen_height / 2 -
		window2_height / 2, window2_width, window2_height);
	kiss_label_new(&label_res, &window2, "", window2.rect.x +
		kiss_up.w, window2.rect.y + kiss_vslider.h);
	label_res.textcolor = kiss_blue;
	kiss_progressbar_new(&progressbar, &window2, window2.rect.x +
		kiss_up.w - kiss_edge, window2.rect.y + window2.rect.h / 2 -
		kiss_bar.h / 2 - kiss_border,
		window2.rect.w - 2 * kiss_up.w + 2 * kiss_edge);
	kiss_button_new(&button_ok2, &window2, "OK", window2.rect.x +
		window2.rect.w / 2 - kiss_normal.w / 2,
		progressbar.rect.y + progressbar.rect.h +
		2 * kiss_vslider.h);

	dirent_read(&textbox1, &vscrollbar1, &textbox2, &vscrollbar2,
		&label_sel);
	/* Do that, and all widgets associated with the window will show */
	window1.visible = 1;

	while (!quit) {

		/* Some code may be written here */

		SDL_Delay(10);
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = 1;

			kiss_window_event(&window2, &e, &draw);
			kiss_window_event(&window1, &e, &draw);
			textbox1_event(&textbox1, &e, &vscrollbar1,
				&textbox2, &vscrollbar2, &label_sel, &draw);
			vscrollbar1_event(&vscrollbar1, &e, &textbox1,
				&draw);
			textbox2_event(&textbox2, &e, &vscrollbar2, &entry,
				&draw);
			vscrollbar2_event(&vscrollbar2, &e, &textbox2, &draw);
			button_ok1_event(&button_ok1, &e, &window1, &window2,
				&label_sel, &entry, &label_res,	&progressbar,
				&draw);
			button_cancel_event(&button_cancel, &e, &quit,
				&draw);
			kiss_entry_event(&entry, &e, &draw);
			button_ok2_event(&button_ok2, &e, &window1, &window2,
				&progressbar, &draw);
		}

		vscrollbar1_event(&vscrollbar1, NULL, &textbox1, &draw);
		vscrollbar2_event(&vscrollbar2, NULL, &textbox2, &draw);
		kiss_progressbar_event(&progressbar, NULL, &draw);

		if (!draw) continue;
		SDL_RenderClear(renderer);

		kiss_window_draw(&window1, renderer);
		kiss_label_draw(&label1, renderer);
		kiss_label_draw(&label2, renderer);
		kiss_textbox_draw(&textbox1, renderer);
		kiss_vscrollbar_draw(&vscrollbar1, renderer);
		kiss_textbox_draw(&textbox2, renderer);
		kiss_vscrollbar_draw(&vscrollbar2, renderer);
		kiss_label_draw(&label_sel, renderer);
		kiss_entry_draw(&entry, renderer);
		kiss_button_draw(&button_ok1, renderer);
		kiss_button_draw(&button_cancel, renderer);
		kiss_window_draw(&window2, renderer);
		kiss_label_draw(&label_res, renderer);
		kiss_pogressbar_draw(&progressbar, renderer);
		kiss_button_draw(&button_ok2, renderer);

		SDL_RenderPresent(renderer);
		draw = 0;
	}
	kiss_clean(&objects);
	return 0;
}

