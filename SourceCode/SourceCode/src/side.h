#pragma once

#include <array>
#include <cassert>

namespace Engine
{
    // Enum class defining possible axes for object orientation
    enum class Axis {
        X,
        Y,
        Z
    };

    // Class representing a side of an object defined by an axis and direction
    class Side
    {
    public:
        // Constructor to initialize Side with specified axis and direction, marked noexcept for efficiency
        constexpr Side(Axis axis, bool positive) noexcept : axis_(axis), positive_(positive) {}

        // Returns a normal vector based on the axis and direction
        [[nodiscard]] constexpr std::array<float, 3> normal() const noexcept {
            // Initializes normal vector with zeroes, sets value based on axis and direction
            std::array<float, 3> norm = { 0.0f, 0.0f, 0.0f };
            norm[static_cast<size_t>(axis_)] = positive_ ? 1.0f : -1.0f;
            return norm;
        }

        // Returns an array of four identical normal vectors
        [[nodiscard]] constexpr std::array<std::array<float, 3>, 4> normals() const noexcept {
            auto norm = normal();
            return { norm, norm, norm, norm };
        }

        // Sets the axis of the Side, returns reference for chaining
        inline Side& set_axis(Axis axis) noexcept { axis_ = axis; return *this; }

        // Sets the direction of the Side, returns reference for chaining
        inline Side& set_positive(bool positive) noexcept { positive_ = positive; return *this; }

        // Getter for axis
        constexpr Axis get_axis() const noexcept { return axis_; }

        // Getter for direction
        constexpr bool get_positive() const noexcept { return positive_; }

        // Equality operator, compares two Side objects for identical axis and direction
        bool operator==(const Side& other) const noexcept = default;

    private:
        Axis axis_;     // Axis of the Side (X, Y, or Z)
        bool positive_; // Direction of the Side (true for positive, false for negative)
    };
}