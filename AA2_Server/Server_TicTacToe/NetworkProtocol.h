#pragma once

enum class PacketType {
    LoginRequest,
    LoginSuccess,
    LoginFailed,
    RegisterRequest,
    RegisterSuccess,
    RegisterFailed
};