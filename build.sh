#!/bin/sh

set -x -e

OUTDIR=bin
FILENAME=main

if [ $OUTDIR ]; then 
  rm -rf $OUTDIR
fi

mkdir -p $OUTDIR

gcc -S $FILENAME.c -o $OUTDIR\/$FILENAME.s -Wall
gcc -c $OUTDIR\/$FILENAME.s -o $OUTDIR\/$FILENAME.o 
gcc $OUTDIR\/$FILENAME.o -o $OUTDIR\/$FILENAME 

# create some white space after script
echo ""
