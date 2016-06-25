/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2015 AscEmu Team <http://www.ascemu.org/>
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
 * Copyright (C) 2005-2007 Ascent Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"


#define SWIMMING_TOLERANCE_LEVEL -0.08f
#define MOVEMENT_PACKET_TIME_DELAY 500

#ifdef WIN32

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#define DELTA_EPOCH_IN_USEC 11644473600000000ULL

uint32 TimeStamp()
{
    //return timeGetTime();

    FILETIME ft;
    uint64 t;
    GetSystemTimeAsFileTime(&ft);

    t = (uint64)ft.dwHighDateTime << 32;
    t |= ft.dwLowDateTime;
    t /= 10;
    t -= DELTA_EPOCH_IN_USEC;

    return uint32(((t / 1000000L) * 1000) + ((t % 1000000L) / 1000));
}

uint32 mTimeStamp()
{
    return timeGetTime();
}

#else

uint32 TimeStamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

uint32 mTimeStamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

#endif

void WorldSession::HandleMoveWorldportAckOpcode(WorldPacket& recv_data)
{
    GetPlayer()->SetPlayerStatus(NONE);
    if (_player->IsInWorld())
    {
        // get outta here
        return;
    }
    LOG_DEBUG("WORLD: got MSG_MOVE_WORLDPORT_ACK.");

    if (_player->m_CurrentTransporter && _player->GetMapId() != _player->m_CurrentTransporter->GetMapId())
    {
        /* wow, our pc must really suck. */
        Transporter* pTrans = _player->m_CurrentTransporter;

        float c_tposx = pTrans->GetPositionX() + _player->transporter_info.x;
        float c_tposy = pTrans->GetPositionY() + _player->transporter_info.y;
        float c_tposz = pTrans->GetPositionZ() + _player->transporter_info.z;


        _player->SetMapId(pTrans->GetMapId());
        _player->SetPosition(c_tposx, c_tposy, c_tposz, _player->GetOrientation());

        WorldPacket dataw(SMSG_NEW_WORLD, 20);

        dataw << pTrans->GetMapId();
        dataw << c_tposx;
        dataw << c_tposy;
        dataw << c_tposz;
        dataw << _player->GetOrientation();

        SendPacket(&dataw);
    }
    else
    {
        _player->m_TeleportState = 2;
        _player->AddToWorld();
    }
}

