import QtQuick 2.4
import QtQuick.Controls 1.4
import TensoSensor 1.0

Rectangle {
	width: 800
	height: 400
        opacity: 0.9
        border.color: "black"
        border.width: 3
        radius: 5
	property real paramA: 0.0
	property real paramB: 0.0
	
        Text {
		font.pointSize: 24
		anchors.horizontalCenter: parent.horizontalCenter
        	text: "Configuration"
        }
        Text {
		y: 150
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "Dlugosc naciagu przed pomiarem:"
        }
/*
        Text {
		y: 180
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "Parameter B:"
        }
*/

        TextInput {
		id: minlength
		y: 150
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+TensoSensor.minMeasuredLength
		color: "grey"
		validator: DoubleValidator {
			bottom: 0.1
			top: 400.0
			decimals: 2
			notation: DoubleValidator.StandardNotation
		}
        }
/*
        TextInput {
		id: paramb
		y: 180
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+paramB
		color: "grey"
        }
*/

	Button {
		text: "Accept"
		x: 200
		y: 300
		isDefault: true
		onClicked: {
			console.log("ACCEPTED");
			TensoSensor.startingLength = parseFloat(minlength.text)
			console.log("A " + TensoSensor.startingLength)
			console.log("minMeasuredLength " + TensoSensor.minMeasuredLength)
			parent.visible = false
			parent.focus = false
		}
	}
	Button {
		text: "Cancel"
		x: 340
		y: 300
		onClicked: {
			console.log("CANCELED");
			parent.visible = false
			parent.focus = false
		}
	}
}

