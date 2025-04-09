
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

// 카메라 월드 좌표.
vector g_CameraWorld;

texture g_Texture;
float g_Bright;

// 안개가 시작되는 거리.
float g_fFogDistance = 10.f;

// 안개 색.
vector g_vFogColor = vector(1.f, 1.f, 1.f, 1.f);

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
    
    // 안개 범위 바깥인가?
    if (In.vDistance >= g_fFogDistance)
    {
        // 안개와의 거리 차이.
        float distance = saturate(In.vDistance - g_fFogDistance);
        
        // 선형 보간.
        float4 color = lerp(Out.vColor, g_vFogColor, distance);
        
        // 안개 색 입힘.
        Out.vColor.rgb = color * g_Bright;
        
        // 안개 색상 무시함.   
        if (Out.vColor.r >= (g_vFogColor.r - distance) * g_Bright ||
        Out.vColor.g >= (g_vFogColor.g - distance) * g_Bright ||
        Out.vColor.b >= (g_vFogColor.b - distance) * g_Bright)
        {
            discard;
        }
        
        // 검은색은 위에 조건문 안 먹어서 따로 예외처리함.
        if (Out.vColor.r <= 0.f &&
        Out.vColor.g <= 0.f &&
        Out.vColor.b <= 0.f)
        {
            discard;
        }
    }
    else
    {
        Out.vColor.rgb *= g_Bright;
    }
    
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
        PixelShader = compile ps_3_0 PS_MAIN();
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