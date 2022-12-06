cbuffer cbuf
{
    float3 color : COLOR;
};

float4 main() : SV_TARGET
{
	return float4(color, 1.0f);
}