
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

// 카메라 월드 좌표.
vector g_CameraWorld;

texture g_Texture;

// 안개가 시작되는 범위.
float g_fFogStartRange = 5.f;

// 안개가 끝나는 범위.
float g_fFogEndRange = 25.f;

// 안개 색.
vector g_vFogColor = vector(0.529f, 0.808f, 0.922f, 1.f);

// 스캔 시작 범위.
float g_fScanStartRange;

// 스캔 색.
vector g_vScanColor = vector(1.f, 0.f, 1.f, 1.f);

// 블럭 타입.
// Client_Enum ITEMNAME 참조.
int g_iBlockType;

// 철.
int g_iIronType = 900;

// 석탄.
int g_iCoalType = 901;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vInstancePos : TEXCOORD1; //인스턴싱 버퍼 (위치)
    float1 vBright : TEXCOORD2; //인스턴싱 버퍼 (밝기)
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float1 vBright : TEXCOORD2; //인스턴싱 버퍼 (밝기)
    float1 vDistance : TEXCOORD3; // 카메라와 블럭 거리.
};

sampler2D DefaultSampler : register(s0);

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    // 인스턴스 위치를 월드 변환에 추가
    float4 worldPosition = float4(In.vPosition + In.vInstancePos, 1.0f); // vPosition을 float4로 처리
    worldPosition = mul(worldPosition, g_WorldMatrix); // 월드 변환
    
    // 카메라와 월드 변환 된 블럭 거리 계산.
    Out.vDistance = length(worldPosition - g_CameraWorld);
    
    worldPosition = mul(worldPosition, g_ViewMatrix); // 뷰 변환
    worldPosition = mul(worldPosition, g_ProjMatrix); // 프로젝션 변환
        
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
    float1 vBright : TEXCOORD2; //인스턴싱 버퍼 (밝기)
    float1 vDistance : TEXCOORD3; // 카메라와 블럭 거리.
};

struct PS_OUT
{
    vector vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;            
    
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);    
          
    if (In.vDistance >= g_fFogEndRange)
    {
        // 안개 끝나는 범위 바깥에 있는 얘들은 아예 안 보이게 무시.
        discard;
    }
    else if (In.vDistance >= g_fFogStartRange)
    {             
        // 안개 시작 범위에선 안개색상 지정.
        
        // 안개 끝 범위 - 시작 범위 결과 값.
        float fEnd = g_fFogEndRange - g_fFogStartRange;         // N
        
        // 안개 끝 범위랑 현재 블럭 거리를 빼서 얼마 차이나는지 구한다.
        float fCurrent = g_fFogEndRange - In.vDistance;         // N~0                
        
        // 선형보간에 쓰일 결과 값.
        float fResult = fCurrent / fEnd;                        // 1~0
        
        // 선형 보간.
        float4 color = lerp(g_vFogColor, Out.vColor, fResult);
        
        // 안개 색 입힘.
        Out.vColor.rgb = color * In.vBright;
    }
    else
    {
        // 안개 범위 안이면 기본색.
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

PS_OUT PS_MAIN_SCAN(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = tex2D(DefaultSampler, In.vTexcoord);
    
    // 스캔 범위 끝.
    float fScanEndRange = g_fScanStartRange + 3.f;
     
    if (In.vDistance >= g_fFogEndRange)
    {
        // 안개 끝나는 범위 바깥에 있는 얘들은 아예 안 보이게 무시.
        discard;
    }
    else if (In.vDistance >= fScanEndRange)
    {
        // 스캔 범위 바깥에 있는 얘들은 안개 쉐이딩 적용.
        
        // 안개 끝 범위 - 시작 범위 결과 값.
        float fEnd = g_fFogEndRange - g_fFogStartRange; // N
        
        // 안개 끝 범위랑 현재 블럭 거리를 빼서 얼마 차이나는지 구한다.
        float fCurrent = g_fFogEndRange - In.vDistance; // N~0                
        
        // 선형보간에 쓰일 결과 값.
        float fResult = fCurrent / fEnd; // 1~0
        
        // 선형 보간.
        float4 color = lerp(g_vFogColor, Out.vColor, fResult);
        
        // 안개 색 입힘.
        Out.vColor.rgb = color * In.vBright;
    }
    else if (In.vDistance >= g_fScanStartRange)
    {
        // 스캔 시작 범위에선 스캔 색상 지정.               
        
        // 스캔 끝 범위 - 스캔 시작 범위 결과 값.
        float fEnd = fScanEndRange - g_fScanStartRange; // N
        
        // 스캔 끝 범위랑 현재 블럭 거리를 빼서 얼마 차이나는지 구한다.
        float fCurrent = fScanEndRange - In.vDistance; // N~0                
        
        // 선형보간에 쓰일 결과 값.
        float fResult = fCurrent / fEnd;                // 1~0
        
        // 선형 보간.
        float4 color = lerp(g_vScanColor, Out.vColor, fResult);
        
        // 스캔 색 입힘.
        Out.vColor.rgb = color * In.vBright;
    }
    else
    {
        // 스캔 범위 안에 있으면 투명 블럭.
        Out.vColor.rgb *= 1.f;
        Out.vColor.a = 0.1f;
        
        // 광물들은 보이게 함.
        if (g_iBlockType == g_iIronType || g_iBlockType == g_iCoalType)
        {
            Out.vColor.a = 1.f;
        }
    }        
    
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

    pass ScanPass
    {
        ALPHABLENDENABLE = true;        
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        BlendOp = Add;
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_SCAN();
    }
}