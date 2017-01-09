import QtQuick 2.4

Rectangle {
	width: parent.fieldWidth
	height: parent.fieldHeight
        color: "white"
        opacity: 0.9
        border.color: "black"
        border.width: 3
        radius: 5
        property string field_label
        property string value_text

        Text {
        	y: 10
		font.pointSize: 12
        	anchors.horizontalCenter: parent.horizontalCenter
        	text: field_label
        }
        Text {
        	y: 24
		font.pointSize: 32
        	anchors.horizontalCenter: parent.horizontalCenter
        	text: value_text
        }
}

