// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for license information.

#pragma once

namespace canvas
{
    class StubCanvasDrawingSessionAdapter : public ICanvasDrawingSessionAdapter
    {
    public:
        StubCanvasDrawingSessionAdapter()
        {}

        virtual void EndDraw() override
        {
            // nothing
        }

        virtual D2D1_POINT_2F GetRenderingSurfaceOffset() override
        {
            return D2D1::Point2F(0, 0);
        }

#if WINVER > _WIN32_WINNT_WINBLUE
        virtual ComPtr<IInkD2DRenderer> CreateInkRenderer() override
        {
            return nullptr;
        }

        virtual bool IsHighContrastEnabled() override
        {
            return false;
        }
#endif
    };
}
