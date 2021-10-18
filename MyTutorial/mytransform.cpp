#include "mytransform.h"

namespace MyTransform
{

glm::mat4 translate(glm::mat4 const &m, glm::vec3 const &v) {
    return glm::translate(m, v);
}
glm::mat4 rotate(glm::mat4 const &m, const float &angle, glm::vec3 const &v) {
    return glm::rotate(m, angle, v);
}
glm::mat4 scale(glm::mat4 const &m, glm::vec3 const &v) {
    return glm::scale(m, v);
}
glm::mat4 operator*(glm::mat4 const &m1, glm::mat4 const &m2) {
    return glm::operator*(m1, m2);
}

}   // namespace MyTransform
