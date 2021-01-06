#-------------------------------------------------
#
# Project created by QtCreator 2020-10-21T22:20:45
#
#-------------------------------------------------

QT       += core gui
QT       += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = App
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        commu/patient_form.cpp \
    commu/imageinfo.cpp \
    commu/prediction.cpp \
        general_processing/general_processing.cpp \
        load/download_form.cpp \
        load/upload_form.cpp \
        login/signup_form.cpp \
    predict/predict_form.cpp \
    segmentation/segmentation_worker.cpp \
        utils/general_util.cpp \
        login/loginchecker.cpp \
        login/loginform.cpp \
        commu/communhttp.cpp \
        commu/patient.cpp \
        commu/userinfo.cpp \
        main.cpp \
        mainwindow.cpp \
        registration/registration_worker.cpp \
    utils/vtk_point_picker_callback.cpp \
        voxel2mesh/Voxel2Mesh.cpp

HEADERS += \
        commu/patient_form.h \
        commu/imageinfo.h \
        commu/prediction.h \
        general_processing/general_processing.h \
        load/download_form.h \
        load/upload_form.h \
        login/signup_form.h \
        predict/predict_form.h \
        segmentation/segmentation_worker.h \
        utils/general_util.h \
        login/loginchecker.h \
        login/loginform.h \
        commu/communhttp.h \
        commu/patient.h \
        commu/urlbase.h \
        commu/userinfo.h \
        mainwindow.h \
        errorcode.h \
        registration/registration_worker.h \
        struct_define.h \
        utils/vtk_point_picker_callback.h \
        voxel2mesh/Voxel2Mesh.h

FORMS += \
        commu/patient_form.ui \
        load/download_form.ui \
        load/upload_form.ui \
        login/loginform.ui \
        login/signup_form.ui \
        mainwindow.ui \
        predict/predict_form.ui

INCLUDEPATH += /usr/local/include \
           /usr/local/include/vtk-8.2 \
           /usr/local/include/ITK-5.0

