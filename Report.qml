import QtQuick 2.6
import QtQuick.Controls 1.4
import TensoSensor 1.0

import "js/functions.js" as Control

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
	property int buttonWidth: 100
	property int buttonHeight: 60

        Text {
		font.pointSize: 24
		anchors.horizontalCenter: parent.horizontalCenter
        	text: "Measure report:"
        }
        Text {
		y: 120
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "Lot identification:"
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

        TextInput {
                id: lotid
                y: 120
                font.pointSize: 16
                anchors.horizontalCenter: parent.horizontalCenter
                text: ""+TensoSensor.lotId
                color: "green"
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
        	text: ""+workUp.toFixed(2)
        }
        Text {
		y: 210
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+workHold.toFixed(2)
        }
        Text {
		y: 240
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+workUpAndHold.toFixed(2)
        }
        Text {
		y: 270
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+workDown.toFixed(2)
        }
        Text {
		y: 300
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+turns
        }

        TensoButton {
                id: save_button
                button_label_default: "Save"
		button_label: button_label_default
                x: parent.width / 2 - 100
                y: parent.height - 100
                property string action_text: "save"
                property bool activated: TensoSensor.operation == TensoSensor.SENSOR_OPERATION_SAVE_REPORT
                MouseArea {
                        anchors.fill: parent
                        onClicked: {
				TensoSensor.lotId = lotid.text
				Control.save_report()
			}
                }
        }

        TensoButton {
                id: discard_button
                button_label_default: "Discard"
		button_label: button_label_default
                x: parent.width / 2 + 100
                y: parent.height - 100
                property string action_text: "discard"
                property bool activated: TensoSensor.operation == TensoSensor.SENSOR_OPERATION_DISCARD_REPORT
                MouseArea {
                        anchors.fill: parent
                        onClicked: Control.discard_report()
                }
        }

}

