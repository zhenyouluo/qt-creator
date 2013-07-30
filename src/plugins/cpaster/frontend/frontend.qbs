import qbs.base 1.0
import "../../../tools/QtcTool.qbs" as QtcTool

QtcTool {
    name: "cpaster"

    Depends {
        name: "Qt"
        submodules: ["gui", "network"]
    }
    Depends { name: "Core" }

    cpp.includePaths: ["../../"]
    cpp.rpaths: [
        "$ORIGIN/../lib/qtcreator",
        "$ORIGIN/../lib/qtcreator/plugins",
        "$ORIGIN/../lib/qtcreator/plugins/QtProject"
    ]

    Group {
        name: "Frontend Sources"
        files: [
            "main.cpp",
            "argumentscollector.h", "argumentscollector.cpp"
        ]
    }

    Group {
        name: "Plugin Sources"
        prefix: "../"
        files: [
            "cpasterconstants.h",
            "kdepasteprotocol.h", "kdepasteprotocol.cpp",
            "pastebindotcaprotocol.h", "pastebindotcaprotocol.cpp",
            "pastebindotcomprotocol.h", "pastebindotcomprotocol.cpp",
            "protocol.h", "protocol.cpp",
            "urlopenprotocol.h", "urlopenprotocol.cpp",
        ]
    }
}
