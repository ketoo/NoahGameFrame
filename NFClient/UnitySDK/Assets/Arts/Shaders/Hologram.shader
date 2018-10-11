Shader "Custom/Hologram"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		[HDR]_HoloColor("Hologram Color", Color) = (0, 0.7, 0, 1)
		_HoloNoise("Noise Texture", 2D) = "white" {}
		_ScanlineData("scanline: x = speed, y = scale | noise: z = Scale, w = Strength", Vector) = (4,1,10,0.6)
		_RimPower("Rim Strength", Range(0.1,5.0)) = 0.5
		_MinAlpha("Minimum Alpha", Range(0.0,1.0)) = 0.2
	}
	SubShader
	{
		Tags { "RenderType"="Transparent" "Queue"="Transparent" }
		
		LOD 100

		Pass{
			ZWrite On
			ColorMask 0
		}

		Pass
		{
			Blend SrcAlpha OneMinusSrcAlpha

			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			// make fog work
			#pragma multi_compile_fog
			
			#include "UnityCG.cginc"

			struct appdata
			{
				float4 vertex : POSITION;
				float4 normal : NORMAL;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				float4 normal : TEXCOORD1;
				float4 worldPos : TEXCOORD2;
				float3 viewDir : TEXCOORD3;
				UNITY_FOG_COORDS(4)
				float4 vertex : SV_POSITION;
			};

			sampler2D _MainTex;
			sampler2D _HoloNoise;
			float4 _MainTex_ST;
			float4 _HoloColor;
			float4 _ScanlineData;
			float _RimPower;
			float _MinAlpha;
			
			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.uv = TRANSFORM_TEX(v.uv, _MainTex);
				o.normal = v.normal;
				o.worldPos = o.vertex;
				o.viewDir = ObjSpaceViewDir(v.vertex);
				UNITY_TRANSFER_FOG(o,o.vertex);
				return o;
			}
			
			fixed4 frag (v2f i) : SV_Target
			{
				float atten = pow(1.0f - saturate(dot(i.normal.xyz, normalize(i.viewDir))), _RimPower);
				atten = max(_MinAlpha, atten);

				// sample the texture
				fixed4 col = tex2D(_MainTex, i.uv) * _HoloColor;
				
				float y = (i.worldPos.y + _Time.y * _ScanlineData.x) * _ScanlineData.y;

				float4 scanLine = tex2D(_HoloNoise, float2(0.5, y));
				float4 scanNoise = tex2D(_HoloNoise, float2(i.worldPos.x * _ScanlineData.z, y));

				clip(scanLine.x - 0.1f);

				atten *= scanLine.z + scanNoise * _ScanlineData.w;

				// apply fog
				UNITY_APPLY_FOG(i.fogCoord, col);

				col.a = atten;

				return col;
			}
			ENDCG
		}
	}
}
