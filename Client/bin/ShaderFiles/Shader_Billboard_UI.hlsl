matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture g_Texture;
float g_Bright;
float g_BossHp; 

sampler TextureSampler = sampler_state
{
    texture = g_Texture;

    MINFILTER = POINT;
    MAGFILTER = POINT;
    MIPFILTER = POINT;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float3 vWorldPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix).xyz;
    
    return Out;
};

struct PS_IN
{
    float4 vPostion : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float3 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = tex2D(TextureSampler, In.vTexcoord);
    Out.vColor.rgb *= g_Bright;

    return Out;
}

PS_OUT PS_BossHp(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    if (In.vTexcoord.x > g_BossHp)
        discard; // ¶Ç´Â clip(-1); µÑ ´Ù µÊ
	
    Out.vColor = tex2D(TextureSampler, In.vTexcoord);
    Out.vColor.rgb *= g_Bright;

    return Out;
}

technique DefaultTechnique
{

    pass Defulat_with_AlphaTest
    {
        AlphaTestEnable = TRUE;
        AlphaFunc = greater;
        AlphaRef = 40;
        CULLMODE = NONE;
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }

    pass BossHp_with_AlphaTest
    {
        AlphaTestEnable = TRUE;
        AlphaFunc = greater;
        AlphaRef = 40;
        CULLMODE = NONE;
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_BossHp();
    }

}