include("/Users/lch/Desktop/improve/cpp/learning/day01/qt-hello/QtHello/build/Desktop_arm_darwin_generic_mach_o_64bit_Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/QtHello-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "QtHello.app"
)
