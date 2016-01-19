#include "my_utilities.h"



/*! my_utilities constructor
 */
my_utilities::my_utilities()
{

}
//---------------------------------------------------------------
/*! my_utilities destrudctor
 */
my_utilities::~my_utilities()
{
}
//---------------------------------------------------------------
/*! return the input \n
parameters: \n
Float_t input
 */
Float_t my_utilities::myReturn(Float_t input)
{
   std::cout << "input is " << TMath::Abs(input) << std::endl;
   return input;
}
bool my_utilities::is_point(TVector3& T)
{
   bool answer = T.X() > 0.0;
   return answer;
}