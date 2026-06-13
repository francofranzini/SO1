-module(pingpong).
-export([play/1]).

ping() ->
    receive
        {0, _PongPid} ->
            io:format("Ping ~p termino~n", [self()]);
        {N, PongPid} ->
            PongPid ! {N - 1, self()},
            ping()
    end.

pong() ->
    receive
        {0, _PingPid} ->
            io:format("Pong ~p termino~n", [self()]);
        {N, PingPid} ->
            PingPid ! {N - 1, self()},
            pong()
    end.

play(N) when N > 0 ->
    PingPid = spawn(fun ping/0),
    PongPid = spawn(fun pong/0),
    PongPid ! {N, PingPid}.