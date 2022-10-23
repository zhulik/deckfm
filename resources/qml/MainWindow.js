function openFile(path, mime) {
    if (mime.split("/")[0] === "video")  {
        loadQMLApp("VideoPlayer/VideoPlayer.qml", p => p.source = path)
    }

    if (mime === "text/x-qml") {
        loadQMLApp(path)
        return
    }

}


function loadQMLApp(path, cb) {
    const component = Qt.createComponent(`./AppLoader.qml`, Component.Asynchronous, stackView)

    const initialize = () => {
        const instance = component.createObject(stackView)

        instance.loaded.connect(() => {
                                    if (cb) {
                                        cb(instance.item)
                                    }
                                    stackView.push(instance)
                                })
        instance.closed.connect(() => stackView.pop())

        instance.source = path
    }

    if (component.status === Component.Ready) {
        initialize()
    } else {
        component.statusChanged.connect(initialize);
    }


    return
}
