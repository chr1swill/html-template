#!/bin/sh

set -x -e

OUTDIR=bin
LIBNAME=libhtml-template.a
FILENAME=main
CFLAGS="-Wall -Wextra"

if [ $OUTDIR ]; then 
  rm -rf $OUTDIR
fi

mkdir -p $OUTDIR

gcc -c src/html-template.c -o "$OUTDIR/html-template.o" $CFLAGS

ar rcs "$OUTDIR/$LIBNAME" "$OUTDIR/html-template.o"

gcc -c example/main.c -o "$OUTDIR/main.o" $CFLAGS -I./src

gcc "$OUTDIR/main.o" -L"$OUTDIR" -lhtml-template -o "$OUTDIR/$FILENAME"

# create some white space after script
echo ""
