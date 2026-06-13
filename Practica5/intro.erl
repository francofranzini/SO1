-module(intro).
-export([init/0]).
match_test () ->
{A,B} = {5,4},
{C,D2} = {5,4},
{B,A} = {4,5},
{D,D} = {5,5}.
string_test () -> [
    helloworld == 'helloworld',
    "helloworld" < 'helloworld',
    helloworld == "helloworld",
    [$h,$e,$l,$l,$o,$w,$o,$r,$l,$d] == "helloworld",
    [104,101,108,108,111,119,111,114,108,100] < {104,101,108,108,111,119,111,114,108,100},
    [104,101,108,108,111,119,111,114,108,100] > 1,
    [104,101,108,108,111,119,111,114,108,100] == "helloworld"].

tuple_test (P1, P2) ->
    io:fwrite("El nombre de P1 es ~p y el apellido de P2 es ~p~n", [nombre(P1), apellido(P2)]).

apellido (P) -> 
    {persona, _, {apellido, AP}} = P,
    AP. 
nombre ({persona, {nombre, Nom}, _}) -> Nom.
    
filtrar_por_apellido(Personas, Apellido) -> 
    [P || P <- Personas, apellido(P) =:= Apellido].
filtrar_por_apellido2(Personas, Apellido) -> 
    lists:filter(fun(P) -> apellido(P) =:= Apellido end, Personas).
filtrar_por_apellido3(Personas, Apellido) -> 
    [{nombre, Nom} || {persona, {nombre, Nom}, {apellido, Ap}} <- Personas, Ap =:= Apellido].

init () ->
    P1 = {persona, {nombre, "Juan"}, {apellido, "Gomez"}},
    P2 = {persona, {nombre, "Carlos"}, {apellido, "Garcia"}},
    P3 = {persona, {nombre, "Javier"}, {apellido, "Garcia"}},
    P4 = {persona, {nombre, "Rolando"}, {apellido, "Garcia"}},
    match_test(),
    tuple_test(P1, P2),
    io:fwrite("~p ~n",[string_test()]),
    Garcias = filtrar_por_apellido2([P4, P3, P2, P1], "Garcia").