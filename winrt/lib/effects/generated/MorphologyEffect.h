// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for license information.

// This file was automatically generated. Please do not edit it manually.

#pragma once

namespace ABI { namespace Microsoft { namespace Graphics { namespace Canvas { namespace Effects 
{
    using namespace ::Microsoft::WRL;
    using namespace ABI::Microsoft::Graphics::Canvas;

    class MorphologyEffect : public RuntimeClass<
        IMorphologyEffect,
        MixIn<MorphologyEffect, CanvasEffect>>,
        public CanvasEffect
    {
        InspectableClass(RuntimeClass_Microsoft_Graphics_Canvas_Effects_MorphologyEffect, BaseTrust);

    public:
        MorphologyEffect();

        EFFECT_PROPERTY(Mode, MorphologyEffectMode);
        EFFECT_PROPERTY(Width, int32_t);
        EFFECT_PROPERTY(Height, int32_t);
        EFFECT_PROPERTY(Source, IGraphicsEffectSource*);

        EFFECT_PROPERTY_MAPPING();
    };
}}}}}
