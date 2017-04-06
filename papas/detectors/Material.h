#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

namespace papas {
/// @brief Describes material used in a detector layer.
class Material {
public:
  /** Constructor
   @param[in] name name of material
   @param[in] x0 radiation length
   @param[in] lambda interaction length
   */
  Material(std::string name, double x0, double lambda);
  /** path  length before decay within material
   @param[in] isEm boolean to say whether the particle is Electromagnetic
  */
  virtual double pathLength(bool isEm) const;
  std::string name() const { return m_name; }  ///< Material name

private:
  std::string m_name;  ///< name of material
  double m_x0;         ///<radiation length
  double m_lambdaI;    ///< interaction length
};

}  // end namespace papas
#endif
