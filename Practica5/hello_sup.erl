-module(hello_sup).
-export([start/0]).

hello(Intervalo, Falla) ->
    T1 = erlang:monotonic_time(millisecond),
    timer:sleep(Intervalo),
    T2 = erlang:monotonic_time(millisecond),
    io:fwrite("Hola~n"),
    Falla2 = Falla - (T2 - T1),
    if Falla2 < 0 -> throw(error);
    true->ok
    end,
    hello(Intervalo, Falla2).

worker() ->
    hello(3000, 10000).


supervisar(Pid) ->
    receive
        {'EXIT', Pid, _} -> 
            io:fwrite("Se cayo el worker, lo reinicio...~n"),
            NuevoWorker = spawn_link(fun worker/0),
            supervisar(NuevoWorker)
    end,
    ok.
start() ->
    process_flag(trap_exit, true),
    Pid = spawn_link(fun worker/0),
    supervisar(Pid).