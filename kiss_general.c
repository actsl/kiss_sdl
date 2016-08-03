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

int kiss_makerect(SDL_Rect *rect, int x, int y, int w, int h)
{
	if (!rect) return -1;
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	return 0;
}

int kiss_pointinrect(int x, int y, SDL_Rect *rect)
{
	return x >= rect->x && x < rect->x + rect->w &&
		y >= rect->y && y < rect->y + rect->h;
}

int kiss_utf8next(char *str, int index)
{
	int i;

	if (!str || index < 0) return -1;
	if (!str[index]) return 0;
	for (i = 1; str[index + i]; i++)
		if ((str[index + i] & 128) == 0 ||
			(str[index + i] & 224) == 192 ||
			(str[index + i] & 240) == 224 ||
			(str[index + i] & 248) == 240)
			break;
	return i;
}

int kiss_utf8prev(char *str, int index)
{
	int i;

	if (!str || index < 0) return -1;
	if (!index) return 0;
	for (i = 1; index - i; i++)
		if ((str[index - i] & 128) == 0 ||
			(str[index - i] & 224) == 192 ||
			(str[index - i] & 240) == 224 ||
			(str[index - i] & 248) == 240)
			break;
	return i;
}

int kiss_utf8fix(char *str)
{
	int len, i;

	if (!str || !str[0]) return -1;
	len = strlen(str);
	for (i = len - 1; i >= 0 && len - 1 - i < 3; i--) {
		if ((str[i] & 224) == 192 && len - 1 - i < 1) str[i] = 0;
		if ((str[i] & 240) == 224 && len - 1 - i < 2) str[i] = 0;
		if ((str[i] & 248) == 240 && len - 1 - i < 3) str[i] = 0;
	}
	return 0;
}

char *kiss_string_copy(char *dest, size_t size, char *str1, char *str2)
{
	unsigned int len;
	char *p;

	if (!dest) return NULL;
	strcpy(dest, "");
	if (size < 2) return dest;
	if (str1) strncpy(dest, str1, size);
	dest[size - 1] = 0;
	len = strlen(dest);
	if (!str2 || size - 1 <= len) return dest;
	p = dest;
	strncpy(p + len, str2, size - len);
	dest[size - 1] = 0;
	kiss_utf8fix(dest);
	return dest;
}

int kiss_string_compare(const void *a, const void *b)
{
	return strcmp(*((char **) a), *((char **) b));
}

char *kiss_backspace(char *str)
{
	int len;

	if (!str) return NULL;
	if (!(len = strlen(str))) return NULL;
	str[len - 1] = 0;
	kiss_utf8fix(str);
	return str; 
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
		a->data = (void **) realloc(a->data,
			a->size * sizeof(void *));
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
		a->data = (void **) realloc(a->data,
			a->size * sizeof(void *));
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

