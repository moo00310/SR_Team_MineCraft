matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture g_Texture;
bool g_isColl;
float g_alpha;
float g_alpha1;
float GlowIntensity = 1.2f; // ���� ���� (���� Ű��� �� �����)
float g_Time;

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
}


