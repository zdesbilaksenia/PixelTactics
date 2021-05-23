#pragma once

enum class GameMsgTypes : uint32_t {
    ErrorMessage,
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
    MakeLobby,
    GetLobbies,
    JoinLobby,
    LobbyEmpty,
    LobbyWaitingForPlayer,
    LobbyFull,
    LobbyGameStart,
    LobbyGameOver,
    HeroesStats,
    LobbyPlayerDisconnected,
    LobbyLeave,
    GetHeroesStats,
    GameLeaderChoice,
    GameHelloToPlayer,
    GameString,
    GameStr
};

