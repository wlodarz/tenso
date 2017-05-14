
function emergency_stop() {
	console.log("Clicked STOP")

	moving(0)
	reporting(0)

	TensoSensor.operation = TensoSensor.SENSOR_OPERATION_STOP
	TensoSensor.operationcompleted = 0
	TensoSensor.suboperation = 0

	zeroing_button.button_label = zeroing_button.button_label_default
	test1_button.button_label = test1_button.button_label_default
	test2_button.button_label = test2_button.button_label_default
}

function measure1() {
	console.log("Measure1 " + parent.activated)
	if (TensoSensor.operation == 0) {
		console.log("op: idle -> measure");
		parent.button_label = "Measure"
		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_MEASURE1
		TensoSensor.suboperation = TensoSensor.MEASURE1_SUBOPERATION_TILLMAXFORCE
		TensoSensor.operationcompleted = 0
		canvas.forceArray.length = 0
		canvas.lengthArray.length = 0
	}
	else {
		console.log("other state - do nothing")
	}
}

function measure2() {
	console.log("Measure2 " + parent.activate)
}

function calibrate() {
	console.log("CALIBRATE")
	if (TensoSensor.operation == 0) {
		console.log("op: idle -> calibrate");
		parent.button_label = "Calibrating"
		TensoSensor.operation = TensoSensor.SENSOR_OPERATION_CALIBRATE
		TensoSensor.suboperation = TensoSensor.NONE_SUBOPERATION
		TensoSensor.operationcompleted = 0
	}
	else {
		console.log("other state - do nothing")
	}
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
	console.log("ZEROING")
	TensoSensor.operation = TensoSensor.SENSOR_OPERATION_ZERO
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
	reporting_widget.steps = TensoSensor.measureIndex
	reporting_widget.power = TensoSensor.calculatedPower
	if (on_off == 1) {
		reporting_widget.focus=true
		reporting_widget.visible=true
	} else {
		reporting_widget.focus=false
		reporting_widget.visible=false
	}
}
