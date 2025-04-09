
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

// 카메라 월드 좌표.
vector g_CameraWorld;

texture g_Texture;

// 안개가 시작되는 거리.
float g_fFogDistance = 10.f;

// 안개 색.
vector g_vFogColor = vector(1.f, 1.f, 1.f, 1.f);

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float3 vInstancePos : TEXCOORD1; //인스턴싱 버퍼 (위치만)
    float1 vBright : TEXCOORD2; //인스턴싱 버퍼 (밝기)
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float1 vBright : TEXCOORD2; //인스턴싱 버퍼 (밝기)
    float1 vDistance : TEXCOORD3; // 카메라와 블럭 거리.
};

sampler2D DefaultSampler : register(s0);

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    // 인스턴스 위치를 월드 변환에 추가
    float4 worldPosition = float4(In.vPosition + In.vInstancePos, 1.0f); // vPosition을 float4로 처리
    worldPosition = mul(worldPosition, g_WorldMatrix); // 월드 변환
    
    // 카메라와 월드 변환 된 블럭 거리 계산.
    Out.vDistance = length(worldPosition - g_CameraWorld);
    
    worldPosition = mul(worldPosition, g_ViewMatrix); // 뷰 변환
    worldPosition = mul(worldPosition, g_ProjMatrix); // 프로젝션 변환
    
    
    
    Out.vPosition = worldPosition;
    Out.vTexcoord = In.vTexcoord;
    Out.vBright = In.vBright;

    return Out;
}


struct PS_IN
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float1 vBright : TEXCOORD2; //인스턴싱 버퍼 (밝기)
    float1 vDistance : TEXCOORD3; // 카메라와 블럭 거리.
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
        Out.vColor.rgb = color * In.vBright;
        
        // 안개 색상 무시함.   
        if (Out.vColor.r >= (g_vFogColor.r - distance) * In.vBright ||
        Out.vColor.g >= (g_vFogColor.g - distance) * In.vBright ||
        Out.vColor.b >= (g_vFogColor.b - distance) * In.vBright)
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
        Out.vColor.rgb *= In.vBright;
    }        
    
    return Out;
}

PS_OUT PS_MAIN_RED(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = vector(1.f, 0.f, 0.f, 1.f);
        
    return Out;
}

technique DefaultTechnique
{
    pass DefaultPass
    { 
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
        AlphaTestEnable = TRUE;
        AlphaFunc = GREATER;
        AlphaRef = 254;
        CullMode = NONE;
        Lighting = FALSE;
    }

    pass asdouble
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_RED();
    }
}