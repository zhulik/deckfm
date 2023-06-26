import QtQuick 2.15 as Q

Q.ListModel {
    id: root

    property var data: []

    onDataChanged: {
        let newData
        if (Array.isArray(data)) {
            newData = data
        } else {
            if (typeof(data) === 'object' && data !== null) {
                newData = Object.values(data)
            } else {
                newData = []
            }
        }

        root.clear()

        newData.forEach(row => {
            if (typeof(row) !== 'object') {
                row = {value: row}
            }

            root.append(row)
        })
    }
}
