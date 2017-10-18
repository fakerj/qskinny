/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "QskBoxClipNode.h"
#include "QskBoxRenderer.h"
#include "QskBoxShapeMetrics.h"
#include "QskBoxBorderMetrics.h"
#include "QskFunctions.h"

static inline uint qskMetricsHash( const QskBoxShapeMetrics& shape,
    const QskBoxBorderMetrics& border )
{
    uint hash = 13000;

    hash = shape.hash( hash );
    return border.hash( hash );
}

QskBoxClipNode::QskBoxClipNode():
    m_hash( 0 ),
    m_geometry( QSGGeometry::defaultAttributes_Point2D(), 0 )
{
    setGeometry( &m_geometry );
}

QskBoxClipNode::~QskBoxClipNode()
{
}

void QskBoxClipNode::setBox( const QRectF& rect,
    const QskBoxShapeMetrics& shape, const QskBoxBorderMetrics& border )
{
    const uint hash = qskMetricsHash( shape, border );
    if ( hash == m_hash && rect == m_rect )
        return;

    m_rect = rect;
    m_hash = hash;

    if ( shape.isRectangle() )
    {
        if ( m_geometry.vertexCount() > 0 )
            m_geometry.allocate( 0 );

        setIsRectangular( true );
        setClipRect( qskValidOrEmptyInnerRect( rect, border.widths() ) );
    }
    else
    {
        setIsRectangular( false );
        QskBoxRenderer().renderFill( rect, shape, border, m_geometry );
    }

    markDirty( QSGNode::DirtyGeometry );
}