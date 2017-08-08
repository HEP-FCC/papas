#!/bin/bash
FILE="compare.out"
PHYSICS="physicsdd.txt"
MATCHPHYSICS=$FCCPAPASCPP/data/required_clic_physics_dd.txt
ROOTFILE=$FCCPAPASCPP/data/ee_Z_ddbar.root
if [ -f $FILE ]
then
  rm $FILE
fi
if [ -f $PHYSICS ]
then
  rm $PHYSICS
fi
$FCCPAPASCPP/bin/example_pdebug $ROOTFILE CLIC $PHYSICS
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

