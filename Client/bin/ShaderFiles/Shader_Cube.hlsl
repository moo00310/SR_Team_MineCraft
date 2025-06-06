
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

// 카메라 월드 좌표.
vector g_CameraWorld;

texture g_Texture;
float g_Bright;

// 안개가 시작되는 범위.
float g_fFogStartRange = 5.f;

// 안개가 끝나는 범위.
float g_fFogEndRange = 25.f;

// 안개 색.
vector g_vFogColor = vector(0.529f, 0.808f, 0.922f, 1.f);

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
    float1 vDistance : TEXCOORD1; // 카메라와 블럭 거리.
};

sampler2D DefaultSampler : register(s0);

VS_OUT VS_MAIN_SKYBOX(VS_IN In)
{
    VS_OUT Out;

    // 인스턴스 위치를 월드 변환에 추가
    float4 worldPosition = float4(In.vPosition, 0.1f); // vPosition을 float4로 처리
    worldPosition = mul(worldPosition, g_WorldMatrix); // 월드 변환
    
    // 카메라와 월드 변환 된 블럭 거리 계산.
    Out.vDistance = length(worldPosition - g_CameraWorld);
    
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
    
    // 카메라와 월드 변환 된 블럭 거리 계산.
    Out.vDistance = length(worldPosition - g_CameraWorld);
    
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
    float1 vDistance : TEXCOORD1; // 카메라와 블럭 거리.
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
        
    if (In.vDistance >= g_fFogEndRange)
    {
        // 안개 끝나는 거리 바깥에 있는 얘들은 아예 안 보이게 무시.
        discard;
    }
    else if (In.vDistance >= g_fFogStartRange)
    {
        // 안개 시작 범위에선 안개색상 지정.
        
        // 안개 끝 범위 - 시작 범위 결과 값.
        float fEnd = g_fFogEndRange - g_fFogStartRange;         // N
        
        // 안개 끝 범위랑 현재 블럭 거리를 빼서 얼마 차이나는지 구한다.
        float fCurrent = g_fFogEndRange - In.vDistance;         // N~0                
        
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
    
    return Out;
}

PS_OUT PS_MAIN_HIT(PS_IN In)
{
    PS_OUT Out;
    
    float3 hitColor = float3(1.0, 0.2, 0.2);
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);
    Out.vColor.rgb = lerp(Out.vColor.rgb, hitColor, 0.5);
    Out.vColor.rgb *= g_Bright; 
    return Out;
}

PS_OUT PS_MAIN_BOOM(PS_IN In)
{
    PS_OUT Out;
    
    float3 hitColor = float3(1.0, 1.0, 1.0);
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);
    Out.vColor.rgb = lerp(Out.vColor.rgb, hitColor, 0.5);
    Out.vColor.rgb *= g_Bright;
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

    pass TPS_Cube
    {
        ZENABLE = true;
        ZWRITEENABLE = true;
        LIGHTING = true;
        VertexShader = compile vs_3_0 VS_MAIN_Steve();
        PixelShader = compile ps_3_0 PS_MAIN_STEVE();
    }

    pass TPS_Rect
    {
        CULLMODE = NONE;
        ALPHATESTENABLE = true;
        ALPHAREF = 100;
        ALPHAFUNC = GREATER;
        VertexShader = compile vs_3_0 VS_MAIN_Steve();
        PixelShader = compile ps_3_0 PS_MAIN_STEVE();
    }

    pass FPS_Cube
    {
        ZENABLE = false;
        ZWRITEENABLE = false;
        LIGHTING = true;
        VertexShader = compile vs_3_0 VS_MAIN_Steve();
        PixelShader = compile ps_3_0 PS_MAIN_STEVE();
    }

    pass FPS_Rect
    {
        CULLMODE = NONE;
        ZENABLE = false;
        ZWRITEENABLE = false;
        ALPHATESTENABLE = true;
        ALPHAREF = 100;
        ALPHAFUNC = GREATER;
        VertexShader = compile vs_3_0 VS_MAIN_Steve();
        PixelShader = compile ps_3_0 PS_MAIN_STEVE();
    }

    pass Hit
    {
        ZENABLE = true;
        ZWRITEENABLE = true;
        LIGHTING = true;
        VertexShader = compile vs_3_0 VS_MAIN_Steve();
        PixelShader = compile ps_3_0 PS_MAIN_HIT();
    }

    pass Boom
    {
        ZENABLE = true;
        ZWRITEENABLE = true;
        LIGHTING = true;
        VertexShader = compile vs_3_0 VS_MAIN_Steve();
        PixelShader = compile ps_3_0 PS_MAIN_BOOM();
    }

}