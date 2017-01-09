import QtQuick 2.4
import TensoSensor 1.0

Rectangle {
	width: 800
	height: 600
        opacity: 0.9
        border.color: "black"
        border.width: 3
        radius: 5
	property int steps: 0
	property int power: 0
	
        Text {
		font.pointSize: 24
		anchors.horizontalCenter: parent.horizontalCenter
        	text: "Measure report:"
        }
        Text {
		y: 150
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "Number of measures:"
        }
        Text {
		y: 180
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "Power:"
        }

        Text {
		y: 150
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+steps
        }
        Text {
		y: 180
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+power
        }
}

