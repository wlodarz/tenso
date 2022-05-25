
function start_operation(op_title) {
	console.log("start_operation "+op_title)
	TensoSensor.operationcompleted = 0
	// parent.button_label = ""+op_title
}

function emergency_stop() {
	console.log("Clicked STOP")

	moving(0)
	reporting(0)
	config(0)
	counting(0)

	TensoSensor.operation = TensoSensor.SENSOR_OPERATION_STOP
	TensoSensor.suboperation = 0
	start_operation("Stop")

	zeroing_button.button_label = zeroing_button.button_label_default
	test1_button.button_label = test1_button.button_label_default
	test2_button.button_label = test2_button.button_label_default

	mainwindow.focus = true
}

function measure1() {
	console.log("MEASURE1")
	if (TensoSensor.operation == 0) {
		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_MEASURE1
		TensoSensor.suboperation = TensoSensor.MEASURE1_SUBOPERATION_TILLMAXFORCE
		start_operation("Measure1")
		counting(0)
	}
	else {
		console.log("other state - do nothing")
	}
}

function measure2() {
	console.log("MEASURE2")
	if (TensoSensor.operation == 0) {
		canvas.clearRequest()
		canvas.requestPaint()
		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_MEASURE2
		TensoSensor.suboperation = TensoSensor.MEASURE2_SUBOPERATION_TILLSTARTMEASUREFORCE
		start_operation("Measure2")
		counting(0)
	}
	else {
		console.log("other state - do nothing")
	}
}

function calibrate() {
	console.log("CALIBRATE")
	if (TensoSensor.operation == 0) {
		console.log("op: idle -> calibrate");
		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_CALIBRATE
		TensoSensor.suboperation = TensoSensor.CALIBRATE_SUBOPERATION_TILL_FORCE_MAX
		start_operation("Calibrate")
	}
	else {
		console.log("other state - do nothing")
	}
}

function moving(on_off) {
	console.log("MOVING")
	if (on_off == 1) {
		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_MOVE
		TensoSensor.movedirection = 0
		moving_widget.focus=true
		moving_widget.visible=true
		start_operation("Moving")
	} else {
		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_IDLE
		moving_widget.focus=false
		moving_widget.visible=false
	}
}

function gostart() {
	if (TensoSensor.operation == TensoSensor.SENSOR_OPERATION_IDLE) {
		console.log("op: idle -> gostart")
		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_GOSTART
		TensoSensor.suboperation = TensoSensor.GOSTART_SUBOPERATION_INPROGRESS
		start_operation("GoStart")
	}
	if (TensoSensor.operation == TensoSensor.SENSOR_OPERATION_GOSTART && TensoSensor.operationcompleted == 1) {
		console.log("op: gostart -> idle")
		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_IDLE 
		TensoSensor.suboperation = NONE_SUBOPERATION
		TensoSensor.operationcompleted = 0
	}
}

function zeroing() {
	console.log("ZEROING")
	if (TensoSensor.operation == TensoSensor.SENSOR_OPERATION_IDLE) {
		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_ZERO
		start_operation("Zeroing")
	}
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
	console.log("REPORT")
	if (TensoSensor.operation == TensoSensor.SENSOR_OPERATION_IDLE) {
		reporting_widget.stepsUp = TensoSensor.measureIndex
		reporting_widget.workUp = TensoSensor.calculatedWorkUp
		reporting_widget.workHold = TensoSensor.calculatedWorkHold
		reporting_widget.workUpAndHold = TensoSensor.calculatedWorkUp + TensoSensor.calculatedWorkHold
		reporting_widget.workDown = TensoSensor.calculatedWorkDown
		reporting_widget.turns = TensoSensor.turns
		if (on_off == 1) {
			reporting_widget.focus=true
			reporting_widget.visible=true
			start_operation("Reporting")
		} else {
			reporting_widget.focus=false
			reporting_widget.visible=false
		}
	}
}

function config(on_off) {
	console.log("CONFIG")
	if (on_off == 1) {
		config_widget.focus=true
		config_widget.visible=true
	} else {
		config_widget.focus=false
		config_widget.visible=false
	}
}

function counting(on_off) {
	console.log("COUNTING")
	if (on_off == 1) {
		counter.focus=true
		counter.visible=true
	} else {
		counter.focus=false
		counter.visible=false
	}
}

function save_report() {
	console.log("Saving report")
	TensoSensor.savereportflag=1
	reporting_widget.focus=false
	reporting_widget.visible=false
}

function discard_report() {
	console.log("Discarding report")
	reporting_widget.focus=false
	reporting_widget.visible=false
}

