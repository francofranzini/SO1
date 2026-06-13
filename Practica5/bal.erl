-module(bal).
-export([bal/1]).

bal(Servs) ->
    receive
        {req, Arg, Pid} ->
            Proc = lists:nth(rand:uniform(length(Servs)), Servs),
            spawn(fun() -> comunicacion(Proc, Arg, Pid) end),
            bal(Servs)
    end.


comunicacion(P, {req, Arg, id}, Pid) ->
    P ! { req, Arg, self() },
    receive
        Reply -> Pid ! Reply
    end.