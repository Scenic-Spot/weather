import QtQuick 2.0
import QtQuick.Controls 2.0
Item {
    property alias myHeight: rect.height
    property alias myWidth: rect.width
    Rectangle{
        id:rect
        width: 480
        height: 240
        opacity: 0.5
        color: "#444444"
        radius: 15
//左侧图片
        Image {
            id: image
            height: rect.height
            width: rect.width / 2
            anchors {
                left: parent.left
                top: parent.top          
            }

            source:"qrc:/Images/cloudy.png"
        }
//右上侧温度文本
        Text{
            id:text1
            text:"21℃"
            height:30
            width:30
            font.pixelSize: 36
            font.bold:false
           // font.family: Times
            color:"blue"
            anchors{
                top:parent.top
                left:image.right
                topMargin: 10
            }

        }
//右下侧出行建议
        Text{
            id:text2
            text:"出行建议：穿件毛衣，多带点水。"
            height:20
            width:100
            font.pixelSize: 16
            font.bold:true
            color:"white"
            anchors{
                bottom:rect.bottom
                left:image.right
                topMargin: 10
                top:text1.bottom
            }
            wrapMode: Text.WordWrap//进行换行
        }
    }
}
