matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture g_Texture;
bool g_isColl;
float g_alpha;
float g_alpha1;
float GlowIntensity = 1.2f; // ���� ���� (���� Ű��� �� �����)
float g_Time;

float g_burn;
float g_burnResult;
float g_LoadingPercent;

// ȸ���� ������ UV ��ǥ ���� �Լ�
float2 RotateUV(float2 uv, float angle)
{
    float2 center = float2(0.5, 0.5);
    float2 delta = uv - center;
    float cosA = cos(angle);
    float sinA = sin(angle);

    float2 rotated;
    rotated.x = delta.x * cosA - delta.y * sinA;
    rotated.y = delta.x * sinA + delta.y * cosA;
    return rotated + center;
}

sampler TextureSampler = sampler_state
{
	texture = g_Texture;
	//MINFILTER = linear;
    //MAGFILTER = linear;
    //MIPFILTER = linear;

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
	VS_OUT Out = (VS_OUT)0;

	matrix			matWV, matWVP;

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
	PS_OUT Out = (PS_OUT)0; 

	Out.vColor = tex2D(TextureSampler, In.vTexcoord);

	return Out;
}

/* ����(�׿�) ȿ�� ���� */
PS_OUT PS_Fluorescent(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0; 

	Out.vColor = tex2D(TextureSampler, In.vTexcoord);
	
	// �ʷϻ����� ����
	 float3 baseGreen = float3(0.0f, 1.0f, 0.0f); // �ʷϻ� �迭 ���� 
	 Out.vColor.rgb = lerp(Out.vColor.rgb, baseGreen, 0.9f); // ���� �ؽ�ó�� �ʷϻ� ����

	// ���� ȿ�� ��ȭ (�ؽ��� ���� + GlowIntensity �� ����)
	//float3 glowColor = Out.vColor.rgb + float3(0.3f, 1.2f, 0.3f) * GlowIntensity; 
	float3 glowColor = Out.vColor.rgb + float3(0.0f, 1.5f, 0.0f) * GlowIntensity; 

	// ������ �ʹ� ������� �ʵ��� ����
	Out.vColor.rgb = saturate(glowColor);

	return Out;
}

/* ��¦�̴� ȿ�� */
PS_OUT PS_PlayerHunger(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0; 

	Out.vColor = tex2D(TextureSampler, In.vTexcoord);
	
	// �ð��� ���� ��¦�̴� ȿ��
	float flicker = 0.75f + 0.25f * sin(g_Time * 10.0f); // �ð��� ���� ��¦��


	// ��¦�̴� ȿ�� ����
	Out.vColor.rgb *= flicker;  
	
	return Out;
}

/* ��Ʈ ���� ���� */
PS_OUT PS_FontEnhance(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0; 

	// ���� �ؽ�ó ���ø�
    Out.vColor = tex2D(TextureSampler, In.vTexcoord);

	// ��� ��ȭ
	float brightnessBoost = 1.2f;
	Out.vColor.rgb *= brightnessBoost;

	// ��� ���� (��ο� �κ��� �� ��Ӱ�, ���� �κ��� �� ���)
	//Out.vColor.rgb = pow(Out.vColor.rgb, 1.3f); // 1.2~1.5 ���̰� ������
	Out.vColor.rgb = saturate((Out.vColor.rgb - 0.4) * 1.5 + 0.4);

	  Out.vColor.rgb = lerp(Out.vColor.rgb, Out.vColor.rgb * 1.5, 0.3);

	// ������ ������� ���̵��� ���� (�ʹ� ������ ���� ������� ����)
     if (Out.vColor.r > 0.9 && Out.vColor.g > 0.9 && Out.vColor.b > 0.9)
    {
        Out.vColor.rgb = float3(1.0, 1.0, 1.0);
    }

	// ���İ� ���� (������ �κ� ���ְ� �����ϰ�)
	    if (Out.vColor.a < 0.1) discard;

    return Out;
}

