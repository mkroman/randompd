/*
 * randompd - add a random track to your mpd playlist
 * Copyright (C) 2014 Mikkel Kroman <mk@maero.dk>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once
#ifndef __RANDOMPD_H
#define __RANDOMPD_H

#define kTrackListSize 8192 // The default track list size
#define kTrackListIncrement 100 // The number of tracks to increment by every
                                // time the list is overrun.


#include <mpd/client.h>

static struct mpd_connection* g_mpd_connection = 0;

void print_usage(const char* arg);
int create_connection(const char* host, unsigned port);
int close_connection();
int add_random_track();
int update_file_list();

typedef struct track {
	unsigned index;
	const char* path;
} track_t;

typedef struct track_list_t {
	size_t size;
	size_t index;
	track_t* tracks;
} track_list_t;

static track_list_t g_track_list = { 0, 0, 0 };

#endif
