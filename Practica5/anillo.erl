-module(anillo).
-export([init/0]).

proceso() ->
    receive
        {next, Pidnext} -> loopProc(Pidnext)
    end.
loopProc(Pidnext) ->
    receive
        {msg, 0} ->
            io:fwrite("Me llego el 0 y soy ~p ~n", [self()]),
            Pidnext ! exit,
            exit(normal);
        {msg, N} ->
            io:fwrite("Soy ~p y me llego ~p~n", [self(), N]),
            Pidnext ! {msg, N-1},
            loopProc(Pidnext);
        exit ->
            Pidnext ! exit,
            exit(normal)
    end.

init() ->
    Nprocs = 5,
    Valor = 15,
    Pids = [spawn(fun proceso/0) || _ <- lists:seq(1, Nprocs)], %crea los procesos sin sucesor
    encadenar(Pids),
    Monitores = [erlang:monitor(process, P) || P <- Pids],
    hd(Pids) ! {msg, Valor},
    [receive{'DOWN', Ref, process, _, _} -> ok end || Ref <- Monitores],
    io:format("Todos los procesos terminaron correctamente.~n").


encadenar(Pids) ->
    encadenar(Pids, hd(Pids)).
encadenar([Pid|Rest], First) ->
    case Rest of
        [] -> Pid ! {next, First};
        [Next | _] -> Pid ! {next, Next}, encadenar(Rest, First)
    end.

