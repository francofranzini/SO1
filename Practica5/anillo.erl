-module(anillo).
-export([init/0]).

proceso(Pid) ->
    receive
        {msg, N} -> 

    end.

init() ->
    Nprocs = 5,
    Valor = 15,
    Pids = [spawn(fun proceso/1) || _ <- 1..Nprocs],
    Pids[i] ! Pids[i+1],
    Pids[Nprocs] ! Pids[1],
    hd(Pids) ! {Hola, Valor}.
