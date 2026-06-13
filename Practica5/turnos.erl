-module(turnos).
-export([server/0]).

%%% Al instanciar una VM donde corre erlang se crean
%%% subprocesos mucho mas ligeros que los hilos en C
%%% con su propia stack, y la vm maneja el scheduling, garbage collector
%%% y el heap inicial de cada spawn es pequeño comparado al 
%%%

server() ->
	{ok, ListenSocket} = gen_tcp:listen(8000, [{reuseaddr, true}]),
	wait_connect(ListenSocket, 0).

wait_connect(ListenSocket, N) ->
	{ok, Socket} = gen_tcp:accept(ListenSocket),
	spawn (fun () -> wait_connect (ListenSocket, N+1) end),
	get_request(Socket).

get_request(Socket) ->
	io:fwrite("Esperando mensajes de ~p~n", [Socket]),
	receive
		_X -> ok,
		get_request(Socket)
	end.
