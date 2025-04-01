
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture g_Texture;
float g_Bright;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

sampler2D DefaultSampler : register(s0);

VS_OUT VS_MAIN_SKYBOX(VS_IN In)
{
    VS_OUT Out;

    // 인스턴스 위치를 월드 변환에 추가
    float4 worldPosition = float4(In.vPosition, 0.1f); // vPosition을 float4로 처리
    worldPosition = mul(worldPosition, g_WorldMatrix); // 월드 변환
    worldPosition = mul(worldPosition, g_ViewMatrix); // 뷰 변환
    worldPosition = mul(worldPosition, g_ProjMatrix); // 프로젝션 변환
    
    
    Out.vPosition = worldPosition;
    Out.vNormal = In.vNormal;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

VS_OUT VS_MAIN_Steve(VS_IN In)
{
    VS_OUT Out;

    // 인스턴스 위치를 월드 변환에 추가
    float4 worldPosition = float4(In.vPosition, 1.0f); // vPosition을 float4로 처리
    worldPosition = mul(worldPosition, g_WorldMatrix); // 월드 변환
    worldPosition = mul(worldPosition, g_ViewMatrix); // 뷰 변환
    worldPosition = mul(worldPosition, g_ProjMatrix); // 프로젝션 변환
    
    
    Out.vPosition = worldPosition;
    Out.vNormal = In.vNormal;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}


struct PS_IN
{
    float4 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : COLOR0;
};

PS_OUT PS_MAIN_SKYBOX(PS_IN In)
{
    PS_OUT Out;

    float3 skyColor;
    float blendFactor = saturate(In.vTexcoord.y);
    skyColor = lerp(float3(0.0, 0.5, 1.0), float3(1.0, 1.0, 1.0), blendFactor);

    
    if (In.vNormal.y < -0.5)
    {
        skyColor = float3(0.498, 0.749, 1.0);
    }

    // 밝기 적용
    Out.vColor.rgb = skyColor * g_Bright;
    Out.vColor.a = 1.0; // 알파값 1.0 (불투명)

    return Out;
}


PS_OUT PS_MAIN_STEVE(PS_IN In)
{
    PS_OUT Out;

    Out.vColor= tex2D(DefaultSampler, In.vTexcoord);
    Out.vColor.rgb *= g_Bright; // 알파값 1.0 (불투명) 
    
   // Out.vColor = float4(1, 0, 0, 1);
    
    return Out;
}

technique DefaultTechnique
{
    pass SkyBoxPass
    {
        ZENABLE = false;
        ZWRITEENABLE = false;
        LIGHTING = false;
        CULLMODE = NONE;
        VertexShader = compile vs_3_0 VS_MAIN_SKYBOX();
        PixelShader = compile ps_3_0 PS_MAIN_SKYBOX();
    }

    pass Steve
    {
        ZENABLE = true;
        ZWRITEENABLE = true;
        LIGHTING = true;
        VertexShader = compile vs_3_0 VS_MAIN_Steve();
        PixelShader = compile ps_3_0 PS_MAIN_STEVE();
    }

    pass Rect
    {
        CULLMODE = NONE;
        ALPHATESTENABLE = true;
        ALPHAREF = 100;
        ALPHAFUNC = GREATER;

        VertexShader = compile vs_3_0 VS_MAIN_Steve();
        PixelShader = compile ps_3_0 PS_MAIN_STEVE();

    }

}