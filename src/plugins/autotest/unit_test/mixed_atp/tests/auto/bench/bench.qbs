import qbs

CppApplication {
    type: "application"
    name: "Benchmark Auto Test"
    targetName: "tst_benchtest"

    Depends { name: "cpp" }
    Depends { name: "Qt.test" }

    files: [ "tst_benchtest.cpp" ]

    cpp.defines: base.concat("SRCDIR=" + path)
}
