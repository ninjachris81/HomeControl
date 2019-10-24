#include "include/controller/infocontroller.h"
#include "include/constants_qt.h"

#include "include/controller/controllermanager.h"

QString InfoController::CONTROLLER_NAME = QStringLiteral("InfoController");

InfoController::InfoController(QObject *parent) : ControllerBase(parent)
{
}

QString InfoController::getName() {
    return CONTROLLER_NAME;
}

QStringList InfoController::getTopicPath() {
    return QStringList() << MQTT_PATH_INFOS;
}

QStringList InfoController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::INFOS_LABELS);
}

QString InfoController::getEnumName() {
    return "MQTT_INFOS";
}

QVariant::Type InfoController::getDefaultValueType() {
    return QVariant::Int;
}

qint64 InfoController::getValueLifetime(int index) {
    Q_UNUSED(index)
    return LIFETIME_MID;
}

void InfoController::onInit() {
    qDebug() << Q_FUNC_INFO << m_mode;

    if (m_mode==ControllerBase::VALUE_OWNER_SERVER) {
        connect(&m_systemTimeTimer, &QTimer::timeout, [this]() {
            qDebug() << "Sending system time";
            setValue(EnumsDeclarations::INFOS_SYSTEM_TIME, QDateTime::currentDateTime().toString(Qt::ISODate));
        });
        m_systemTimeTimer.start(5000);
    }
}

void InfoController::onMqttConnected() {
    qDebug() << Q_FUNC_INFO;
}

void InfoController::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    if (m_mode==ControllerBase::VALUE_OWNER_CLIENT) {
        qDebug() << "Server time:" << value.toString();

        if (QDateTime::currentDateTime().secsTo(QDateTime::fromString(value.toString(), Qt::ISODate))>120) {

            if (!m_timeIsOffset) {
                m_timeIsOffset = true;
                Q_EMIT(timeIsOffsetChanged(true));
            }
            qWarning() << "Server and Client time/date is out of sync";
        } else {
            if (m_timeIsOffset) {
                m_timeIsOffset = false;
                Q_EMIT(timeIsOffsetChanged(false));
            }
        }
    }
}

bool InfoController::timeIsOffset() {
    return m_timeIsOffset;
}
