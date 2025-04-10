
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

// ī�޶� ���� ��ǥ.
vector g_CameraWorld;

texture g_Texture;

// �Ȱ��� ���۵Ǵ� �Ÿ�.
float g_fFogStartDistance = 5.f;

// �Ȱ��� ������ �Ÿ�.
float g_fFogEndDistance = 25.f;

// �Ȱ� ��.
vector g_vFogColor = vector(0.529f, 0.808f, 0.922f, 1.f);

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vInstancePos : TEXCOORD1; //�ν��Ͻ� ���� (��ġ)
    float1 vBright : TEXCOORD2; //�ν��Ͻ� ���� (���)
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float3 vNormal : NORMAL;
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
    Out.vNormal = In.vNormal;
    Out.vTexcoord = In.vTexcoord;
    Out.vBright = In.vBright;

    return Out;
}


struct PS_IN
{
    float4 vPosition : POSITION;
    float3 vNormal : NORMAL;
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
          
    if (In.vDistance >= g_fFogEndDistance)
    {
        // �Ȱ� ������ �Ÿ� �ٱ��� �ִ� ����� �ƿ� �� ���̰� ����.
        discard;
    }
    else if (In.vDistance >= g_fFogStartDistance)
    {             
        // �Ȱ� ���� �Ÿ����� �Ȱ����� ����.
        
        // �Ȱ� ���Ÿ� - ���۰Ÿ� ��� ��.
        float fEnd = g_fFogEndDistance - g_fFogStartDistance;   // N
        
        // �Ȱ� ���Ÿ��� ���� �� �Ÿ��� ���� �� ���̳����� ���Ѵ�.
        float fCurrent = g_fFogEndDistance - In.vDistance;      // N~0                
        
        // ���������� ���� ��� ��.
        float fResult = fCurrent / fEnd;                        // 1~0
        
        // ���� ����.
        float4 color = lerp(g_vFogColor, Out.vColor, fResult);
        
        // �Ȱ� �� ����.
        Out.vColor.rgb = color * In.vBright;
    }
    else
    {
        // �Ȱ� ���� ���̸� �⺻��.
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
    }

    pass asdouble
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_RED();
    }
}