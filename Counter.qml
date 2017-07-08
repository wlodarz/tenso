import QtQuick 2.4
import TensoSensor 1.0

Rectangle {
	width: 200
	height: 200
        opacity: 0.9
        border.color: "black"
        border.width: 3
        radius: 5
	property int counterValue: 0
	
        Text {
		font.pointSize: 56
		anchors.centerIn: parent
        	text: ""+counterValue
		color: "red"
        }
}

