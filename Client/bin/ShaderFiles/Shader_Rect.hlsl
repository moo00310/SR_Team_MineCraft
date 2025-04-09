
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

// ī�޶� ���� ��ǥ.
vector g_CameraWorld;

texture g_Texture;
float g_Bright;

// �Ȱ��� ���۵Ǵ� �Ÿ�.
float g_fFogDistance = 10.f;

// �Ȱ� ��.
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
    float1 vDistance : TEXCOORD1; // ī�޶�� �� �Ÿ�.
};

sampler2D DefaultSampler : register(s0);

VS_OUT VS_MAIN_SKYBOX(VS_IN In)
{
    VS_OUT Out;
    
    float4 worldPosition = float4(In.vPosition, 0.1f); // 
    worldPosition = mul(worldPosition, g_WorldMatrix); // ���� ��ȯ
    
    // ī�޶�� ���� ��ȯ �� �� �Ÿ� ���.
    Out.vDistance = length(worldPosition - g_CameraWorld);
    
    worldPosition = mul(worldPosition, g_ViewMatrix); // �� ��ȯ
    worldPosition = mul(worldPosition, g_ProjMatrix); // �������� ��ȯ
    
    
    Out.vPosition = worldPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

VS_OUT VS_MAIN_ITEMRECT(VS_IN In)
{
    VS_OUT Out;
    
    float4 worldPosition = float4(In.vPosition, 1.0f); // 
    worldPosition = mul(worldPosition, g_WorldMatrix); // ���� ��ȯ
    
    // ī�޶�� ���� ��ȯ �� �� �Ÿ� ���.
    Out.vDistance = length(worldPosition - g_CameraWorld);
    
    worldPosition = mul(worldPosition, g_ViewMatrix); // �� ��ȯ
    worldPosition = mul(worldPosition, g_ProjMatrix); // �������� ��ȯ
    
    
    Out.vPosition = worldPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}


struct PS_IN
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float1 vDistance : TEXCOORD1; // ī�޶�� �� �Ÿ�.
};

struct PS_OUT
{
    vector vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);
    
    // �Ȱ� ���� �ٱ��ΰ�?
    if (In.vDistance >= g_fFogDistance)
    {
        // �Ȱ����� �Ÿ� ����.
        float distance = saturate(In.vDistance - g_fFogDistance);
        
        // ���� ����.
        float4 color = lerp(Out.vColor, g_vFogColor, distance);
        
        // �Ȱ� �� ����.
        Out.vColor.rgb = color * g_Bright;
        
        // �Ȱ� ���� ������.   
        if (Out.vColor.r >= (g_vFogColor.r - distance) * g_Bright ||
        Out.vColor.g >= (g_vFogColor.g - distance) * g_Bright ||
        Out.vColor.b >= (g_vFogColor.b - distance) * g_Bright)
        {
            discard;
        }
        
        // �������� ���� ���ǹ� �� �Ծ ���� ����ó����.
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
    
    // �������̸� �ȼ��� ����
    if (Out.vColor.r == 0 && Out.vColor.g == 0 && Out.vColor.b == 0)
        discard;

    return Out;
}

PS_OUT PS_MAIN_SWORD_AURA(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);       
    
    // �ȼ� ������
    if (Out.vColor.a < 0.2f)
        discard;
    
    // ���������� ��.
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