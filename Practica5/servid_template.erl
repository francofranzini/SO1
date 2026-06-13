-module(servid_template).
%%%%%%
%% Pequeño ejercicio de clase
%% el servidor aceptara 4 tipos de pedidos/mensajes:
%% + nuevoId(Nombre, PidResp) -> Se generará un nuevo identificar para `Nombre` y se responderá al cliente.
%% + buscarId(Id, PidResp) -> Se responde a `PidResp` el nombre asociado a `Id`.
%% + verLista(PidResp) -> Se envía a `Pidresp` la lista de pares (Id,Nombre).
%% + finalizar(PidResp) -> Se finaliza el servicio y se responde con un `ok`.
%%

%% Creación y eliminación del servicio
-export([iniciar/0, finalizar/1]).

%% Servidor
-export([serverinit/1]).

%% Librería de Acceso
-export([nuevoNombre/2, quienEs/2, listaDeIds/1]).

%% Funciones de testing
-export([test/0,client/1]).

% Iniciar crea el proceso servidor, y devuelve el PId.
iniciar() ->
  todo.

%%%%%%%%%%%%%% Librería de Acceso
%% Dado un nombre y un servidor le pide que cree un identificador único.
nuevoNombre(Nombre, Servidor) ->
  todo.

%% Función que recupera el nombre desde un Id
quienEs(Id, Servidor) ->
  todo.

%% Pedimos la lista completa de nombres e identificadores.
listaDeIds(Servidor) ->
  todo.

% Ya implementada :D!
finalizar(Servidor) ->
  todo.

%%%%%%%%%%% Servidor
%% Función de servidor de nombres.
serverinit(PInit) ->
    %% Comenzamos con un mapa nuevo, y un contador en 0.
    todo.

servnombres(Map, N) ->
    receive
        %% Llega una petición para crear un Id para nombre
        {nuevoId, Nombre, CId} -> todo;
        %% Llega una petición para saber el nombre de tal Id
        {buscarId, NId, CId} -> todo;
        %% Entrega la lista completa de Ids con Nombres.
        {verLista, CId} -> todo;
        %% Cerramos el servidor.
        {finalizar, CId } -> todo;
        _ -> todo
    end.

client(Server) ->
  nuevoNombre(Server, pepa),
  listaDeIds(Server),
  nuevoNombre(Server, pepe),
  listaDeIds(Server),
  quienEs(Server, 1),
  quienEs(Server, 2),
  finalizar(Server),
  ok.

test() ->
  Server = servid:iniciar(),
  spawn(?MODULE, client, [Server]),
  ok.
