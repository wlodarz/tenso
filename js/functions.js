
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
