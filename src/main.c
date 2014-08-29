#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <mpd/client.h>

static struct mpd_connection* g_mpd_connection;

void print_usage(const char* arg0)
{
	printf("Usage: %s [track|artist|album]\n", arg0);
	printf("randompd version: %s\n\n", RANDOMPD_VERSION);
}

int main(int argc, char** argv)
{
	int choice;
	while (1)
	{
		static struct option long_options[] =
		{
			/* Use flags like so:
			{"verbose",	no_argument,	&verbose_flag, 'V'}*/
			/* Argument styles: no_argument, required_argument, optional_argument */
			{"version", no_argument,	0,	'v'},
			{"help",	no_argument,	0,	'h'},
			
			{0,0,0,0}
		};
	
		int option_index = 0;
	
		/* Argument parameters:
			no_argument: " "
			required_argument: ":"
			optional_argument: "::" */
	
		choice = getopt_long( argc, argv, "vh",
					long_options, &option_index);
	
		if (choice == -1)
			break;
	
		switch( choice )
		{
			case 'v':
				
				break;
	
			case 'h':
				print_usage(argv[0]);
				return EXIT_SUCCESS;
				
				break;
	
			case '?':
				/* getopt_long will have already printed an error */
				break;
	
			default:
				/* Not sure how to get here... */
				return EXIT_FAILURE;
		}
	}
	
	/* Deal with non-option arguments here */
	if ( optind < argc )
	{
		while ( optind < argc )
		{
			
		}
	}

	int success = 0;

	g_mpd_connection = mpd_connection_new(0, 0, 0);

	if (!g_mpd_connection) {
		fprintf(stderr, "could not allocate mpd connection\n");
		return 1;
	}

	success = mpd_send_list_all(g_mpd_connection, 0);

	if (!success) {
		fprintf(stderr, "could not read media library\n");
		return 1;
	}

	struct mpd_pair* item = 0;

	while ((item = mpd_recv_pair_named(g_mpd_connection, "file"))) {
		printf("%s\n", item->value);
		mpd_return_pair(g_mpd_connection, item);
	}

	mpd_response_finish(g_mpd_connection);

	return 0;
}
