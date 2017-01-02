import QtQuick 2.4


Rectangle {
	width: 320
	height: 80

        Text {
                y: 10
                font.pointSize: 20
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Error: tensometr missing"

	}
	MouseArea {
       		anchors.fill: parent
       		onClicked: {
			Qt.quit()
       		}
	}
}

