import QtQuick 2.4
import TensoSensor 1.0

Rectangle {
	width: parent.buttonWidth
	height: parent.buttonHeight
        opacity: 0.9
        border.color: "black"
        border.width: 3
        radius: 5
	color: activated ? "red" : "white"
	property string button_label
	property string button_label_default
        Text {
		font.pointSize: 16
		anchors.centerIn: parent
        	text: button_label
        }
}

