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

#ifndef _kiss_sdl_h
#define _kiss_sdl_h

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#define KISS_MAX_LENGTH 100
#define KISS_MIN_LENGTH 10
#define KISS_MAX_LABEL 500
#define KISS_SLIDER_PADDING 4
#define KISS_FRAME_EDGE 4
#define KISS_BORDER 10
#define KISS_SPACING 0.5

enum {OTHER_TYPE, WINDOW_TYPE, RENDERER_TYPE, TEXTURE_TYPE, SURFACE_TYPE,
	FONT_TYPE, STRING_TYPE, ARRAY_TYPE};

typedef struct kiss_array {
	void **data;
	int *id;
	int length;
	int size;
	int ref;
} kiss_array;

typedef struct kiss_window {
	SDL_Rect rect;
	int visible;
	int focus;
	SDL_Color bg;
} kiss_window;

typedef struct kiss_label {
	SDL_Rect rect;
	int visible;
	char text[KISS_MAX_LABEL];
	SDL_Color textcolor;
} kiss_label;

typedef struct kiss_button {
	SDL_Rect rect;
	int visible;
	int focus;
	int textx;
	int texty;
	char text[KISS_MAX_LENGTH];
	int active;
	int prelight;
	SDL_Color textcolor;
} kiss_button;

typedef struct kiss_scrollbar {
	SDL_Rect rect;
	int visible;
	int focus;
	SDL_Rect uprect;
	SDL_Rect downrect;
	SDL_Rect sliderrect;
	int maxpos;
	double fraction;
	double step;
	unsigned int lasttick;
	int downclicked;
	int upclicked;
} kiss_scrollbar;

typedef struct kiss_progressbar {
	SDL_Rect rect;
	int visible;
	SDL_Rect barrect;
	int active;
	int width;
	double fraction;
	double step;
	SDL_Color bg;
	unsigned int lasttick;
	int run;
} kiss_progressbar;

typedef struct kiss_entry {
	SDL_Rect rect;
	int visible;
	int focus;
	int textx;
	int texty;
	char text[KISS_MAX_LENGTH];
	int active;
	int textlength;
	SDL_Color textcolor;
	SDL_Color bg;
} kiss_entry;

typedef struct kiss_textbox {
	SDL_Rect rect;
	int visible;
	int focus;
	kiss_array *array;
	SDL_Rect textrect;
	int firstline;
	int maxlines;
	int textlength;
	int highlightline;
	int selectedline;
	SDL_Color textcolor;
	SDL_Color bg;
} kiss_textbox;

SDL_Color kiss_white, kiss_black, kiss_green, kiss_blue, kiss_lightblue;
TTF_Font *kiss_textfont, *kiss_buttonfont;
SDL_Texture *kiss_normal, *kiss_prelight, *kiss_active, *kiss_bar,
	*kiss_up, *kiss_down, *kiss_slider;
int kiss_text_spacing, kiss_text_lineheight, kiss_text_advance,
	kiss_text_fontheight, kiss_button_textlength, kiss_button_advance,
	kiss_button_texty, kiss_screen_width, kiss_screen_height,
	kiss_bar_width, kiss_bar_height, kiss_button_width,
	kiss_button_height, kiss_slider_width, kiss_up_height,
	kiss_down_height, kiss_slider_height, kiss_click_interval,
	kiss_textfont_size, kiss_progress_interval;

int kiss_makerect(SDL_Rect *rect, int x, int y, int h, int w);
int kiss_makecolor(SDL_Color *color, int r, int g, int b, int a);
int kiss_pointinrect(int x, int y, SDL_Rect *rect);
char *kiss_string_copy(char *dest, size_t size, char *source1,
	char *source2);
int kiss_string_compare(const void *a, const void *b);
int kiss_array_new(kiss_array *a);
void *kiss_array_data(kiss_array *a, int index);
int kiss_array_id(kiss_array *a, int index);
int kiss_array_assign(kiss_array *a, int index, int id, void *data);
int kiss_array_append(kiss_array *a, int id, void *data);
int kiss_array_appendstring(kiss_array *a, int id, char *text1, char *text2);
int kiss_array_insert(kiss_array *a, int index, int id, void *data);
int kiss_array_remove(kiss_array *a, int index);
int kiss_array_free(kiss_array *a);
SDL_Texture *kiss_texttexture(char *message, TTF_Font *font,
	SDL_Color color, SDL_Renderer *renderer);
int kiss_rendertexture(SDL_Texture *image, SDL_Renderer *renderer,
	int x, int y, SDL_Rect *clip);
int kiss_draw_triplerect(SDL_Renderer *renderer, SDL_Rect *rect, int edge);
SDL_Renderer* kiss_init(char* title, kiss_array *a);
int kiss_clean(kiss_array *a);
int kiss_window_new(kiss_window *window, int x, int y, int w, int h);
int kiss_window_event(kiss_window *window, SDL_Event *event, int *draw);
int kiss_window_draw(kiss_window *window, SDL_Renderer *renderer,
	kiss_window *w);
int kiss_label_new(kiss_label *label, char *text, int x, int y);
int kiss_label_draw(kiss_label *label, SDL_Renderer *renderer,
	kiss_window *w);
int kiss_button_new(kiss_button *button, char *text, int x, int y);
int kiss_button_event(kiss_button *button, SDL_Event *event, int *draw);
int kiss_button_draw(kiss_button *button, SDL_Renderer *renderer,
	kiss_window *w);
int kiss_scrollbar_new(kiss_scrollbar *scrollbar, int x, int y, int h);
int kiss_scrollbar_event(kiss_scrollbar *scrollbar, SDL_Event *event,
	int *draw);
int kiss_scrollbar_draw(kiss_scrollbar *scrollbar, SDL_Renderer *renderer,
	kiss_window *w);
int kiss_progressbar_new(kiss_progressbar *progressbar, int x, int y, int w);
int kiss_progressbar_event(kiss_progressbar *progressbar, SDL_Event *event,
	int *draw);
int kiss_pogressbar_draw(kiss_progressbar *progressbar,
	SDL_Renderer *renderer, kiss_window *w);
int kiss_entry_new(kiss_entry *entry, char *text, int x, int y, int w);
int kiss_entry_event(kiss_entry *entry, SDL_Event *event, int *draw);
int kiss_entry_draw(kiss_entry *entry, SDL_Renderer *renderer,
	kiss_window *w);
int kiss_textbox_new(kiss_textbox *textbox, kiss_array *a, int x, int y,
	int w, int h);
int kiss_textbox_event(kiss_textbox *textbox, SDL_Event *event, int *draw);
int kiss_textbox_draw(kiss_textbox *textbox, SDL_Renderer *renderer,
	kiss_window *w);

#ifdef __cplusplus
};
#endif

#endif /* _kiss_sdl_h */

