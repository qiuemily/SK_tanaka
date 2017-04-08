#!/bin/bash
#PBS -l nodes=1:ppn=8,walltime=1:00:00
#PBS -N WCSim_Images

source ${HOME}/pre_run.sh

#IN_ROOT_FILE=$WCSIM_DIR/ROOT_FILES/$ROOT_FILE

#OUT_IMAGE_FILE=$WCSIM_DIR/IMAGES/$IMAGE_FILE
#OUT_EVT_INFO=$WCSIM_DIR/EVT_INFO/$INFO_FILE

cd $WCSIM_DIR

#root -b -q /home/t/tanaka/qiuemily/WCSim_build/mydir/read_wcsim_images_e.cc $IN_ROOT_FILE $OUT_IMAGE_FILE $OUT_EVT_INFO
root -b -q /home/t/tanaka/qiuemily/WCSim_build/mydir/read_wcsim_images_e.cc
