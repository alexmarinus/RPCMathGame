#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <rpc/rpc.h>
#include <math.h>
#include <inttypes.h>
#include <limits.h>

#include "math_game.h"

/*Calcularea radacinii patrate pentru un numar intreg
pe 64 de biti*/
uint64_t square_root_64b (uint64_t number) {
	uint64_t root = sqrt(number) - 0x1p-20;
	if (number - root * root > 2 * root)
		root ++;
	return root;
}

/*Verificarea daca nu numar pe 64 de biti este prim*/
int is_prime_64b (uint64_t n) {
	if (n == 2 || n == 3)
		return 1;

	if (n % 2 == 0 || n % 3 == 0)
		return 0;

	uint64_t root = square_root_64b (n);
	uint64_t k = 1;

	/*Se parcurg mai putine numere pentru verificare,
	si anume, mutliplii de 6, +/- 1, care nu depasesc
	radicalul numarului*/
	while (1) {
		if ((6 * k - 1) > root)
			break;
		if (n % (6 * k - 1) == 0)
			return 0;
		if ((6 * k + 1) > root)
			break;
		if (n % (6 * k + 1) == 0)
			return 0;
		k ++;
	}
	return 1;
}


/*Determinarea celui mai apropiat numar prim cu proprietatea
din cerinta*/
uint64_t closest_prime_number_game (uint64_t number) {
	uint64_t lowerprime, upperprime;

	//Caz de baza
	if ( number < 5 )
		return 5;

	//Caz de baza
	if ( is_prime_64b (number) && (number % 4 == 1) )
		return number;
	else {
		//Gasirea de numere prime in ambele sensuri
		lowerprime = upperprime = number;
		while (!is_prime_64b (lowerprime) || !(lowerprime % 4 == 1))
			lowerprime --;
		while (!is_prime_64b (upperprime) || !(upperprime % 4 == 1))
			upperprime ++;
		if ((number - lowerprime) < (upperprime - number))
			return lowerprime;
		else
			return upperprime;
	}
	
}

/*Functie pentru completarea structurii de date utilizata ca
raspuns la cerea clientului*/
math_game_result *find_fermat_numbers_1_svc
	(math_game_request *request, struct svc_req *cl) {

	static math_game_result result;
	uint64_t n = request->number;
	uint64_t closest_prime = closest_prime_number_game (n);
	uint64_t number1, number2;
	
	//Valoarea de adevar a raspunsului
	if (closest_prime == n)
		strcpy (result.answer, "YES");
	else
		strcpy (result.answer, "NO");

	//Determinarea numerelor conform criteriului sumelor patratelor
	number1 = square_root_64b (closest_prime);
	while (1) {
		number2 = square_root_64b (closest_prime - number1 * number1);
		if (number1 * number1 + number2 * number2 == closest_prime)
			break;
		number1 --;
	}

	result.closest_prime = closest_prime;
	result.number1 = number1;
	result.number2 = number2;
	return (&result);
}