LIBS += -L/usr/local/lib \
-lvtkChartsCore-8.2 -lvtkCommonColor-8.2 -lvtkCommonComputationalGeometry-8.2 -lvtkCommonCore-8.2 -lvtkCommonDataModel-8.2 \
-lvtkCommonExecutionModel-8.2 -lvtkCommonMath-8.2 -lvtkCommonMisc-8.2 -lvtkCommonSystem-8.2 -lvtkCommonTransforms-8.2 -lvtkDICOMParser-8.2 \
-lvtkDomainsChemistry-8.2 -lvtkDomainsChemistryOpenGL2-8.2 -lvtkFiltersAMR-8.2 -lvtkFiltersCore-8.2 -lvtkFiltersExtraction-8.2 \
-lvtkFiltersFlowPaths-8.2 -lvtkFiltersGeneral-8.2 -lvtkFiltersGeneric-8.2 -lvtkFiltersGeometry-8.2 -lvtkFiltersHybrid-8.2 \
-lvtkFiltersHyperTree-8.2 -lvtkFiltersImaging-8.2 -lvtkFiltersModeling-8.2 -lvtkFiltersParallel-8.2 -lvtkFiltersParallelImaging-8.2 \
-lvtkFiltersPoints-8.2 -lvtkFiltersProgrammable-8.2 -lvtkFiltersSMP-8.2 -lvtkFiltersSelection-8.2 -lvtkFiltersSources-8.2 \
-lvtkFiltersStatistics-8.2 -lvtkFiltersTexture-8.2 -lvtkFiltersTopology-8.2 -lvtkFiltersVerdict-8.2 -lvtkGUISupportQt-8.2 \
-lvtkGUISupportQtSQL-8.2 -lvtkGeovisCore-8.2 -lvtkIOAMR-8.2 -lvtkIOAsynchronous-8.2 -lvtkIOCityGML-8.2 \
-lvtkIOCore-8.2 -lvtkIOEnSight-8.2 -lvtkIOExodus-8.2 -lvtkIOExport-8.2 -lvtkIOExportOpenGL2-8.2 -lvtkIOExportPDF-8.2 -lvtkIOGeometry-8.2 \
-lvtkIOImage-8.2 -lvtkIOImport-8.2 -lvtkIOInfovis-8.2 -lvtkIOLSDyna-8.2 -lvtkIOLegacy-8.2 -lvtkIOMINC-8.2 -lvtkIOMovie-8.2 -lvtkIONetCDF-8.2 \
-lvtkIOPLY-8.2 -lvtkIOParallel-8.2 -lvtkIOParallelXML-8.2 -lvtkIOSQL-8.2 -lvtkIOSegY-8.2 -lvtkIOTecplotTable-8.2 -lvtkIOVeraOut-8.2 \
-lvtkIOVideo-8.2 -lvtkIOXML-8.2 -lvtkIOXMLParser-8.2 -lvtkImagingColor-8.2 -lvtkImagingCore-8.2 -lvtkImagingFourier-8.2 -lvtkImagingGeneral-8.2 \
-lvtkImagingHybrid-8.2 -lvtkImagingMath-8.2 -lvtkImagingMorphological-8.2 -lvtkImagingSources-8.2 \
-lvtkImagingStatistics-8.2 -lvtkImagingStencil-8.2 -lvtkInfovisCore-8.2 -lvtkInfovisLayout-8.2 -lvtkInteractionImage-8.2 \
-lvtkInteractionStyle-8.2 -lvtkInteractionWidgets-8.2 -lvtkNetCDF-8.2 -lvtkParallelCore-8.2 -lvtkRenderingAnnotation-8.2 \
-lvtkRenderingContext2D-8.2 -lvtkRenderingContextOpenGL2-8.2 -lvtkRenderingCore-8.2 -lvtkRenderingFreeType-8.2 -lvtkRenderingGL2PSOpenGL2-8.2 \
-lvtkRenderingImage-8.2 -lvtkRenderingLOD-8.2 -lvtkRenderingLabel-8.2 -lvtkRenderingOpenGL2-8.2 -lvtkRenderingQt-8.2 \
-lvtkRenderingVolume-8.2 -lvtkRenderingVolumeOpenGL2-8.2 -lvtkViewsContext2D-8.2 -lvtkViewsCore-8.2 -lvtkViewsInfovis-8.2 -lvtkViewsQt-8.2 \
-lvtkdoubleconversion-8.2 -lvtkexodusII-8.2 -lvtkexpat-8.2 -lvtkfreetype-8.2 -lvtkgl2ps-8.2 -lvtkglew-8.2 -lvtkhdf5-8.2 -lvtkhdf5_hl-8.2 \
-lvtkjpeg-8.2 -lvtkjsoncpp-8.2 -lvtklibharu-8.2 -lvtklibxml2-8.2 -lvtklz4-8.2 -lvtklzma-8.2 -lvtkmetaio-8.2 -lvtkogg-8.2 -lvtkpng-8.2 \
-lvtkproj-8.2 -lvtkpugixml-8.2 -lvtksqlite-8.2 -lvtksys-8.2 -lvtktheora-8.2 -lvtktiff-8.2 -lvtkverdict-8.2 -lvtkzlib-8.2 \
-lITKBiasCorrection-5.0.1 -lITKCommon-5.0.1 -lITKDICOMParser-5.0.1 -lITKEXPAT-5.0.1 -lITKIOBMP-5.0.1 -lITKIOBioRad-5.0.1 \
-lITKIOBruker-5.0.1 -lITKIOCSV-5.0.1 -lITKIOGDCM-5.0.1 -lITKIOGE-5.0.1 -lITKIOGIPL-5.0.1 -lITKIOHDF5-5.0.1 \
-lITKIOIPL-5.0.1 -lITKIOImageBase-5.0.1 -lITKIOJPEG-5.0.1 -lITKIOJPEG2000-5.0.1 -lITKIOLSM-5.0.1 -lITKIOMINC-5.0.1 \
-lITKIOMRC-5.0.1 -lITKIOMeshBYU-5.0.1 -lITKIOMeshBase-5.0.1 -lITKIOMeshFreeSurfer-5.0.1 -lITKIOMeshGifti-5.0.1 -lITKIOMeshOBJ-5.0.1 \
-lITKIOMeshOFF-5.0.1 -lITKIOMeshVTK-5.0.1 -lITKIOMeta-5.0.1 -lITKIONIFTI-5.0.1 -lITKIONRRD-5.0.1 -lITKIOPNG-5.0.1 \
-lITKIOSiemens-5.0.1 -lITKIOSpatialObjects-5.0.1 -lITKIOStimulate-5.0.1 -lITKIOTIFF-5.0.1 -lITKIOTransformBase-5.0.1 -lITKIOTransformHDF5-5.0.1 \
-lITKIOTransformInsightLegacy-5.0.1 -lITKIOTransformMatlab-5.0.1 -lITKIOVTK-5.0.1 -lITKIOXML-5.0.1 -lITKKLMRegionGrowing-5.0.1 -lITKLabelMap-5.0.1 \
-lITKMesh-5.0.1 -lITKMetaIO-5.0.1 -lITKNrrdIO-5.0.1 -lITKOptimizers-5.0.1 -lITKOptimizersv4-5.0.1 -lITKPath-5.0.1 \
-lITKPolynomials-5.0.1 -lITKQuadEdgeMesh-5.0.1 -lITKSpatialObjects-5.0.1 -lITKStatistics-5.0.1 -lITKTestKernel-5.0.1 -lITKTransform-5.0.1 \
-lITKTransformFactory-5.0.1 -lITKVNLInstantiation-5.0.1 -lITKVTK-5.0.1 -lITKVideoCore-5.0.1 -lITKVideoIO-5.0.1 -lITKVtkGlue-5.0.1 \
-lITKWatersheds-5.0.1 -lITKgiftiio-5.0.1 -lITKniftiio-5.0.1 -lITKznz-5.0.1 -litkNetlibSlatec-5.0.1 -litkdouble-conversion-5.0.1 \
-litkgdcmCommon-5.0.1 -litkgdcmDICT-5.0.1 -litkgdcmDSED-5.0.1 -litkgdcmIOD-5.0.1 -litkgdcmMEXD-5.0.1 -litkgdcmMSFF-5.0.1 \
-litkgdcmcharls-5.0.1 -litkgdcmjpeg12-5.0.1 -litkgdcmjpeg16-5.0.1 -litkgdcmjpeg8-5.0.1 -litkgdcmopenjp2-5.0.1 -litkgdcmsocketxx-5.0.1 \
-litkgdcmuuid-5.0.1 -litkjpeg-5.0.1 -litklbfgs-5.0.1 -litkminc2-5.0.1 -litknetlib-5.0.1 -litkopenjpeg-5.0.1 \
-litkpng-5.0.1 -litksys-5.0.1 -litktestlib-5.0.1 -litktiff-5.0.1 -litkv3p_netlib-5.0.1 -litkvcl-5.0.1 \
-litkvnl-5.0.1 -litkvnl_algo-5.0.1 -litkzlib-5.0.1 \
# -framework OpenGL -framework glut \
#-lopencv_dnn -lopencv_ml -lopencv_objdetect -lopencv_shape \
#-lopencv_stitching -lopencv_superres -lopencv_videostab \
#-lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio \
#-lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_imgproc -lopencv_flann -lopencv_core
-lITKBiasCorrection-5.0 -lITKCommon-5.0 -lITKDICOMParser-5.0 -lITKEXPAT-5.0 -lITKIOBMP-5.0 -lITKIOBioRad-5.0 -lITKIOBruker-5.0 \
-lITKIOCSV-5.0 -lITKIOGDCM-5.0 -lITKIOGE-5.0 -lITKIOGIPL-5.0 -lITKIOHDF5-5.0 -lITKIOIPL-5.0 -lITKIOImageBase-5.0 \
-lITKIOJPEG-5.0 -lITKIOJPEG2000-5.0 -lITKIOLSM-5.0 -lITKIOMINC-5.0 -lITKIOMRC-5.0 -lITKIOMeshBYU-5.0 -lITKIOMeshBase-5.0 \
-lITKIOMeshFreeSurfer-5.0 -lITKIOMeshGifti-5.0 -lITKIOMeshOBJ-5.0 -lITKIOMeshOFF-5.0 -lITKIOMeshVTK-5.0 -lITKIOMeta-5.0 -lITKIONIFTI-5.0 \
-lITKIONRRD-5.0 -lITKIOPNG-5.0 -lITKIOSiemens-5.0 -lITKIOSpatialObjects-5.0 -lITKIOStimulate-5.0 -lITKIOTIFF-5.0 -lITKIOTransformBase-5.0 \
-lITKIOTransformHDF5-5.0 -lITKIOTransformInsightLegacy-5.0 -lITKIOTransformMatlab-5.0 -lITKIOVTK-5.0 -lITKIOXML-5.0 \
-lITKKLMRegionGrowing-5.0 -lITKLabelMap-5.0 \
-lITKMesh-5.0 -lITKMetaIO-5.0 -lITKNrrdIO-5.0 -lITKOptimizers-5.0 -lITKOptimizersv4-5.0 -lITKPath-5.0 -lITKPolynomials-5.0 \
-lITKQuadEdgeMesh-5.0 -lITKSpatialObjects-5.0 -lITKStatistics-5.0 -lITKTestKernel-5.0 -lITKTransform-5.0 -lITKTransformFactory-5.0 \
-lITKVNLInstantiation-5.0 \
-lITKVTK-5.0 -lITKVideoCore-5.0 -lITKVideoIO-5.0 -lITKVtkGlue-5.0 -lITKWatersheds-5.0 -lITKgiftiio-5.0 -lITKniftiio-5.0 \
-lITKznz-5.0 -litkNetlibSlatec-5.0 -litkdouble-conversion-5.0 -litkgdcmCommon-5.0 -litkgdcmDICT-5.0 -litkgdcmDSED-5.0 -litkgdcmIOD-5.0 \
-litkgdcmMEXD-5.0 -litkgdcmMSFF-5.0 -litkgdcmcharls-5.0 \
-litkopenjpeg-5.0 -litkpng-5.0 -litksys-5.0 \
-litktestlib-5.0 -litktiff-5.0 -litkv3p_netlib-5.0 -litkvcl-5.0 -litkvnl-5.0 -litkvnl_algo-5.0 -litkzlib-5.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Login/login.qss

RESOURCES     = mainwindow.qrc

RC_ICONS = ../resources/MedPlatform.ico
RC_FILE = ../resources/myapp.rc
