matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture g_Texture;

struct VS_IN
{
    float3 vPosition : POSITION;
    vector vColor : COLOR0;
};

sampler ParticleSampler = sampler_state
{
    texture = g_Texture;
};

struct PS_IN
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    vector vColor : COLOR0;
};

struct PS_OUT
{
    vector vColor : COLOR0;
};

PS_IN VS_Main(VS_IN In)
{
    PS_IN Out;
    
    // 인스턴스 위치를 월드 변환에 추가
    float4 worldPosition = float4(In.vPosition, 1.f); // vPosition을 float4로 처리
    worldPosition = mul(worldPosition, g_WorldMatrix); // 월드 변환
    worldPosition = mul(worldPosition, g_ViewMatrix); // 뷰 변환
    worldPosition = mul(worldPosition, g_ProjMatrix); // 프로젝션 변환
    
    Out.vPosition = worldPosition;
    Out.vTexcoord = float2(worldPosition.x, worldPosition.y);
    Out.vColor = In.vColor;    
    
    return Out;
}

PS_OUT PS_Main(PS_IN In)
{
    PS_OUT Out;
        
    Out.vColor = tex2D(ParticleSampler, In.vTexcoord);    
    Out.vColor.rgb = float3(0.f, 1.f, 0.f);    
    
    return Out;
}

technique DefaultTechnique
{
    pass ParticlePass
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}