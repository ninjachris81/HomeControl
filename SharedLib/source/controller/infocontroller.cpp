#include "include/controller/infocontroller.h"
#include "include/constants_qt.h"

#include "include/controller/controllermanager.h"

QString InfoController::CONTROLLER_NAME = QStringLiteral("InfoController");
Q_LOGGING_CATEGORY(LG_INFO_CONTROLLER, "InfoController");

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
    qCDebug(LG_INFO_CONTROLLER) << Q_FUNC_INFO << m_mode;

    if (m_parent->isServer()) {
        qCDebug(LG_INFO_CONTROLLER) << "Connecting to system time";

        connect(&m_systemTimeTimer, &QTimer::timeout, [this]() {
            qCDebug(LG_INFO_CONTROLLER) << "Sending system time";
            setValue(EnumsDeclarations::INFOS_SYSTEM_TIME, QDateTime::currentDateTime().toString(Qt::ISODate));
        });
        m_systemTimeTimer.start(5000);

#ifdef WIN32
    // nothing
        qCDebug(LG_INFO_CONTROLLER) << "Ignoring system temp on WIN32";
#else
        qCDebug(LG_INFO_CONTROLLER) << "Starting to read system temp";
        connect(&m_readSystemTempProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &InfoController::onReadSystemTemp);
        startReadSystemTemp();
#endif

    }
}

void InfoController::onMqttConnected() {
    qCDebug(LG_INFO_CONTROLLER) << Q_FUNC_INFO;
}

void InfoController::onValueChanged(int index, QVariant value) {
    qCDebug(LG_INFO_CONTROLLER) << Q_FUNC_INFO << index << value;

    if (m_mode==ControllerBase::VALUE_OWNER_CLIENT) {
        qCDebug(LG_INFO_CONTROLLER) << "Server time:" << value.toString();

        if (QDateTime::currentDateTime().secsTo(QDateTime::fromString(value.toString(), Qt::ISODate))>120) {

            if (!m_timeIsOffset) {
                m_timeIsOffset = true;
                Q_EMIT(timeIsOffsetChanged(true));
            }
            qCWarning(LG_INFO_CONTROLLER) << "Server and Client time/date is out of sync";
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

void InfoController::startReadSystemTemp() {
    qDebug() << Q_FUNC_INFO;

    m_readSystemTempProcess.start("vcgencmd measure_temp", QIODevice::ReadOnly);
}

void InfoController::onReadSystemTemp(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitCode)

    qDebug() << Q_FUNC_INFO;

    if (exitStatus==QProcess::NormalExit) {
        QString output = m_readSystemTempProcess.readAll();

        qDebug() << Q_FUNC_INFO << output;

        int i = output.indexOf("'");

        if (output.startsWith("temp=") && i>5) {
            double tempVal = output.mid(5, i-5).toDouble();

            setValue(EnumsDeclarations::INFOS_SYSTEM_TEMP, tempVal);

            m_readSystemTempProcess.kill();

            QTimer::singleShot(10000, this, &InfoController::startReadSystemTemp);
        } else {
            qWarning() << "Error while parsing temp string" << output;
        }
    } else {
        qWarning() << "Error while reading system temp" << m_readSystemTempProcess.exitCode() << m_readSystemTempProcess.errorString();
    }
}

