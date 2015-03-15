import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

Rectangle {
	//x: parent.textMariginX
        //y: parent.textFieldHeight * 1
        width: parent.fieldWidth
        height: parent.fieldHeight
        color: "white"
        opacity: 0.9
        border.color: "black"
        border.width: 3
        radius: 10
        property string field_label
	property string inputMask
	property int minValue: 1
	property int maxValue: 200
	property string suffix
	property bool field_focus
	property int value_default

	function getValue() { return spinBox.value; }

        Text {
        	y: 6
		font.pointSize: 12
        	anchors.horizontalCenter: parent.horizontalCenter
        	text: field_label
        }
	SpinBox {
		id: spinBox
		x: 4
		y: 24
		width: parent.width - 8
		height: parent.height - 32
		minimumValue: 1
		maximumValue: 399
		horizontalAlignment: Qt.AlignRight
		font.pixelSize: 32
		suffix: parent.suffix
		focus: parent.field_focus

/*
		onEditingFinished: {
			console.log("value: " + value)
			parent.value_int = value
		}
*/
		Component.onCompleted: {
			value = parent.value_default
		}
	}
}

