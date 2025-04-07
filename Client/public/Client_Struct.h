#pragma once
#include <functional> // ��� �� ����
#include "GameInstance.h"

namespace Client
{
    struct _int3 {
        int x, y, z;

        bool operator==(const _int3& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };
}

// �ؽ� �Լ��� std ���ӽ����̽����� Ư��ȭ!
namespace std {
    template <>
    struct hash<Client::_int3> {
        std::size_t operator()(const Client::_int3& v) const noexcept {
            std::size_t hx = std::hash<int>{}(v.x);
            std::size_t hy = std::hash<int>{}(v.y);
            std::size_t hz = std::hash<int>{}(v.z);
            return ((hx ^ (hy << 1)) >> 1) ^ (hz << 1);
        }
    };
}

namespace Client
{
    typedef struct tagBlockDesc
    {
        BLOCKTYPE eBlockType{ BLOCKTYPE::BLOCKTYPE_END };
        _float3 fPosition{ 0.f, 0.f, 0.f };
    } BLOCKDESC;
}
