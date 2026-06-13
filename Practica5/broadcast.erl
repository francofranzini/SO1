-module(broadcast).

%% Librería
% Funciones control
-export([start/0,stop/1]).
% Funciones interacción
-export([broadcast/2,suscribir/2]).
%%%
-export([loopBroadcast/1]).

-export([test/0,client/1]).


start() ->
    spawn(fun() -> loopBroadcast([]) end).


stop(Broadcast) ->
    Broadcast ! stop.

broadcast(Broadcast, Msg) ->
    Broadcast ! {broadcast, Msg}.

suscribir(Broadcast, Pid) ->
    Broadcast ! {suscribir, Pid}.

desuscribir(Broadcast) ->
    Broadcast ! {desuscribir, self()}.

loopBroadcast(RegisteredPids) ->
    receive
        {suscribir, Pid} -> loopBroadcast([Pid|RegisteredPids]);
        {desuscribir, Pid} -> loopBroadcast(lists:delete(Pid, RegisteredPids));
        {broadcast, Msg} -> lists:foreach(fun(P) -> P ! {Msg} end, RegisteredPids), loopBroadcast(RegisteredPids);
        stop -> lists:foreach(fun(P) -> P ! stop end, RegisteredPids), exit(normal)
    end.

loopClient() ->
    receive
        {Msg} -> io:fwrite("Soy el cliente y recibi ~p~n", [Msg]), loopClient();
        stop -> ok
    end.

client(Broadcast) ->
    C =spawn(fun() -> loopClient() end),
    suscribir(Broadcast, C).


test() ->
    Server = start(),
    spawn(fun() -> client(Server) end),
    spawn(fun() -> client(Server) end),
    spawn(fun() -> client(Server) end),
    timer:sleep(100),
    broadcast(Server, "Hola a todos"),
    timer:sleep(100),
    broadcast(Server, "Segundo mensaje"),
    timer:sleep(100),
    stop(Server).






