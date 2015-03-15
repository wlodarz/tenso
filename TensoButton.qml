import QtQuick 2.4
import TensoSensor 1.0

Rectangle {
	width: parent.fieldWidth
	height: parent.fieldHeight
        opacity: 0.9
        border.color: "black"
        border.width: 3
        radius: 10
	color: activated ? "red" : "white"
	property string button_label
        Text {
		font.pointSize: 16
		anchors.centerIn: parent
        	text: button_label
        }
}

