#!/bin/sh

set -x -e

OUTDIR=bin
LIBNAME=libhtml-template.a
FILENAME=test

DEBUG=0

CFLAGS="-Wall -Wextra"

if [ "$DEBUG" -eq 1 ]; then
  CFLAGS="$CFLAGS -fsanitize=address -ggdb"
  LDFLAGS="-fsanitize=address"
else 
  LDFLAGS=""
fi

if [ -d $OUTDIR ]; then 
  rm -rf $OUTDIR
fi

mkdir -p $OUTDIR

gcc -c src/html-template.c -o "$OUTDIR/html-template.o" $CFLAGS

ar rcs "$OUTDIR/$LIBNAME" "$OUTDIR/html-template.o"

gcc -c example/${FILENAME}.c -o "$OUTDIR/${FILENAME}.o" $CFLAGS -I./src

gcc "$OUTDIR/${FILENAME}.o" -L"$OUTDIR" -lhtml-template -o "$OUTDIR/$FILENAME" $LDFLAGS

# create some white space after script
echo ""
