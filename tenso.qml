import QtQuick 2.6
import TensoSensor 1.0


Rectangle {
	width: 1280
	height: 800
	property int menuWidth: 160
	focus: true

	function emergency_stop() {
		console.log("Clicked STOP")

		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_IDLE
		TensoSensor.operationcompleted = 0
		TensoSensor.suboperation = 0

		moving(0)
		reporting(0)

		zeroing_button.button_label = zeroing_button.button_label_default
		test1_button.button_label = test1_button.button_label_default
		test2_button.button_label = test2_button.button_label_default
	}

	function test_mode_1(mode) {
		console.log("Clicked zeroing " + parent.activated)
		if (TensoSensor.operation == TensoSensor.SENSOR_OPERATION_IDLE) {
			console.log("op: idle -> prerun");
			parent.button_label = "PreRun"
			TensoSensor.operation = TensoSensor.SENSOR_OPERATION_MEASURE1
			TensoSensor.operationcompleted = 0
			TensoSensor.suboperation = TensoSensor.MEASURE1_SUBOPERATON_20CM

			canvas.forceArray.length = 0
			canvas.lengthArray.length = 0
		}
		if (TensoSensor.operation == TensoSensor.SENSOR_OPERATION_MEASURE1 && TensoSensor.operationcompleted == 1) {
			switch (TensoSensor.suboperation) {
				case MEASURE1_SUBOPERATION_20CM:
				{
					console.log("op: prerun -> measure")
					parent.button_label = "Measure"
					TensoSensor.suboperation=TensoSensor.MEASURE1_SUBOPERATION_LEN
					TensoSensor.operationcompleted = 0
				}
				break;
				case MEASURE1_SUBOPERATION_MEASURE_LEN:
				{
					console.log("op: measure -> run100cm")
					parent.button_label = "Run100"
					TensoSensor.suboperation=TensoSensor.MEASURE1_SUBOPERATION_100CM
					TensoSensor.operationcompleted = 0
				}
				break;
				case MEASURE1_SUBOPERATION_100CM:
				{
					console.log("op: measure -> idle")
					parent.button_label = parent.button_label_default
					TensoSensor.operation=TensoSensor.SENSOR_OPERATION_IDLE
					TensoSensor.suboperation=NONE_SUBOPERATION
					TensoSensor.operationcompleted = 0
				}
				break;
				default:
					console.log("op: zeroing state: unknown " + TensoSensor.suboperation)
			}
		} else {
			console.log("other state - do nothing")
		}
        }

	function test_mode_2() {
		console.log("Clicked zeroing " + parent.activated)
		if (TensoSensor.operation == 0) {
			console.log("op: idle -> till40kg");
			parent.button_label = "TILL40KG"
			TensoSensor.operation = TensoSensor.SENSOR_OPERATION_MEASURE2
			TensoSensor.suboperation = TensoSensor.MEASURE2_SUBOPERATION_TILL40KG
			TensoSensor.operationcompleted = 0
			canvas.forceArray.length = 0
			canvas.lengthArray.length = 0
		}
		else {
			console.log("other state - do nothing")
		}
	}

	function calibrate() {
		console.log("CALIBRATE - UNSUPPORTED")
/*
		if (TensoSensor.operation == TensoSensor.SENSOR_OPERATION_CALIBRATE) {
			console.log("op: idle -> motoroff")
			parent.button_label = "MotorOff"
			TensoSensor.operation = TensoSensor.SENSOR_OPERATION_ZERO
			TensoSensor.suboperation = TensoSensor.ZERO_SUBOPERATION_MOTOROFF
			TensoSensor.operationcompleted = 0
		}
		if (TensoSensor.operation == 1 && TensoSensor.operationcompleted == 1) {
			switch (TensoSensor.suboperation) {
				case TensoSensor.ZERO_SUBOPERATION_MOTOROFF:
				{
					console.log("op: motoroff -> till40kg")
					parent.button_label = "Till40kg"
					TensoSensor.suboperation=TensoSensor.ZERO_SUBOPERATION_TILL40KG
					TensoSensor.operationcompleted = 0
				}
				break;
				case TensoSensor.ZERO_SUBOPERATION_TILL40KG:
				{
					console.log("op: till40kg -> idle")
					parent.button_label = parent.button_label_default
					TensoSensor.operation=TensoSensor.SENSOR_OPERATION_IDLE
					TensoSensor.suboperation=NONE_SUBOPERATION
					TensoSensor.operationcompleted = 0
				}
				break;
				default:
					console.log("op: zeroing state: unknown " + TensoSensor.suboperation)
			}
		} else {
			console.log("other state - do nothing")
		}
*/
	}

	function moving(on_off) {
		if (on_off == 1) {
			TensoSensor.operation = TensoSensor.SENSOR_OPERATION_MOVE
			TensoSensor.movedirection = 0
			moving_widget.focus=true
			moving_widget.visible=true
		} else {
			TensoSensor.operation = TensoSensor.SENSOR_OPERATION_IDLE
			moving_widget.focus=false
			moving_widget.visible=false
		}
	}

	function parking() {
		if (TensoSensor.operation == TensoSensor.SENSOR_OPERATION_IDLE) {
			console.log("op: idle -> parking")
			TensoSensor.operation = TensoSensor.SENSOR_OPERATION_PARK
			TensoSensor.suboperation = TensoSensor.PARK_SUBOPERATION_INPROGRESS
			TensoSensor.operationcompleted = 0
		}
		if (TensoSensor.operation == TensoSensor.SENSOR_OPERATION_PARK && TensoSensor.operationcompleted == 1) {
			console.log("op: parking -> idle")
			TensoSensor.operation = TensoSensor.SENSOR_OPERATION_IDLE 
			TensoSensor.suboperation = NONE_SUBOPERATION
			TensoSensor.operationcompleted = 0
		}
	}

	function zeroing() {
		console.log("ZEROING - UNSUPPORTED")
	}

	function move_left() {
		console.log("MOVE LEFT movedirection " + TensoSensor.movedirection)
		if (TensoSensor.movedirection > -3) TensoSensor.movedirection -= 1
	}

	function move_right() {
		console.log("MOVE RIGHT movedirection " + TensoSensor.movedirection)
		if (TensoSensor.movedirection < 3) TensoSensor.movedirection += 1
	}

	function reporting(on_off) {
		//console.log("POWER " + TensoSensor.calculatedPower)
		reporting_widget.steps = TensoSensor.measureIndex
		reporting_widget.power = TensoSensor.calculatedPower
		if (on_off == 1) {
			TensoSensor.moving = 1
			reporting_widget.focus=true
			reporting_widget.visible=true
		} else {
			TensoSensor.moving = 0
			reporting_widget.focus=false
			reporting_widget.visible=false
		}
	}

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
			//console.log("Measure Started in Rectangle length " + pullTime.value_int + " steps " + holdTime.value_int)
			canvas.forceArray.length = 0
			canvas.lengthArray.length = 0
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
                		onClicked: {
					moving(1)
				}
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
                		onClicked: {
					zeroing()
				}
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
                		onClicked: {
					calibrate()
				}
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
                		onClicked: {
					test_mode_1()
				}
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
                		onClicked: {
					test_mode_2()
				}
			}
		}
		TensoButton {
			id: park_button
			button_label: "Park"
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 3
			property string action_text: "park"
        		property bool activated: TensoSensor.operation == TensoSensor.SENSOR_OPERATION_PARK
			MouseArea {
                		anchors.fill: parent
                		onClicked: {
					parking()
				}
        		}
		}
		TensoButton {
			id: report_button
			button_label: "Report"
			x: parent.fieldMariginX
			y: parent.height - parent.fieldMariginY - parent.buttonHeight * 2
			property string action_text: "repot"
        		property bool activated: false
			MouseArea {
                		anchors.fill: parent
                		onClicked: {
					reporting(1)
				}
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
                		onClicked: {
					emergency_stop()
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

		signal clearRequest

		onClearRequest: {
			var ctx = canvas.getContext('2d');
			ctx.reset()
			forceArray.length = 0
			lengthArray.length = 0
			arcsPainted = false
		}
		onPaint:{
			function val_to_screen(val, maxval, width)
			{
				return val * width / maxval;
			}

			function max_to_single(max)
			{
				if (max < 100) {
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
			ctx.stroke()

			// y
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

			ctx.closePath()
			ctx.stroke()

			// now we painting points
			ctx.strokeStyle = "black"

			ctx.beginPath()
			ctx.moveTo(x_zero, y_zero)
			for (var i=0; i<lengthArray.length; i++) {
				if (lengthArray[i] < TensoSensor.maxLength && forceArray[i] < TensoSensor.maxForce) {
					var to_x = x_zero + val_to_screen(lengthArray[i], TensoSensor.maxLength, pic_width)
					var to_y = y_zero - val_to_screen(forceArray[i], TensoSensor.maxForce, pic_height)
					//console.log("to_x " + to_x + " to_y " + to_y)
					ctx.lineTo(to_x, to_y)
				}
			}
			ctx.stroke()
		}
		Component.onCompleted: {
		}
	    }
	}

	Keys.onPressed: {
		console.log("pressed")
		event.accepted = true
		switch (event.key) {
			case Qt.Key_1:
				test_mode_1()
				break
			case Qt.Key_2:
				test_mode_2()
				break
			case Qt.Key_M:
				move_mode()
				break
			case Qt.Key_P:
				parking()()
				break
			case Qt.Key_Z:
				zeroing()
				break
			case Qt.Key_Left:
				move_left()
				break
			case Qt.Key_Right:
				move_right()
				break
			case Qt.Key_Escape:
				emergency_stop()
				break
			default:
				event.accepted = false
				console.log("key not supported")
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
}

