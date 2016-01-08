//
//  enumhelper.h
//  Project
//
//  Created by Alice Robson on 06/01/16.
//
//

#ifndef enumhelper_h
#define enumhelper_h

#include <boost/preprocessor.hpp>

#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)    \
case elem : return BOOST_PP_STRINGIZE(elem);

#define ENUM_WITH_STRING(name, enumerators)                \
enum name {                                                                   \
BOOST_PP_SEQ_ENUM(enumerators)                                                \
};                                                                            \
                                                                              \
inline const char* to_str(name v)                                           \
{                                                                             \
switch (v)                                                                    \
{                                                                             \
BOOST_PP_SEQ_FOR_EACH(                                                        \
X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,                          \
name,                                                                         \
enumerators                                                                   \
)                                                                             \
default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";                     \
}                                                                             \
}

#endif /* enumhelper_h */
