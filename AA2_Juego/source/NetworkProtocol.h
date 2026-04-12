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
    // --- SALAS ---
    CreateRoomRequest, // Cliente pide crear sala
    JoinRoomRequest,   // Cliente pide unirse a sala
    RoomSuccess,       // El servidor dice "OK, sala creada/unido"
    RoomError,         // El servidor dice "Error: Sala llena o no existe"
    GameStart          // El servidor avisa a los dos de que empieza lo bueno
};