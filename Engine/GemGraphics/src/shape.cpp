#include <Gem/Graphics/shapes/sphere.h>
#include <cmath>
#include <iostream>

namespace Gem {
    namespace Graphics {
        namespace Shapes {

            Sphere::Sphere(unsigned int latitudeSegments, unsigned int longitudeSegments)
                : latitudeSegments_(latitudeSegments), longitudeSegments_(longitudeSegments) {
                generateData();
            }

            Sphere::~Sphere() {

                VAO_.cleanup();
                VBO_.cleanup();
                EBO_.cleanup();
            }

            void Sphere::generateData() {
                for (unsigned int y = 0; y <= latitudeSegments_; ++y) {
                    for (unsigned int x = 0; x <= longitudeSegments_; ++x) {
                        float xSegment = static_cast<float>(x) / static_cast<float>(longitudeSegments_);
                        float ySegment = static_cast<float>(y) / static_cast<float>(latitudeSegments_);
                        float xPos = std::cos(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);
                        float yPos = std::cos(ySegment * M_PI);
                        float zPos = std::sin(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);

                        vertices_.push_back(xPos);
                        vertices_.push_back(yPos);
                        vertices_.push_back(zPos);
                    }
                }

                bool oddRow = false;
                for (unsigned int y = 0; y < latitudeSegments_; ++y) {
                    if (!oddRow) { // even rows: y == 0, y == 2; and so on
                        for (unsigned int x = 0; x <= longitudeSegments_; ++x) {
                            indices_.push_back(y * (longitudeSegments_ + 1) + x);
                            indices_.push_back((y + 1) * (longitudeSegments_ + 1) + x);
                        }
                    }
                    else {
                        for (int x = longitudeSegments_; x >= 0; --x) {
                            indices_.push_back((y + 1) * (longitudeSegments_ + 1) + x);
                            indices_.push_back(y * (longitudeSegments_ + 1) + x);
                        }
                    }
                    oddRow = !oddRow;
                }
            }

            const std::vector<float>& Sphere::getVertices() const {
                return vertices_;
            }

            const std::vector<unsigned int>& Sphere::getIndices() const {
                return indices_;
            }

            void Sphere::initialize() {
                VAO_.generate();
                VBO_.generate();
                EBO_.generate();
                
                VAO_.link_attrib(VBO_, GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

                // Position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glBindVertexArray(0); // Unbind VAO
            }

            void Sphere::render() const {
                glBindVertexArray(VAO_);
                glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }

        } // namespace Shapes
    } // namespace Graphics
} // namespace Gem