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
  spawn(?MODULE, loopBroadcast, [[]]).

stop(Broadcast) ->
  todo.

broadcast(Broadcast, Msg) ->
  Broadcast ! {brod, Msg}.

registrar(Broadcast) ->
  Broadcast ! {reg, self()}.

loopBroadcast(RegisteredPids) ->
  receive
    {reg, PidClient} -> loopBroadcast([PidClient | RegisteredPids]);  
    {brod, Msg}      -> lists:foreach(fun(Pid) -> Pid ! Msg end, RegisteredPids), loopBroadcast(RegisteredPids)
  end.
  % Ayuda: Utilizar la función lists:foreach(fun (Pid) -> Pid ! Msg end , PidsList)

client(Broadcast) ->
  registrar(Broadcast),
  loopClient().

loopClient() ->
  receive
    Msg -> io:fwrite("client ~p recieved ~p ~n", [self(), Msg]), loopClient()
  end.

test() ->
  Broadcast = start(),
  spawn(?MODULE, client, [Broadcast]),
  spawn(?MODULE, client, [Broadcast]),
  timer:sleep(1000),
  broadcast(Broadcast , hola).







