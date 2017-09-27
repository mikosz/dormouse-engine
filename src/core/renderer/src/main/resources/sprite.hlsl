struct VIn {
	float2 posL : POSITION;
	float2 tex : TEXCOORD;
};

struct PIn {
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
};

cbuffer sprite {
	matrix toNDC;
};

Texture2D sprite_texture : register(t0);
SamplerState sprite_sampler : register(s0);

PIn vs(VIn vin) {
	PIn pin;
	
	float4 pos = float4(vin.posL, 0.0f, 1.0f);
	pin.posH = mul(pos, toNDC);
	pin.tex = vin.tex;
	
	return pin;
}

float4 ps(PIn pin) : SV_TARGET {
	return sprite_texture.Sample(sprite_sampler, pin.tex);
}
