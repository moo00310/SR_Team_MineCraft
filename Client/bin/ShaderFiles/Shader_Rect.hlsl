
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

// 카메라 월드 좌표.
vector g_CameraWorld;

texture g_Texture;
float g_Bright;

// 안개가 시작되는 거리.
float g_fFogStartDistance = 5.f;

// 안개가 끝나는 거리.
float g_fFogEndDistance = 25.f;

// 안개 색.
vector g_vFogColor = vector(0.529f, 0.808f, 0.922f, 1.f);

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float1 vDistance : TEXCOORD1; // 카메라와 블럭 거리.
};

sampler2D DefaultSampler : register(s0);

VS_OUT VS_MAIN_SKYBOX(VS_IN In)
{
    VS_OUT Out;
    
    float4 worldPosition = float4(In.vPosition, 0.1f); // 
    worldPosition = mul(worldPosition, g_WorldMatrix); // 월드 변환
    
    // 카메라와 월드 변환 된 블럭 거리 계산.
    Out.vDistance = length(worldPosition - g_CameraWorld);
    
    worldPosition = mul(worldPosition, g_ViewMatrix); // 뷰 변환
    worldPosition = mul(worldPosition, g_ProjMatrix); // 프로젝션 변환
    
    
    Out.vPosition = worldPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

VS_OUT VS_MAIN_ITEMRECT(VS_IN In)
{
    VS_OUT Out;
    
    float4 worldPosition = float4(In.vPosition, 1.0f); // 
    worldPosition = mul(worldPosition, g_WorldMatrix); // 월드 변환
    
    // 카메라와 월드 변환 된 블럭 거리 계산.
    Out.vDistance = length(worldPosition - g_CameraWorld);
    
    worldPosition = mul(worldPosition, g_ViewMatrix); // 뷰 변환
    worldPosition = mul(worldPosition, g_ProjMatrix); // 프로젝션 변환
    
    
    Out.vPosition = worldPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}


struct PS_IN
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float1 vDistance : TEXCOORD1; // 카메라와 블럭 거리.
};

struct PS_OUT
{
    vector vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);
        
    if (In.vDistance >= g_fFogEndDistance)
    {
        // 안개 끝나는 거리 바깥에 있는 얘들은 아예 안 보이게 무시.
        discard;
    }
    else if (In.vDistance >= g_fFogStartDistance)
    {
        // 안개 시작 거리에선 안개색상 지정.
        
        // 안개 끝거리 - 시작거리 결과 값.
        float fEnd = g_fFogEndDistance - g_fFogStartDistance;   // N
        
        // 안개 끝거리랑 현재 블럭 거리를 빼서 얼마 차이나는지 구한다.
        float fCurrent = g_fFogEndDistance - In.vDistance;      // N~0                
        
        // 선형보간에 쓰일 결과 값.
        float fResult = fCurrent / fEnd;                        // 1~0
        
        // 선형 보간.
        float4 color = lerp(g_vFogColor, Out.vColor, fResult);
        
        // 안개 색 입힘.
        Out.vColor.rgb = color * g_Bright;
    }
    else
    {
        // 안개 범위 안이면 기본색.
        Out.vColor.rgb *= g_Bright;
    }
    
    // 검정색이면 픽셀을 버림
    if (Out.vColor.r == 0 && Out.vColor.g == 0 && Out.vColor.b == 0)
        discard;

    return Out;
}

// 구름은 안개 쉐이딩에 안 걸리게 따로 빼놈.
PS_OUT PS_MAIN_CLOUD(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);
    
    Out.vColor.rgb *= g_Bright;
    
    // 검정색이면 픽셀을 버림
    if (Out.vColor.r == 0 && Out.vColor.g == 0 && Out.vColor.b == 0)
        discard;

    return Out;
}

PS_OUT PS_MAIN_SWORD_AURA(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);       
    
    // 픽셀 버리기
    if (Out.vColor.a < 0.2f)
        discard;
    
    // 빨간색으로 줌.
    Out.vColor.r = 1.f;        
    Out.vColor.a = 0.1f;
    
    return Out;
}


technique DefaultTechnique
{
    pass SkyBoxPass
    {
        ZENABLE = true;
        ZWRITEENABLE = true;
        LIGHTING = false;
        CULLMODE = NONE;
        VertexShader = compile vs_3_0 VS_MAIN_SKYBOX();
        PixelShader = compile ps_3_0 PS_MAIN_CLOUD();
    }

    pass ItemRectPass
    {
        ZENABLE = true;
        ZWRITEENABLE = true;
        LIGHTING = false;
        CULLMODE = NONE;
        VertexShader = compile vs_3_0 VS_MAIN_ITEMRECT();
        PixelShader = compile ps_3_0 PS_MAIN();
    }

    pass SwordAuraPass
    {
        ZENABLE = true;
        ZWRITEENABLE = true;
        LIGHTING = false; 
        CULLMODE = NONE;
        ALPHATESTENABLE = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        BlendOp = Add;
        VertexShader = compile vs_3_0 VS_MAIN_ITEMRECT();
        PixelShader = compile ps_3_0 PS_MAIN_SWORD_AURA();
    }
}