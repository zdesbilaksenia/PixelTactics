#pragma once

enum class GameMsgTypes : uint32_t {
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
    MakeLobby,
    JoinLobby,
    LobbyWaitingForPlayer,
    LobbyFull,
    LobbyGameStart,
    LobbyGameOver,
    LobbyPlayerDisconnected,
    GetHeroesStats,
    HeroesStats,

    GameLeaderChoice


};

