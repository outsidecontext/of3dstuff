#!/bin/bash

# Dependencies for cinekinect

cd ../../addons/

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

${PREFIX}patriciogonzalezvivo/ofxTuio
${PREFIX}obviousjim/ofxDOF