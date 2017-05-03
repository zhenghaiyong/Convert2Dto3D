#!/bin/bash

if [ $# != 1 ]; then
    echo "USAGE: $0 inputMovie"
    echo " e.g.: $0 Avatar.mp4"
    exit 7
fi

inputMovie=$1
nameFile=${inputMovie##*/}
nameMovie=${nameFile%.*}
extMovie=${nameFile##*.}
outputVideo=${nameFile}
output50Video=${nameMovie}-50.avi
output60Video=${nameMovie}-60.avi
output70Video=${nameMovie}-70.avi
output80Video=${nameMovie}-80.avi
output90Video=${nameMovie}-90.avi
output100Video=${nameMovie}-100.avi

Convert2Dto3D=Convert2Dto3DFlipping

${Convert2Dto3D} ${outputVideo} ${output50Video} 50
${Convert2Dto3D} ${outputVideo} ${output60Video} 60
${Convert2Dto3D} ${outputVideo} ${output70Video} 70
${Convert2Dto3D} ${outputVideo} ${output80Video} 80
${Convert2Dto3D} ${outputVideo} ${output90Video} 90
${Convert2Dto3D} ${outputVideo} ${output100Video} 100
