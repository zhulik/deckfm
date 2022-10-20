import QtQuick 2.15
import QtQuick3D 1.15

PerspectiveCamera {
    id: root

    fieldOfView: 75

    function move(dX, dY) {
        const forwardVelocity = v3dMulC(root.forward, dY * 3)
        const strafeVelocity = v3dMulC(root.right, dX * 3)
        position = v3dSum(position, forwardVelocity, strafeVelocity)
    }


    function pan(dX, dY) {
        root.eulerRotation.y -= dX
        root.eulerRotation.x -= dY

        if (Math.abs(root.eulerRotation.x) > 90) {
            root.eulerRotation.x += dY
        }
    }

    function v3dSum() { // multiple Vector3D arguments
        let [x, y, z] = [0 , 0, 0]

        for (var i = 0; i < arguments.length; i++) {
            x += arguments[i].x
            y += arguments[i].y
            z += arguments[i].z
        }
        return Qt.vector3d(x, y, z)
    }

    function v3dMulC(vec, n) { // multiple Vetctor3D arguments
        return Qt.vector3d(vec.x * n, vec.y * n, vec.z * n);
    }

    onEulerRotationChanged: {
        if (root.eulerRotation.x > 90) {
            root.eulerRotation.x = 90
        }

        if (root.eulerRotation.x < -90) {
            root.eulerRotation.x = -90
        }
    }
}