PS_OUT PS_MissionMainUi(PS_IN In)
{
    PS_OUT Out;

    float4 texColor = tex2D(TextureSampler, In.vTexcoord);
    texColor.a *= 0.9;

    Out.vColor = texColor;
    return Out;
}

PS_OUT PS_BurnUi(PS_IN In)
{
    PS_OUT Out;
    if (In.vTexcoord.y < (1.0 - g_burn))
        discard; // �Ǵ� clip(-1); �� �� ��
	
    float4 texColor = tex2D(TextureSampler, In.vTexcoord);

    Out.vColor = texColor;
    return Out;
}

PS_OUT PS_BurnResultUi(PS_IN In)
{
    PS_OUT Out;
    if (In.vTexcoord.x > (1.0 - g_burnResult))
        discard; // �Ǵ� clip(-1); �� �� ��
	
    float4 texColor = tex2D(TextureSampler, In.vTexcoord);

    Out.vColor = texColor;
    return Out;
}

/* �߾� �ε� ������ ȿ���� Pixel Shader */
PS_OUT PS_LoadingIcon(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    // ȸ�� ���� (2 * PI * �ε� �ۼ�Ʈ)
    float angle = g_LoadingPercent * 6.2831853f;

    // ȸ���� UV ���
    float2 rotatedUV = RotateUV(In.vTexcoord, angle);

    // �ؽ�ó ���ø�
    float4 texColor = tex2D(TextureSampler, rotatedUV);

    // ��� ���� (�ε� �ۼ�Ʈ�� �����Ҽ��� ���)
    float brightness = lerp(0.5f, 1.5f, g_LoadingPercent);
    texColor.rgb *= brightness;

    // ���� ���
    Out.vColor = texColor;
    return Out;
}


/* Technique ���� */
technique DefaultTechnique
{
  pass DefaultPass_with_AlphaTest 
  {
		AlphaTestEnable = TRUE;
		AlphaFunc = greater;
		AlphaRef = 40;
		CULLMODE = NONE;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
   }
   
   pass FluorescentPass 
   {
		AlphablendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		BlendOp = Add;

		CULLMODE = CCW;			// �޸� �����Ͽ� ��ħ ����
		ZWRITEENABLE = FALSE;  // ���� ���� ���⸦ ��Ȱ��ȭ
		ZENABLE = TRUE;        // ���� �׽�Ʈ Ȱ��ȭ

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_Fluorescent();
    }

   pass PlayerHungerPass 
   {
		AlphablendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		BlendOp = Add;

		CULLMODE = NONE;
		ZWRITEENABLE = FALSE;  // ���� ���� ���⸦ ��Ȱ��ȭ
		ZENABLE = TRUE;        // ���� �׽�Ʈ Ȱ��ȭ

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_PlayerHunger();
    }

	pass FontEnhancePass 
	{
        AlphaTestEnable = TRUE;  
        AlphaFunc = GREATER;    
        AlphaRef = 80;     
		
      	CULLMODE = NONE;		
        ZWRITEENABLE = FALSE;   
        ZENABLE = FALSE;        

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
	}

    pass MissionMainUiPass
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;
        BlendOp = ADD;


        CULLMODE = NONE;
        ZWRITEENABLE = FALSE;
        ZENABLE = FALSE;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MissionMainUi();
    }

    pass FurnaceBurnPass
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;
        BlendOp = ADD;


        CULLMODE = NONE;
        ZWRITEENABLE = FALSE;
        ZENABLE = FALSE;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_BurnUi();
    }

    pass FurnaceBurnResultPass
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;
        BlendOp = ADD;


        CULLMODE = NONE;
        ZWRITEENABLE = FALSE;
        ZENABLE = FALSE;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_BurnResultUi();
    }

    pass P0
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;
        BlendOp = ADD;

        CULLMODE = NONE;
        ZWRITEENABLE = FALSE;
        ZENABLE = FALSE;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_LoadingIcon();
    }
}


