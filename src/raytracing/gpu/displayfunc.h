/*
	Copyright (c) 2009 David Bucciarelli (davibu@interfree.it)

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _DISPLAYFUNC_H
#define	_DISPLAYFUNC_H

#if defined(__linux__) || defined(__APPLE__)
#include <sys/time.h>
#elif defined (WIN32)
#include <windows.h>
#else
        Unsupported Platform !!!
#endif

// Jens's patch for MacOS
#if defined(__APPLE__)
#include <GLut/glut.h>
#else
//#include <GL/glut.h>
#include <GL/freeglut.h>
#endif

#include "renderconfig.h"

extern RenderConfig *config;

extern void InitGlut(int argc, char *argv[], unsigned int width, unsigned int height, unsigned int rayPerPixel);
extern void RunGlut();

extern double WallClockTime();

#endif	/* _DISPLAYFUNC_H */

