build:
	rpcgen -C math_game.x
	gcc -o fermatserver fermatserver.c math_game_svc.c math_game_xdr.c -lnsl -lm
	gcc -o mygame mygame.c math_game_clnt.c math_game_xdr.c -lnsl -lm
clean:
	rm -f mygame fermatserver math_game.h math_game_svc.c math_game_clnt.c math_game_xdr.c *~
