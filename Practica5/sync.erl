-module(sync).
-export([createLock/0, lock/1, unlock/1, destroyLock/1]).
-export([createSem/1, semP/1, semV/1, destroySem/1]).
-export([testLock/0, testSem/0]).

createLock () ->
	spawn(fun() -> loopLock([]) end).
loopLock(Pids) ->
	receive
		{lock, Pid} ->
			case Pids of 
				[] -> Pid ! granted, loopLock([Pid]);
				_ -> loopLock(Pids ++ [Pid])
			end;
		{unlock, Pid} ->
			case Pids of
				[] -> loopLock([]);
				[Pid] -> loopLock([]);
				[Pid|Rest]-> hd(Rest) ! granted, loopLock(Rest)
			end;
		destroy -> exit(normal)
	end.




lock (_L) -> 
	_L ! {lock, self()},
	receive
		granted -> ok
	end.
unlock (_L) -> 
	_L ! {unlock, self()}.
destroyLock (_L) -> _L ! destroy.

createSem (_N) -> 
	spawn(fun() -> semLoop(_N, []) end).

semLoop(Cur, Pids) ->
	receive
		{wait, Pid} ->
			if Cur > 0 ->
					Cur2 = Cur - 1,
					Pid ! pass, semLoop(Cur2, Pids);
			else -> 
					semLoop(Cur, Pids ++ [Pid])
			end;
		post -> 
			case Pids of
				[] -> Cur2 = Cur + 1, semLoop(Cur2, []);
				[Pid | Rest] -> Pid ! pass, semLoop(Cur, Rest)
			end;
		destroy -> ok
	end.


destroySem (_S) -> _S ! destroy.
semP (_S) -> 
	_S ! {wait, self()},
	receive
		pass -> ok
	end.
semV (_S) -> 
	_S ! post.

f (L, W) ->
	lock(L),
	% regioncritica(),
	io:format("uno ~p~n", [self()]),
	io:format("dos ~p~n", [self()]),
	io:format("tre ~p~n", [self()]),
	io:format("cua ~p~n", [self()]),
	unlock(L),
	W ! finished.

waiter (L, 0) -> destroyLock(L);
waiter (L, N) -> receive finished -> waiter(L, N-1) end.

waiter_sem (S, 0) -> destroySem(S);
waiter_sem (S, N) -> receive finished -> waiter_sem(S, N-1) end.

testLock () ->
	L = createLock(),
	W = spawn(fun () -> waiter(L, 3) end),
	spawn (fun () -> f(L, W) end),
	spawn (fun () -> f(L, W) end),
	spawn (fun () -> f(L, W) end),
	ok.

sem (S, W) ->
	semP(S),
	%regioncritica(), bueno, casi....
	io:format("uno ~p~n", [self()]),
	io:format("dos ~p~n", [self()]),
	io:format("tre ~p~n", [self()]),
	io:format("cua ~p~n", [self()]),
	io:format("cin ~p~n", [self()]),
	io:format("sei ~p~n", [self()]),
	semV(S),
	W ! finished.

testSem () ->
	S = createSem(2), % a lo sumo dos usando io al mismo tiempo
	W = spawn (fun () -> waiter_sem (S, 5) end),
	spawn (fun () -> sem (S, W) end),
	spawn (fun () -> sem (S, W) end),
	spawn (fun () -> sem (S, W) end),
	spawn (fun () -> sem (S, W) end),
	spawn (fun () -> sem (S, W) end),
	ok.
