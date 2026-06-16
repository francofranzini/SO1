-module(hello).
-export([init/0]).

hello() ->
    io:fwrite("Hola~n").

regular(Fun, Intervalo, Falla) ->
    T1 = erlang:monotonic_time(millisecond),
    timer:sleep(Intervalo),
    T2 = erlang:monotonic_time(millisecond),
    NuevoT = Falla - (T2 - T1),
    Fun(),
    if NuevoT < 0 ->
        io:fwrite("FALLA~n"),
        throw(falla);
        true -> ok
    end,
    regular(Fun, Intervalo, NuevoT).

init() ->
    
    try regular(fun hello/0, 3000, 10000) of
        Result -> {ok, Result}
    catch
        error:Reason -> {error, Reason};
        throw:Reason -> {throw, Reason}
    end,
    init(),
    ok.