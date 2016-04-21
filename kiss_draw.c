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

  kiss_sdl version 0.8.8
*/

#include "kiss_sdl.h"

int kiss_textfont_size = 15;
int kiss_buttonfont_size = 12;
int kiss_click_interval = 140;
int kiss_progress_interval = 50;
int kiss_slider_padding = 2;
int kiss_edge = 2;
int kiss_border = 6;
double kiss_spacing = 0.5;
SDL_Color kiss_white = {255, 255, 255, 255};
SDL_Color kiss_black = {0, 0, 0, 255};
SDL_Color kiss_green = {0, 150, 0, 255};
SDL_Color kiss_blue = {0, 0, 255, 255};
SDL_Color kiss_lightblue = {200, 225, 255, 255};

/* Can be rewritten for proportional fonts or unicode */
int kiss_maxlength(int ftype, int width, char *str)
{
	if (ftype == TEXT_FONT && width /
		kiss_text_advance < KISS_MAX_LENGTH)
		/* Buffer length, text plus terminating '\0' */
		return width / kiss_text_advance + 1;
	else if (ftype == BUTTON_FONT && width /
		kiss_button_advance < KISS_MAX_LENGTH)
		return width / kiss_button_advance + 1;
	return -1;
}

/* Can be rewritten for proportional fonts or unicode */
int kiss_textwidth(int ftype, char *str1, char *str2)
{
	int length;

	length = 0;
	if (!str1 && !str2) return -1;
	if (str1) length += strlen(str1);
	if (str2) length += strlen(str2);
	if (ftype == TEXT_FONT)
		return length * kiss_text_advance;
	else if (ftype == BUTTON_FONT)
		return length * kiss_button_advance;
	return -1;
}

int kiss_rendertexture(SDL_Renderer *renderer, SDL_Texture *image,
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

int kiss_rendertext(SDL_Renderer *renderer, char *text, int x, int y,
	int ftype, SDL_Color color)
{
	SDL_Surface *surface;
	SDL_Texture *texture;

	if (!text || !renderer) return -1;
	if (ftype == TEXT_FONT)
		surface = TTF_RenderText_Blended(kiss_textfont, text, color);
	else if (ftype == BUTTON_FONT)
		surface = TTF_RenderText_Blended(kiss_buttonfont, text,
			color);
	else
		return -1;
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (surface) SDL_FreeSurface(surface);
	kiss_rendertexture(renderer, texture, x, y, NULL);
	SDL_DestroyTexture(texture);
	return 0;
}

int kiss_fillrect(SDL_Renderer *renderer, SDL_Rect *rect, SDL_Color color)
{
	if (!renderer || !rect) return -1;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, rect);
	return 0;
}

int kiss_decorate(SDL_Renderer *renderer, SDL_Rect *rect, SDL_Color color,
	int edge)
{
	SDL_Rect outlinerect;
	int d, i;

	d = 2 * edge;
	if (!renderer || !rect || rect->w < d + 6 || rect->h < d + 6)
		return -1;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (i = 0; i < 2; i++) {
		kiss_makerect(&outlinerect, rect->x + edge + i,
			rect->y + edge + i, rect->w - d - i - i,
			rect->h - d - i - i);
		SDL_RenderDrawRect(renderer, &outlinerect);
	}
	return 0;
}

SDL_Renderer* kiss_init(char* title, kiss_array *a, int w, int h)
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Rect srect;
	int button_fontheight;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GetDisplayBounds(0, &srect);
	if (!a || w > srect.w || h > srect.h) {
		SDL_Quit();
		return NULL;
	}
	kiss_screen_width = w;
	kiss_screen_height = h;
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	window = SDL_CreateWindow(title, srect.w / 2 - w / 2,
		srect.h / 2 - h / 2, w, h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	kiss_array_new(a);
	kiss_array_append(a, WINDOW_TYPE, window);
	kiss_array_append(a, RENDERER_TYPE, renderer);
	kiss_textfont = TTF_OpenFont("kiss_font.ttf", kiss_textfont_size);
	kiss_array_append(a, FONT_TYPE, kiss_textfont);
	kiss_text_fontheight = TTF_FontHeight(kiss_textfont);
	kiss_text_spacing = (int) kiss_spacing * kiss_text_fontheight;
	kiss_text_lineheight = kiss_text_fontheight + kiss_text_spacing;
	kiss_text_descent = 1 - TTF_FontDescent(kiss_textfont);
	TTF_GlyphMetrics(kiss_textfont, 'W', NULL, NULL, NULL, NULL,
		&kiss_text_advance);
	kiss_buttonfont = TTF_OpenFont("kiss_font.ttf",
		kiss_buttonfont_size);
	kiss_array_append(a, FONT_TYPE, kiss_buttonfont);
	button_fontheight = TTF_FontHeight(kiss_buttonfont);
	TTF_GlyphMetrics(kiss_buttonfont, 'W', NULL, NULL, NULL, NULL,
		&kiss_button_advance);
	kiss_normal = IMG_LoadTexture(renderer, "kiss_normal.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_normal);
	SDL_QueryTexture(kiss_normal, NULL, NULL, &kiss_button_width,
		&kiss_button_height);
	kiss_button_texty = kiss_button_height / 2 - button_fontheight / 2;
	kiss_prelight = IMG_LoadTexture(renderer, "kiss_prelight.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_prelight);
	kiss_active = IMG_LoadTexture(renderer, "kiss_active.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_active);
	kiss_bar = IMG_LoadTexture(renderer, "kiss_bar.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_bar);
	SDL_QueryTexture(kiss_bar, NULL, NULL, &kiss_bar_width,
		&kiss_bar_height);
	kiss_vslider = IMG_LoadTexture(renderer, "kiss_vslider.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_vslider);
	SDL_QueryTexture(kiss_vslider, NULL, NULL, &kiss_vslider_width,
		&kiss_vslider_height);
	kiss_up_height = kiss_vslider_width;
	kiss_down_height = kiss_vslider_width;
	kiss_hslider = IMG_LoadTexture(renderer, "kiss_hslider.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_hslider);
	SDL_QueryTexture(kiss_hslider, NULL, NULL, &kiss_hslider_width,
		&kiss_hslider_height);
	kiss_left_width = kiss_hslider_height;
	kiss_right_width = kiss_hslider_height;
	kiss_up = IMG_LoadTexture(renderer, "kiss_up.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_up);
	kiss_down = IMG_LoadTexture(renderer, "kiss_down.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_down);
	kiss_left = IMG_LoadTexture(renderer, "kiss_left.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_left);
	kiss_right = IMG_LoadTexture(renderer, "kiss_right.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_right);
	kiss_selected = IMG_LoadTexture(renderer, "kiss_selected.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_selected);
	SDL_QueryTexture(kiss_selected, NULL, NULL, &kiss_selected_width,
		&kiss_selected_height);
	kiss_unselected = IMG_LoadTexture(renderer, "kiss_unselected.png");
	kiss_array_append(a, TEXTURE_TYPE, kiss_unselected);
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

