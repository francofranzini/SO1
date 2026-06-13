-module(broadcast).

%% Librería
% Funciones control
-export([start/0,stop/1]).
% Funciones interacción
-export([broadcast/2,registrar/1]).
%%%
-export([loopBroadcast/1]).

-export([test/0,client/1]).


start() ->
  todo.

stop(Broadcast) ->
  todo.

broadcast(Broadcast, Msg) ->
  todo.

registrar(Broadcast) ->
  todo.

loopBroadcast(RegisteredPids) ->
  todo. % Ayuda: Utilizar la función lists:foreach(fun (Pid) -> Pid ! Msg end , PidsList)

client(Broadcast) ->
  todo.

test() ->
  todo.







