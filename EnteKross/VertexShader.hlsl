cbuffer cbuf
{
    matrix transform;
};

struct VSOut
{
    float4 col : Color;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float4 col : Color )
{
    VSOut vOut;
    vOut.pos = mul(float4(pos, 1.0f), transform);
    vOut.col = col;
    return vOut;
}