#pragma once

#pragma region PROTOTYPE_TAG

// 파티클 프로토타입 태그.

// 비 텍스쳐.
static const _wstring PROTOTYPE_COMPONENT_TEXTURE_RAIN = TEXT("Prototype_Component_Texture_Rain");

// 비 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_RAIN = TEXT("Prototype_GameObject_ParticleRain");

// 비 UI파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_UI_PARTICLE_RAIN = TEXT("Prototype_GameObject_UIParticleRain");

// 달리기 텍스쳐.
static const _wstring PROTOTYPE_COMPONENT_TEXTURE_DASH = TEXT("Prototype_Component_Texture_Dash");

// 달리기 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_DASH = TEXT("Prototype_GameObject_ParticleDash");

// 채굴/파괴 텍스쳐.
static const _wstring PROTOTYPE_COMPONENT_TEXTURE_MINING = TEXT("Prototype_Component_Texture_Mining");

// 흙 채굴 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_MINING = TEXT("Prototype_GameObject_ParticleSandMining");

// 흙 파괴 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_DESTROY = TEXT("Prototype_GameObject_ParticleSandDestroy");

// 나무 채굴 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING = TEXT("Prototype_GameObject_ParticleWoodMining");

// 돌 채굴 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING = TEXT("Prototype_GameObject_Particle_Stone_Mining");

// 돌 파괴 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_DESTROY = TEXT("Prototype_GameObject_Particle_Stone_Destroy");

// 나뭇잎 채굴 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_LEAF_MINING = TEXT("Prototype_GameObject_Particle_Leaf_Mining");

// 나뭇잎 파괴 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_LEAF_DESTROY = TEXT("Prototype_GameObject_Particle_Leaf_Destroy");

// 폭발 텍스쳐.
static const _wstring PROTOTYPE_COMPONENT_TEXTURE_EXPLOSION = TEXT("Prototype_Component_Texture_Explosion");

// 폭발 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_EXPLOSION = TEXT("Prototype_GameObject_Particle_Explosion");

// 사망 텍스쳐.
static const _wstring PROTOTYPE_COMPONENT_TEXTURE_DIE = TEXT("Prototype_Component_Texture_Die");

// 사망 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_DIE = TEXT("Prototype_GameObject_Particle_Die");

// 불꽃 텍스쳐.
static const _wstring PROTOTYPE_COMPONENT_TEXTURE_FLAME = TEXT("Prototype_Component_Texture_Flame");

// 불꽃 검 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME = TEXT("Prototype_GameObject_ParticleSwordFlame");

// 폭죽 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_FIRE_CRACKER = TEXT("Prototype_GameObject_Particle_Fire_Cracker");

// 폭죽 길 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_CRACKER_LOAD = TEXT("Prototype_GameObject_Particle_Cracker_Load");

// 플래시 텍스쳐.
static const _wstring PROTOTYPE_COMPONENT_TEXUTRE_GLITTER = TEXT("Prototype_Component_Texture_Glitter");

// 플래시 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_GLITTER = TEXT("Prototype_GameObject_Particle_Glitter");

// 먹는 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_EATING = TEXT("Prototype_GameObject_Particle_Eating");

// 검기 텍스쳐.
static const _wstring PROTOTYPE_COMPONENT_TEXTURE_SWORD_AURA = TEXT("Prototype_Component_Texture_Sword_Aura");

// 검기 오브젝트.
static const _wstring PROTOTYPE_GAMEOBJECT_SWORD_AURA = TEXT("Prototype_GameObject_Sword_Aura");

// 파티클 길 오브젝트.
static const _wstring PROTOTYPE_GAMEOBJECT_CRACKER_LOAD = TEXT("Prototype_GameObject_Cracker_Load");

// 폭죽?같은 텍스쳐.
static const _wstring PROTOTYPE_COMPONENT_TEXTURE_GLINT = TEXT("Prototype_Component_Texture_Glint");

// 검기에 쓸 파티클 텍스쳐.
static const _wstring PROTYPE_COMPONENT_TEXTURE_GLOW = TEXT("Prototype_Component_Texture_Glow");

// 검기 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_AURA = TEXT("Prototype_GameObject_Particle_Sword_Aura");

// 워든 원거리 공격 텍스쳐.
static const _wstring PROTOTYPE_COMPONENT_TEXTURE_SONIC_BOOM = TEXT("Prototype_Component_Texture_Sonic_Boom");

// 워든 원거리 공격 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_SONIC_BOOM = TEXT("Prototype_GameObject_Particle_Sonic_Boom");

// 화로 불 파티클.
static const _wstring PROTOTYPE_GAMEOBJECT_PARTICLE_FURNACE = TEXT("Prototype_GameObject_Particle_Furnace");

/////////////////////////////////////////////////////////////////////////////

// VIBuffer_Only 클래스 컴포넌트 태그.
static const _wstring PROTOTYPE_COMPONENT_VIBUFFER_ONLY = TEXT("Prototype_Component_VIBuffer_Only");

// 깨지는 블럭 원형 객체 태그.
static const _wstring PROTOTYPE_GAMEOBJECT_DESTROY_CUBE = TEXT("Prototype_Gameobject_Destroy_Cube");

// 깨지는 블럭 텍스쳐 태그.
static const _wstring PROTOTYPE_COMPONENT_TEXTURE_DESTROY = TEXT("Prototype_Component_Texture_Destroy");

// 파티클 쉐이더 태그.
static const _wstring PROTOTYPE_COMPONENT_SHADER_PARTICLE = TEXT("Prototype_Component_Shader_Particle");

// UI 드랍 아이템 태그.
static const _wstring PROTOTYPE_GAMEOBJECT_UI_DROP_ITEM = TEXT("Prototype_GameObject_UIDropItem");

#pragma endregion

#pragma region LAYER_TAG

// 파티클 레이어.
static const _wstring LAYER_PARTICLE = TEXT("Layer_Particle");

// 깨지는 블럭 레이어.
static const _wstring LAYER_DESTROY_CUBE = TEXT("Layer_Destroy_Cube");

static const _wstring LAYER_UI_POOL = TEXT("Layer_UI_Pool");

// 이펙트 레이어.
static const _wstring LAYER_EFFECT = TEXT("Layer_Effect");

#pragma endregion