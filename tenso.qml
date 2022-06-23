import QtQuick 2.6
import TensoSensor 1.0

import "js/functions.js" as Control

Rectangle {
	id: mainwindow
	width: 1280
	height: 800
	property int menuWidth: 160
	opacity: 0.9

	//Counter {
	//	id: counter
	//	//visible: (TensoSensor.Suboperation == TensoSensor.MEASURE2_SUBOPERATION_HOLDMAXFORCE) ? true : false
	//	visible: true
	//	x: parent.menuWidth + 60
	//	y: 20
	//	counterValue: TensoSensor.secondsCounter
	//}

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
		property int buttonWidth: parent.menuWidth-20
		property int buttonHeight: 60
		signal measureProcessStarted

		onMeasureProcessStarted: {
			console.log("Measure Started in Rectangle length " + pullTime.value_int + " steps " + holdTime.value_int)
			canvas.clearRequest()
			canvas.requestPaint()
			console.log("total time " + timeField.time)
		}

		// status text fields
		TensoField {
			id: forceField
			x: parent.fieldMariginX
			y: parent.fieldMariginY + parent.fieldHeight * 0
			field_label: "Force"
			value_text: "" + TensoSensor.currentForce.toFixed(1)
		}
		TensoField {
			id: lengthField
			x: parent.fieldMariginX
			y: parent.fieldMariginY + parent.fieldHeight * 1
			field_label: "Length"
			value_text: "" + TensoSensor.currentLength.toFixed(2)
		}
		TensoField {
			id: counterField
			x: parent.fieldMariginX
			y: parent.fieldMariginY + parent.fieldHeight * 2
			field_label: "Counter"
			value_text: "" + TensoSensor.secondsHoldCounter
		}

		TensoButton {
			id: config_button
			button_label_default: "Config"
			button_label: button_label_default
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 9
			property string action_text: "config"
        		property bool activated: false
			MouseArea {
                		anchors.fill: parent
                		onClicked: Control.config(1)
        		}
		}

		TensoButton {
			id: moving_button
			button_label_default: "Move"
			button_label: button_label_default
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 8
			property string action_text: "moving"
        		property bool activated: TensoSensor.operation == TensoSensor.SENSOR_OPERATION_MOVE
			MouseArea {
                		anchors.fill: parent
                		onClicked: Control.moving(1)
        		}
		}

		TensoButton {
			id: zeroing_button
			button_label_default: "Zero"
			button_label: button_label_default
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 7
			property string action_text: "zeroing"
        		property bool activated: TensoSensor.operation == TensoSensor.SENSOR_OPERATION_ZERO
			MouseArea {
                		anchors.fill: parent
                		onClicked: Control.zeroing()
        		}
		}

		TensoButton {
			id: calibraing_button
			button_label_default: "Calibrate"
			button_label: button_label_default
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 6
			property string action_text: "calibrate"
        		property bool activated: TensoSensor.operation == TensoSensor.SENSOR_OPERATION_CALIBRATE
			MouseArea {
                		anchors.fill: parent
                		onClicked: Control.calibrate()
        		}
		}

		TensoButton {
			id: test1_button
			button_label_default: "Measure#1"
			button_label: button_label_default
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 5
			property string action_text: "measure1"
        		property bool activated: TensoSensor.operation == TensoSensor.SENSOR_OPERATION_MEASURE1
			MouseArea {
                		anchors.fill: parent
                		onClicked: Control.measure1()
        		}
		}

		//
		TensoButton {
			id: test2_button
			button_label_default: "Measure#2"
			button_label: button_label_default
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 4
			property string action_text: "measure2"
        		property bool activated: TensoSensor.operation == TensoSensor.SENSOR_OPERATION_MEASURE2
			MouseArea {
                		anchors.fill: parent
                		onClicked: Control.measure2()
			}
		}
		TensoButton {
			id: gostart_button
			button_label: "GoStart"
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 3
			property string action_text: "gostart"
        		property bool activated: TensoSensor.operation == TensoSensor.SENSOR_OPERATION_GOSTART
			MouseArea {
                		anchors.fill: parent
                		onClicked: Control.gostart()
        		}
		}
		TensoButton {
			id: report_button
			button_label: "Report"
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 2
			property string action_text: "report"
        		property bool activated: false
			MouseArea {
                		anchors.fill: parent
                		onClicked: Control.reporting(1)
        		}
		}
		TensoButton {
			button_label: "Stop"
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 1
			property string action_text: "stop"
        		property bool activated: TensoSensor.operation == TensoSensor.SENSOR_OPERATION_STOP
			MouseArea {
                		anchors.fill: parent
                		onClicked: Control.emergency_stop()
        		}
		}

		Connections {
			target: TensoSensor
			function onMeasureUpdatedChanged() {
				var len = TensoSensor.currentLength
				var force = TensoSensor.currentForce
				console.log("measure updated : length " + len + " force " + force)
				console.log("suboperation " + TensoSensor.suboperation)
				TensoSensor.measureUpdate = 0
				switch (TensoSensor.measurePhase) {

					case TensoSensor.MEASURE_PHASE_UP:
						canvas.forceUpArray.push(force)
						canvas.lengthUpArray.push(len)
						break

					case TensoSensor.MEASURE_PHASE_HOLD:
						canvas.forceHoldArray.push(force)
						canvas.lengthHoldArray.push(len)
						break

					case TensoSensor.MEASURE_PHASE_DOWN:
						canvas.forceDownArray.push(force)
						canvas.lengthDownArray.push(len)
						break
				}

				canvas.requestPaint()
		
				if (TensoSensor.Suboperation == TensoSensor.MEASURE2_SUBOPERATION_HOLDMAXFORCE)
					Control.counting(1)
				else
					Control.counting(0)
			}
			function onMeasureLevelsUpdatedChanged() {
				var len = TensoSensor.currentLength
				var force = TensoSensor.currentForce

				console.log("measure levels : length " + len + " force " + force)

				TensoSensor.measureLevelsUpdate = 0
				canvas.forceLevelsArray.push(force)
				canvas.lengthLevelsArray.push(len)
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
		property var forceUpArray: []
		property var lengthUpArray: []
		property var forceHoldArray: []
		property var lengthHoldArray: []
		property var forceDownArray: []
		property var lengthDownArray: []
		property var forceLevelsArray: []
		property var lengthLevelsArray: []

		signal clearRequest

		onClearRequest: {
			var ctx = canvas.getContext('2d');
			ctx.reset()
			forceUpArray.length = 0
			lengthUpArray.length = 0
			forceHoldArray.length = 0
			lengthHoldArray.length = 0
			forceDownArray.length = 0
			lengthDownArray.length = 0
			forceLevelsArray.length = 0
			lengthLevelsArray.length = 0
			arcsPainted = false
		}
		onPaint:{
			function val_to_screen(val, maxval, width)
			{
				return val * width / maxval;
			}

			function max_to_single(max)
			{
				if (max < 1) {
					return 0.1;
				} else if (max < 10) {
					return 1;
				} else if (max < 100) {
					return 10;
				} else if (max < 1000) {
					return 100;
				} else if (max < 10000) {
					return 1000;
				} else {
					return 10000;
				}
			}

			var ctx = canvas.getContext('2d');
			var x_zero = mariginX
			var y_zero = height - mariginY
			var pic_width = width - 2 * mariginX
			var pic_height = height - 2 * mariginY

			var maxX = TensoSensor.maxForce
            		var maxY = TensoSensor.maxLength

			ctx.reset()

			// scale

			ctx.strokeStyle = "steelblue"
			ctx.beginPath()
			// x
			ctx.moveTo(x_zero, y_zero)
			ctx.lineTo(x_zero + pic_width, y_zero)

			// y
			//ctx.beginPath()
			ctx.moveTo(x_zero, y_zero)
			ctx.lineTo(x_zero, y_zero - pic_height)

			// x - length
			var x_offset = 20
			var y_offset = 10
            		var single_x = max_to_single(TensoSensor.maxLength)
			var single_y = max_to_single(TensoSensor.maxForce)
		        var steps_x = TensoSensor.maxLength / single_x
			var steps_y = TensoSensor.maxForce / single_y

			// steps y
			for (var i=0; i<steps_y; i++) {
				var start_x = x_zero
				var start_y = y_zero - val_to_screen(i * single_y, TensoSensor.maxForce, pic_height)
				ctx.moveTo(start_x - x_offset, start_y)
				ctx.lineTo(start_x, start_y)
				var range_text = "" + (i*single_y)
				var text_offset = ctx.measureText(range_text).width
				ctx.fillText(range_text, start_x - text_offset - 1, start_y - y_offset / 2)
			}
			// max y
			var start_y = y_zero - pic_height
			ctx.moveTo(start_x - x_offset, start_y)
			ctx.lineTo(start_x, start_y)
			var range_text = "" + TensoSensor.maxForce
			var text_offset = ctx.measureText(range_text).width
			ctx.fillText(range_text, start_x - text_offset - 1, start_y - y_offset / 2)
			var text_offset = ctx.measureText("force").width
			ctx.fillText("force", start_x - text_offset - 1, start_y - y_offset / 2 - 10)

			// steps x
			for (var i=0; i<steps_x; i++) {
                		var start_x = x_zero + val_to_screen(i * single_x, TensoSensor.maxLength, pic_width)
				var start_y = y_zero
				ctx.moveTo(start_x, start_y)
				ctx.lineTo(start_x, start_y + y_offset)
				var range_text = "" + (i*single_x)
				var text_x_offset = ctx.measureText(range_text).width
				ctx.fillText(range_text, start_x - text_x_offset - 1, start_y + 10)
			}
			// max y
			var start_x = x_zero + pic_width
			ctx.moveTo(start_x, start_y)
			ctx.lineTo(start_x, start_y + y_offset)
            		var range_text = "" + TensoSensor.maxLength
			var text_offset = ctx.measureText(range_text).width
			ctx.fillText(range_text, start_x - text_offset - 1, start_y + 10)
			text_offset = ctx.measureText("length").width
			ctx.fillText("length", start_x - text_offset - 1, start_y + 20)

			ctx.stroke()

			// now we painting Level lines
			ctx.strokeStyle = "purple"
			ctx.beginPath()
			for (var i=0; i<lengthLevelsArray.length; i++) {

                    		var to_x = x_zero + val_to_screen(lengthLevelsArray[i], TensoSensor.maxLength, pic_width)
				var to_y = y_zero - val_to_screen(0, TensoSensor.maxForce, pic_height)
				ctx.moveTo(to_x, to_y)
				//console.log("x " + to_x + " y " + to_y)

                    		to_x = x_zero + val_to_screen(lengthLevelsArray[i], TensoSensor.maxLength, pic_width)
				to_y = y_zero - val_to_screen(forceLevelsArray[i], TensoSensor.maxForce, pic_height)
				ctx.lineTo(to_x, to_y)
				//console.log("x " + to_x + " y " + to_y)

                    		to_x = x_zero + val_to_screen(0, TensoSensor.maxLength, pic_width)
				to_y = y_zero - val_to_screen(forceLevelsArray[i], TensoSensor.maxForce, pic_height)
				ctx.moveTo(to_x, to_y)
				//console.log("x " + to_x + " y " + to_y)

                    		to_x = x_zero + val_to_screen(lengthLevelsArray[i], TensoSensor.maxLength, pic_width)
				to_y = y_zero - val_to_screen(forceLevelsArray[i], TensoSensor.maxForce, pic_height)
				ctx.lineTo(to_x, to_y)
				//console.log("x " + to_x + " y " + to_y)

				var text = ""+lengthLevelsArray[i].toFixed(2)+"m "+forceLevelsArray[i].toFixed(2)+"kg"
				//ctx.fillText(text, x_zero+2, to_y-5)
				//ctx.fillText(text, to_x-20, to_y-5)
				ctx.fillText(text, x_zero+4, y_zero-pic_height+10+i*20)
				
			}
			ctx.stroke()

			// now we painting Up points
			ctx.strokeStyle = "blue"
			ctx.beginPath()
			//ctx.moveTo(x_zero, y_zero)
			if (lengthUpArray > 0) {
                    			var to_x = x_zero + val_to_screen(lengthUpArray[0], TensoSensor.maxLength, pic_width)
					var to_y = y_zero - val_to_screen(forceUpArray[0], TensoSensor.maxForce, pic_height)
					ctx.moveTo(to_x, to_y)
			}
			for (var i=0; i<lengthUpArray.length; i++) {
                		if (lengthUpArray[i] < TensoSensor.maxLength && forceUpArray[i] < TensoSensor.maxForce) {
                    			var to_x = x_zero + val_to_screen(lengthUpArray[i], TensoSensor.maxLength, pic_width)
					var to_y = y_zero - val_to_screen(forceUpArray[i], TensoSensor.maxForce, pic_height)
					//console.log("Hold points start " + to_x + " " + to_y)
					ctx.lineTo(to_x, to_y)
				}
			}
			ctx.stroke()
			// now we painting Hold points
			ctx.strokeStyle = "red"
			ctx.beginPath()
			//ctx.moveTo(x_zero, y_zero)
			if (lengthHoldArray > 0) {
                    			var to_x = x_zero + val_to_screen(lengthHoldArray[0], TensoSensor.maxLength, pic_width)
					var to_y = y_zero - val_to_screen(forceHoldArray[0], TensoSensor.maxForce, pic_height)
					ctx.moveTo(to_x, to_y)
					console.log("Hold points start " + to_x + " " + to_y)
			}
			for (var i=0; i<lengthHoldArray.length; i++) {
                		if (lengthHoldArray[i] < TensoSensor.maxLength && forceHoldArray[i] < TensoSensor.maxForce) {
                    			var to_x = x_zero + val_to_screen(lengthHoldArray[i], TensoSensor.maxLength, pic_width)
					var to_y = y_zero - val_to_screen(forceHoldArray[i], TensoSensor.maxForce, pic_height)
					ctx.lineTo(to_x, to_y)
				}
			}
			ctx.stroke()

			// now we painting Down points
			ctx.strokeStyle = "green"
			ctx.beginPath()
			//ctx.moveTo(x_zero, y_zero)
			if (lengthDownArray > 0) {
                    			var to_x = x_zero + val_to_screen(lengthDownArray[0], TensoSensor.maxLength, pic_width)
					var to_y = y_zero - val_to_screen(forceDownArray[0], TensoSensor.maxForce, pic_height)
					ctx.moveTo(to_x, to_y)
			}
			for (var i=0; i<lengthDownArray.length; i++) {
                		if (lengthDownArray[i] < TensoSensor.maxLength && forceDownArray[i] < TensoSensor.maxForce) {
                    			var to_x = x_zero + val_to_screen(lengthDownArray[i], TensoSensor.maxLength, pic_width)
					var to_y = y_zero - val_to_screen(forceDownArray[i], TensoSensor.maxForce, pic_height)
					ctx.lineTo(to_x, to_y)
				}
			}
			ctx.stroke()
			ctx.closePath()

		}
		Component.onCompleted: {
		}
	    }
	}

	Moving {
		id: moving_widget
		visible: false
		focus: true
		x: (parent.width - width) / 2
		y: (parent.height - height) / 2
	}
	Report {
		id: reporting_widget
		visible: false
		focus: true
		x: (parent.width - width) / 3
		y: (parent.height - height) / 3
	}
	Config {
		id: config_widget
		visible: false
		focus: true
		x: (parent.width - width) / 3
		y: (parent.height - height) / 3
	}

	Keys.onPressed: {
		console.log("pressed")
		event.accepted = true
		switch (event.key) {
			case Qt.Key_1:
				Control.test_mode_1()
				break
			case Qt.Key_2:
				Control.test_mode_2()
				break
			case Qt.Key_M:
				Control.move_mode()
				break
			case Qt.Key_P:
				Control.parking()()
				break
			case Qt.Key_Z:
				Control.zeroing()
				break
			case Qt.Key_Left:
				Control.move_left()
				break
			case Qt.Key_Right:
				Control.move_right()
				break
			case Qt.Key_Escape:
				Control.emergency_stop()
				break
			default:
				event.accepted = false
				console.log("key not supported")
		}
	}
}

