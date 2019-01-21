#!/bin/bash
#This file runs a Clic example to produce a physics debug output file
#The physics output is then compared against the required physics file
#The required file will need to be updated when the physics changes
FILE="examples/compare.out"
PHYSICS="examples/physicsdd.txt"
MATCHPHYSICS=data/required_clic_physics_dd.txt
ROOTFILE=data/ee_Z_ddbar.root
if [ -f $FILE ]
then
  rm $FILE
fi
if [ -f $PHYSICS ]
then
  rm $PHYSICS
fi
example_pdebug $ROOTFILE CLIC $PHYSICS
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
