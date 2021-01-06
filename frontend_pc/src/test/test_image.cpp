#include "test_image.h"


testImage::testImage()
{

}

testImage::~testImage(){

}

void testImage::initTestCase(){
    // first log in the user
    QString email = "test@test.com";
    QString pwd = "hello123";
    QVERIFY(usr.log_user_in(&requester, email, pwd));
    QVERIFY(usr._token() != "");
    // set the token of the patient class
    patient::set_token(usr._token());
    // set the token of the image class
    imageInfo::setToken(usr._token());
    imageInfo::setFilePath("/Users/levius/Desktop/tempdata");
}

void testImage::cleanupTestCase(){

}

void testImage::test_case1(){
    // test download images
    // verify the user information
//    QVERIFY(usr._email() == "test@test.com");
//    QString patientID = "123";
//    QString ctime = "2020-12-25";
//    imageInfo imagerequester(nullptr);
//    imagerequester.getImagesHttp(patientID, ctime);
//    QEventLoop eventloop;
//    eventloop.exec();
}

void testImage::test_case2(){
    // test getting ctime of a patient
//    QString patientId = "123";
//    imageInfo image_requester;
//    QVector<QString> result = image_requester.getCtimeHttp(patientId);
//    for (int i = 0; i < result.size(); i++){
//        qDebug() << result[i];
//    }
}

void testImage::test_case3(){
    // test upload images
//    QString patientId = "123";
//    QString ctime = "2020-11-28";
//    QString filepath = "/Users/levius/Desktop/m (10).jpg";
//    imageInfo image_requester;
//    image_requester.uploadImageHttp(patientId, ctime, filepath);
}

void testImage::test_case4(){
    // test predict images
//    QString patientId = "123";
//    QString filepath = "/Users/levius/Desktop/m (10).jpg";
//    imageInfo image_requester;
//    image_requester.predictImageHttp(filepath, patientId);
}

void testImage::test_case5(){
    // test upload image mark
    QString folderpath = "/Users/levius/Desktop/tempdata/123_2020-12-25";
    imageInfo image_requester;
    int result;
//    result = image_requester.uploadImgMark(folderpath, 0, 0, 0.0, 0.0, 0.0, 0.0);
//    QVERIFY(result == SUCCESS);
    // test invalid cases
    result = image_requester.uploadImgMark("", 0, 0, 0.0, 0.0, 0.0, 0.0);
    QVERIFY(result == INVALID_FILEPATH);
    result = image_requester.uploadImgMark(folderpath, 5, 2, 0.0, 0.0, 0.0, 0.0);
    QVERIFY(result == INVALID_LAYER);
    result = image_requester.uploadImgMark(folderpath, 0, 3, 0.0, 0.0, 0.0, 0.0);
    QVERIFY(result == INVALID_VIEW);
}

void testImage::test_case6(){
    // test get marks
    QString folderpath = "/Users/levius/Desktop/tempdata/123_2020-12-25";
    imageInfo image_requester;
    QVector<imageInfo::imgMark>result = image_requester.getAllMarks(folderpath, 0);
    QVERIFY(result.length() != 0);
    qDebug() << result.length();
}
