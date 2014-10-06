#ifndef NETHOST_H
#define NETHOST_H

#include "EnginePrerequisites.h"

#include "core/logmanager.h"
#include <enet/enet.h>
#include <list>
#include <map>
#include <cstdio>

#include "net/netobject.h"
#include "game/gameobject.h"
#include "demos/demo.h"
#include "demos/demonetserver.h"

namespace Caelum {

enum Net_Channel_Type {
    NET_CHANNEL_RELIABLE = 0,
    NET_CHANNEL_UNRELIABLE = 1
};

class NetHost {
  public:
    NetHost(int port) {
        if (port != 0) {
            mAddress.host = ENET_HOST_ANY;
            mAddress.port = enet_uint16(port);
            mHost = enet_host_create(&mAddress, 10, 2, 0, 0);
        } else {
            mHost = enet_host_create(NULL, 1, 2, 0, 0);
        }
        if (!mHost) {
            log("Error creating host");
            exit(1);
        }
        mCommand = new NetCommand();
        mTransform = new NetTransform();
    }
    virtual ~NetHost() {
        delete mTransform;
        delete mCommand;
        enet_host_destroy(mHost);
    }

    // Server functions
    void update() {
        while (enet_host_service (mHost, &mEvent, 0) > 0) {
            switch (mEvent.type) {
            case ENET_EVENT_TYPE_CONNECT:
                /*log(String("A new client connected from ")
                    +StringUtils::to_string(int(mEvent.peer -> address.host))+":"+
                    +StringUtils::to_string(int(mEvent.peer -> address.port)));*/
                /* Store any relevant client information here. */
                printf ("A new client connected from %x:%u.\n",
                                mEvent.peer -> address.host,
                                mEvent.peer -> address.port);
                //mEvent.peer->data = "Client information";
                handleConnectedServer();
                break;
            case ENET_EVENT_TYPE_RECEIVE: {
                /*log(String("A packet of length ")
                    +StringUtils::to_string(int(mEvent.packet -> dataLength))
                    +" containing "+String(mEvent.packet->data)
                    +" was received from "+String(mEvent.peer->data)
                    +" on channel"+StringUtils::to_string(int(mEvent.channelID))
                    +"\n");*/
                printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                                mEvent.packet -> dataLength,
                                mEvent.packet -> data,
                                mEvent.peer -> data,
                                mEvent.channelID);
                String data = reinterpret_cast<char*>(mEvent.packet->data);
                if (mEvent.channelID == enet_uint8(NET_CHANNEL_RELIABLE))
                    mCommand->loadBuffer(data);
                else
                    mTransform->loadBuffer(data);
                handlePacket();
                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy (mEvent.packet);
            }
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf ("%s disconected.\n", mEvent.peer->data);
                /* Reset the peer's client information. */
                handleDisconnectedServer();
                mEvent.peer->data = NULL;
            }
        }
    }

    virtual void handleConnectedServer() {}
    virtual void handleConnectedClient() {}
    virtual void handlePacket() = 0;
    virtual void handleDisconnectedServer() {}
    virtual void handleDisconnectedClient() {}

    void send(NetObject *obj, ENetPeer *p, Net_Channel_Type type) {
        ENetPacket* packet = enet_packet_create(obj->toBuffer().c_str(),
                                                obj->calculateSize(),
                                                (type == NET_CHANNEL_RELIABLE)?ENET_PACKET_FLAG_RELIABLE:0);
        if (!p) {
            enet_peer_send(p, enet_uint8(type), packet);
        } else {
            enet_host_broadcast(mHost, enet_uint8(type), packet);
        }
        enet_host_flush(mHost);
    }

    void connect(const String &ip, int port) {
        ENetAddress address;
        ENetPeer *peer;

        enet_address_set_host(&address, ip.c_str());
        address.port = enet_uint16(port);
        //address.port = 1234;
        peer = enet_host_connect(mHost, &address, 2, 0);
        if (!peer) {
            log("Error connecting to peer.");
            exit(1);
        }
        /* Wait up to 5 seconds for the connection attempt to succeed. */
        if (enet_host_service (mHost, &mEvent, 5000) > 0 &&
            mEvent.type == ENET_EVENT_TYPE_CONNECT) {
            log("Connection succeeded.");
            handleConnectedClient();
        } else {
            log("Connection failed.");
        }
        mPeers.push_back(peer);
    }

