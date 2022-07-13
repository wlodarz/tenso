import QtQuick 2.4
import QtQuick 2.15
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
        	text: "Starting length"
        }
        Text {
		y: 180
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "Month"
        }
        Text {
		y: 210
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "Year"
        }
        Text {
		y: 240
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "Batch"
        }
        Text {
		y: 270
		font.pointSize: 16
		anchors.left: parent.left
		anchors.margins: 20
        	text: "Number"
        }

        TextInput {
		id: startlength
		y: 150
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+TensoSensor.lotStartLength
		color: "grey"
		validator: DoubleValidator {
			bottom: 0.1
			top: 400.0
			decimals: 2
			notation: DoubleValidator.StandardNotation
		}
        }
        TextInput {
		id: mm
		y: 180
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+TensoSensor.lotMM
		color: "grey"
		validator: RegularExpressionValidator {
			regularExpression: /[01][0-9]$/
		}
        }
        TextInput {
		id: yyyy
		y: 210
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+TensoSensor.lotYYYY
		color: "grey"
		validator: RegularExpressionValidator {
			regularExpression: /[12][0-9][0-9][0-9]$/
		}
        }
        TextInput {
		id: segment
		y: 240
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+TensoSensor.lotSegment
		color: "grey"
		validator: DoubleValidator {
			bottom: 0.1
			top: 400.0
			decimals: 2
			notation: DoubleValidator.StandardNotation
		}
        }
        TextInput {
		id: number
		y: 270
		font.pointSize: 16
		anchors.horizontalCenter: parent.horizontalCenter
        	text: ""+TensoSensor.lotNumber
		color: "grey"
		validator: DoubleValidator {
			bottom: 0.1
			top: 400.0
			decimals: 2
			notation: DoubleValidator.StandardNotation
		}
        }

	Button {
		text: "Accept"
		x: 200
		y: 300
		isDefault: true
		onClicked: {
			console.log("ACCEPTED");
			TensoSensor.lotStartLength = parseFloat(startlength.text)
			TensoSensor.lotMM = mm.text
			TensoSensor.lotYYYY = yyyy.text
			TensoSensor.lotSegment = parseInt(segment.text)
			TensoSensor.lotNumber = parseInt(number.text)
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

