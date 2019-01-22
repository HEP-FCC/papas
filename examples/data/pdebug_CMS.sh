#!/bin/bash
#This file runs a CMS example to produce a physics debug output file
#The physics output is then compared against the required physics file
#The required file will need to be updated when the physics changes
FILE="examples/compare.out"
PHYSICS="examples/physics.txt"
MATCHPHYSICS="data/required_cms_physics.txt"
ROOTFILE="data/ee_ZH_Zmumu_Hbb.root"

if [ -f $FILE ]
then
rm $FILE
fi
if [ -f $PHYSICS ]
then
rm $PHYSICS
fi
example_pdebug $ROOTFILE CMS $PHYSICS
if ([ -f $PHYSICS ] && [ -f $MATCHPHYSICS ])
then
#echo "difference files"
diff $MATCHPHYSICS $PHYSICS  >  $FILE

if [ -f $FILE ]
then
if [[ -s $FILE ]];
then
echo "$FILE is different"
else
echo "$FILE matches"
rm $FILE
rm $PHYSICS
fi
else
echo "$FILE not found."
fi
else
echo "$PHYSICS not found."
fi
