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

int kiss_screen_width = 1024;
int kiss_screen_height = 768;
int kiss_textfont_size = 24;
int kiss_click_interval = 150;
int kiss_progress_interval = 50;
SDL_Color kiss_white = {255, 255, 255, 255};
SDL_Color kiss_black = {0, 0, 0, 255};
SDL_Color kiss_green = {0, 150, 0, 255};
SDL_Color kiss_blue = {0, 0, 255, 255};
SDL_Color kiss_lightblue = {200, 225, 255, 255};

int kiss_makerect(SDL_Rect *rect, int x, int y, int w, int h)
{
	if (!rect) return -1;
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	return 0;
}

int kiss_makecolor(SDL_Color *color, int r, int g, int b, int a)
{
	if (!color) return -1;
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = a;
	return 0;
}

int kiss_pointinrect(int x, int y, SDL_Rect *rect)
{
	return x >= rect->x && x < rect->x + rect->w &&
		y >= rect->y && y < rect->y + rect->h;
}

char *kiss_string_copy(char *dest, size_t size, char *source1, char *source2)
{
	unsigned int size1, size2, len;
	char *p;

	if (!dest) return NULL;
	if (size < 2) return dest;
	size1 = size - 1;
	if (source1) strncpy(dest, source1, size1);
	dest[size1] = 0;
	len = strlen(dest);
	if (!source2 || size1 - len < 1) return dest;
	size2 = size1 - len;
	p = dest;
	strncpy(p + len, source2, size2);
	return dest;
}

int kiss_string_compare(const void *a, const void *b)
{
	return strcmp(*((char **) a), *((char **) b));
}

int kiss_array_new(kiss_array *a)
{
	if (!a) return -1;
	a->size = KISS_MIN_LENGTH;
	a->length = 0;
	a->ref = 1;
	a->data = (void **) malloc(KISS_MIN_LENGTH * sizeof(void *));
	a->id = (int *) malloc(KISS_MIN_LENGTH * sizeof(int));
	return 0;
}

void *kiss_array_data(kiss_array *a, int index)
{
	if (index < 0 || index >= a->size || !a) return NULL;
	return a->data[index];
}

int kiss_array_id(kiss_array *a, int index)
{
	if (!a || index < 0 || index >= a->size) return 0;
	return a->id[index];
}

int kiss_array_assign(kiss_array *a, int index, int id, void *data)
{
	if (!a || index < 0 || index >= a->length) return -1;
	free(a->data[index]);
	a->data[index] = data;
	a->id[index] = id;
	return 0;
}

int kiss_array_append(kiss_array *a, int id, void *data)
{
	int i;

	if (!a) return -1;
	if (a->length >= a->size) {
		a->size *= 2;
		a->data = (void **) realloc(a->data, a->size *
			sizeof(void *));
		a->id = (int *) realloc(a->id, a->size * sizeof(int));
		for (i = a->length; i < a->size; i++) {
			a->data[i] = NULL;
			a->id[i] = 0;
		}
	}
	a->data[a->length] = data;
	a->id[a->length] = id;
	++a->length;
	return 0;
}

int kiss_array_appendstring(kiss_array *a, int id, char *text1, char *text2)
{
	char *p;

	if (!a) return -1;
	p = (char *) malloc(KISS_MAX_LENGTH);
	kiss_string_copy(p, KISS_MAX_LENGTH, text1, text2);
	kiss_array_append(a, id, p);
	return 0;
}

int kiss_array_insert(kiss_array *a, int index, int id, void *data)
{
	int i;

	if (!a || index < 0 || index >= a->length) return -1;
	if (a->length >= a->size) {
		a->size *= 2;
		a->data = (void **) realloc(a->data, a->size *
			sizeof(void *));
		a->id = (int *) realloc(a->id, a->size * sizeof(int));
		for (i = a->length; i < a->size; i++) {
			a->data[i] = NULL;
			a->id[i] = 0;
		}
	}
	for (i = a->length - 1; i >= index; i--) {
		a->data[i + 1] = a->data[i];
		a->id[i + 1] = a->id[i];
	}
	a->data[index] = data;
	a->id[index] = id;
	++a->length;
	return 0;
}

