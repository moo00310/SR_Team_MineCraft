matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture g_Texture;
bool g_isColl;
float g_alpha;
float g_alpha1;
float GlowIntensity = 1.2f; // 형광 강도 (값을 키우면 더 밝아짐)
float g_Time;

float g_burn;
float g_burnResult;
float g_LoadingPercent;
float g_ShakeStrength;

float startY = 0.618f;   // 물 시작 위치 (vTexcoord.y)
float endY   = 0.9375f;  // 물 끝 위치
float2 lightOrigin = float2(0.41f, 0.15f); // 빨간 네모 중앙 근처


// 회전을 적용한 UV 좌표 생성 함수
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

/* 반짝이는 효과 */
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

/* 폰트 선명도 조절 */
PS_OUT PS_FontEnhance(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0; 

	// 원본 텍스처 샘플링
    Out.vColor = tex2D(TextureSampler, In.vTexcoord);

	// 밝기 강화
	float brightnessBoost = 1.2f;
	Out.vColor.rgb *= brightnessBoost;

	// 대비 증가 (어두운 부분은 더 어둡게, 밝은 부분은 더 밝게)
	//Out.vColor.rgb = pow(Out.vColor.rgb, 1.3f); // 1.2~1.5 사이가 적당함
	Out.vColor.rgb = saturate((Out.vColor.rgb - 0.4) * 1.5 + 0.4);

	  Out.vColor.rgb = lerp(Out.vColor.rgb, Out.vColor.rgb * 1.5, 0.3);

	// 완전한 흰색으로 보이도록 조정 (너무 밝으면 순수 흰색으로 변경)
     if (Out.vColor.r > 0.9 && Out.vColor.g > 0.9 && Out.vColor.b > 0.9)
    {
        Out.vColor.rgb = float3(1.0, 1.0, 1.0);
    }

	// 알파값 조정 (투명한 부분 없애고 선명하게)
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
    if (In.vTexcoord.y < g_burn)
        discard; // 또는 clip(-1); 둘 다 됨
	
    float4 texColor = tex2D(TextureSampler, In.vTexcoord);

    Out.vColor = texColor;
    return Out;
}

PS_OUT PS_BurnResultUi(PS_IN In)
{
    PS_OUT Out;
    if (In.vTexcoord.x > g_burnResult)
        discard; // 또는 clip(-1); 둘 다 됨
	
    float4 texColor = tex2D(TextureSampler, In.vTexcoord);

    Out.vColor = texColor;
    return Out;
}

PS_OUT PS_ShakeEffect(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	float2 offset = float2(sin(g_Time * 40.0f), cos(g_Time * 50.0f)) * g_ShakeStrength;
	Out.vColor = tex2D(TextureSampler, In.vTexcoord + offset);
	return Out;
}

PS_OUT PS_WaterWobble(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    float2 uv = In.vTexcoord;

    float strength = 0.005f + 0.003f * sin(g_Time * 2.0f);
    float frequency = 50.0f + 30.0f * uv.y;

    float offsetX = sin(g_Time * frequency + uv.y * 20.0f) * strength;
    float offsetY = cos(g_Time * frequency + uv.x * 10.0f) * strength * 0.5f;

    float startY = 0.618f;
    float endY   = 0.9375f;

    if (uv.y >= startY && uv.y <= endY)
        uv += float2(offsetX, offsetY);

    Out.vColor = tex2D(TextureSampler, uv);
    return Out;
}

PS_OUT PS_SunShaft(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;
    float2 uv = In.vTexcoord;

    float2 delta = uv - lightOrigin;
    float dist = length(delta);
    
    float ray = 1.0 - smoothstep(0.0, 0.5, dist);  // 광선 길이 조정
    float intensity = ray * 0.4f;                 // 밝기 조절

    float4 tex = tex2D(TextureSampler, uv);
    tex.rgb += float3(1.0f, 1.0f, 0.8f) * intensity; // 노란 빛 추가

    Out.vColor = tex;
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

    pass ShakeEffectPass
	{
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		BlendOp = ADD;
		CULLMODE = NONE;
		ZWRITEENABLE = FALSE;
		ZENABLE = FALSE;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_ShakeEffect();
	}

    pass WaterSelectiveWobblePass
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;
        BlendOp = ADD;

        CULLMODE = NONE;
        ZWRITEENABLE = FALSE;
        ZENABLE = FALSE;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_WaterWobble();
    }

    pass SunShaftPass
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;
        BlendOp = ADD;

        CULLMODE = NONE;
        ZWRITEENABLE = FALSE;
        ZENABLE = FALSE;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_SunShaft();
    }
}


