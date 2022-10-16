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
            }
        }

        root.clear()

        newData.forEach(function(row){
            if (typeof(row) !== 'object') {
                row = {value: row}
            }

            root.append(row)
        })
    }
}
