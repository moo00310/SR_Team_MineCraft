
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture g_Texture;
float g_Bright;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

sampler2D DefaultSampler : register(s0);

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    // 인스턴스 위치를 월드 변환에 추가
    float4 worldPosition = float4(In.vPosition, 0.1f); // vPosition을 float4로 처리
    worldPosition = mul(worldPosition, g_WorldMatrix); // 월드 변환
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
};

struct PS_OUT
{
    vector vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);
    Out.vColor.rgb *= g_Bright;
        // 검정색이면 픽셀을 버림
    if (Out.vColor.r == 0 && Out.vColor.g == 0 && Out.vColor.b == 0)
        discard;

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
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }
}