int kiss_array_remove(kiss_array *a, int index)
{
	int i;

	if (!a || index < 0 || index >= a->length) return -1;
	free(a->data[index]);
	for (i = index; i < a->length - 1; i++) {
		a->data[i] = a->data[i + 1];
		a->id[i] = a->id[i + 1];
	}
	a->data[a->length - 1] = NULL;
	a->id[a->length - 1] = 0;
	--a->length;
	return 0;
}

int kiss_array_free(kiss_array *a)
{
	int i;

	if (!a || !a->ref) return -1;
	if (a->ref > 1) {
		--a->ref;
		return 0;
	}
	if (a->length)
		for (i = 0; i < a->length; i++)
			free (a->data[i]);
	free(a->data);
	free(a->id);
	a->data = NULL;
	a->id = NULL;
	a->size = 0;
	a->length = 0;
	a->ref = 0;
	return 0;
}

SDL_Texture* kiss_texttexture(char *message, TTF_Font *font,
	SDL_Color color, SDL_Renderer *renderer)
{
	SDL_Surface *surface;
	SDL_Texture *texture;

	if (!message || !font || !renderer) return NULL;
	surface = TTF_RenderText_Blended(font, message, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (surface) SDL_FreeSurface(surface);
	return texture;
}

int kiss_rendertexture(SDL_Texture *image, SDL_Renderer *renderer,
	int x, int y, SDL_Rect *clip)
{
	SDL_Rect dst;

	if (!image || !renderer) return -1;
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(image, NULL, NULL, &dst.w, &dst.h);
	if (clip) dst.w = clip->w;
	if (clip) dst.h = clip->h;
	SDL_RenderCopy(renderer, image, clip, &dst);
	return 0;
}

int kiss_draw_triplerect(SDL_Renderer *renderer, SDL_Rect *rect, int edge)
{
	SDL_Rect outlinerect;
	int d, i;

	d = 2 * edge;
	if (!renderer || !rect || rect->w < d + 6 || rect->h < d + 6)
		return -1;
	for (i = 0; i < 3; i++) {
		kiss_makerect(&outlinerect, rect->x + edge + i,
			rect->y + edge + i, rect->w - d - i - i,
			rect->h - d - i - i);
		SDL_RenderDrawRect(renderer, &outlinerect);
	}
	return 0;
}

SDL_Renderer* kiss_init(char* title, kiss_array *a)
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	int button_fontheight;

	if (!a) return NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	window = SDL_CreateWindow(title, 100, 100, kiss_screen_width,
		kiss_screen_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	kiss_array_new(a);
	kiss_array_append(a, WINDOW_TYPE, window);
	kiss_array_append(a, RENDERER_TYPE, renderer);
	kiss_textfont = TTF_OpenFont("kiss_font.ttf", kiss_textfont_size);
	kiss_array_append(a, FONT_TYPE, kiss_textfont);
	kiss_text_fontheight = TTF_FontHeight(kiss_textfont);
	kiss_text_spacing = (int) KISS_SPACING * kiss_text_fontheight;
	kiss_text_lineheight = kiss_text_fontheight + kiss_text_spacing;
	TTF_GlyphMetrics(kiss_textfont, 'W', NULL, NULL, NULL, NULL,
		&kiss_text_advance);
	kiss_buttonfont = TTF_OpenFont("kiss_font.ttf", 18);
	kiss_array_append(a, FONT_TYPE, kiss_buttonfont);
	button_fontheight = TTF_FontHeight(kiss_buttonfont);
	TTF_GlyphMetrics(kiss_buttonfont, 'W', NULL, NULL, NULL, NULL,
		&kiss_button_advance);
	kiss_normal = IMG_LoadTexture(renderer, "kiss_normal.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_normal);
	SDL_QueryTexture(kiss_normal, NULL, NULL, &kiss_button_width,
		&kiss_button_height);
	kiss_button_textlength = kiss_button_width / kiss_button_advance;
	kiss_button_texty = kiss_button_height / 2 - button_fontheight / 2;
	kiss_prelight = IMG_LoadTexture(renderer, "kiss_prelight.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_prelight);
	kiss_active = IMG_LoadTexture(renderer, "kiss_active.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_active);
	kiss_bar = IMG_LoadTexture(renderer, "kiss_bar.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_bar);
	SDL_QueryTexture(kiss_bar, NULL, NULL, &kiss_bar_width,
		&kiss_bar_height);
	kiss_up = IMG_LoadTexture(renderer, "kiss_up.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_up);
	SDL_QueryTexture(kiss_up, NULL, NULL, &kiss_slider_width,
		&kiss_up_height);
	kiss_down = IMG_LoadTexture(renderer, "kiss_down.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_down);
	SDL_QueryTexture(kiss_down, NULL, NULL, &kiss_slider_width,
		&kiss_down_height);
	kiss_slider = IMG_LoadTexture(renderer, "kiss_slider.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_slider);
	SDL_QueryTexture(kiss_slider, NULL, NULL, &kiss_slider_width,
		&kiss_slider_height);
	SDL_StartTextInput();
	return renderer;	
}

int kiss_clean(kiss_array *a)
{
	int i;

	if (!a) return -1;
	if (a->length)
		for (i = 0; i < a->length; i++) {
			if (kiss_array_id(a, i) == FONT_TYPE)
				TTF_CloseFont((TTF_Font *)
					kiss_array_data(a, i));
			else if (kiss_array_id(a, i) == TEXTURE_TYPE)
				SDL_DestroyTexture((SDL_Texture *)
					kiss_array_data(a, i));
			else if (kiss_array_id(a, i) == RENDERER_TYPE)
				SDL_DestroyRenderer((SDL_Renderer *)
					kiss_array_data(a, i));
			else if (kiss_array_id(a, i) == WINDOW_TYPE)
				SDL_DestroyWindow((SDL_Window *)
					kiss_array_data(a, i));
			else if (kiss_array_id(a, i) == ARRAY_TYPE)
				kiss_array_free((kiss_array *)
					kiss_array_data(a, i));
			else
				free (a->data[i]);
		}
	a->length = 0;
	kiss_array_free(a);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}

int kiss_window_new(kiss_window *window, int x, int y, int w, int h)
{
	if (!window) return -1;
	kiss_makecolor(&window->bg, kiss_white.r, kiss_white.g,
		kiss_white.b, kiss_white.a);
	kiss_makerect(&window->rect, x, y, w, h);
	window->visible = 0;
	window->focus = 1;
	return 0;
}

int kiss_window_event(kiss_window *window, SDL_Event *event, int *draw)
{
	if (!window || !window->visible || !event) return 0;
	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_EXPOSED)
		*draw = 1;
	else if (event->type == SDL_MOUSEBUTTONDOWN &&
		kiss_pointinrect(event->button.x, event->button.y,
		&window->rect))
		return 1;
	return 0;
}

int kiss_window_draw(kiss_window *window, SDL_Renderer *renderer,
	kiss_window *w)
{
	if (window && w) window->visible = w->visible;
	if (!window || !window->visible || !renderer) return 0;
	SDL_SetRenderDrawColor(renderer, window->bg.r, window->bg.g,
		window->bg.b, window->bg.a);
	SDL_RenderFillRect(renderer, &window->rect);
	SDL_SetRenderDrawColor(renderer, kiss_blue.r, kiss_blue.g,
		kiss_blue.b, kiss_blue.a);
	kiss_draw_triplerect(renderer, &window->rect, KISS_FRAME_EDGE);
	return 1;
}

int kiss_label_new(kiss_label *label, char *text, int x, int y)
{
	if (!label || !text) return -1;
	kiss_makecolor(&label->textcolor, kiss_black.r, kiss_black.g,
		kiss_black.b, kiss_black.a);
	kiss_makerect(&label->rect, x, y, 0, 0);
	kiss_string_copy(label->text, KISS_MAX_LABEL, text, NULL);
	label->visible = 0;
	return 0;
}

int kiss_label_draw(kiss_label *label, SDL_Renderer *renderer,
	kiss_window *w)
{
	SDL_Texture *image;
	char buf[KISS_MAX_LABEL], *p;
	int len, y;

	if (label && w) label->visible = w->visible;
	if (!label || !label->visible || !renderer) return 0;
	y = label->rect.y + kiss_text_spacing / 2;
	len = strlen(label->text);
	if (len > KISS_MAX_LABEL - 2)
		label->text[len - 1] = '\n';
	else
		strcat(label->text, "\n");
	for (p = label->text; *p; p = strchr(p, '\n') + 1) {
		kiss_string_copy(buf, strcspn(p, "\n") + 1, p, NULL);
		image = kiss_texttexture(buf, kiss_textfont,
			label->textcolor, renderer);
		kiss_rendertexture(image, renderer, label->rect.x, y, NULL);
		SDL_DestroyTexture(image);
		y += kiss_text_lineheight;
	}
	label->text[len] = 0;
	return 1;
}

int kiss_button_new(kiss_button *button, char *text, int x, int y)
{
	if (!button || !text) return -1;
	kiss_makerect(&button->rect, x, y, kiss_button_width,
		kiss_button_height);
	kiss_makecolor(&button->textcolor, kiss_white.r, kiss_white.g,
		kiss_white.b, kiss_white.a);
	kiss_string_copy(button->text, KISS_MAX_LENGTH, text, NULL);
	button->textx = x + kiss_button_width / 2 - strlen(text) *
		kiss_button_advance / 2;
	button->texty = y + kiss_button_texty;
	button->active = 0;
	button->prelight = 0;
	button->visible = 0;
	button->focus = 1;
	return 0;
}

int kiss_button_event(kiss_button *button, SDL_Event *event, int *draw)
{
	if (!button || !button->visible || !button->focus || !event)
		return 0;
	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_EXPOSED)
		*draw = 1;
	else if (event->type == SDL_MOUSEBUTTONDOWN &&
		kiss_pointinrect(event->button.x, event->button.y,
		&button->rect)) {
		button->active = 1;
		*draw = 1;
	} else if (event->type == SDL_MOUSEBUTTONUP &&
		kiss_pointinrect(event->button.x, event->button.y,
		&button->rect)) {
		button->active = 0;
		*draw = 1;
		return 1;
	} else if (event->type == SDL_MOUSEMOTION &&
		kiss_pointinrect(event->motion.x, event->motion.y,
		&button->rect)) {
		button->prelight = 1;
		*draw = 1;
	} else if (event->type == SDL_MOUSEMOTION &&
		!kiss_pointinrect(event->motion.x, event->motion.y,
		&button->rect)) {
		button->prelight = 0;
		*draw = 1;
		if (button->active) {
			button->active = 0;
			return 1;
		}
	}
	return 0;
}

int kiss_button_draw(kiss_button *button, SDL_Renderer *renderer,
	kiss_window *w)
{
	SDL_Texture *image;

	if (button && w) {
		button->visible = w->visible;
		button->focus = w->focus;
	}
	if (!button || !button->visible || !renderer) return 0;
	if (button->active)
		kiss_rendertexture(kiss_active, renderer, button->rect.x,
			button->rect.y, NULL);
	else if (button->prelight && !button->active)
		kiss_rendertexture(kiss_prelight, renderer, button->rect.x,
			button->rect.y, NULL);
	else
		kiss_rendertexture(kiss_normal, renderer, button->rect.x,
			button->rect.y, NULL);
	image = kiss_texttexture(button->text, kiss_buttonfont,
		button->textcolor, renderer);
	kiss_rendertexture(image, renderer, button->textx, button->texty,
		NULL);
	SDL_DestroyTexture(image);
	return 1;
}

int kiss_scrollbar_new(kiss_scrollbar *scrollbar, int x, int y, int h)
{
	if (!scrollbar || kiss_up_height + kiss_down_height +
		kiss_slider_height + 2 * KISS_SLIDER_PADDING > h)
		return -1;
	kiss_makerect(&scrollbar->rect, x, y, kiss_slider_width, h);
	kiss_makerect(&scrollbar->uprect, x, y, kiss_slider_width,
		kiss_up_height + KISS_SLIDER_PADDING);
	kiss_makerect(&scrollbar->downrect, x, y + h - kiss_down_height -
		KISS_SLIDER_PADDING, kiss_slider_width, kiss_down_height +
		KISS_SLIDER_PADDING);
	kiss_makerect(&scrollbar->sliderrect, x, y + scrollbar->uprect.h,
		kiss_slider_width, kiss_slider_height);
	scrollbar->maxpos = h - 2 * KISS_SLIDER_PADDING - kiss_up_height -
		kiss_down_height - kiss_slider_height;
	scrollbar->fraction = 0.;
	scrollbar->step = 0.1;
	scrollbar->upclicked = 0;
	scrollbar->downclicked = 0;
	scrollbar->lasttick = 0;
	scrollbar->visible = 0;
	scrollbar->focus = 1;
	return 0;
}

int kiss_scrollbar_event(kiss_scrollbar *scrollbar, SDL_Event *event,
	int *draw)
{
	int pos;

	if (!scrollbar || !scrollbar->visible || !scrollbar->focus) return 0;
	pos = (int) (scrollbar->maxpos * scrollbar->fraction + 0.5);
	if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))) {
		scrollbar->upclicked = 0;
		scrollbar->downclicked = 0;
		scrollbar->lasttick = 0;
	} else if (scrollbar->upclicked && SDL_GetTicks() >
		scrollbar->lasttick + kiss_click_interval &&
		scrollbar->fraction >= 0.) {
		if (scrollbar->fraction - scrollbar->step >= 0)
			scrollbar->fraction -= scrollbar->step;
		else {
			scrollbar->fraction = 0.;
			scrollbar->upclicked = 0;
		}
		pos = (int) (scrollbar->fraction * scrollbar->maxpos);
		scrollbar->sliderrect.y = scrollbar->uprect.y +
			scrollbar->uprect.h + pos;
		scrollbar->lasttick = SDL_GetTicks();
		*draw = 1;
		return 1;
	} else if (scrollbar->downclicked && SDL_GetTicks() >
		scrollbar->lasttick + kiss_click_interval &&
		scrollbar->fraction <= 1.) {
		if (scrollbar->fraction + scrollbar->step <= 1)
			scrollbar->fraction += scrollbar->step;
		else {
			scrollbar->fraction = 1.;
			scrollbar->downclicked = 0;
		}
		pos = (int) (scrollbar->fraction * scrollbar->maxpos);
		scrollbar->sliderrect.y = scrollbar->uprect.y +
			scrollbar->uprect.h + pos;
		scrollbar->lasttick = SDL_GetTicks();
		*draw = 1;
		return 1;
	}
	if (!event) return 0;
	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_EXPOSED)
		*draw = 1;
	else if (event->type == SDL_MOUSEBUTTONDOWN &&
		kiss_pointinrect(event->button.x, event->button.y,
		&scrollbar->uprect)) {
		if (scrollbar->fraction > 0.) scrollbar->upclicked = 1;
		scrollbar->lasttick = SDL_GetTicks() -
			kiss_click_interval - 1;
	} else if (event->type == SDL_MOUSEBUTTONDOWN &&
		kiss_pointinrect(event->button.x, event->button.y,
		&scrollbar->downrect)) {
		if (scrollbar->fraction < 1.) scrollbar->downclicked = 1;
		scrollbar->lasttick = SDL_GetTicks() -
			kiss_click_interval - 1;
	} else if (event->type == SDL_MOUSEBUTTONUP) {
		scrollbar->upclicked = 0;
		scrollbar->downclicked = 0;
		scrollbar->lasttick = 0;
	} else if (event->type == SDL_MOUSEMOTION &&
		(event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
		kiss_pointinrect(event->motion.x - event->motion.xrel,
		event->motion.y - event->motion.yrel,
		&scrollbar->sliderrect) && event->motion.yrel < 0) {
		if (scrollbar->fraction == 0.) return 0;
		if (pos + event->motion.yrel >= 0)
			pos += event->motion.yrel;
		else
			pos = 0;
		scrollbar->fraction = 1. * pos / scrollbar->maxpos;
		scrollbar->sliderrect.y = scrollbar->uprect.y +
			scrollbar->uprect.h + pos;
		*draw = 1;
		return 1;
	} else if (event->type == SDL_MOUSEMOTION &&
		(event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
		kiss_pointinrect(event->motion.x - event->motion.xrel,
		event->motion.y - event->motion.yrel,
		&scrollbar->sliderrect) && event->motion.yrel > 0) {
		if (scrollbar->fraction == 1.) return 0;
		if (pos + event->motion.yrel <= scrollbar->maxpos)
			pos += event->motion.yrel;
		else
			pos = scrollbar->maxpos;
		scrollbar->fraction = 1. * pos / scrollbar->maxpos;
		scrollbar->sliderrect.y = scrollbar->uprect.y +
			scrollbar->uprect.h + pos;
		*draw = 1;
		return 1;
	}
	return 0;
}

int kiss_scrollbar_draw(kiss_scrollbar *scrollbar, SDL_Renderer *renderer,
	kiss_window *w)
{
	int pos;

	if (scrollbar && w) {
		scrollbar->visible = w->visible;
		scrollbar->focus = w->focus;
	}
	if (!scrollbar || !scrollbar->visible || !renderer) return 0;
	pos = (int) (scrollbar->fraction * scrollbar->maxpos);
	scrollbar->sliderrect.y = scrollbar->uprect.y +
		scrollbar->uprect.h + pos;
	kiss_rendertexture(kiss_up, renderer, scrollbar->uprect.x,
		scrollbar->uprect.y, NULL);
	kiss_rendertexture(kiss_down, renderer, scrollbar->downrect.x,
		scrollbar->downrect.y + KISS_SLIDER_PADDING, NULL);
	kiss_rendertexture(kiss_slider, renderer, scrollbar->sliderrect.x,
		scrollbar->sliderrect.y, NULL);
	return 1;
}

int kiss_progressbar_new(kiss_progressbar *progressbar, int x, int y, int w)
{
	if (!progressbar || w < 2 * KISS_BORDER + 1) return -1;
	kiss_makecolor(&progressbar->bg, kiss_white.r, kiss_white.g,
		kiss_white.b, kiss_white.a);
	kiss_makerect(&progressbar->rect, x, y, w,
		kiss_bar_height + 2 * KISS_BORDER);
	kiss_makerect(&progressbar->barrect, x + KISS_BORDER, y + KISS_BORDER,
		0, kiss_bar_height);
	progressbar->width = w - 2 * KISS_BORDER;
	progressbar->fraction = 0.;
	progressbar->step = 0.02;
	progressbar->lasttick = 0;
	progressbar->run = 0;
	progressbar->visible = 0;
	return 0;
}

int kiss_progressbar_event(kiss_progressbar *progressbar, SDL_Event *event,
	int *draw)
{
	if (!progressbar || !progressbar->visible) return 0;
	if (progressbar->run && SDL_GetTicks() > progressbar->lasttick +
		kiss_progress_interval) {
		progressbar->fraction += progressbar->step;
		if (progressbar->fraction > 1.) {
			progressbar->run = 0;
			progressbar->fraction = 1.;
		}
		progressbar->lasttick = SDL_GetTicks();
		*draw = 1;
	}
	return 1;
}

int kiss_pogressbar_draw(kiss_progressbar *progressbar,
	SDL_Renderer *renderer, kiss_window *w)
{
	SDL_Rect clip;

	if (progressbar && w) progressbar->visible = w->visible;
	if (!progressbar || !progressbar->visible || !renderer)
		return 0;
	SDL_SetRenderDrawColor(renderer, progressbar->bg.r,
		progressbar->bg.g, progressbar->bg.b, progressbar->bg.a);
	SDL_RenderFillRect(renderer, &progressbar->rect);
	SDL_SetRenderDrawColor(renderer, kiss_blue.r, kiss_blue.g,
		kiss_blue.b, kiss_blue.a);
	kiss_draw_triplerect(renderer, &progressbar->rect, KISS_FRAME_EDGE);
	progressbar->barrect.w = (int) (progressbar->width *
		progressbar->fraction + 0.5);
	kiss_makerect(&clip, 0, 0, progressbar->barrect.w,
		progressbar->barrect.h);
	kiss_rendertexture(kiss_bar, renderer, progressbar->barrect.x,
		progressbar->barrect.y, &clip);
	return 1;
}

int kiss_entry_new(kiss_entry *entry, char *text, int x, int y, int w)
{
	if (!entry || w < 2 * KISS_BORDER + kiss_text_advance || !text)
		return -1;
	kiss_makecolor(&entry->bg, kiss_white.r, kiss_white.g,
		kiss_white.b, kiss_white.a);
	kiss_makecolor(&entry->textcolor, kiss_black.r, kiss_black.g,
		kiss_black.b, kiss_black.a);
	kiss_string_copy(entry->text, KISS_MAX_LENGTH, text, NULL);
	kiss_makerect(&entry->rect, x, y, w, kiss_text_fontheight +
		2 * KISS_BORDER);
	entry->textx = x + KISS_BORDER;
	entry->texty = y + KISS_BORDER;
	entry->textlength = (w - 2 * KISS_BORDER) / kiss_text_advance;
	entry->active = 0;
	entry->visible = 0;
	entry->focus = 1;
	return 0;
}

int kiss_entry_event(kiss_entry *entry, SDL_Event *event, int *draw)
{
	int len;

	if (!entry || !entry->visible || !entry->focus || !event) return 0;
	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_EXPOSED)
		*draw = 1;
	else if (event->type == SDL_MOUSEBUTTONDOWN &&
		kiss_pointinrect(event->button.x, event->button.y,
		&entry->rect)) {
		entry->active = 1;
		*draw = 1;
	} else if (event->type == SDL_KEYDOWN && entry->active &&
		event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
		entry->active = 0;
		*draw = 1;
		return 1;
	} else if (event->type == SDL_MOUSEBUTTONDOWN &&
		!kiss_pointinrect(event->button.x, event->button.y,
		&entry->rect)) {
		entry->active = 0;
		*draw = 1;
		return 1;
	} else if (event->type == SDL_TEXTINPUT && entry->active) {
		if (strlen(entry->text) + strlen(event->text.text) <
			entry->textlength)
			strcat(entry->text, event->text.text);
		*draw = 1;
	} else if (event->type == SDL_KEYDOWN && entry->active &&
		event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
		len = strlen(entry->text);
		if (len) entry->text[len - 1] = 0;
		*draw = 1;
	} else if (event->type == SDL_KEYDOWN && entry->active &&
		(event->key.keysym.mod & KMOD_CTRL) &&
		event->key.keysym.scancode == SDL_SCANCODE_U) {
		strcpy(entry->text, "");
		*draw = 1;
	}
	return 0;
}

int kiss_entry_draw(kiss_entry *entry, SDL_Renderer *renderer,
	kiss_window *w)
{
	SDL_Texture *image;

	if (entry && w) {
		entry->visible = w->visible;
		entry->focus = w->focus;
	}
	if (!entry || !entry->visible || !renderer) return 0;
	SDL_SetRenderDrawColor(renderer, entry->bg.r,
		entry->bg.g, entry->bg.b, entry->bg.a);
	SDL_RenderFillRect(renderer, &entry->rect);
	if (entry->active)
		SDL_SetRenderDrawColor(renderer, kiss_green.r, kiss_green.g,
			kiss_green.b, kiss_green.a);
	else
		SDL_SetRenderDrawColor(renderer, kiss_blue.r, kiss_blue.g,
			kiss_blue.b, kiss_blue.a);
	kiss_draw_triplerect(renderer, &entry->rect, KISS_FRAME_EDGE);
	image = kiss_texttexture(entry->text, kiss_textfont,
		entry->textcolor, renderer);
	kiss_rendertexture(image, renderer, entry->textx, entry->texty,
		NULL);
	SDL_DestroyTexture(image);
	return 1;
}

int kiss_textbox_new(kiss_textbox *textbox, kiss_array *a, int x, int y,
	int w, int h)
{
	if (!textbox || !a || h - 2 * KISS_BORDER < kiss_text_lineheight)
		return -1;
	kiss_makecolor(&textbox->bg, kiss_white.r, kiss_white.g,
		kiss_white.b, kiss_white.a);
	kiss_makecolor(&textbox->textcolor, kiss_black.r, kiss_black.g,
		kiss_black.b, kiss_black.a);
	kiss_makerect(&textbox->rect, x, y, w, h);
	kiss_makerect(&textbox->textrect, x + KISS_BORDER,
		y + KISS_BORDER, w - 2 * KISS_BORDER, h - 2 * KISS_BORDER);
	textbox->array = a;
	textbox->firstline = 0;
	textbox->maxlines = (h - 2 * KISS_BORDER) / kiss_text_lineheight;
	textbox->textlength = (h - 2 * KISS_BORDER) / kiss_text_advance;
	textbox->highlightline = -1;
	textbox->selectedline = -1;
	textbox->visible = 0;
	textbox->focus = 1;
	return 0;
}

static int kiss_textbox_numoflines(kiss_textbox *textbox)
{
	int numoflines;

	numoflines = textbox->maxlines;
	if (textbox->array->length - textbox->firstline < textbox->maxlines)
		numoflines = textbox->array->length - textbox->firstline;
	return numoflines;
}

int kiss_textbox_event(kiss_textbox *textbox, SDL_Event *event, int *draw)
{
	int texty, textmaxy, numoflines;

	if (!textbox || !textbox->visible || !textbox->focus || !event ||
		!textbox->array || !textbox->array->length)
		return 0;
	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_EXPOSED)
		*draw = 1;
	else if (event->type == SDL_MOUSEBUTTONDOWN &&
		kiss_pointinrect(event->button.x, event->button.y,
		&textbox->textrect)) {
		numoflines = kiss_textbox_numoflines(textbox);
		texty = event->button.y - textbox->textrect.y;
		textmaxy = numoflines * kiss_text_lineheight;
		if (texty < textmaxy) {
			textbox->selectedline = texty / kiss_text_lineheight;
			return 1;
		}
	} else if (event->type == SDL_MOUSEMOTION &&
		kiss_pointinrect(event->motion.x, event->motion.y,
		&textbox->textrect)) {
		numoflines = kiss_textbox_numoflines(textbox);
		texty = event->motion.y - textbox->textrect.y;
		textmaxy = numoflines * kiss_text_lineheight;
		textbox->highlightline = -1;
		if (texty < textmaxy)
			textbox->highlightline =
				texty / kiss_text_lineheight;
		*draw = 1;
	} else if (event->type == SDL_MOUSEMOTION &&
		!kiss_pointinrect(event->motion.x, event->motion.y,
		&textbox->textrect)) {
		textbox->highlightline = -1;
		*draw = 1;
	}
	return 0;
}

int kiss_textbox_draw(kiss_textbox *textbox, SDL_Renderer *renderer,
	kiss_window *w)
{
	SDL_Texture *image;
	SDL_Rect highlightrect;
	char buf[KISS_MAX_LENGTH];
	int numoflines, i;

	if (textbox && w) {
		textbox->visible = w->visible;
		textbox->focus = w->focus;
	}
	if (!textbox || !textbox->visible || !renderer) return 0;
	SDL_SetRenderDrawColor(renderer, textbox->bg.r,
		textbox->bg.g, textbox->bg.b, textbox->bg.a);
	SDL_RenderFillRect(renderer, &textbox->rect);
	SDL_SetRenderDrawColor(renderer, kiss_blue.r, kiss_blue.g,
		kiss_blue.b, kiss_blue.a);
	kiss_draw_triplerect(renderer, &textbox->rect, KISS_FRAME_EDGE);
	if (textbox->highlightline >= 0) {
		SDL_SetRenderDrawColor(renderer, kiss_lightblue.r,
			kiss_lightblue.g, kiss_lightblue.b,
			kiss_lightblue.a);
		kiss_makerect(&highlightrect, textbox->textrect.x,
			textbox->textrect.y +
			textbox->highlightline * kiss_text_lineheight,
			textbox->textrect.w, kiss_text_lineheight);
		SDL_RenderFillRect(renderer, &highlightrect);
	}
	if (!textbox->array || !textbox->array->length) return 0;
	numoflines = kiss_textbox_numoflines(textbox);
	for (i = 0; i < numoflines; i++) {
		kiss_string_copy(buf, textbox->textlength, 
			(char *) kiss_array_data(textbox->array,
			textbox->firstline + i), NULL);
		image = kiss_texttexture(buf, kiss_textfont,
			textbox->textcolor, renderer);
		kiss_rendertexture(image, renderer, textbox->textrect.x,
			textbox->textrect.y + i * kiss_text_lineheight +
			kiss_text_spacing / 2, NULL);
		SDL_DestroyTexture(image);
	}
	return 1;
}

