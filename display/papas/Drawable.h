//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PAPAS_DRAWABLE_H
#define PAPAS_DRAWABLE_H


#include <string>

namespace papas {

/// This is the base class used for other elements that are to be drawn.
/// The key thing is to have the Draw element
class Drawable {
public:
  Drawable() {};
  virtual ~Drawable() = default;
  virtual void Draw(const std::string& projection)  = 0;

private:
};

}  // end namespace papas

#endif
