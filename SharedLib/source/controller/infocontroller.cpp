#include "include/controller/infocontroller.h"
#include "include/constants_qt.h"

#include "include/controller/controllermanager.h"

QString InfoController::CONTROLLER_NAME = QStringLiteral("InfoController");
Q_LOGGING_CATEGORY(LG_INFO_CONTROLLER, "InfoController");

InfoController::InfoController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_SERVER, parent)
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

ControllerBase::VALUE_BC_INTERVAL InfoController::getValueBCInterval(int index) {
    switch (index) {
    case EnumsDeclarations::INFOS_SYSTEM_TIME:
    case EnumsDeclarations::INFOS_SYSTEM_TEMP:
        return VALUE_BC_FAST;
    case EnumsDeclarations::INFOS_BOILER_TARGET_TEMP:
        return VALUE_BC_NONE;
    }

    return VALUE_BC_NONE;
}

void InfoController::onInit() {
    qCDebug(LG_INFO_CONTROLLER) << Q_FUNC_INFO << m_mode;

    setValue(EnumsDeclarations::INFOS_SYSTEM_TEMP, 0.0);        // default val

    if (m_parent->isServer()) {
        qCDebug(LG_INFO_CONTROLLER) << "Connecting to system time";

        connect(&m_systemTimeTimer, &QTimer::timeout, [this]() {
            qCDebug(LG_INFO_CONTROLLER) << "Sending system time";
            setValue(EnumsDeclarations::INFOS_SYSTEM_TIME, QDateTime::currentDateTime().toString(Qt::ISODate), false, true);
        });
        m_systemTimeTimer.start(SYSTEM_TIME_BC_INTERVAL);

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

    switch (index) {
    case EnumsDeclarations::INFOS_SYSTEM_TIME:
        if (!m_parent->isServer()) {
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
        break;
    case EnumsDeclarations::INFOS_SYSTEM_TEMP:
        // nothing
        break;

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

            QTimer::singleShot(SYSTEM_TEMP_HC_INTERVAL, this, &InfoController::startReadSystemTemp);
        } else {
            qWarning() << "Error while parsing temp string" << output;
        }
    } else {
        qWarning() << "Error while reading system temp" << m_readSystemTempProcess.exitCode() << m_readSystemTempProcess.errorString();
    }
}

