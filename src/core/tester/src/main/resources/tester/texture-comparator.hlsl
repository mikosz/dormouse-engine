Texture2D<float4> reference : register(t0);
Texture2D<float4> actual : register(t1);

RWTexture2D<float4> diff : register(u0);
RWBuffer<bool> same : register(u1);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{

}
