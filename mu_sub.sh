#!/bin/bash
#PBS -l nodes=1:ppn=8,walltime=1:00:00
#PBS -N WCSim_Images_Mu

source ${HOME}/pre_run.sh
 
export IN_ROOT_FILE=${WCSIM_DIR}/${ROOT_FILE} 

export OUT_IMAGE_FILE=${SCRATCH}/WCSim_Output/IMAGES/${IMAGE_FILE}  
export OUT_EVT_INFO=${SCRATCH}/WCSim_Output/EVT_INFO/${INFO_FILE} 

cd ${WCSIM_DIR}

root -b -q 'read_wcsim_images_sub_mu.cc('"\"${IN_ROOT_FILE}\", \"${OUT_IMAGE_FILE}\", \"${OUT_EVT_INFO}\",$START_NO, $END_NO"')'

