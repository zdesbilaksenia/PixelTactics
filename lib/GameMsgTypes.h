#pragma once

enum class GameMsgTypes : uint32_t {
    ErrorMessage,
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
    ConnectToLobby,
    LobbyKill,
    LobbyWaitingForPlayer,
    LobbyFull,
    LobbyGameStart,
    LobbyGameOver,
    LobbyLeave,

    GetHeroesStats,
    GameLeaderChoice,
    GameHelloToPlayer,
    GameString,
    GameStr
};

