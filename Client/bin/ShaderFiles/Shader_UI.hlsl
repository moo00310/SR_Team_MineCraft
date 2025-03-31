matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture g_Texture;
bool g_isColl;
float g_alpha;
float g_alpha1;
float GlowIntensity = 1.2f; // 형광 강도 (값을 키우면 더 밝아짐)
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

/* 형광(네온) 효과 적용 */
PS_OUT PS_Fluorescent(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0; 

	Out.vColor = tex2D(TextureSampler, In.vTexcoord);
	
	// 초록색으로 변경
	 float3 baseGreen = float3(0.0f, 1.0f, 0.0f); // 초록색 계열 강조 
	 Out.vColor.rgb = lerp(Out.vColor.rgb, baseGreen, 0.9f); // 기존 텍스처와 초록색 블렌딩

	// 형광 효과 강화 (텍스쳐 색상 + GlowIntensity 값 조절)
	//float3 glowColor = Out.vColor.rgb + float3(0.3f, 1.2f, 0.3f) * GlowIntensity; 
	float3 glowColor = Out.vColor.rgb + float3(0.0f, 1.5f, 0.0f) * GlowIntensity; 

	// 색상이 너무 밝아지지 않도록 제한
	Out.vColor.rgb = saturate(glowColor);

	return Out;
}

PS_OUT PS_PlayerHunger(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0; 

	Out.vColor = tex2D(TextureSampler, In.vTexcoord);
	
	// 시간에 따른 반짝이는 효과
	float flicker = 0.75f + 0.25f * sin(g_Time * 10.0f); // 시간에 따른 반짝임


	// 반짝이는 효과 적용
	Out.vColor.rgb *= flicker;  
	
	return Out;
}



/* Technique 정의 */
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

		CULLMODE = CCW;			// 뒷면 제거하여 겹침 방지
		ZWRITEENABLE = FALSE;  // 깊이 버퍼 쓰기를 비활성화
		ZENABLE = TRUE;        // 깊이 테스트 활성화

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
		ZWRITEENABLE = FALSE;  // 깊이 버퍼 쓰기를 비활성화
		ZENABLE = TRUE;        // 깊이 테스트 활성화

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_PlayerHunger();
    }
}


