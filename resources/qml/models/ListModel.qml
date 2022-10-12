import QtQuick 2.15 as Q

Q.ListModel {
    id: root

    function populate(rows) {
        root.clear()

        for(let name in rows){
            root.append(rows[name])
        }
    }
}