void WorldSession::HandleMoveTeleportAckOpcode(WorldPacket& recv_data)
{
    uint32 flags, time;
    recv_data >> flags >> time;

    // 15595
    uint8 playerGuid[8];

    playerGuid[5] = recv_data.readBit();
    playerGuid[0] = recv_data.readBit();
    playerGuid[1] = recv_data.readBit();
    playerGuid[6] = recv_data.readBit();
    playerGuid[3] = recv_data.readBit();
    playerGuid[7] = recv_data.readBit();
    playerGuid[2] = recv_data.readBit();
    playerGuid[4] = recv_data.readBit();

    recv_data.ReadByteSeq(playerGuid[4]);
    recv_data.ReadByteSeq(playerGuid[3]);
    recv_data.ReadByteSeq(playerGuid[7]);
    recv_data.ReadByteSeq(playerGuid[6]);
    recv_data.ReadByteSeq(playerGuid[5]);
    recv_data.ReadByteSeq(playerGuid[1]);
    recv_data.ReadByteSeq(playerGuid[3]);
    recv_data.ReadByteSeq(playerGuid[0]);

    WoWGuid guid = *(uint64*)playerGuid;
    if (m_MoverWoWGuid.GetOldGuid() == _player->GetGUID())
    {
        if (sWorld.antihack_teleport && !(HasGMPermissions() && sWorld.no_antihack_on_gm) && _player->GetPlayerStatus() != TRANSFER_PENDING)
        {
            /* we're obviously cheating */
            sCheatLog.writefromsession(this, "Used teleport hack, disconnecting.");
            Disconnect();
            return;
        }

        if (sWorld.antihack_teleport && !(HasGMPermissions() && sWorld.no_antihack_on_gm) && _player->m_position.Distance2DSq(_player->m_sentTeleportPosition) > 625.0f)	/* 25.0f*25.0f */
        {
            /* cheating.... :(*/
            sCheatLog.writefromsession(this, "Used teleport hack {2}, disconnecting.");
            Disconnect();
            return;
        }

        LOG_DEBUG("WORLD: got MSG_MOVE_TELEPORT_ACK.");
        GetPlayer()->SetPlayerStatus(NONE);
        _player->SpeedCheatReset();

        std::list<Pet*> summons = _player->GetSummons();
        for (std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
        {
            // move pet too
            (*itr)->SetPosition((GetPlayer()->GetPositionX() + 2), (GetPlayer()->GetPositionY() + 2), GetPlayer()->GetPositionZ(), M_PI_FLOAT);
        }
        if (_player->m_sentTeleportPosition.x != 999999.0f)
        {
            _player->m_position = _player->m_sentTeleportPosition;
            _player->m_sentTeleportPosition.ChangeCoords(999999.0f, 999999.0f, 999999.0f);
        }
    }

}

struct MovementFlagName
{
    uint32 flag;
    const char* name;
};

static MovementFlagName MoveFlagsToNames[] =
{
    { MOVEMENTFLAG_NONE, "MOVEMENTFLAG_NONE" },
    { MOVEMENTFLAG_FORWARD, "MOVEMENTFLAG_FORWARD" },
    { MOVEMENTFLAG_BACKWARD, "MOVEMENTFLAG_BACKWARD" },
    { MOVEMENTFLAG_STRAFE_LEFT, "MOVEMENTFLAG_STRAFE_LEFT" },
    { MOVEMENTFLAG_STRAFE_RIGHT, "MOVEMENTFLAG_STRAFE_RIGHT" },
    { MOVEMENTFLAG_LEFT, "MOVEMENTFLAG_LEFT" },
    { MOVEMENTFLAG_RIGHT, "MOVEMENTFLAG_RIGHT" },
    { MOVEMENTFLAG_PITCH_UP, "MOVEMENTFLAG_PITCH_UP" },
    { MOVEMENTFLAG_PITCH_DOWN, "MOVEMENTFLAG_PITCH_DOWN" },
    { MOVEMENTFLAG_WALKING, "MOVEMENTFLAG_WALKING" },
    { MOVEMENTFLAG_DISABLE_GRAVITY, "MOVEMENTFLAG_DISABLE_GRAVITY" },
    { MOVEMENTFLAG_ROOT, "MOVEMENTFLAG_ROOT" },
    { MOVEMENTFLAG_FALLING, "MOVEMENTFLAG_FALLING" },
    { MOVEMENTFLAG_FALLING_FAR, "MOVEMENTFLAG_FALLING_FAR" },
    { MOVEMENTFLAG_PENDING_STOP, "MOVEMENTFLAG_PENDING_STOP" },
    { MOVEMENTFLAG_PENDING_STRAFE_STOP, "MOVEMENTFLAG_PENDING_STRAFE_STOP" },
    { MOVEMENTFLAG_PENDING_FORWARD, "MOVEMENTFLAG_PENDING_FORWARD" },
    { MOVEMENTFLAG_PENDING_BACKWARD, "MOVEMENTFLAG_PENDING_BACKWARD" },
    { MOVEMENTFLAG_PENDING_STRAFE_LEFT, "MOVEMENTFLAG_PENDING_STRAFE_LEFT" },
    { MOVEMENTFLAG_PENDING_STRAFE_RIGHT, "MOVEMENTFLAG_PENDING_STRAFE_RIGHT" },
    { MOVEMENTFLAG_PENDING_ROOT, "MOVEMENTFLAG_PENDING_ROOT" },
    { MOVEMENTFLAG_SWIMMING, "MOVEMENTFLAG_SWIMMING" },
    { MOVEMENTFLAG_ASCENDING, "MOVEMENTFLAG_ASCENDING" },
    { MOVEMENTFLAG_DESCENDING, "MOVEMENTFLAG_DESCENDING" },
    { MOVEMENTFLAG_CAN_FLY, "MOVEMENTFLAG_CAN_FLY" },
    { MOVEMENTFLAG_FLYING, "MOVEMENTFLAG_FLYING" },
    { MOVEMENTFLAG_SPLINE_ELEVATION, "MOVEMENTFLAG_SPLINE_ELEVATION" },
    { MOVEMENTFLAG_WATERWALKING, "MOVEMENTFLAG_WATERWALKING" },
    { MOVEMENTFLAG_FALLING_SLOW, "MOVEMENTFLAG_FALLING_SLOW" },
    { MOVEMENTFLAG_HOVER, "MOVEMENTFLAG_HOVER" },
    { MOVEMENTFLAG_NO_COLLISION, "MOVEMENTFLAG_NO_COLLISION" },
};

static const uint32 nmovementflags = sizeof(MoveFlagsToNames) / sizeof(MovementFlagName);
void WorldSession::HandleMovementOpcodes(WorldPacket& recv_data)
{
    CHECK_INWORLD_RETURN

    uint16 opcode = recv_data.GetOpcode();
    Unit* mover = _player;

    if (m_MoverWoWGuid != mover->GetGUID())                    // there must always be a mover
        return;

    if (_player->GetCharmedByGUID() || !_player->IsInWorld() || _player->GetPlayerStatus() == TRANSFER_PENDING || _player->GetTaxiState())
    {
        return;
    }

    /************************************************************************/
    /* Clear standing state to stand.				                        */
    /************************************************************************/
    if (recv_data.GetOpcode() == CMSG_MOVE_START_FORWARD)
        _player->SetStandState(STANDSTATE_STAND);

    /* extract packet */
    MovementInfo movementInfo;
    recv_data >> movementInfo;

    /************************************************************************/
    /* Anti-Teleport                                                        */
    /************************************************************************/

    if (!(HasGMPermissions() && sWorld.no_antihack_on_gm) && !_player->GetCharmedUnitGUID())
    {
        if (sWorld.antihack_teleport && _player->m_position.Distance2DSq(movement_info.GetPos()->x, movement_info.GetPos()->y) > 3025.0f
            && _player->m_runSpeed < 50.0f && !_player->transporter_info.guid)
        {
            sCheatLog.writefromsession(this, "Disconnected for teleport hacking. Player speed: %f, Distance traveled: %f", _player->m_runSpeed, sqrt(_player->m_position.Distance2DSq(movement_info.GetPos()->x, movement_info.GetPos()->y)));
            Disconnect();
            return;
        }
    }

    /************************************************************************/
    /* Remove Emote State                                                   */
    /************************************************************************/
    if (_player->GetEmoteState())
        _player->SetEmoteState(0);

    /************************************************************************/
    /* Make sure the co-ordinates are valid.                                */
    /************************************************************************/
    if (!((movement_info.GetPos()->y >= _minY) && (movement_info.GetPos()->y <= _maxY)) || !((movement_info.GetPos()->x >= _minX) && (movement_info.GetPos()->x <= _maxX)))
    {
        Disconnect();
        return;
    }

    //update the detector
    if (sWorld.antihack_speed && !_player->GetTaxiState() && _player->transporter_info.guid == 0 && !_player->GetSession()->GetPermissionCount())
    {
        // simplified: just take the fastest speed. less chance of fuckups too
        float speed = (_player->flying_aura) ? _player->m_flySpeed : (_player->m_swimSpeed > _player->m_runSpeed) ? _player->m_swimSpeed : _player->m_runSpeed;

        _player->SDetector->AddSample(movement_info.GetPos()->x, movement_info.GetPos()->y, getMSTime(), speed);

        if (_player->SDetector->IsCheatDetected())
            _player->SDetector->ReportCheater(_player);
    }

    /************************************************************************/
    /* Jumping Cheks                                                        */
    /************************************************************************/
    _player->IsPlayerJumping(movementInfo, opcode);

    /************************************************************************/
    /* Fall damage generation                                               */
    /************************************************************************/
    if (opcode == CMSG_MOVE_FALL_LAND && _player )
        _player->HandleFall(movementInfo);
    else
    {
        //whilst player is not falling, continuously update Z axis position.
        //once player lands this will be used to determine how far he fell.
        if (!(movement_info.GetMovementFlags() & MOVEMENTFLAG_FALLING))
            mover->z_axisposition = movement_info.GetPos()->z;
    }

    /************************************************************************/
    /* Breathing                                                            */
    /************************************************************************/
    _player->HandleBreathing(movementInfo, this);

    /************************************************************************/
    /* Transporters                                                         */
    /************************************************************************/


    // Not finished i will rewrit it with new system from Master branch
    if ((mover->transporter_info.guid != 0) && (movement_info.GetTransportGuid().GetOldGuid() == 0))
    {
        /* we left the transporter we were on */

        Transporter *transporter = objmgr.GetTransporter(Arcemu::Util::GUID_LOPART(mover->transporter_info.guid));
        if (transporter != NULL)
            transporter->RemovePassenger(mover);

        mover->transporter_info.guid = 0;
        _player->SpeedCheatReset();

    }
    else
    {
        if (movement_info.GetTransportGuid().GetOldGuid() != 0)
        {

            if (mover->transporter_info.guid == 0)
            {
                //Transporter *transporter = objmgr.GetTransporter(Arcemu::Util::GUID_LOPART(movement_info.GetTransportGuid()));
                //if (transporter != NULL)
                //    transporter->AddPassenger(mover);

                /* set variables */
                //mover->transporter_info.guid = movement_info.GetTransportGuid();
                //mover->transporter_info.flags = movement_info.GetTransportTime();// Maybe TransportTime gets rewrited 
                //mover->transporter_info.x = movement_info.GetTransportPos()->x;
                //mover->transporter_info.y = movement_info.GetTransportPos()->y;
                //mover->transporter_info.z = movement_info.GetTransportPos()->z;

            }
            else
            {
                /* no changes */
                //mover->transporter_info.flags = movement_info.GetTransportTime(); // Maybe TransportTime gets rewrited 
                //mover->transporter_info.x = movement_info.GetTransportPos()->x;
                //mover->transporter_info.y = movement_info.GetTransportPos()->y;
                //mover->transporter_info.z = movement_info.GetTransportPos()->z;
            }
        }
    }

    /************************************************************************/
    /* Update our Positiin                                                  */
    /************************************************************************/
    if (m_MoverWoWGuid.GetOldGuid() == _player->GetGUID())
    {
        _player->SetPosition(movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z, movementInfo.GetPos()->o);
    }

    WorldPacket data(SMSG_PLAYER_MOVE, recv_data.size());
    data << movementInfo;
    mover->SendMessageToSet(&data, _player);
}

void WorldSession::HandleMoveTimeSkippedOpcode(WorldPacket& recv_data)
{}

void WorldSession::HandleMoveNotActiveMoverOpcode(WorldPacket& recv_data)
{
    CHECK_INWORLD_RETURN

    WoWGuid guid;
    recv_data >> guid;

    if (guid == m_MoverWoWGuid)
        return;

    //movement_info.init(recv_data);

    if ((guid != uint64(0)) && (guid == _player->GetCharmedUnitGUID()))
        m_MoverWoWGuid = guid;
    else
        m_MoverWoWGuid.Init(_player->GetGUID());

    // set up to the movement packet
    movement_packet[0] = m_MoverWoWGuid.GetNewGuidMask();
    memcpy(&movement_packet[1], m_MoverWoWGuid.GetNewGuid(), m_MoverWoWGuid.GetNewGuidLen());
}


void WorldSession::HandleSetActiveMoverOpcode(WorldPacket& recv_data)
{
    CHECK_INWORLD_RETURN

    // 15595
    uint8 playerGuid[8];

    playerGuid[7] = recv_data.readBit();
    playerGuid[2] = recv_data.readBit();
    playerGuid[1] = recv_data.readBit();
    playerGuid[0] = recv_data.readBit();
    playerGuid[4] = recv_data.readBit();
    playerGuid[5] = recv_data.readBit();
    playerGuid[6] = recv_data.readBit();
    playerGuid[3] = recv_data.readBit();

    recv_data.ReadByteSeq(playerGuid[3]);
    recv_data.ReadByteSeq(playerGuid[2]);
    recv_data.ReadByteSeq(playerGuid[4]);
    recv_data.ReadByteSeq(playerGuid[0]);
    recv_data.ReadByteSeq(playerGuid[5]);
    recv_data.ReadByteSeq(playerGuid[1]);
    recv_data.ReadByteSeq(playerGuid[6]);
    recv_data.ReadByteSeq(playerGuid[7]);

    uint64 guid = *(uint64*)playerGuid;

    if (guid != m_MoverWoWGuid.GetOldGuid())
    {
        // make sure the guid is valid and we aren't cheating
        if (!(_player->m_CurrentCharm == guid) && !(_player->GetGUID() == guid))
        {
            if (_player->GetCurrentVehicle()->GetOwner()->GetGUID() != guid)
                return;
        }

        // generate wowguid
        if (guid != 0)
            m_MoverWoWGuid.Init(guid);
        else
            m_MoverWoWGuid.Init(_player->GetGUID());

        // set up to the movement packet
        movement_packet[0] = m_MoverWoWGuid.GetNewGuidMask();
        memcpy(&movement_packet[1], m_MoverWoWGuid.GetNewGuid(), m_MoverWoWGuid.GetNewGuidLen());
    }
}

void WorldSession::HandleMoveSplineCompleteOpcode(WorldPacket& recvPacket)
{}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& recvdata)
{
    CHECK_INWORLD_RETURN

        WorldPacket data(SMSG_MOUNTSPECIAL_ANIM, 8);
    data << _player->GetGUID();
    _player->SendMessageToSet(&data, true);
}

