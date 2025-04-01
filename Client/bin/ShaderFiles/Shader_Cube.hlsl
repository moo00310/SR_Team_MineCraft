
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

    // �ν��Ͻ� ��ġ�� ���� ��ȯ�� �߰�
    float4 worldPosition = float4(In.vPosition, 0.1f); // vPosition�� float4�� ó��
    worldPosition = mul(worldPosition, g_WorldMatrix); // ���� ��ȯ
    worldPosition = mul(worldPosition, g_ViewMatrix); // �� ��ȯ
    worldPosition = mul(worldPosition, g_ProjMatrix); // �������� ��ȯ
    
    
    Out.vPosition = worldPosition;
    Out.vNormal = In.vNormal;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

VS_OUT VS_MAIN_Steve(VS_IN In)
{
    VS_OUT Out;

    // �ν��Ͻ� ��ġ�� ���� ��ȯ�� �߰�
    float4 worldPosition = float4(In.vPosition, 1.0f); // vPosition�� float4�� ó��
    worldPosition = mul(worldPosition, g_WorldMatrix); // ���� ��ȯ
    worldPosition = mul(worldPosition, g_ViewMatrix); // �� ��ȯ
    worldPosition = mul(worldPosition, g_ProjMatrix); // �������� ��ȯ
    
    
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

    // ��� ����
    Out.vColor.rgb = skyColor * g_Bright;
    Out.vColor.a = 1.0; // ���İ� 1.0 (������)

    return Out;
}


PS_OUT PS_MAIN_STEVE(PS_IN In)
{
    PS_OUT Out;

    Out.vColor= tex2D(DefaultSampler, In.vTexcoord);
    Out.vColor.rgb *= g_Bright; // ���İ� 1.0 (������) 
    
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