#pragma once
#ifndef __RANDOMPD_H
#define __RANDOMPD_H

#include <mpd/client.h>

static struct mpd_connection* g_mpd_connection = 0;

void print_usage(const char* arg);
int create_connection(const char* host, unsigned port);
int close_connection();
int update_file_list();

#endif
