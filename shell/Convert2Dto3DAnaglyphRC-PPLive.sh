#!/bin/bash

if [ $# != 2 ]; then
    echo "USAGE: $0 inputMovie DOF"
    echo " e.g.: $0 Avatar.mp4 100"
    exit 7
fi

inputMovie=$1
DOF=$2
nameFile=${inputMovie##*/}
nameMovie=${nameFile%.*}
extMovie=${nameFile##*.}
#CropMovieHeadLeader#outputCropMovie=${nameMovie}-crop.${extMovie}
outputAACAudio=${nameMovie}.aac
#outputVideo=${nameMovie}-vo.${extMovie}
outputRCVideo=${nameMovie}-rc.avi
outputMovie=${nameMovie}-3D-RedCyan.${extMovie}

Convert2Dto3D=Convert2Dto3DAnaglyphRC
FFmpeg=ffmpeg

#CropMovieHeadLeader#${FFmpeg} -y -i ${inputMovie} -ss 00:00:00.001 -acodec copy -vcodec copy ${outputCropMovie}
#CropMovieHeadLeader#${FFmpeg} -y -i ${outputCropMovie} -vn -acodec copy ${outputAACAudio}
#CropMovieHeadLeader#${FFmpeg} -y -i ${outputCropMovie} -an -vcodec copy ${outputVideo}

${FFmpeg} -y -i ${inputMovie} -vn -acodec copy ${outputAACAudio}
#${FFmpeg} -y -i ${inputMovie} -an -vcodec copy ${outputVideo}

#${Convert2Dto3D} ${outputVideo} ${outputRCVideo}
${Convert2Dto3D} ${inputMovie} ${outputRCVideo} ${DOF}

${FFmpeg} -y -i ${outputAACAudio} -i ${outputRCVideo} -f mp4 -vcodec libx264 -vpre 2Dto3D -deinterlace -acodec libfaac ${outputMovie}
rm -f ${outputRCVideo}
