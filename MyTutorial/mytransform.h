#ifndef MYTRANSFORM_H
#define MYTRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>     // From wrapper to implement my own code
#include <glm/gtc/type_ptr.hpp>

namespace MyTransform
{

glm::mat4 translate(glm::mat4 const &m, glm::vec3 const &v);
glm::mat4 rotate(glm::mat4 const &m, const float &angle, glm::vec3 const &v);
glm::mat4 scale(glm::mat4 const &m, glm::vec3 const &v);

glm::mat4 operator*(glm::mat4 const &m1, glm::mat4 const &m2);

}

#endif // MYTRANSFORM_H
