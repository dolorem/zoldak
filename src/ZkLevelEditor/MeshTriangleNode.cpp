#include "MeshTriangleNode.h"
#include "MeshLayer.h"

#include <QtCore>
#include <QtGui>
#include <QDebug>

using namespace Zk::LevelEditor;

MeshTriangleNode::MeshTriangleNode(MeshLayer * ml, QGraphicsItem * parent)
	: QGraphicsEllipseItem(parent), color(0, 255, 0)
{
	parentLayer = ml;
	
	setPen(QPen(QBrush(Qt::white), 2.0));
	setBrush(QBrush(color));
	setRect(QRect(-8, -8, 16, 16));
	
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	setZValue(1.0);
}

MeshTriangleNode::~MeshTriangleNode()
{
	
}

void MeshTriangleNode::attachTriangle(MeshTriangle * mt)
{
	linkedTriangles << mt;
}

void MeshTriangleNode::setColor(const QColor & color)
{
	this->color = color;
	setBrush(QBrush(QColor(color)));
	emit colorChanged(this);
}

QColor MeshTriangleNode::getColor() const
{
	return color;
}

void MeshTriangleNode::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsEllipseItem::mousePressEvent(event);
	emit clicked(this);
}

void MeshTriangleNode::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	QMenu menu;
	qDebug() << "DUPA";
	QAction * removeAction = menu.addAction("&Remove");
	menu.exec(event->screenPos());
}

QVariant MeshTriangleNode::itemChange(
	GraphicsItemChange change,
	const QVariant & value
)
{
	if (change == ItemPositionChange)
		emit moved(this, value.toPointF());
	
	return QGraphicsEllipseItem::itemChange(change, value);
}