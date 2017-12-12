const DIMENSION = 4;

struct math_game_request {
	uint64_t number;
};

struct math_game_result{
	char answer[DIMENSION];
	uint64_t closest_prime;
	uint64_t number1;
	uint64_t number2;
};

program MATHGAMEPROG {
	version MATHGAMEVERS {
		math_game_result FIND_FERMAT_NUMBERS(math_game_request) = 1;
	} = 1;
} = 0x40005000;
