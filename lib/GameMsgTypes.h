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
    HeroesStats,

    GameDeck,
    GameHeroesStats,
    GameCanBeAttacked,
    GameLeaderChoice,
    GamePlayerOptionChoice,
    GameFullCoordinates,
    GameShortCoordinates,
    GameHelloToPlayer,
    GameCardFromHandChoice,
    GameStr,
    GameString,
    GameAttackRequest

};

