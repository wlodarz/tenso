import QtQuick 2.4
import TensoSensor 1.0

Rectangle {
	width: 800
	height: 600
        opacity: 0.9
        border.color: "black"
        border.width: 3
        radius: 5
	property int stepsUp: 0
	property real workUp: 0.0
	property real workUpAndHold: 0.0
	property real workHold: 0.0
	property real workDown: 0.0
	property real turns: 0.0
	
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
        	text: "WorkUp:"
        }
        Text {
		y: 210
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "WorkHold:"
        }
        Text {
		y: 240
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "WorkUpAndHold:"
        }
        Text {
		y: 270
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "WorkDown:"
        }
        Text {
		y: 300
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "Turns:"
        }

        Text {
		y: 150
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+stepsUp
        }
        Text {
		y: 180
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+workUp
        }
        Text {
		y: 210
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+workHold
        }
        Text {
		y: 240
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+workUpAndHold
        }
        Text {
		y: 270
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+workDown
        }
        Text {
		y: 300
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+turns
        }
}