    void disconnect(ENetPeer *p) {
        bool disconnected = false;
        enet_peer_disconnect(p, 0);
        // Gentle disconnection
        while (enet_host_service(mHost, &mEvent, 3000) > 0) {
            switch (mEvent.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(mEvent.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                log("Disconnection succeeded.");
                handleDisconnectedClient();
                disconnected = true;
                break;
            default:
                break;
            }
        }
        // Force disconnection
        if (!disconnected) {
            enet_peer_reset(p);
        }
        mPeers.remove(p);
    }

    void log(const String& msg) {
        Caelum::LogManager::getSingleton().logMessage(msg);
    }

  protected:
    ENetEvent mEvent;
    ENetAddress mAddress;
    ENetHost *mHost;
    std::list<ENetPeer*> mPeers;
    NetCommand *mCommand;
    NetTransform *mTransform;
};

//---------------------------------------------------
// -------NET SERVER-----------------
//---------------------------------------------------

class NetServer
        : public NetHost {
  public:
    NetServer(DemoNetServer *demo, int port) : NetHost(port) {mDemo = demo;}
    virtual ~NetServer() {}

    void registerOgre(GameObject *obj) {
        mObjects[0] = obj;
        mNetCount++;
    }

    void upload() {
        GameObject *obj;
        for (std::map<int, GameObject*>::iterator it = mObjects.begin();
             it != mObjects.end();
             ++it) {
            obj = it->second;
            log("LLEGO HASTA AQUI");
            mTransform->setProperties(it->first, obj->getDerivedPosition(), obj->getDerivedOrientation());
            for (std::list<ENetPeer*>::iterator it = mPeers.begin();
                 it != mPeers.end();
                 ++it) {
                this->send(mTransform, 0, NET_CHANNEL_UNRELIABLE);
            }
            log("YA NO LLEGO MÁS");
        }
    }

    void handleConnectedServer() {
        int id;
        for (std::map<int,GameObject*>::iterator it = mObjects.begin();
             it != mObjects.end();
             ++it) {
            id  = it->first;
            mCommand->clear();
            mCommand->addParam("CREATE");
            mCommand->addParam(id);
            mCommand->addParam((id>0)?"BARREL":"");
            log("Mandando crear nuevo contenido");
            log(mCommand->toBuffer().c_str());
            this->send(mCommand, mEvent.peer, NET_CHANNEL_RELIABLE);
        }
    }
    void handlePacket() {
        if (mCommand->isCommand("LAUNCH")) {
            int id = mNetCount++;
            String name = StringUtils::to_string(id);
            GameObject *obj = mDemo->mScene->createGameObject(name);
            Entity *ent = mDemo->mRenderLayer->createEntity(name, "Barrel.mesh");
            RigidBody *rb = mDemo->mPhysicsLayer->createRigidBody(name, ent, Vector3::UNIT_SCALE, 1, PhysicsLayer::PHY_SHAPE_CONVEX);
            obj->attachComponent(ent);
            obj->attachComponent(rb);
            mObjects[id] = obj;

            mCommand->clear();
            mCommand->addParam("CREATE");
            mCommand->addParam(id);
            mCommand->addParam("BARREL");
            this->send(mCommand, 0, NET_CHANNEL_RELIABLE);
        }
    }
    void handleDisconnectedServer() {
    }

    std::map<int,Caelum::GameObject*> mObjects;
    DemoNetServer *mDemo;
    static int mNetCount;
};

//---------------------------------------------------
// ---------NET CLIENT ------------------------------
//---------------------------------------------------

class NetClient
        : public NetHost {
  public:
    NetClient(Demo *demo) : NetHost(0) { mDemo = demo;}
    virtual ~NetClient() {mObjects.clear();}

    void launch(const Vector3 &pos, const Vector3 &dir) {
        mCommand->clear();
        mCommand->addParam("LAUNCH");
        mCommand->addParam(pos.x);
        mCommand->addParam(pos.y);
        mCommand->addParam(pos.z);
        mCommand->addParam(dir.x);
        mCommand->addParam(dir.y);
        mCommand->addParam(dir.z);
        if (mPeers.size() > 0)
            this->send(mCommand, *(mPeers.begin()), NET_CHANNEL_RELIABLE);
    }

    void addObject(int id, GameObject *obj) {
        mObjects[id] = obj;
    }

    void handleConnectedClient() {}
    void handlePacket() {
        if (mEvent.channelID == enet_uint8(NET_CHANNEL_RELIABLE)) {
            if (mCommand->isCommand("CREATE")) {
                // create and add object
                String name = mCommand->getParam(1);
                GameObject *obj = mDemo->mScene->createGameObject(name);
                Entity *ent; RigidBody *rb;
                if (mCommand->isParam(2, "BARREL")) {
                    ent = mDemo->mRenderLayer->createEntity(name, "Barrel.mesh");
                    rb = mDemo->mPhysicsLayer->createRigidBody(name, ent, Vector3::UNIT_SCALE, 1, PhysicsLayer::PHY_SHAPE_CONVEX);
                } else {
                    ent = mDemo->mRenderLayer->createEntity(name, "Sinbad.mesh");
                    ent->getSkeleton()->setSkeletonBlending(ANIMBLEND_CUMULATIVE);
                    rb = mDemo->mPhysicsLayer->createRigidBody(name, ent, Vector3::UNIT_SCALE, 5, PhysicsLayer::PHY_SHAPE_SPHERE);
                    rb->setFixedYaw();
                }
                obj->attachComponent(ent);
                obj->attachComponent(rb);
                addObject(mCommand->getIntParam(1), obj);
            }
        } else {
            // Modify object
            GameObject *obj = mObjects[mTransform->mID];
            if (obj != NULL) {
                obj->setDerivedPosition(mTransform->mPos);
                obj->setDerivedOrientation(mTransform->mOri);
                obj->notifyPosOri();
            }
        }
    }
    void handleDisconnectedClient() {}

    std::map<int,Caelum::GameObject*> mObjects;
    Demo *mDemo;
};


}

#endif // NETHOST_H