void WorldSession::HandleWorldportOpcode(WorldPacket& recv_data)
{
    CHECK_INWORLD_RETURN

    uint32 unk;
    uint32 mapid;
    float x, y, z, o;
    recv_data >> unk >> mapid >> x >> y >> z >> o;

    //printf("\nTEST: %u %f %f %f %f", mapid, x, y, z, o);

    if (!HasGMPermissions())
    {
        SendNotification("You do not have permission to use this function.");
        return;
    }

    LocationVector vec(x, y, z, o);
    _player->SafeTeleport(mapid, 0, vec);
}

void WorldSession::HandleTeleportToUnitOpcode(WorldPacket& recv_data)
{
    CHECK_INWORLD_RETURN

    uint8 unk;
    Unit* target;
    recv_data >> unk;

    if (!HasGMPermissions())
    {
        SendNotification("You do not have permission to use this function.");
        return;
    }

    if ((target = _player->GetMapMgr()->GetUnit(_player->GetSelection())) == NULL)
        return;

    _player->SafeTeleport(_player->GetMapId(), _player->GetInstanceID(), target->GetPosition());
}

void WorldSession::HandleTeleportCheatOpcode(WorldPacket& recv_data)
{

    CHECK_INWORLD_RETURN

        float x, y, z, o;
    LocationVector vec;

    if (!HasGMPermissions())
    {
        SendNotification("You do not have permission to use this function.");
        return;
    }

    recv_data >> x >> y >> z >> o;
    vec.ChangeCoords(x, y, z, o);
    _player->SafeTeleport(_player->GetMapId(), _player->GetInstanceID(), vec);
}