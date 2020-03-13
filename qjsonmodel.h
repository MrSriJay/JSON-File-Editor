#ifndef QJSONMODEL_H
#define QJSONMODEL_H

#include "qjsontreeitem.h"

#include <QAbstractItemModel>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QIcon>

class QJsonModel;
class QJsonItem;



class QJsonModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QJsonModel(QObject *parent = nullptr);
    QJsonModel(const QString& fileName, QObject *parent = nullptr);
    QJsonModel(QIODevice * device, QObject *parent = nullptr);
    QJsonModel(const QByteArray& json, QObject *parent = nullptr);
    ~QJsonModel();
    bool load(const QString& fileName);
    bool load(QIODevice * device);
    bool loadJson(const QByteArray& json);
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QJsonDocument json() const;

private:

    QJsonValue genJson(QJsonTreeItem *) const;

    QJsonTreeItem * mRootItem;
    QStringList mHeaders;


};

#endif // QJSONMODEL_H
