#!/bin/bash
#PBS -l nodes=1:ppn=8,walltime=1:00:00
#PBS -N WCSim_Images_E

source ${HOME}/pre_run.sh 
 
#export IN_ROOT_FILE=${WCSIM_DIR}/${ROOT_FILE}
#export OUT_IMAGE_FILE=${SCRATCH}/WCSim_Output/IMAGES/${IMAGE_FILE}
#export OUT_EVT_INFO=${SCRATCH}/WCSim_Output/EVT_INFO/${INFO_FILE}

export IN_ROOT_FILE=${WCSIM_DIR}/ROOT_FILES/e-_500_file0${FILE_NO}.root

export OUT_IMAGE_FILE=${SCRATCH}/WCSim_Output/IMAGES/images_e-_500_file0${FILE_NO}.txt
export OUT_EVT_INFO=${SCRATCH}/WCSim_Output/EVT_INFO/evt_info_e-_500_file0${FILE_NO}.txt

cd ${WCSIM_DIR}

root -b -q 'read_wcsim_images_sub_e.cc('"\"${IN_ROOT_FILE}\", \"${OUT_IMAGE_FILE}\", \"${OUT_EVT_INFO}\", $START_NO, $END_NO"')'
