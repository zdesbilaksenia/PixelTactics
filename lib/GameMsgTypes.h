#pragma once

enum class GameMsgTypes : uint32_t {
    ErrorMessage,
    ServerAccept,
    ConnectToLobby,
    LobbyWaitingForPlayer,
    LobbyFull,
    LobbyGameStart,
    LobbyGameOver,
    LobbyLeave
};

