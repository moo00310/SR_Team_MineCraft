
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

// ī�޶� ���� ��ǥ.
vector g_CameraWorld;

texture g_Texture;

// �Ȱ��� ���۵Ǵ� �Ÿ�.
float g_fFogDistance = 10.f;

// �Ȱ� ��.
vector g_vFogColor = vector(1.f, 1.f, 1.f, 1.f);

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float3 vInstancePos : TEXCOORD1; //�ν��Ͻ� ���� (��ġ��)
    float1 vBright : TEXCOORD2; //�ν��Ͻ� ���� (���)
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float1 vBright : TEXCOORD2; //�ν��Ͻ� ���� (���)
    float1 vDistance : TEXCOORD3; // ī�޶�� �� �Ÿ�.
};

sampler2D DefaultSampler : register(s0);

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    // �ν��Ͻ� ��ġ�� ���� ��ȯ�� �߰�
    float4 worldPosition = float4(In.vPosition + In.vInstancePos, 1.0f); // vPosition�� float4�� ó��
    worldPosition = mul(worldPosition, g_WorldMatrix); // ���� ��ȯ
    
    // ī�޶�� ���� ��ȯ �� �� �Ÿ� ���.
    Out.vDistance = length(worldPosition - g_CameraWorld);
    
    worldPosition = mul(worldPosition, g_ViewMatrix); // �� ��ȯ
    worldPosition = mul(worldPosition, g_ProjMatrix); // �������� ��ȯ
    
    
    
    Out.vPosition = worldPosition;
    Out.vTexcoord = In.vTexcoord;
    Out.vBright = In.vBright;

    return Out;
}


struct PS_IN
{
    float4 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float1 vBright : TEXCOORD2; //�ν��Ͻ� ���� (���)
    float1 vDistance : TEXCOORD3; // ī�޶�� �� �Ÿ�.
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
        Out.vColor.rgb = color * In.vBright;
        
        // �Ȱ� ���� ������.   
        if (Out.vColor.r >= (g_vFogColor.r - distance) * In.vBright ||
        Out.vColor.g >= (g_vFogColor.g - distance) * In.vBright ||
        Out.vColor.b >= (g_vFogColor.b - distance) * In.vBright)
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