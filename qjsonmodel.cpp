
#include "qjsonmodel.h"
#include <QFile>
#include <QDebug>
#include <QFont>


QJsonModel::QJsonModel(QObject *parent)
    : QAbstractItemModel(parent)
    , mRootItem{new QJsonTreeItem}
{
    mHeaders.append("Key");
    mHeaders.append("Value");
}

QJsonModel::QJsonModel(const QString& fileName, QObject *parent)
    : QAbstractItemModel(parent)
    , mRootItem{new QJsonTreeItem}
{
    mHeaders.append("key");
    mHeaders.append("value");
    load(fileName);
}

QJsonModel::QJsonModel(QIODevice * device, QObject *parent)
    : QAbstractItemModel(parent)
    , mRootItem{new QJsonTreeItem}
{
    mHeaders.append("key");
    mHeaders.append("value");
    load(device);
}

QJsonModel::QJsonModel(const QByteArray& json, QObject *parent)
    : QAbstractItemModel(parent)
    , mRootItem{new QJsonTreeItem}
{
    mHeaders.append("key");
    mHeaders.append("value");
    loadJson(json);
}

QJsonModel::~QJsonModel()
{
    delete mRootItem;
}

bool QJsonModel::load(const QString &fileName)
{
    QFile file(fileName);
    bool success = false;
    if (file.open(QIODevice::ReadOnly)) {
        success = load(&file);
        file.close();
    }
    else success = false;

    return success;
}

bool QJsonModel::load(QIODevice *device)
{
    return loadJson(device->readAll());
}

bool QJsonModel::loadJson(const QByteArray &json)
{
    auto const& jdoc = QJsonDocument::fromJson(json);

    if (!jdoc.isNull())
    {
        beginResetModel();
        delete mRootItem;
        if (jdoc.isArray()) {
            mRootItem = QJsonTreeItem::load(QJsonValue(jdoc.array()));
            mRootItem->setType(QJsonValue::Array);

        } else {
            mRootItem = QJsonTreeItem::load(QJsonValue(jdoc.object()));
            mRootItem->setType(QJsonValue::Object);
        }
        endResetModel();
        return true;
    }

    qDebug()<<Q_FUNC_INFO<<"cannot load json";
    return false;
}


QVariant QJsonModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())
        return QVariant();


    QJsonTreeItem *item = static_cast<QJsonTreeItem*>(index.internalPointer());


    if (role == Qt::DisplayRole) {

        if (index.column() == 0)
            return QString("%1").arg(item->key());

        if (index.column() == 1)
            return QString("%1").arg(item->value());
    } else if (Qt::EditRole == role) {
        if (index.column() == 1) {
            return QString("%1").arg(item->value());
        }
    }



    return QVariant();

}

bool QJsonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int col = index.column();
    if (Qt::EditRole == role) {
        if (col == 1) {
            QJsonTreeItem *item = static_cast<QJsonTreeItem*>(index.internalPointer());
                item->setValue(value.toString());
                emit dataChanged(index, index, {Qt::EditRole});
                return true;
        }
    }

    return false;
}



QVariant QJsonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {

        return mHeaders.value(section);
    }
    else
        return QVariant();
}

QModelIndex QJsonModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QJsonTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<QJsonTreeItem*>(parent.internalPointer());

    QJsonTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex QJsonModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QJsonTreeItem *childItem = static_cast<QJsonTreeItem*>(index.internalPointer());
    QJsonTreeItem *parentItem = childItem->parent();

    if (parentItem == mRootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int QJsonModel::rowCount(const QModelIndex &parent) const
{
    QJsonTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<QJsonTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int QJsonModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

Qt::ItemFlags QJsonModel::flags(const QModelIndex &index) const
{
    int col   = index.column();
    auto item = static_cast<QJsonTreeItem*>(index.internalPointer());

    auto isArray = QJsonValue::Array == item->type();
    auto isObject = QJsonValue::Object == item->type();

    if ((col == 1) && !(isArray || isObject)) {
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    } else {
        return QAbstractItemModel::flags(index);
    }
}

QJsonDocument QJsonModel::json() const
{

    auto v = genJson(mRootItem);
    QJsonDocument doc;

    if (v.isObject()) {
        doc = QJsonDocument(v.toObject());
    } else {
        doc = QJsonDocument(v.toArray());
    }

    return doc;
}

QJsonValue  QJsonModel::genJson(QJsonTreeItem * item) const
{
    auto type   = item->type();
    int  nchild = item->childCount();

    if (QJsonValue::Object == type) {
        QJsonObject jo;
        for (int i = 0; i < nchild; ++i) {
            auto ch = item->child(i);
            auto key = ch->key();
            jo.insert(key, genJson(ch));
        }
        return  jo;
    } else if (QJsonValue::Array == type) {
        QJsonArray arr;
        for (int i = 0; i < nchild; ++i) {
            auto ch = item->child(i);
            arr.append(genJson(ch));
        }
        return arr;
    } else {
        QJsonValue va(item->value());
        return va;
    }

}
