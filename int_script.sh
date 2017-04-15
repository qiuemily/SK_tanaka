source ${HOME}/pre_run.sh

FILE_NO=01
START_NO=25
END_NO=80

IN_ROOT_FILE=${WCSIM_DIR}/ROOT_FILES/e-_500_file0${FILE_NO}.root

OUT_IMAGE_FILE=${WCSIM_DIR}/IMAGES/test_images_mu+_500_file0${FILE_NO}.txt
OUT_EVT_INFO=${WCSIM_DIR}/EVT_INFO/test_evt_info_mu+_500_file0${FILE_NO}.txt
OUT_PRINT_FILE=${WCSIM_DIR}/PRINT/test_print_mu+_500_file0${FILE_NO}.txt

cd ${WCSIM_DIR}
#root -b -q 'read_wcsim_images_sub_e.cc("/home/t/tanaka/qiuemily/WCSim_build/mydir/ROOT_FILES/e-_50_file001.root", "/home/t/tanaka/qiuemily/WCSim_build/mydir/IMAGES/test_int_e-_50_file001.txt", "/home/t/tanaka/qiuemily/WCSim_build/mydir/EVT_INFO/test_info_int_e-_50_file001.txt")'

#root -b -q "read_wcsim_images_sub_e.cc('/home/t/tanaka/qiuemily/WCSim_build/mydir/ROOT_FILES/e-_50_file001.root', '/home/t/tanaka/qiuemily/WCSim_build/mydir/IMAGES/test_int_e-_50_file001.txt', '/home/t/tanaka/qiuemily/WCSim_build/mydir/EVT_INFO/test_info_int_e-_50_file001.txt')"

#root -b -q 'read_wcsim_images_sub_e.cc("$IN_ROOT_FILE", "${OUT_IMAGE_FILE}", "${OUT_EVT_INFO")'

root -b -q 'read_wcsim_images_sub_e.cc('"\"${IN_ROOT_FILE}\", \"${OUT_IMAGE_FILE}\", \"${OUT_EVT_INFO}\", \"${OUT_PRINT_FILE}\", $START_NO, $END_NO"')'

#echo 'read_wcsim_images_sub_e.cc('"\"${IN_ROOT_FILE}\", \"${OUT_IMAGE_FILE}\", \"${OUT_EVT_INFO}\""')'

#IN_ROOT_FILE="\"${WCSIM_DIR}/${ROOT_FILE}\""

#OUT_IMAGE_FILE='${WCSIM_DIR}/IMAGES/${IMAGE_FILE}'
#OUT_EVT_INFO='${WCSIM_DIR}/EVT_INFO/${INFO_FILE}'

#cd ${WCSIM_DIR}
#root -b -q 'read_wcsim_images_sub_e.cc($IN_ROOT_FILE, ${OUT_IMAGE_FILE}, ${OUT_EVT_INFO})'
