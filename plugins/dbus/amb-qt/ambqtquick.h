#include <QQmlExtensionPlugin>

class AmbQtQuickPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:

	void registerTypes(const char *uri);
};
