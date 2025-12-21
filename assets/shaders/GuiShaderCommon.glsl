layout(set=0, binding=0) uniform sampler LinearTextureSampler;
layout(set=0, binding=1) uniform sampler NearestTextureSampler;

struct GuiElement
{
    uint type;
    int padding;
    float borderRoundRadius;
    float borderSize;

    vec2 rectangleMin;
    vec2 rectangleMax;
    
    vec2 sourceImageRectangleMin;
    vec2 sourceImageRectangleMax;

    vec4 firstColor;
    vec4 secondColor;
    vec4 borderColor;
};

layout(set=1, binding=0, std430) buffer GuiElementsBlock
{
	GuiElement[] list;
} GuiElementList;

layout(set=2, binding=0) uniform texture2D Texture;

layout( push_constant ) uniform constants
{
    bool hasTopLeftNDCOrigin;
    uint padding;
    vec2 framebufferReciprocalExtent;
} PushConstants;

const uint GuiElementType_SolidRectangle = 0;
const uint GuiElementType_HorizontalGradient = 1;
const uint GuiElementType_VerticalGradient = 2;
const uint GuiElementType_TextCharacter = 3;
const uint GuiElementType_TexturedRectangle = 4;
