#pragma once

#include <Gem/Graphics/buffer.h>
#include <Gem/Graphics/vao.h>
#include <vector>

namespace Gem {
    namespace Graphics {
        namespace Shapes {

            /**
             * @class Sphere
             * @brief Represents a 3D sphere shape with vertex and index data.
             */
            class Sphere {

            public:

                Sphere(unsigned int latitudeSegments = 32, unsigned int longitudeSegments = 32);
                ~Sphere();

                /**
                 * @brief Retrieves the vertex data for the sphere.
                 * @return A vector of floats representing vertex positions.
                 */
                const std::vector<float>& getVertices() const;

                /**
                 * @brief Retrieves the index data for the sphere.
                 * @return A vector of unsigned integers representing indices.
                 */
                const std::vector<unsigned int>& getIndices() const;

                /**
                 * @brief Initializes OpenGL buffers for the sphere.
                 */
                void initialize();

                /**
                 * @brief Renders the sphere.
                 */
                void render() const;

            private:

                /**
                 * @brief Generates vertex and index data for the sphere.
                 */
                void generateData();

            private:

                unsigned int latitudeSegments_;
                unsigned int longitudeSegments_;

                std::vector<float> vertices_;
                std::vector<unsigned int> indices_;

                Gem::Graphics::VAO VAO_;
                Gem::Graphics::Buffer VBO_, EBO_;

            };

        } // namespace Shapes
    } // namespace Graphics
} // namespace Gem