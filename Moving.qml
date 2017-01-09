import QtQuick 2.4
import TensoSensor 1.0

Rectangle {
	width: 324
	height: 124
        opacity: 0.9
        border.color: "black"
        border.width: 3
        radius: 5
	Rectangle {
		x: 2
		y: 2
		width: 160
		height: 120
		color: "red"
		opacity: 0.8
		Image {
			id: left_arrow
			source: "images/left.png"
			anchors.fill: parent
			MouseArea {
				anchors.fill: parent
				onClicked: {
					console.log("MOVING LEFT")
					move_left()
				}
			}
		}
	}
	Rectangle {
		x: 162
		y: 2
		width: 160
		height: 120
		color: "darkred"
		opacity: 0.8
		Image {
			id: right_arrow
			source: "images/right.png"
			anchors.fill: parent
			MouseArea {
				anchors.fill: parent
				onClicked: {
					console.log("MOVING RIGHT")
					move_right()
				}
			}
		}
	}
	
	Keys.onPressed: {
		console.log("moving")
		event.accepted = true
		switch(event.key) {
			case Qt.Key_Left:
				console.log("MOVING LEFT")
				break
			case Qt.Key_Right:
				console.log("MOVING RIGHT")
				break
		}
		event.accepted = false
	}
}

