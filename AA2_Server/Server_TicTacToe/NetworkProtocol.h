#pragma once

enum class PacketType {
    LoginRequest,
    LoginSuccess,
    LoginFailed,
    RegisterRequest,
    RegisterSuccess,
    RegisterFailed,
    RankingRequest,
    RankingResponse,
    CreateRoomRequest,
    JoinRoomRequest,
    RoomSuccess,
    RoomError,
    GameStart
};