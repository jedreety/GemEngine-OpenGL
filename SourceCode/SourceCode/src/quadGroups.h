#pragma once

#include <array>
#include <vector>
#include <cassert>
#include "Quad.h"
#include "Face.h"
#include "Side.h"

namespace Engine {

    class QuadGroups {
    public:
        // Constructor
        QuadGroups() = default;

        // Adds a Quad to a specific side
        void add_quad(const Side& side, const Quad& quad) {
            size_t index = side_to_index(side);
            assert(index < groups_.size());
            groups_[index].emplace_back(&quad);
        }

        // Efficiently iterates over all Faces
        template<typename Func>
        void for_each_face(Func&& func) const {
            for (size_t index = 0; index < groups_.size(); ++index) {
                const auto& quads = groups_[index];
                if (quads.empty()) continue;

                Side side = index_to_side(index);
                for (const Quad* quad : quads) {
                    func(Face(side, *quad));
                }
            }
        }

    private:
        // Array of vectors storing pointers to Quads, one for each side
        std::array<std::vector<const Quad*>, 6> groups_;

        // Maps Side to index
        static constexpr size_t side_to_index(const Side& side) noexcept {
            size_t axis_index = static_cast<size_t>(side.get_axis());
            return axis_index * 2 + (side.get_positive() ? 0 : 1);
        }

        // Maps index to Side
        static constexpr Side index_to_side(size_t index) noexcept {
            Axis axis = static_cast<Axis>(index / 2);
            bool positive = (index % 2 == 0);
            return Side(axis, positive);
        }
    };

}
