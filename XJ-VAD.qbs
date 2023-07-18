import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        Depends { name: "Qt.core" }
        Depends { name: "Qt.quick" }
        Depends { name: "Qt.charts" }
        Depends { name: "Qt.serialport" }
        Depends { name: "Qt.network" }
        Depends { name: "Qt.printsupport" }

        /* ok3568 编译环境无法加载声音模块, okt507可以 */
        //Depends { name: "Qt.multimedia" }

        // Additional import path used to resolve QML modules in Qt Creator's code model
        property pathList qmlImportPaths: []

        cpp.cxxLanguageVersion: "c++11"

        cpp.defines: [
            // The following define makes your compiler emit warnings if you use
            // any feature of Qt which as been marked deprecated (the exact warnings
            // depend on your compiler). Please consult the documentation of the
            // deprecated API in order to know how to port your code away from it.
            "QT_DEPRECATED_WARNINGS",

            // You can also make your code fail to compile if you use deprecated APIs.
            // In order to do so, uncomment the following line.
            // You can also select to disable deprecated APIs only up to a certain version of Qt.
            //"QT_DISABLE_DEPRECATED_BEFORE=0x060000" // disables all the APIs deprecated before Qt 6.0.0

            ///home/xj/OKT507-linux-sdk/out/t507/okt507/longan/buildroot/host/aarch64-buildroot-linux-gnu/sysroot/lib
        ]

        // technicaltest.* 是测试类，非功能实体类
        files: [
            "EcgFilter.cpp",
            "EcgFilter.h",
            "IABP_ABP.cpp",
            "IABP_ABP.h",
            "IABP_ECG.cpp",
            "IABP_ECG.h",
            "UdiskWatcher.cpp",
            "UdiskWatcher.h",
            "dataprocess.cpp",
            "dataprocess.h",
            "datashow.cpp",
            "datashow.h",
            "datastruct.h",
            "highspeeddata.cpp",
            "highspeeddata.h",
            "logger.cpp",
            "logger.h",
            "lowspeeddata.cpp",
            "lowspeeddata.h",
            "macro.h",
            "main.cpp",
            "mycrc.cpp",
            "mycrc.h",
            "publicdata.cpp",
            "publicdata.h",
            "qaesencryption.cpp",
            "qaesencryption.h",
            "qml.qrc",
            "qmllanguage.cpp",
            "qmllanguage.h",
            "senderdata.cpp",
            "senderdata.h",
            "serialport.cpp",
            "serialport.h",
            "technicaltest.cpp",
            "technicaltest.h",
        ]

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
