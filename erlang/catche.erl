-module(catche).
-export([div_zero/2]). 

div_zero(N, M)->
    case catch (N/M) of
        {'EXIT', {Reason, Stack}} -> io:fwrite("DIV By Zero ~n");
        Res->Res
    end.