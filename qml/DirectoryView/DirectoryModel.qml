import QtQuick 2.15
import Qt.labs.folderlistmodel 2.15
import Qt.labs.platform 1.1

FolderListModel {
    id: model
    folder: StandardPaths.writableLocation(StandardPaths.HomeLocation)

    showDirsFirst: true

    // A workaround for `depends on non-NOTIFYable properties:DirectoryModel_QMLTYPE_22::showHidden`
    property bool _showHidden

    on_ShowHiddenChanged: {
        showHidden = _showHidden
    }

    readonly property bool canGoUp: folder && parentFolder.toString() !== ""

    property string path: {
        pathFromUrl(folder)
    }

    function mime(path) {
        return fs_bridge.mime(path)
    }

    function icon(path) {
        return fs_bridge.icon(path)
    }

    function fileSizeStr(path) {
        return fs_bridge.file_size_str(path, _showHidden)
    }

    function pathFromUrl(path) {
        return fs_bridge.path_from_url(path)
    }

    function goUp() {
        if (!canGoUp) {
            return
        }

        folder = parentFolder
    }

    function goHome() {
        folder = StandardPaths.writableLocation(StandardPaths.HomeLocation)
    }

    function canCd(path) {
        return fs_bridge.isreadable(path)
    }

    function cd(path) {
        if (canCd(path)) {
            folder = path
        }
    }

    function cdIndex(index) {
        if (!get(index, "fileIsDir")) {
            return
        }
        const path = get(index, "fileUrl")
        cd(path)
    }
}
