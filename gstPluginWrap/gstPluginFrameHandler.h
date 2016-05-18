/*
 * Copyright (C) 2013 Vladimir Chernov
 *
 * This file is part of gstPluginWrap library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GSTPLUGINFRAMEHANDLER_H_
#define GSTPLUGINFRAMEHANDLER_H_

#include <stdint.h>

namespace gstPluginWrap {

/**
 * Abstract class for frame processing inside GStreamer plugin.
 */
class PureFrameHandler {
public:
    virtual ~PureFrameHandler() {}

    virtual void process(uint8_t* buffer) = 0;
};

/**
 * Frame processing handler with GStreamer properties.
 */
template<class PropT>
class FrameHandler: public PureFrameHandler {
public:
    virtual ~FrameHandler() {}

    void setProperties(PropT* properties) {
        this->properties = properties;
    }

protected:
    PropT* properties;
};

} // namespace gstPluginWrap

#endif // GSTPLUGINFRAMEHANDLER_H_
