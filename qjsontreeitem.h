
#include <qstring.h>
#include <QAbstractItemModel>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QIcon>

class QJsonTreeItem
{
public:
    QJsonTreeItem(QJsonTreeItem * parent = nullptr);
    ~QJsonTreeItem();
    void appendChild(QJsonTreeItem * item);
    QJsonTreeItem *child(int row);
    QJsonTreeItem *parent();
    int childCount() const;
    int row() const;
    void setKey(const QString& key);
    void setValue(const QString& value);
    void setType(const QJsonValue::Type& type);
    QString key() const;
    QString value() const;
    QJsonValue::Type type() const;


    static QJsonTreeItem* load(const QJsonValue& value, QJsonTreeItem * parent = 0);

protected:


private:
    QString mKey;
    QString mValue;
    QJsonValue::Type mType;
    QList<QJsonTreeItem*> mChilds;
    QJsonTreeItem * mParent;


};
