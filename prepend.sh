#!/bin/sh

# Prepends one line to h and cpp files in current directory

for f in *.h *.cpp
do
    echo "/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */"|cat - $f > /tmp/out && mv /tmp/out $f
done
