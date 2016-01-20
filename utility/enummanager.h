//
//  Created by Alice Robson on 02/12/15.
//
//

#ifndef EnumManager_h
#define EnumManager_h

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
/* Creates an enum class for which the following functions are defined
// << (output to stream as string)
// e_str (return the string)
for example:
 ENUM_WITH_STRINGS(Projection,xy,yz,xz );

 Projection P=Projection::xy;
 cout<<P;
 cout<<to_str(P);
 */

#define ENUM_WITH_STRINGS(name, ...) enum class name { __VA_ARGS__, __COUNT}; \
   inline std::ostream& operator<<(std::ostream& os, name value) { \
      std::string enumName = #name; \
      std::string str = #__VA_ARGS__; \
      int len = str.length(); \
      std::vector<std::string> strings; \
      std::ostringstream temp; \
      for(int i = 0; i < len; i ++) { \
         if(isspace(str[i])) continue; \
         else if(str[i] == ',') { \
            strings.push_back(temp.str()); \
            temp.str(std::string());\
         } \
         else temp<< str[i]; \
      } \
      strings.push_back(temp.str()); \
      os << enumName << "::" << strings[static_cast<int>(value)]; \
      return os;} \
   inline std::string to_str(name value) { \
      std::string enumName = #name; \
      std::string str = #__VA_ARGS__; \
      int len = str.length(); \
      std::vector<std::string> strings; \
      std::ostringstream temp; \
      for(int i = 0; i < len; i ++) { \
         if(isspace(str[i])) continue; \
         else if(str[i] == ',') { \
            strings.push_back(temp.str()); \
            temp.str(std::string());\
         } \
         else temp<< str[i]; \
      } \
      strings.push_back(temp.str()); \
      return strings[static_cast<int>(value)];}


#endif /* EnumManager_h */
