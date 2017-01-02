import QtQuick 2.4
import TensoSensor 1.0

Rectangle {
	width: 1280
	height: 800
	property int menuWidth: 160

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
		property int fieldWidth: parent.menuWidth-20
		property int fieldHeight: 80
		//property bool started: TensoSensor.startMeasure
		signal measureProcessStarted

		onMeasureProcessStarted: {
			//console.log("Measure Started in Rectangle length " + pullTime.value_int + " steps " + holdTime.value_int)
			canvas.forceArray.length = 0
			canvas.lengthArray.length = 0
			canvas.maxForce = 0
			canvas.clearRequest()
			console.log("total time " + timeField.time)
		}

		// status text fields
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
			value_text: "" + TensoSensor.currentLength
		}

		// input text fields
//		TensoInputField {
//			id: maxLength
//			x: parent.fieldMariginX
//			y: parent.fieldMariginY * 2 + parent.fieldHeight * 3
//			field_label: "Max Length"
//			value_default: TensoSensor.maxLength
//			inputMask: "009"
//			minValue: 1
//			maxValue: 200
//			suffix: ""
//		}

		TensoButton {
			id: zeroing_button
			button_label_default: "Zeroing"
			button_label: button_label_default
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.fieldHeight * 5
			property string action_text: "zeroing"
        		property bool activated: TensoSensor.operation == 1
			MouseArea {
                		anchors.fill: parent
                		onClicked: {
					console.log("Clicked zeroing " + parent.activated)
					if (TensoSensor.operation == 0) {
						console.log("op: idle -> motoroff");
						parent.button_label = "MotorOff"
						TensoSensor.operation = 1
						TensoSensor.operationcompleted = 0
						TensoSensor.suboperation = 0x0010
					}
					if (TensoSensor.operation == 1 && TensoSensor.operationcompleted == 1) {
						switch (TensoSensor.suboperation) {
							case 0x0010: // motor off
							{
								console.log("op: motoroff -> till40kg")
								parent.button_label = "Till40kg"
								TensoSensor.suboperation=0x0011
								TensoSensor.operationcompleted = 0
							}
							break;
							case 0x0011: // till40kg
							{
								console.log("op: till40kg -> idle")
								parent.button_label = parent.button_label_default
								TensoSensor.operation=0
								TensoSensor.suboperation=0
								TensoSensor.operationcompleted = 0
							}
							break;
							default:
								console.log("op: zeroing state: unknown " + TensoSensor.suboperation)
						}
					} else {
						console.log("other state - do nothing");
					}
                		}
        		}
		}

		TensoButton {
			id: test1_button
			button_label_default: "Test#1"
			button_label: button_label_default
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.fieldHeight * 4
			property string action_text: "test1"
        		property bool activated: TensoSensor.operation == 2
			MouseArea {
                		anchors.fill: parent
                		onClicked: {
					console.log("Clicked zeroing " + parent.activated)
					if (TensoSensor.operation == 0) {
						console.log("op: idle -> prerun");
						parent.button_label = "PreRun"
						TensoSensor.operation = 2
						TensoSensor.operationcompleted = 0
						TensoSensor.suboperation = 0x0020

						canvas.forceArray.length = 0
						canvas.lengthArray.length = 0
						canvas.maxForce = 0
					}
					if (TensoSensor.operation == 2 && TensoSensor.operationcompleted == 1) {
						switch (TensoSensor.suboperation) {
							case 0x0020: // measure
							{
								console.log("op: prerun -> measure")
								parent.button_label = "Measure"
								TensoSensor.suboperation=0x0021
								TensoSensor.operationcompleted = 0
							}
							break;
							case 0x0021:
							{
								console.log("op: measure -> run100cm")
								parent.button_label = "Run100"
								TensoSensor.suboperation=0x0022
								TensoSensor.operationcompleted = 0
							}
							break;
							case 0x0022: // till40kg
							{
								console.log("op: measure -> idle")
								parent.button_label = parent.button_label_default
								TensoSensor.operation=0
								TensoSensor.suboperation=0
								TensoSensor.operationcompleted = 0
							}
							break;
							default:
								console.log("op: zeroing state: unknown " + TensoSensor.suboperation)
						}
					} else {
						console.log("other state - do nothing");
					}
                		}
        		}
		}

		//
		TensoButton {
			id: test2_button
			button_label_default: "Test#2"
			button_label: button_label_default
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.fieldHeight * 3
			property string action_text: "test2"
        		property bool activated: TensoSensor.operation == 3
			MouseArea {
                		anchors.fill: parent
                		onClicked: {
					console.log("Clicked zeroing " + parent.activated)
					if (TensoSensor.operation == 0) {
						console.log("op: idle -> motoroff");
						parent.button_label = "MotorOff"
						TensoSensor.operation = 3
						TensoSensor.operationcompleted = 0
						TensoSensor.suboperation = 0x0010
					}
					if (TensoSensor.operation == 3 && TensoSensor.operationcompleted == 1) {
						switch (TensoSensor.suboperation) {
							case 0x0010: // motor off
							{
								console.log("op: motoroff -> till40kg")
								parent.button_label = "Till40kg"
								TensoSensor.suboperation=0x0011
								TensoSensor.operationcompleted = 0
							}
							break;
							case 0x0011: // till40kg
							{
								console.log("op: till40kg -> idle")
								parent.button_label = parent.button_label_default
								TensoSensor.operation=0
								TensoSensor.suboperation=0
								TensoSensor.operationcompleted = 0
							}
							break;
							default:
								console.log("op: zeroing state: unknown " + TensoSensor.suboperation)
						}
					} else {
						console.log("other state - do nothing");
					}
                		}
        		}
		}
		TensoButton {
			id: park_button
			button_label: "Park"
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.fieldHeight * 2
			property string action_text: "park"
        		property bool activated: TensoSensor.parking == 1
			MouseArea {
                		anchors.fill: parent
                		onClicked: {
					if (TensoSensor.operation == 0) {
						console.log("op: idle -> parking");
						TensoSensor.parking = 1
						TensoSensor.operation = 10
						TensoSensor.operationcompleted = 0
						TensoSensor.suboperation = 10 * 0x0010
					}
					if (TensoSensor.operation == (10 * 0x0010) && TensoSensor.operationcompleted == 1) {
						console.log("op: parking -> idle");
						TensoSensor.parking = 0
						TensoSensor.operation = 0
						TensoSensor.operationcompleted = 0
						TensoSensor.suboperation = 0
					}
                		}
        		}
		}
		TensoButton {
			button_label: "Stop"
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.fieldHeight * 1
			property string action_text: "stop"
        		property bool activated: TensoSensor.forceStop == 1
			MouseArea {
                		anchors.fill: parent
                		onClicked: {
					console.log("Clicked STOP")

                        		TensoSensor.forceStop = 1
					TensoSensor.parking = 0
					TensoSensor.operation = 0
					TensoSensor.operationcompleted = 0
					TensoSensor.suboperation = 0

					zeroing_button.button_label = zeroing_button.button_label_default
					test1_button.button_label = test1_button.button_label_default
					test2_button.button_label = test2_button.button_label_default
                		}
        		}
		}

		Connections {
			target: TensoSensor
			onMeasureUpdatedChanged: {
				var len = TensoSensor.currentLength
				var force = TensoSensor.currentForce + 10 * canvas.forceArray.length
				console.log("measure updated : length " + len + " force " + force)
				TensoSensor.measureUpdate = 0
				canvas.forceArray.push(force)
				canvas.lengthArray.push(len)
				if (force > canvas.maxForce) canvas.maxForce = force
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
		property var forceArray: []
		property var lengthArray: []
		property var maxForce: 0

		signal clearRequest

		onClearRequest: {
			var ctx = canvas.getContext('2d');
			ctx.reset()
			forceArray.length = 0
			lengthArray.length = 0
			arcsPainted = false
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

				ctx.reset()
				ctx.beginPath()
				ctx.moveTo(startX, startY)
				for (var i=0; i<lengthArray.length; i++) {
					//console.log("Point " + pointArray[i])
					//ctx.lineTo(startX + i*5 , startY - (pointArray[i]*3))
					ctx.lineTo(startX + i, startY - (forceArray[i] * pic_height / canvas.maxForce))
				}
				ctx.stroke()
			}
		}
		Component.onCompleted: {
		}
	    }
	}
}

