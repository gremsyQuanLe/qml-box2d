/*
 * box2dbody.h
 * Copyright (c) 2010-2011 Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright (c) 2011 Daker Fernandes Pinheiro <daker.pinheiro@openbossa.org>
 * Copyright (c) 2011 Tan Miaoqing <miaoqing.tan@nokia.com>
 * Copyright (c) 2011 Antonio Aloisio <antonio.aloisio@nokia.com>
 * Copyright (c) 2011 Joonas Erkinheimo <joonas.erkinheimo@nokia.com>
 * Copyright (c) 2011 Antti Krats <antti.krats@digia.com>
 *
 * This file is part of the Box2D QML plugin.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef BOX2DBODY_H
#define BOX2DBODY_H

#include <QQuickItem>
#include <Box2D.h>

class Box2DFixture;
class Box2DWorld;

class b2Body;
class b2World;

/**
 * The Box2D body, build up from a list of shapes.
 */
class Box2DBody : public QQuickItem
{
    Q_OBJECT

    Q_ENUMS(BodyType)
    Q_PROPERTY(float linearDamping READ linearDamping WRITE setLinearDamping NOTIFY linearDampingChanged)
    Q_PROPERTY(float angularDamping READ angularDamping WRITE setAngularDamping NOTIFY angularDampingChanged)
    Q_PROPERTY(BodyType bodyType READ bodyType WRITE setBodyType NOTIFY bodyTypeChanged)
    Q_PROPERTY(bool bullet READ isBullet WRITE setBullet NOTIFY bulletChanged)
    Q_PROPERTY(bool sleepingAllowed READ sleepingAllowed WRITE setSleepingAllowed NOTIFY sleepingAllowedChanged)
    Q_PROPERTY(bool fixedRotation READ fixedRotation WRITE setFixedRotation NOTIFY fixedRotationChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive)
    Q_PROPERTY(bool awake READ isAwake WRITE setAwake)
    Q_PROPERTY(QPointF linearVelocity READ linearVelocity WRITE setLinearVelocity NOTIFY linearVelocityChanged)
    Q_PROPERTY(float angularVelocity READ angularVelocity WRITE setAngularVelocity NOTIFY angularVelocityChanged)
    Q_PROPERTY(QQmlListProperty<Box2DFixture> fixtures READ fixtures)
    Q_PROPERTY(float gravityScale READ gravityScale WRITE setGravityScale NOTIFY gravityScaleChanged)

public:
    enum BodyType {
        Static = 0,
        Kinematic,
        Dynamic
    };

    explicit Box2DBody(QQuickItem *parent = 0);
    ~Box2DBody();

    float linearDamping() const;
    void setLinearDamping(float linearDamping);

    float angularDamping() const;
    void setAngularDamping(float angularDamping);

    BodyType bodyType() const;
    void setBodyType(BodyType bodyType);

    bool isBullet() const;
    void setBullet(bool bullet);

    bool sleepingAllowed() const;
    void setSleepingAllowed(bool sleepingAllowed);

    bool fixedRotation() const;
    void setFixedRotation(bool fixedRotation);

    bool isActive() const;
    void setActive(bool active);

    bool isAwake() const;
    void setAwake(bool awake);

    QPointF linearVelocity() const;
    void setLinearVelocity(const QPointF &velocity);

    float angularVelocity() const;
    void setAngularVelocity(float velocity);

    float gravityScale() const;
    void setGravityScale(float gravityScale);

    QQmlListProperty<Box2DFixture> fixtures();

    void initialize(b2World *world);
    void synchronize();
    void cleanup(b2World *world);

    Q_INVOKABLE void applyForce(const QPointF &force,const QPointF &point);
    Q_INVOKABLE void applyTorque(qreal torque);
    Q_INVOKABLE void applyLinearImpulse(const QPointF &impulse, const QPointF &point);
    Q_INVOKABLE QPointF getWorldCenter() const;
    Q_INVOKABLE float getMass() const;
    Q_INVOKABLE void resetMassData();
    Q_INVOKABLE float getInertia() const;
    Q_INVOKABLE QPointF getLinearVelocityFromWorldPoint(const QPointF &point) const;
    Q_INVOKABLE QPointF getLinearVelocityFromLocalPoint(const QPointF &point) const;

    void componentComplete();
    b2Body *body() const;
    b2World *world() const;

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

signals:
    void linearDampingChanged();
    void angularDampingChanged();
    void bodyTypeChanged();
    void bulletChanged();
    void sleepingAllowedChanged();
    void fixedRotationChanged();
    void linearVelocityChanged();
    void angularVelocityChanged();
    void bodyCreated();
    void gravityScaleChanged();
    void positionChanged(const QPointF &position);

private slots:
    void onRotationChanged();

private:
    b2Body *mBody;
    b2World *mWorld;
    b2BodyDef mBodyDef;
    bool mSynchronizing;
    bool mInitializePending;
    QList<Box2DFixture*> mFixtures;

    static void append_fixture(QQmlListProperty<Box2DFixture> *list,
                               Box2DFixture *fixture);
    static int count_fixture(QQmlListProperty<Box2DFixture> *list);
    static Box2DFixture *at_fixture(QQmlListProperty<Box2DFixture> *list, int index);
};

inline float Box2DBody::linearDamping() const
{
    return mBodyDef.linearDamping;
}

inline float Box2DBody::angularDamping() const
{
    return mBodyDef.angularDamping;
}

inline Box2DBody::BodyType Box2DBody::bodyType() const
{
    return static_cast<Box2DBody::BodyType>(mBodyDef.type);
}

inline bool Box2DBody::isBullet() const
{
    return mBodyDef.bullet;
}

inline bool Box2DBody::sleepingAllowed() const
{
    return mBodyDef.allowSleep;
}

inline bool Box2DBody::fixedRotation() const
{
    return mBodyDef.fixedRotation;
}

inline bool Box2DBody::isActive() const
{
    return mBodyDef.active;
}

inline float Box2DBody::gravityScale() const
{
    return mBodyDef.gravityScale;
}

#endif // BOX2DBODY_H
