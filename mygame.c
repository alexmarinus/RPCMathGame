#include <stdio.h> 
#include <time.h> 
#include <rpc/rpc.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <sys/time.h>
#include "math_game.h"

int main (int argc, char *argv[]){

	CLIENT *handle;

	math_game_request request;
	math_game_result *result;
	uint64_t number;
	int i, num_tests;

	if (argc != 4) {
		printf("Usage: %s server_ip_addr in_file_name \
			out_file_name\n", argv[0]);
		exit(1);
	}

	//Conectare la server si creare handle asociat clientului
	handle=clnt_create(
		argv[1],
		MATHGAMEPROG,
		MATHGAMEVERS,
		"tcp");
	
	if (handle == NULL) {
		clnt_pcreateerror(argv[1]);
		return -1;
	}

	FILE *in = fopen(argv[2], "r");
	if (in == NULL) {
		printf ("Error opening the input file!\n");
		exit(1);
	}

	FILE *out = fopen (argv[3], "w");
	if (out == NULL) {
		printf ("Error creating the output file!\n");
		exit(2);
	}

	/*Pe prima linie din fisierul de intrare este
	numarul de solicitari*/
	fscanf (in, "%d", &num_tests);

	//Pe urmatoarele linii sunt numerele efective
	for (i = 0; i < num_tests; i ++) {
		fscanf (in, "%"PRIu64, &number);
		request.number = number;
		result = find_fermat_numbers_1(&request, handle);
		if (result == NULL) {
			clnt_perror(handle, argv[1]);
			return -2;
		}
		fprintf (out, "%s ", result->answer);
		if (strcmp (result->answer, "NO") == 0)
			fprintf (out, "%"PRIu64" ", result->closest_prime);
		fprintf (out, "%"PRIu64" ", result->number1);
		fprintf (out, "%"PRIu64"\n", result->number2);
	}

	fclose (in);
	fclose (out);

	clnt_destroy(handle);

	return 0;
}

