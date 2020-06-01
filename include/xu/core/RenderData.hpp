// MIT License
//
// Copyright (c) 2020 Xu Collaborators
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <vector>
#include <xu/core/Definitions.hpp>

namespace xu {

enum class DrawCommandType { NewLayer, MergeLayer, DrawTriangles };

struct XU_API DrawCommand {
    DrawCommandType type;
};

// The RenderData contains one implicit default layer, which does not need to be
// added using NewLayer
class XU_API RenderData {
public:
    // Iterator that can iterate over the drawcommands in the RenderData, keeping track of things like the current layer, vertex offset, etc.
    // This class satisfies the BidirectionalIterator concept
    class XU_API Iterator {
    public:
        using UnderlyingType = std::vector<DrawCommand>::const_iterator;

        Iterator() = default;
        Iterator(UnderlyingType it, size_t layer = 0);

        Iterator(Iterator const &) = default;
        Iterator &operator=(Iterator const &) = default;

        DrawCommand const &operator*() const;
        DrawCommand const *operator->() const;

        // Returns the index of the current drawing layer. When the current draw command type is NewLayer or MergeLayer, 
        // the old layer is returned and the next iterator will have an updated layer index.
        size_t CurrentLayer() const;

        // Returns the index of the layer to merge with. Only valid if the draw command type is MergeLayer and the current layer is not the
        // default layer (the layer at index 0). Equivalent to CurrentLayer() - 1
        size_t MergeTarget() const;

        Iterator& operator++();
        Iterator operator++(int);

        Iterator operator--();
        Iterator operator--(int);

        bool operator==(Iterator const &rhs) const;
        bool operator!=(Iterator const &rhs) const;

        bool operator<(Iterator const &rhs) const;
        bool operator<=(Iterator const &rhs) const;

        bool operator>(Iterator const &rhs) const;
        bool operator>=(Iterator const &rhs) const;

    private:
        UnderlyingType it;
        size_t currentLayer = 0;
    };

    size_t NumLayers() const;
    
    Iterator Begin() const;
    Iterator End() const;

private:
    friend class Context;

    std::vector<DrawCommand> commands;

    std::vector<float> vertices;
    std::vector<uint32_t> indices;
};

} // namespace xu