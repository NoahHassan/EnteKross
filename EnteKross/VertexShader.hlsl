struct VSOut
{
    float4 col : Color;
    float4 pos : SV_Position;
};

VSOut main(float2 pos : Position, float4 col : Color )
{
    VSOut vOut;
    vOut.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
    vOut.col = col;
    return vOut;
}