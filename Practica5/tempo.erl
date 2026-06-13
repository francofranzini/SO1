-module(tempo).
-export([wait/1, crono/3]).

wait(MS) ->
    timer:sleep(MS).

crono(Fun, Hasta, Periodo) ->
    spawn(fun() -> loop(Fun, Hasta, Periodo) end).

loop(Fun, Hasta, Periodo) ->
    T1 = erlang:monotonic_time(millisecond),
    timer:sleep(Periodo),
    Fun(),
    T2 = erlang:monotonic_time(millisecond),
    Restante = Hasta - (T2 - T1),
    if Restante > 0 -> loop(Fun, Restante, Periodo);
        true -> ok
    end.
    

