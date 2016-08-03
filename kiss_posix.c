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

char *kiss_getcwd(char *buf, int size)
{
#ifdef _MSC_VER
	return _getcwd(buf, size);
#else
	return getcwd(buf, size);
#endif
}

int kiss_chdir(char *path)
{
#ifdef _MSC_VER
	return _chdir(path);
#else
	return chdir(path);
#endif
}

int kiss_getstat(char *pathname, kiss_stat *buf)
{
#ifdef _MSC_VER
#ifdef _WIN32
	return _stat32(pathname, buf);
#else
	return _stat64(pathname, buf);
#endif
#endif
#ifndef _MSC_VER
	return stat(pathname, buf);
#endif
}

/* Search pattern must end with * in Visual C */
kiss_dir *kiss_opendir(char *name)
{
#ifdef _MSC_VER
	kiss_dir *dir;

	if (!name || strlen(name) > KISS_MAX_LENGTH - 2) return NULL;
	dir = malloc(sizeof(kiss_dir));
	dir->ent.d_name = NULL;
	strcpy(dir->name, name);
	dir->fhandle = (ptrdiff_t) _findfirst(dir->name, &dir->fdata);
	if ((int) dir->fhandle == -1) {
		free (dir);
		return NULL;
	}
	return dir;
#else
	return opendir(name);
#endif
}

kiss_dirent *kiss_readdir(kiss_dir *dirp)
{
#ifdef _MSC_VER
	if(dirp->ent.d_name &&
		(int) _findnext(dirp->fhandle, &dirp->fdata) == -1)
		return NULL;
	dirp->ent.d_name = dirp->fdata.name;
	return &dirp->ent;
#else
	return readdir(dirp);
#endif
}

int kiss_closedir(kiss_dir *dirp)
{
#ifdef _MSC_VER
	int n;

	if (!dirp || dirp->fhandle == -1) return -1;
	n = (int) _findclose(dirp->fhandle);
	free(dirp);
	return n;
#else
	return closedir(dirp);
#endif
}

int kiss_isdir(kiss_stat s)
{
#ifdef _MSC_VER
	return s.st_mode & _S_IFDIR;
#else
	return S_ISDIR(s.st_mode);
#endif
}

int kiss_isreg(kiss_stat s)
{
#ifdef _MSC_VER
	return s.st_mode & _S_IFREG;
#else
	return S_ISREG(s.st_mode);
#endif
}
 
