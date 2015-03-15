import QtQuick 2.4
import TensoSensor 1.0

Rectangle {
	width: 1280
	height: 800
	property int menuWidth: 120

	Rectangle {
		x: 0
		width: menuWidth
		height: parent.height
		color: "silver"
		opacity: 0.9
                border.color: "black"
		border.width: 3
                radius: 10
		property int fieldMariginX: 10
		property int fieldMariginY: 10
		property int fieldWidth: 100
		property int fieldHeight: 80
		property bool started: TensoSensor.startMeasure

		signal measureProcessStarted

		onMeasureProcessStarted: {
			//console.log("Measure Started in Rectangle length " + pullTime.value_int + " steps " + holdTime.value_int)
			canvas.pointArray.length = 0
			canvas.clearRequest()
			//console.log("Point Array size " + canvas.pointArray.length)
			console.log("total time " + timeField.time)
		}

		/* status text fields */
		TensoField {
			id: forceField
			x: parent.fieldMariginX
			y: parent.fieldMariginY + parent.fieldHeight * 0
			field_label: "C.Force"
			value_text: "" + TensoSensor.currentForce
		}

		TensoField {
			id: lengthField
			x: parent.fieldMariginX
			y: parent.fieldMariginY + parent.fieldHeight * 1
			field_label: "C.Length"
			property int time: TensoSensor.currentLength
			value_text: "" + time
		}
		TensoField {
			id: timeField
			x: parent.fieldMariginX
			y: parent.fieldMariginY + parent.fieldHeight * 2
			field_label: "C.Time"
			//property int time: TensoSensor.pullTime + TensoSensor.holdTime
			property int time: TensoSensor.currentHoldTime
			value_text: "" + time
		}

		/* input text fields */
		TensoInputField {
			id: maxLength
			x: parent.fieldMariginX
			y: parent.fieldMariginY * 2 + parent.fieldHeight * 3
			field_label: "Max Length"
			value_default: TensoSensor.maxLength
			inputMask: "009"
			minValue: 1
			maxValue: 200
			suffix: ""
		}
		TensoInputField {
			id: maxForce
			x: parent.fieldMariginX
			y: parent.fieldMariginY * 2 + parent.fieldHeight * 4
			field_label: "Max Force"
			value_default: TensoSensor.maxForce
			inputMask: "009"
			minValue: 1
			maxValue: 128
			suffix: ""
		}
		TensoInputField {
			id: holdTime
			x: parent.fieldMariginX
			y: parent.fieldMariginY * 2 + parent.fieldHeight * 5
			field_label: "Hold Time"
			value_default: TensoSensor.holdTime
			inputMask: "009"
			minValue: 1
			maxValue: 128
			suffix: ""
		}


		/* buttons */
		TensoButton {
			button_label: "Start"
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.fieldHeight * 2
			property string action_text: "start"
        		property bool activated: TensoSensor.startMeasure == 1
			MouseArea {
                		anchors.fill: parent
                		onClicked: {
					console.log("startMeasure " + TensoSensor.startMeasure)
                        		if (TensoSensor.startMeasure == 0) {
						TensoSensor.holdTime = holdTime.getValue()
						TensoSensor.maxLength = maxLength.getValue()
						TensoSensor.maxForce = maxForce.getValue()
                                		parent.parent.measureProcessStarted()
                                		console.log("clicked " + parent.parent.started)
                        			TensoSensor.startMeasure = 1
                        		}
                		}
        		}
		}
		TensoButton {
			button_label: "Stop"
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.fieldHeight * 1
			property string action_text: "stop"
        		property bool activated: TensoSensor.startMeasure == 0
			MouseArea {
                		anchors.fill: parent
                		onClicked: {
					console.log("startMeasure " + TensoSensor.startMeasure)
                        		if (TensoSensor.startMeasure == 1) {
                                		parent.parent.measureProcessStarted()
                                		console.log("clicked " + parent.parent.started)
                        			TensoSensor.startMeasure = 0
                        		}
                		}
        		}
		}

		Connections {
			target: TensoSensor
			onForceChanged: {
				//console.log("data step " + step + " val " + value);
				canvas.pointArray.push(value)
				//console.log("Array " +  canvas.pointArray)
				canvas.requestPaint()
			}
		}
	}
	Rectangle {
		x: parent.menuWidth
		width: parent.width - parent.menuWidth
		height: parent.height
		opacity: 0.9
                border.color: "black"
		border.width: 3
                radius: 10
		
	    Canvas {
		id:canvas
		anchors.fill: parent
		property bool arcsPainted: false
		property int mariginX: 40
		property int mariginY: 40
		property var pointArray: []

		signal clearRequest

		onClearRequest: {
			var ctx = canvas.getContext('2d');
			ctx.reset()
		}
		onPaint:{
			var ctx = canvas.getContext('2d');
			var startX = mariginX
			var startY = height - mariginY
			var pic_width = width - 2 * mariginX
			var pic_height = height - 2 * mariginY
			//console.log("Painting")


			if (!arcsPainted) {
				arcsPainted = true

				ctx.reset()
				ctx.strokeStyle = "steelblue"

				ctx.beginPath()
				ctx.moveTo(startX, startY)
				ctx.lineTo(startX + pic_width, startY)
				ctx.stroke()

				ctx.beginPath()
				ctx.moveTo(startX, startY)
				ctx.lineTo(startX, startY - pic_height)
				ctx.closePath()
				ctx.stroke()
			} else {
				ctx.strokeStyle = "black"

				ctx.beginPath()
				ctx.moveTo(startX, startY)
				for (var i=0; i<pointArray.length; i++) {
					//console.log("Point " + pointArray[i])
					ctx.lineTo(startX + i*5 , startY - (pointArray[i]*3))
				}
				ctx.stroke()
			}
		}
		Component.onCompleted: {
		}
	    }
	}
}

