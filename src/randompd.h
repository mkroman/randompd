#pragma once
#ifndef __RANDOMPD_H
#define __RANDOMPD_H

#include <mpd/client.h>

static struct mpd_connection* g_mpd_connection;

void print_usage(const char* arg);

#endif
