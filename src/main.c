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

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <mpd/client.h>

#include "randompd.h"

void print_usage(const char* arg0)
{
	printf("Usage: %s\n", arg0);
	printf("randompd version: %s\n\n", VERSION);
}

int create_connection(const char* host, unsigned port)
{
	g_mpd_connection = mpd_connection_new(host, port, 0);

	if (g_mpd_connection == NULL) {
		fprintf(stderr, "could not allocate mpd connection\n");
		return 0;
	}

	return 1;
}

int close_connection()
{
	mpd_connection_free(g_mpd_connection);

	return 1;
}

int add_random_track()
{
	track_t* track;

	track = &g_track_list.tracks[rand() % g_track_list.size];
	printf("Adding %s\n", track->path);

	return mpd_run_add(g_mpd_connection, track->path);
}

int update_file_list()
{
	int success = 0;
	track_t* track;
	struct mpd_pair* item = 0;
	size_t filename_len;

	success = mpd_send_list_all(g_mpd_connection, 0);

	if (!success) {
		fprintf(stderr, "could not read media library\n");
		return 0;
	}

	while ((item = mpd_recv_pair_named(g_mpd_connection, "file"))) {
		if ((g_track_list.index + 1) > g_track_list.size) {
			// increase the list size
			track = realloc(g_track_list.tracks,
				((g_track_list.size + kTrackListIncrement) * sizeof(track_t)));

			if (track == NULL)
				return 0;

			g_track_list.tracks = track;
			g_track_list.size += kTrackListIncrement;
		}

		filename_len = strlen(item->value);

		track = &g_track_list.tracks[g_track_list.index];
		track->index = g_track_list.index;
		track->path = malloc(filename_len + 1);

		strncpy((char*)track->path, item->value, filename_len + 1);
		
		g_track_list.index++;

		mpd_return_pair(g_mpd_connection, item);
	}

	// resize the track list so it fits nice and snug
	g_track_list.size = g_track_list.index;
	g_track_list.tracks = (track_t*)realloc(g_track_list.tracks,
							 (g_track_list.index * sizeof(track_t)));

	mpd_response_finish(g_mpd_connection);

	return 1;
}

int main(int argc, char** argv)
{
	int o = 0;
	int option_index = 0;
	const char* mpd_host = 0;
	unsigned int mpd_port = 0;

	static struct option long_options[] =
	{
		/* Use flags like so:
		{"verbose",	no_argument,	&verbose_flag, 'V'}*/
		/* Argument styles: no_argument, required_argument, optional_argument */
		{"version", no_argument,	0,	'v'},
		{"help",    no_argument,	0,	'h'},
		{"host",    optional_argument, 0, 'm'},
		{"port",    optional_argument, 0, 'p'},
		
		{0,0,0,0}
	};

	while ((o = getopt_long(argc, argv, "vhm:p:", long_options, &option_index)) != -1)
	{
		if (o == 'h') {
			printf("Usage: %s [num]\n\n", argv[0]);
			printf("randompd " VERSION " © 2014 Mikkel Kroman\n\n");
			printf("Options:\n");
			printf("  -m, --host=<host>  Connect to server on <host>\n");
			printf("  -p, --port=<port>  Connect to server port <port>\n\n");

			return EXIT_SUCCESS;

		} else if (o == 'm') {
			mpd_host = optarg;
		} else if (o == 'p') {
			mpd_port = atoi(optarg);
		} else if (o == 'v') {
			printf("randompd " VERSION " © 2014 Mikkel Kroman\n");

			return EXIT_SUCCESS;
		} else {
			return EXIT_FAILURE;
		}
	}

	struct timeval time;
	gettimeofday(&time, NULL);
	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

	// allocate and initialize the track list
	g_track_list.tracks = calloc(sizeof(track_t), kTrackListSize);
	g_track_list.size = kTrackListSize;
	g_track_list.index = 0;

	if (!create_connection(mpd_host, mpd_port))
		return EXIT_FAILURE;

	if (!update_file_list())
		return EXIT_FAILURE;

	if (argc > option_index + 1) {
		int num = atoi(argv[option_index + 1]);

		for (int i = 0; i < num; i++)
			add_random_track();
	}
	else
	{
		if (!add_random_track())
			return EXIT_FAILURE;
	}

	close_connection();

	return 0;
}
