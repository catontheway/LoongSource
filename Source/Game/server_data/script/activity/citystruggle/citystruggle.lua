FX_Normal_Guild_Mem		=	40			-- ��սʱ��ͨ���ɽ�����������
FX_Hold_Guild_Mem		=	80			-- ��սʱͳ�ΰ��ɽ�����������
--CityStruggle_Accept_Time = {7200}  	    -- ������7200��

-- 1Ϊ��������ռ��״̬��0Ϊ��������ռ��״̬��2Ϊ��С�Ľ�����·��
LongZhu_State = {LongZhuID, GuildID, Belong, ChangeMin, RoleID}	-- ռ������İ���ID�� ������������Ӫ�� ������Ӫת����ʱ�䣨��λ�����ӣ��� ���ڷ�ӡ�����ID
YinYangZhu_State = {}

Door_State = {}
Door_State[1] = {CreatureID, x, y, z, Opened, Change_Min, Repair_Time, HpNode}		-- ����
Door_State[2] = {CreatureID, x, y, z, Opened, Change_Min, Repair_Time, HpNode}		-- ����
Door_State[3] = {CreatureID, x, y, z, Opened, Change_Min, Repair_Time, HpNode}		-- ����

-- Door_State = {Opened, Change_Min, Repair_Time}	-- �ŵĴ�״̬�������Ƶ�ʱ�䣬���޵Ĵ���

Material_State = {}		-- ���ʿ�Ļٻ�״̬�������ٵ�ʱ��

-- ������TypeID
-- 4900030: // ����������
-- 4900043: // ̫��������
-- 4900044: // ���������
-- 4900045: // ���������
-- 4900046: // ����������
YinYangZhuTypeID = {4900030, 4900043, 4900044, 4900045, 4900046}

-- �л��
Atk_ZhaoHunFan = {CreatureID, use, x, y, z}					-- �л���Ƿ���ʹ�ã������
Def_ZhaoHunFan = {CreatureID, use, x, y, z}

-- ��������������Ӫ���	1--��������	0--��������
Role_Alliance = {}

-- ��ս��ʼ������־	0Ϊδ��ʼ����������1Ϊ��ʼ��������
CityStruggle_IsStart = 0
CityStruggle_IsOver = 1

-- �ȫ��ʱ�䣨���ӣ�
CityStruggle_CurMin = 0

-- ��ս�id
CityStruggle_ActID = 0

-- ��Ӫ����ʱ��
Atk_Revive_Time = 0
Def_Revive_Time = 0

-- �ػ���״̬
Eudemon_State = {EudemonTally, NeedPerson, NeedSecond, NowPerson, BeginSummon, Summoned}

-- ��¼��ǰ�����ٻ��˵�RoleID
Eudemon_Stone = {}

-- ��������������
local PlayerNum = {}
PlayerNum[0] = 0
PlayerNum[1] = 0

-- ��ս�жϵ���
function CityStruggle_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	-- ��սδ��ʽ��ʼʱ�޷������κι���
	if CityStruggle_IsStart ~= 1 then
	    if true == unit.IsPlayer(SrcID) and true == unit.IsCreature(TargetID) then
		    local TypeID = unit.GetUnitTypeID(MapID, InstanceID, TargetID)
			local Role_GuildID = guild.GetRoleGuildID(SrcID)
			local Master_GuilID = city.GetCityAppointedAtt(3, 2)
			local Role_ZhiWei = -1
			if Role_GuildID ~= nil and Role_GuildID ~= 4294967295 then
				Role_ZhiWei = guild.GetRoleGuildPos(Role_GuildID, SrcID)
			end
			if Role_ZhiWei ~= -1 and Role_ZhiWei ==2 and TypeID == 1534162 and Role_GuildID == Master_GuilID then
			    return false, true, false, true
			elseif (Role_ZhiWei ~=2 or Role_GuildID ~= Master_GuilID) and TypeID == 1534162 then
                local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 3447)  --ֻ�г����ſ��Թ����ѵ�ħ��
				msg.DispatchRoleMsgEvent(SrcID, MsgID)
				return true, false, false, true
			elseif TypeID == 1534163 or TypeID == 1534164 or TypeID == 1534165 then
			    return false, true, false, true
			elseif TypeID == 4900710 then
			    return false, false, true, false
			end
		elseif true == unit.IsCreature(SrcID) then
		    local TypeID = unit.GetUnitTypeID(MapID, InstanceID, SrcID)
		    if TypeID <= 1534165 and TypeID >= 1534162 then
			    if true == unit.IsCreature(TargetID) then
				    return true, false, false, true
				elseif true == unit.IsPlayer(TargetID) then
				    return false, true, false, true
				end
			end
		elseif true == unit.IsPlayer(SrcID) and true == unit.IsPlayer(TargetID) then
		    return true, false, false, true
		end
		return false, false, false, true
	end

    -- ��Ҽ�ĵ����ж�
    if true == unit.IsPlayer(SrcID) and true == unit.IsPlayer(TargetID) then
        if Role_Alliance[SrcID] ~= Role_Alliance[TargetID] then
            return false, true, false, true
        end
	    return true, false, false, true
    end

    -- �������������ٻ��ޣ����������ʿ⣬�л�ᦵĵ����ж�
    if true == unit.IsPlayer(SrcID) and true == unit.IsCreature(TargetID) then
		-- ����  ������Ϊ����
		local UnitTypeID = unit.GetUnitTypeID(MapID, InstanceID, TargetID)
		if UnitTypeID == 4900029 or UnitTypeID == 4900030 or UnitTypeID == 4900043 or UnitTypeID == 4900044 or UnitTypeID == 4900045 or UnitTypeID == 4900046 then
			return false, true, true, true
		end

        if Role_Alliance[SrcID] == 0	then		-- ��������  ���˴������ж��л�ᦵĵ��ѹ�ϵ��

			if UnitTypeID == 4900027 then			-- ���ʿ�
					return false, true, false, true
			elseif UnitTypeID == 4900028 then		-- �л��
				if TargetID == Atk_ZhaoHunFan.CreatureID then
					return true, false, false, true
				else
					return false, true, false, true
				end
			else
				return false, true, false, true
			end

		else										-- ��������

			if UnitTypeID == 4900027 then			-- ���ʿ�
				return true, false, false, true
			elseif UnitTypeID == 4900028 then		-- �л��
				if TargetID == Def_ZhaoHunFan.CreatureID then
					return true, false, false, true
				else
					return false, true, false, true
				end
			elseif UnitTypeID == 4900049 or UnitTypeID == 4900050 then		-- ʯ��
				return false, false, true, false
			else
				return true, false, false, true
			end

        end

    end

    if true == unit.IsCreature(SrcID) and true == unit.IsPlayer(TargetID) then
		local UnitTypeID = unit.GetUnitTypeID(MapID, InstanceID, SrcID)
		if UnitTypeID == 4900028 then										-- �л�ᦵĹ⻷buff
			if SrcID == Atk_ZhaoHunFan.CreatureID and Role_Alliance[TargetID] == 0 then
				return true, false, false, true
			elseif SrcID == Def_ZhaoHunFan.CreatureID and Role_Alliance[TargetID] == 1 then
				return true, false, false, true
			else
				return false, false, false, false
			end

        elseif Role_Alliance[TargetID] == 0 then
            return false, true, false, true
        end
	    return true, false, false, true
    end


    return false, false, false, false
end

-- �ÿ���Ӹ���
function CityStruggle_OnTimerMin(actID)
	if CityStruggle_IsOver == 1 then
		return
	end

	CityStruggle_CurMin = CityStruggle_CurMin + 1

	-- ��ʼ������һСʱ��û���������仯�����������ʤ��60
	if LongZhu_State.GuildID == -1 and CityStruggle_CurMin >= 60 then
		CityStruggle_IsOver = 1
		CityStruggle_OnOver()
	end

	-- ��ӡ����󱣳�20���ӣ���ս����
	if LongZhu_State.ChangeMin ~= nil and (CityStruggle_CurMin - LongZhu_State.ChangeMin >= 20) then
		CityStruggle_IsOver = 1
		CityStruggle_OnOver()
	end

	-- ÿ15���ӣ���������ʿ��+5
	if math.fmod(CityStruggle_CurMin, 15) == 0 then
		for k, v in pairs(Role_Alliance) do
			if v == 1 then
				unit.ModAttValue(city_id[actID].MapID, -1, k, 63, 5)
				FX_ResetMorale(city_id[actID].MapID, -1, k)
			end
		end
	end

	-- ���ʿⱻ���ٹ�
	local bWuZiKuNumChanged = 0
	for k, v in pairs(Material_State) do
		if Material_State[k].Destroied == 1 then
			--[[if CityStruggle_CurMin - Material_State[k].Change_Min == 5 then
				-- �ָ����ʿⱻ���ƶԽ������˵ĸ���ʱ��ͷ�
				Atk_Revive_Time = Atk_Revive_Time - 20
				city.SetReviveTime(city_id[actID].CityID, 0, Atk_Revive_Time)
			end
			]]

			if CityStruggle_CurMin - Material_State[k].Change_Min == 5 then
				-- ����ˢ�����ʿ�
				map.MapCreateCreature(city_id[actID].MapID, -1, 4900027, Material_State[k].x, Material_State[k].y, Material_State[k].z)

				-- �ָ����ʿⱻ���ƶԽ������˵ĸ���ʱ��ͷ�
				Def_Revive_Time = Def_Revive_Time - 5
				city.SetReviveTime(city_id[actID].CityID, 1, Def_Revive_Time)

				Material_State[k].Destroied = 0

				-- �����㲥
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 1038)			-- �������˵�һ�����ʿ�������ˢ��
				msg.DispatchMapMsgEvent(MsgID, city_id[actID].MapID, -1)

				-- ��ʶ���ʿ���������
				bWuZiKuNumChanged = 1
			end
		end
	end

	-- ͬ�����ʿ�����
	if bWuZiKuNumChanged == 1 then
		local wuzikuNum = 0
		for k, v in pairs(Material_State) do
			if Material_State[k].Destroied ~= 1 then
				wuzikuNum = wuzikuNum + 1
			end
		end
		SycAllRoleOneInfo(city_id[actID].MapID, -1, 5, 0, wuzikuNum)
	end

	local DoorCreatureID = city_door[city_id[actID].DefenceLevel].CreatureID

	-- ���ű����ƹ�
	for k, v in pairs(Door_State) do
		if Door_State[k].Opened == 1 then
			if Door_State[k].Repair_Time < 3 then
				if CityStruggle_CurMin - Door_State[k].Change_Min == 20 then
					Door_State[k].CreatureID = map.MapCreateCreature(city_id[actID].MapID, -1, DoorCreatureID, Door_State[k].x, Door_State[k].y, Door_State[k].z)
					Door_State[k].Repair_Time = Door_State[k].Repair_Time + 1
					Door_State[k].HpNode = math.floor(math.pow(0.5, Door_State[k].Repair_Time) * 9)
					local hp = (1 - math.pow(0.5, Door_State[k].Repair_Time)) * unit.GetAttValue(city_id[actID].MapID, -1, Door_State[k].CreatureID, 8)
					unit.ChangeHP(city_id[actID].MapID, -1, Door_State[k].CreatureID, -hp)
					-- �رճ���
					map.OpenCloseDoorByPos(city_id[actID].MapID, -1, Door_State[k].x, Door_State[k].y, Door_State[k].z, false)

					-- ��Ҫ�㲥
					local id_msg = 1034+k-1
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, id_msg)			-- �ݻ���һ���������˵����ʿ�
					msg.DispatchMapMsgEvent(MsgID, city_id[actID].MapID, -1)

					-- ͬ������״̬����ͼ�������
					SycAllRoleOneInfo(city_id[actID].MapID, -1, 7, k, 0)
				end
			end
		end
	end

	-- ��Ծʱ���1
	for k,v in pairs(Role_Alliance) do
		city.AddRoleData(city_id[actID].MapID, -1, city_id[actID].CityID, k, 0, 1)
	end

	-- ˢ��ս������
	city.SortByScore(city_id[actID].CityID)

end

-- ��ʼ��
function CityStruggle_OnInit(actID)
	--act.InitEventTime("CityStruggle_Accept_Time", 1, actID)
end

-- ��ʱ��
function CityStruggle_OnTimer(actID, Second)
end

-- ���ʼ
function CityStruggle_OnStart(actID)

	CityStruggle_ActID = actID
	CityStruggle_CurMin = 0
	Role_Alliance = {}

	-- ��ս��ʼ
	city.SetCityStruggleState(city_id[actID].CityID, 1)
	CityStruggle_IsStart = 1
	CityStruggle_IsOver = 0

	-- ���������������״̬
	LongZhu_State.GuildID = -1		-- ����ռ�����ID��ʼ��Ϊ-1
	LongZhu_State.Belong = 1		-- �����ʼΪ������������
	LongZhu_State.RoleID = nil		-- �������ڷ�ӡ�����

	-- ��ʼ��������
	YinYangZhu_State[4900030] = 1
	YinYangZhu_State[4900043] = 1
	YinYangZhu_State[4900044] = 1
	YinYangZhu_State[4900045] = 0
	YinYangZhu_State[4900046] = 0

	-- �������ʿ��״̬
	Material_State[1] = {Destroied, Change_Min, x, y, z}
	Material_State[2] = {Destroied, Change_Min, x, y, z}
	Material_State[3] = {Destroied, Change_Min, x, y, z}
	Material_State[4] = {Destroied, Change_Min, x, y, z}

	Material_State[1].Destroied = 0
	Material_State[1].Change_Min = 0
	Material_State[2].Destroied = 0
	Material_State[2].Change_Min = 0
	Material_State[3].Destroied = 0
	Material_State[3].Change_Min = 0
	Material_State[4].Destroied = 0
	Material_State[4].Change_Min = 0

	-- �����л��ʹ��״̬
	Atk_ZhaoHunFan.use = 0
	Atk_ZhaoHunFan.x = 0
	Atk_ZhaoHunFan.y = 0
	Atk_ZhaoHunFan.z = 0
	Def_ZhaoHunFan.use = 0
	Def_ZhaoHunFan.x = 0
	Def_ZhaoHunFan.y = 0
	Def_ZhaoHunFan.z = 0

	-- �����ػ�ʯ״̬
	Eudemon_Stone = {}

	-- ˢ������
	for k, v in pairs(city_guard[city_id[actID].GuardIndex]) do
		map.MapDeleteCreature(city_id[actID].MapID, -1, v.CreatureID)	-- ɾ������
		map.MapCreateCreature(city_id[actID].MapID, -1, 1300948, v.x, v.y, v.z)	-- ˢ������
	end

	-- ������Ӫ����ʱ��
	local AtkYinYangNum = 0
	local DefYinYangNum = 0
	for k, v in pairs(YinYangZhu_State) do
		if v == 0 then
			AtkYinYangNum = AtkYinYangNum + 1
		else
			DefYinYangNum = DefYinYangNum + 1
		end
	end

	Atk_Revive_Time = 25 - AtkYinYangNum * 5
	Def_Revive_Time = 25 - DefYinYangNum * 5
	city.SetReviveTime(city_id[actID].CityID, 0, Atk_Revive_Time)
	city.SetReviveTime(city_id[actID].CityID, 1, Def_Revive_Time)

	-- ����ػ��������Ϣ
	Eudemon_State.EudemonTally = city.GetCityAppointedAtt(city_id[actID].CityID, 4)
	Eudemon_State.Summoned = 0
	Eudemon_State.NeedPerson = math.floor( 10 + (1000 - Eudemon_State.EudemonTally) / 50 )
	Eudemon_State.NeedSecond = math.floor( 20 + (1000 - Eudemon_State.EudemonTally) / 10 )
	Eudemon_State.NowPerson = 0
	Eudemon_State.BeginSummon = 0

	-- ���з���ֵ
	local CityDefence = city.GetCityAppointedAtt(city_id[actID].CityID, 3)
	if CityDefence < 1000 then
		city_id[actID].DefenceLevel = 1
	elseif CityDefence < 3000 then
		city_id[actID].DefenceLevel = 2
	elseif CityDefence < 4000 then
		city_id[actID].DefenceLevel = 3
	elseif CityDefence < 6000 then
		city_id[actID].DefenceLevel = 4
	elseif CityDefence < 7000 then
		city_id[actID].DefenceLevel = 5
	elseif CityDefence < 8000 then
		city_id[actID].DefenceLevel = 6
	elseif CityDefence < 9000 then
		city_id[actID].DefenceLevel = 7
	else
		city_id[actID].DefenceLevel = 8
	end

	-- ���ݳ��з���ֵȷ�����ŵ�ģ��
	local DoorModelID = city_door[city_id[actID].DefenceLevel].ModelID
	city.ChangeCityDoor(city_id[actID].MapID, DoorModelID)

	-- ˢ�����Ź���
	local DoorCreatureID = city_door[city_id[actID].DefenceLevel].CreatureID
	local Door_Pos = {}
	Door_Pos[1] = {}
	Door_Pos[2] = {}
	Door_Pos[3] = {}

	Door_Pos[1], Door_Pos[2], Door_Pos[3] = map.GetDoorPos(city_id[actID].MapID)

	-- ��ʼ��������
	local index = 0
	for k, v in pairs(Door_Pos) do
		Door_State[k].CreatureID = map.MapCreateCreature(city_id[actID].MapID, -1, DoorCreatureID, Door_Pos[k][index+1], Door_Pos[k][index+2], Door_Pos[k][index+3])
		Door_State[k].x = Door_Pos[k][index+1]
		Door_State[k].y = Door_Pos[k][index+2]
		Door_State[k].z = Door_Pos[k][index+3]
		Door_State[k].Opened = 0
		Door_State[k].Change_Min = 0
		Door_State[k].Repair_Time = 0
		Door_State[k].HpNode = 9
		index = index + 3
	end

	-- �ر����г���
	map.OpenCloseAllDoors(city_id[actID].MapID, -1, false)

	-- ˢ������
	-- ��ɾ�����ǳ�ս�ڼ������
	map.MapDeleteCreature(city_id[actID].MapID, -1, LongZhu_State.LongZhuID)
	LongZhu_State.LongZhuID = map.MapCreateCreature(city_id[actID].MapID, -1, 4900029, 731, 11180, 754)
	unit.AddBuff(city_id[actID].MapID, -1, LongZhu_State.LongZhuID, 9999201, LongZhu_State.LongZhuID)		-- ����ط���Ч

	-- �����������
	local RoleTbl = map.GetMapAllRoleID(city_id[actID].MapID, -1)
	for k, v in pairs(RoleTbl) do
		-- ������͵ص�
		local num = table.getn(FX_Out_Pos)
		local index = math.random(1, num)
		role.RoleGotoNewMap(city_id[actID].MapID, -1, v, 3017298383, FX_Out_Pos[index].x, FX_Out_Pos[index].y, FX_Out_Pos[index].z)
	end

	-- �������в�ս����Ϊ��ս״̬
	local AtkGuildTbl = city.GetConfirmGuild(city_id[actID].CityID, 0)
	local DefGuildTbl = city.GetConfirmGuild(city_id[actID].CityID, 1)

	if AtkGuildTbl ~= nil then
		for k, v in pairs(AtkGuildTbl) do
			guild.SetGuildStatus(v, 8)
		end
	end

	if DefGuildTbl ~= nil then
		for k, v in pairs(DefGuildTbl) do
			guild.SetGuildStatus(v, 8)
		end
	end

	PlayerNum[0] = 0
	PlayerNum[1] = 0

	-- ��ʼ����ս���ս��
	city.InitCSRoleData(city_id[actID].CityID)

end

-- �����
function CityStruggle_OnEnd(actID)
	city.SetCityStruggleState(0)

	CityStruggle_IsStart = 0

	if CityStruggle_IsOver == 0 then
		CityStruggle_IsOver = 1
		CityStruggle_OnOver()
	end

	-- ��ձ�
	Role_Alliance = {}
	YinYangZhu_State = {}
	LongZhu_State = {}

	CityStruggle_ActID = nil
	city_guard[city_id[actID].GuardIndex] = {}
	city_id[actID].GuardNum = 0

	-- �����ػ�ʯ״̬
	Eudemon_Stone = {}
end

-- ��㲥
function CityStruggle_OnBroadCast(state)
	if state == 0 then					-- ��ս������ʼ
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1019)
		msg.DispatchWorldMsgEvent(MsgID)

	elseif state == 1 then				-- ��ս��ʼ
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1020)
		msg.DispatchWorldMsgEvent(MsgID)

	elseif state == 2 then				-- ��ս��������


	elseif state == 3 then				-- ��ս����

	else

	end

end

-- ��Ҹ���
function CityStruggle_RoleRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)

    ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
	ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)

	if Type == 2 then			-- ���ܸ���
		local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
		return ReviveHP, ReviveMP, x, y, z, MapID
	elseif Type == 3 then		-- ��������
		return ReviveHP, ReviveMp, x, y, z, RebornMapID
	end

	if CityStruggle_IsStart ~= 1 then
	    return ReviveHP, ReviveMP, 808, 10350, 575, MapID
	end

	local x, y, z
	local force = city.GetRoleForce(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, RoleID)
	local GuildID = guild.GetRoleGuildID(RoleID)
	local GuildTbl = city.GetConfirmGuild(city_id[CityStruggle_ActID].CityID, force)

	-- ·�˵Ļ�������ڵ�
	if GuildTbl == nil then
		return ReviveHP, ReviveMP, 890, 11259, 2307, 3017298383
	end

	for k, v in pairs(GuildTbl) do
		if GuildID == v then
			if force == 0 then
				-- ����ð��ɵļ����
				x = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k+5].x
				y = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k+5].y
				z = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k+5].z
			elseif force == 1 then
				x = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k].x
				y = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k].y
				z = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k].z
			else

			end
			break
		end
	end

	-- ��ȡ�����Ӫ
	if Role_Alliance[RoleID] == 0 then
		if Atk_ZhaoHunFan.use == 1 then
			x = Atk_ZhaoHunFan.x
			y = Atk_ZhaoHunFan.y
			z = Atk_ZhaoHunFan.z
		else
		end
	elseif Role_Alliance[RoleID] == 1 then
		if Def_ZhaoHunFan.use == 1 then
			x = Def_ZhaoHunFan.x
			y = Def_ZhaoHunFan.y
			z = Def_ZhaoHunFan.z
		else
		end
	else
	end


	-- ���һ��15���޵е�buff
	unit.AddBuff(MapID, InstanceID, RoleID, 9999001, RoleID)    --��һ��15�����޵�buff

	RebornMapID = city_id[CityStruggle_ActID].MapID

	return ReviveHP, ReviveMP, x, y, z, RebornMapID
end

-- ������ߺ��ܽ���˵�ͼ
function CityStruggle_CanEnterWhenOnline(MapID, RoleID)
	return 3017298383, 890, 11259, 2307
end

-- ��ҽ����ͼ
function CityStruggle_OnPlayerEnter(MapID, InstanceID, RoleID)
	-- �ǳ�սʱ�䲻����������ж�
	if CityStruggle_IsStart ~= 1 then
		return
	end

    -- ���һ�����ͼ���ж���������Ӫ
    local force = city.GetRoleForce(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, RoleID)

    -- ��������
    if force == 0 then
        if Role_Alliance[RoleID] == nil then
            Role_Alliance[RoleID] = 0
			PlayerNum[force] = PlayerNum[force] + 1
        end

    -- ��������
    elseif force == 1 then
        if Role_Alliance[RoleID] == nil then
            Role_Alliance[RoleID] = 1
			PlayerNum[force] = PlayerNum[force] + 1
        end
    else

    end

	-- ����һ�����ս����¼
	city.AddRoleDataInfo(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, RoleID)

	-- ���ͻ��˷�����Ϣ
	city.EnterLeaveCityStruggle(city_id[CityStruggle_ActID].CityID, RoleID, 1, Role_Alliance[RoleID])

	-- ���ͻ���ͬ����ս״̬
	SendCityStruggleInfo(MapID, InstanceID, RoleID)
	SycAllRoleOneInfo(MapID, InstanceID, 0, force, PlayerNum[force])

	-- �������ӷ�ӡ����
	local LearnedSkill1 = role.IsLearnedSkill(RoleID, 90015)
	local LearnedSkill2 = role.IsLearnedSkill(RoleID, 90016)
	local LearnedSkill3 = role.IsLearnedSkill(RoleID, 90018)
	if LearnedSkill1 == nil then
		role.AddSkill(RoleID, 9001501)
	end
	if LearnedSkill2 == nil then
		role.AddSkill(RoleID, 9001601)
	end

	--if force == 1 and LearnedSkill3 == nil then
	--	role.AddSkill(RoleID, 9001801)
	--end

	-- �������ʿ��Ϊ100
	unit.SetAttValue(MapID, InstanceID, RoleID, 63, 100)
end

-- ����뿪��ͼ
function CityStruggle_OnPlayerLeave(MapID, InstanceID, RoleID)
	-- �������ʿ��Ϊ100
	unit.SetAttValue(MapID, InstanceID, RoleID, 63, 100)

	-- ��������ڷ�ӡ�������ң�ֹͣ��ӡ���ı�����״̬
	if LongZhu_State.RoleID == RoleID then
		unit.CancelBuff(MapID, InstanceID, LongZhu_State.LongZhuID, 9999101)		-- ȥ����ӡʱ��Ч
		unit.CancelBuff(MapID, InstanceID, RoleID, 9999401)						-- ȥ����ӡ���޵�buff

		-- ͬ������״̬����ͼ�������
		SycAllRoleOneInfo(MapID, InstanceID, 2, 0, Role_Alliance[RoleID])

		LongZhu_State.RoleID = nil				-- ȥ����ǰ��ӡ��Ҽ�¼
	end

	-- �ǳ�սʱ�����������ж�
	if CityStruggle_IsOver == 1 then
		if unit.IsInState(MapID, InstanceID, RoleID, 8) then	-- ��ս�����󴫳�ȡ����ս״̬
			city.EnterLeaveCityStruggle(3, RoleID, 0, 0)

		elseif unit.IsInState(MapID, InstanceID, RoleID, 9) then
			city.EnterLeaveCityStruggle(3, RoleID, 0, 1)

		else

		end

		return
	end

	-- ���ͻ��˷�����Ϣ
	city.EnterLeaveCityStruggle(city_id[CityStruggle_ActID].CityID, RoleID, 0, Role_Alliance[RoleID])

	if Role_Alliance[RoleID] ~= nil then
		if Role_Alliance[RoleID] == 0 then
			PlayerNum[0] = PlayerNum[0] - 1
		else
			PlayerNum[1] = PlayerNum[1] - 1
		end

		SycAllRoleOneInfo(MapID, InstanceID, 0, Role_Alliance[RoleID], PlayerNum[Role_Alliance[RoleID]])

		Role_Alliance[RoleID] = nil
	end

end

-- ���͵�ǰ��ս��Ϣ���ͻ���
function SendCityStruggleInfo(MapID, InstanceID, RoleID)
	-- �����ӡ״̬
	local bSealing
	if LongZhu_State.RoleID == nil then
		bSealing = false
	else
		bSealing = true
	end

	-- ��ս��������ʱ
	local CountDown
	if LongZhu_State.ChangeMin == nil then
		CountDown = (60 - CityStruggle_CurMin)*60
	else
		CountDown = (20 - (CityStruggle_CurMin - LongZhu_State.ChangeMin))*60
	end

	-- ������ռ��״̬
	local index = 1
	local YinYangZhu = {}
	for k,v in pairs(YinYangZhu_State) do
		local YinYangZhuIndex = CityStruggle_GetYinYangZhuIndex(k)
		if YinYangZhuIndex ~= nil then
			YinYangZhu[YinYangZhuIndex+1] = v
		end
	end

	-- ���ʿ�����
	local nWuZiKuNum = 0
	for k, v in pairs(Material_State) do
		if Material_State[k].Destroied == true then

		else
			nWuZiKuNum = nWuZiKuNum + 1
		end
	end

	-- ����״̬
	index = 1
	local Door = {}
	Door[1] = {Pos, bDestroied, nCountDown}
	Door[2] = {Pos, bDestroied, nCountDown}
	Door[3] = {Pos, bDestroied, nCountDown}

	for k, v in pairs(Door_State) do
		Door[k].Pos = k
		Door[k].bDestroied = Door_State[k].Opened
		if Door_State[k].Repair_Time >= 3 then
			Door[k].nCountDown = -1
		else
			Door[k].nCountDown = 20 - (CityStruggle_CurMin - Door_State[k].Change_Min)
		end
	end

	-- ͳ��˫������
--[[	local AtkNum = 0
	local DefNum = 0
	for k, v in pairs(Role_Alliance) do
		if v == 0 then
			AtkNum = AtkNum + 1
		else
			DefNum = DefNum + 1
		end
	end
]]
	city.SendCityStruggleInfo(
								MapID,				InstanceID,			RoleID,				LongZhu_State.Belong,	bSealing,
								CountDown,			YinYangZhu[1],		YinYangZhu[2],		YinYangZhu[3],			YinYangZhu[4],
								YinYangZhu[5],		nWuZiKuNum,			Door[1].Pos,		Door[1].bDestroied,		Door[1].nCountDown,
								Door[2].Pos,		Door[2].bDestroied, Door[2].nCountDown, Door[3].Pos,			Door[3].bDestroied,
								Door[3].nCountDown, PlayerNum[0],		PlayerNum[1]
								)
end

-- ����һ����ս��Ϣ
function SendOneCityStruggleInfo(MapID, InstanceID, RoleID, Type1, Type2, Data)
--[[
	Type1 == 0			-- ������������ı�

	Type1 == 1			-- �������ڱ���ӡ

	Type1 == 2			-- ���鱻ռ��

	Type1 == 3			-- ��������ռ��

	Type1 == 4			-- ���ʿ������ı�

	Type1 == 5			-- ���Ź���

]]

	city.SendOneCityStruggleInfo(MapID, InstanceID, RoleID, Type1, Type2, Data)
end

-- ���ͼ���������ͬ����ս״̬
function SycAllRoleOneInfo(MapID, InstanceID, Type1, Type2, Data)
	for k, v in pairs(Role_Alliance) do
		city.SendOneCityStruggleInfo(MapID, InstanceID, k, Type1, Type2, Data)
	end
end

-- ��������
function CityStruggle_OnCreatureDie(MapID, InstanceID, CreID, CreTypeID, KillerID)
	-- ��ɱ�ػ��񣬽�������ʿ������
end

-- �������
function CityStruggle_OnRoleDie(MapID, InstanceID, RoleID, KillerID)
	if unit.IsPlayer(KillerID) == true then
		-- �ж�����Ƿ��ǰ���
		local guildID = guild.GetRoleGuildID(RoleID)
		if guildID ~= 4294967295 then
			if guild.GetRoleGuildPos(guildID, RoleID) == 2 then
				-- ��ɱ��������ɱ�߰��ɹ�ѫ+10
				local killGuildID = guild.GetRoleGuildID(KillerID)
				if guildID ~= 4294967295 then
					guild.ModifyMemberExploit(guildID, KillerID, 10)
				end

				-- ��ɱ���������ɳ�Աʿ��+5 ����ɱ����ȫ���Աʿ��-5
				for k, v in pairs(Role_Alliance) do
					local GID = guild.GetRoleGuildID(k)
					if killGuildID == GID then
						unit.ModAttValue(MapID, InstanceID, k, 63, 5)
					elseif guildID == GID then
						unit.ModAttValue(MapID, InstanceID, k, 63, -5)
					else

					end
					FX_ResetMorale(MapID, InstanceID, k)
				end

				-- �㲥�㲥
				local MsgID = msg.BeginMsgEvent()
				local id_msg = 0
				if Role_Alliance[RoleID] == 0 then -- �������˻�ɱ�������˰���
					id_msg = 1042
				else
					id_msg = 1041
				end
				msg.AddMsgEvent(MsgID, 101, id_msg)
				msg.AddMsgEvent(MsgID, 19, guildID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

			else
				-- ��ɱ��ͨ���ڣ���ɱ�߰��ɹ�ѫ+1
				local killGuildID = guild.GetRoleGuildID(KillerID)
				if guildID ~= 4294967295 then
					guild.ModifyMemberExploit(guildID, KillerID, 1)
				end

				-- ��ɱ��ʿ��+1����ɱ��ʿ��-1
				unit.ModAttValue(MapID, InstanceID, KillerID, 63, 1)
				FX_ResetMorale(MapID, InstanceID, KillerID)
				unit.ModAttValue(MapID, InstanceID, RoleID, 63, -1)
				FX_ResetMorale(MapID, InstanceID, RoleID)
			end

			-- ���Ӿ���
			local Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			local AddExp = role.GetKillObjExp(Level)
			if AddExp ~= nil then
				role.AddRoleExp(MapID, InstanceID, KillerID, AddExp)
			end

			-- С�����ӻ�ɱ��һ��֣�����ɱ�ߣ�
			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, KillerID)
			if TeamID~= 4294967295 then
				local TeamMem = {}
				TeamMem[1], TeamMem[2], TeamMem[3], TeamMem[4], TeamMem[5], TeamMem[6] = role.GetRoleTeamMemID(TeamID)
				for k,v in pairs(TeamMem) do
					if v ~= nil and v ~= KillerID then
						city.AddKillRoleNum(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, v, RoleID)
					end
				end
			end

			-- ��ɱ�����ӻ�ɱ����
			city.AddKillRoleNum(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, KillerID, RoleID)

			-- �õ���ɱ����������ܶ��Ҫ�㲥����
			local KillNum = city.GetRoleKillNum(MapID, InstanceID, KillerID, city_id[CityStruggle_ActID].CityID)
			if KillNum == 20 or KillNum == 50 or KillNum == 100 then
				local MsgID = msg.BeginMsgEvent()
				local id_msg = 0
				if Role_Alliance[KillerID] == 0 then -- �������˳�Աɱ�����ˣ�
					id_msg = 1040
				else
					id_msg = 1039
				end
				msg.AddMsgEvent(MsgID, 101, id_msg)
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 9, KillNum)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			end
		end
	end
end

-- ��ս����
function CityStruggle_OnOver()
	local CityID = city_id[CityStruggle_ActID].CityID

	-- ����ͳ��Ȩ�ı�
	if LongZhu_State.Belong == 0 then
		-- ʧȥͳ��Ȩ���ɶ������200������
		guild.ModifyGuildReputation(city.GetCityAppointedAtt(cityID, 2), -1, -200, 607)
		city.ChangeCityHolder(CityID, LongZhu_State.GuildID)
	end

	-- ɾ������
	map.MapDeleteCreature(city_id[CityStruggle_ActID].MapID, -1, Door_State[1].CreatureID)
	map.MapDeleteCreature(city_id[CityStruggle_ActID].MapID, -1, Door_State[2].CreatureID)
	map.MapDeleteCreature(city_id[CityStruggle_ActID].MapID, -1, Door_State[3].CreatureID)

	-- �����г���
	map.OpenCloseAllDoors(city_id[CityStruggle_ActID].MapID, -1, true)


	-- �㲥������ս
	if LongZhu_State.GuildID == -1 then
		LongZhu_State.GuildID = city.GetCityAppointedAtt(CityID, 2)
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1021)
	msg.AddMsgEvent(MsgID, 6, city_id[CityStruggle_ActID].MapID)
	msg.AddMsgEvent(MsgID, 19, LongZhu_State.GuildID)
	msg.AddMsgEvent(MsgID, 6, city_id[CityStruggle_ActID].MapID)
	msg.DispatchWorldMsgEvent(MsgID)

    -- ���������ĸı�
	local Reputation = 0

	-- ����������������ı�ֵ ��50 + �������˰��ɵİ�������ƽ��ֵ * 0.1��
	local AtkAddRep = 0
	local DefAddRep = 0
	local AtkGuildTbl = city.GetConfirmGuild(CityID, 0)
	if AtkGuildTbl ~= nil then
		for k, v in pairs(AtkGuildTbl) do
			Reputation = Reputation + guild.GetGuildReputation(AtkGuildTbl[k])
		end
		DefAddRep = 50 + Reputation / table.getn(AtkGuildTbl) * 0.1
	end

	-- ����������������ı�ֵ
	Reputation = 0
	local DefGuildTbl = city.GetConfirmGuild(CityID, 1)
	if DefGuildTbl ~= nil then
		for k, v in pairs(DefGuildTbl) do
			Reputation = Reputation + guild.GetGuildReputation(DefGuildTbl[k])
		end
		AtkAddRep = 50 + Reputation / table.getn(DefGuildTbl) * 0.1
	end

	-- ȡ����ս���ɵ���ս״̬
	if AtkGuildTbl ~= nil then
		for k, v in pairs(AtkGuildTbl) do
			guild.UnsetGuildStatus(v, 8)
		end
	end

	if DefGuildTbl ~= nil then
		for k, v in pairs(DefGuildTbl) do
			guild.UnsetGuildStatus(v, 8)
		end
	end

	if LongZhu_State.Belong == 0 then
		DefAddRep = 0 - DefAddRep
	else
		AtkAddRep = 0 - AtkAddRep
	end

	-- ��������
	if AtkGuildTbl ~= nil then
		for k, v in pairs(AtkGuildTbl) do
			guild.ModifyGuildReputation(AtkGuildTbl[k], -1, AtkAddRep, 607)
		end
	end

	if DefGuildTbl ~= nil then
		for k, v in pairs(DefGuildTbl) do
			guild.ModifyGuildReputation(DefGuildTbl[k], -1, DefAddRep, 607)
		end
	end

	-- �õ�ͳ��Ȩ�İ��ɶ�������200������
	guild.ModifyGuildReputation(city.GetCityAppointedAtt(cityID, 2), -1, 200, 607)

	-- ���˹�ѫ����
	for k, v in pairs(Role_Alliance) do
		-- �����һ���
		local nScore = city.GetRoleCSScore(city_id[CityStruggle_ActID].MapID, -1, city_id[CityStruggle_ActID].CityID, k)
		-- ��ð���ID
		local guildID = guild.GetRoleGuildID(k)

		local nExploitGain = 0
		local nContributeGain = 0

		if Role_Alliance[k] == LongZhu_State.Belong then

			--[[
			if guildID == LongZhu_State.GuildID then
				-- ���ɻ�ó�սʤ���һ�ó���ͳ��Ȩ�����ɹ�ѫ+200
				guild.ModifyMemberExploit(guildID, k, 200)
			else
				-- ��ó�սʤ����δ��ó���ͳ��Ȩ�����ɹ�ѫ+100
				guild.ModifyMemberExploit(guildID, k, 100)
			end
			]]
			-- ʤ�����ɽ���
			nExploitGain = nScore*3
			nContributeGain = nScore/2

		else
			-- δ��ó�սʤ�����ɽ���
			nExploitGain = nScore
			nContributeGain = nScore/3
		end

		guild.ModifyMemberExploit(guildID, k, nExploitGain)			-- ��ѫ����
		guild.ModifyRoleContribute(guildID, k, nContributeGain, 0)	-- ���׽���

		city.AddRoleData(city_id[CityStruggle_ActID].MapID, -1,city_id[CityStruggle_ActID].CityID, k, 7, nExploitGain)		-- ��ѫ����
		city.AddRoleData(city_id[CityStruggle_ActID].MapID, -1,city_id[CityStruggle_ActID].CityID, k, 8, nContributeGain)	-- ���׽���
	end

	-- ������������
	city.SortByScore(CityID)
	-- ��ȡ����ǰ20�����
	local RankRoleID = city.GetCSRankedRoleID(CityID, 20)

	-- ������ǰ20����ҷ��ƺŽ���
	if RankRoleID ~= nil then
		for k, v in pairs(RankRoleID) do

			-- ����������1����һ�óƺ�"ʮ��ɱһ��"
			if k == 1 then
				role.SigTitleEvent(v, 258)
			end

			-- ����������2��3����һ�óƺ�"ǧ�ﲻ����"
			if k == 2 or k == 3 then
				role.SigTitleEvent(v, 259)
			end

			-- ����������4��10����һ�óƺ�"���˷���ȥ"
			if k >= 4 and k <= 10 then
				role.SigTitleEvent(v, 260)
			end

			-- ����������11��20����һ�óƺ�"���������"
			if k >= 11 and k <= 20 then
				role.SigTitleEvent(v, 261)
			end
		end
	end

	-- ��������
	--[[ role.AddRoleItem(RankRoleID[1])
	......
	]]

    -- ����սʧ�����˴��ͳ�����
	local FailForce
	if LongZhu_State.Belong == 0 then
		FailForce = 1
	else
		FailForce = 0
	end

	for k, v in pairs(Role_Alliance) do
		if v == FailForce then
			-- ������͵ص�
			local num = table.getn(FX_Out_Pos)
			local index = math.random(1, num)
			role.RoleGotoNewMap(city_id[CityStruggle_ActID].MapID, -1, k, 3017298383, FX_Out_Pos[index].x, FX_Out_Pos[index].y, FX_Out_Pos[index].z)
		end

		-- ��������ҷ��͸���ս����Ϣ
		role.SendCityStruggleRecord2Role(k);
	end

	-- ��������Ϸǳ�ս����Ч
	unit.AddBuff(city_id[CityStruggle_ActID].MapID, -1, LongZhu_State.LongZhuID, 9999201, LongZhu_State.LongZhuID)		-- ��ӷǳ�ս��Ч

	-- ɾ���л��
	if Atk_ZhaoHunFan.use == 1 then
		map.MapDeleteCreature(city_id[CityStruggle_ActID].MapID, -1, Atk_ZhaoHunFan.CreatureID)
	end

	if Def_ZhaoHunFan.use == 1 then
		map.MapDeleteCreature(city_id[CityStruggle_ActID].MapID, -1, Def_ZhaoHunFan.CreatureID)
	end

	CityStruggle_IsStart = 0

	-- ��ȫ����������������ͬ��buff
	act.AddAllRoleBuff(2027901);
end

-- �����ս��������npc�Ի�
function C4900005_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 1026)		--�����Ƿ�Ҫǰ���������ǣ���
		msg.AddMsgEvent(MsgID, 21, 4)
		msg.AddMsgEvent(MsgID, 1, 21)
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 22)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017298383, 890, 225, 2307)
	end
end

-- �����ս����npc�Ի�
function C4900004_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	--����ǵ�һ�ζԻ�
	if TalkIndex == -1 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["CS_FX_TalkIndex"], 1)
	end

	local stepindex = role.GetRoleScriptData(RoleID, 1, RoleDataType["CS_FX_TalkIndex"])

	if stepindex == 1 then
		if CityStruggle_IsStart == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1001)		--����ս�Ѿ���ʼ���Ƿ�Ҫ�μӡ�
			msg.AddMsgEvent(MsgID, 21, 4)
			msg.AddMsgEvent(MsgID, 1, 1002)
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 1003)
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			stepindex = 2
		else
			-- �ǳ�ս�Ի�
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1001)		-- ��ռ����ɽ�����Ҫ·��
			msg.AddMsgEvent(MsgID, 21, 4)
			msg.AddMsgEvent(MsgID, 1, 1002)		-- ��Ҫ����
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 1003)		-- ��������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			stepindex = 3
		end

	elseif stepindex == 2 then
		if TalkIndex ~= 4 then
			return
		end

		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local CityID = city_id[CityStruggle_ActID].CityID
		if level < 60 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1006)	-- �ȼ��������60��
			msg.AddMsgEvent(MsgID, 21, 4)
			msg.AddMsgEvent(MsgID, 1, 21)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local force = city.GetRoleForce(MapID, InstanceID, CityID, RoleID)
			if force == 2 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1005)	-- ����Ȩ�����ս
				msg.AddMsgEvent(MsgID, 21, 4)
				msg.AddMsgEvent(MsgID, 1, 21)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else
				local GuildID = guild.GetRoleGuildID(RoleID)
				local Num = CityStruggle_GetGuildMemNum(GuildID)
				local HoldGuildID = city.GetCityAppointedAtt(CityID, 2)

				if Num >= FX_Normal_Guild_Mem then
					if GuildID ~= HoldGuildID then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 1007)	-- ���ɲ��������Ѵ�����
						msg.AddMsgEvent(MsgID, 21, 4)
						msg.AddMsgEvent(MsgID, 1, 21)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)

					elseif GuildID == HoldGuildID and Num >= FX_Hold_Guild_Mem then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 1007)	-- ���ɲ��������Ѵ�����
						msg.AddMsgEvent(MsgID, 21, 4)
						msg.AddMsgEvent(MsgID, 1, 21)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)

					else
						-- ����ӵ�а��ɣ�����ָ���ص�
						local x, y, z
						local GuildTbl = city.GetConfirmGuild(CityID, 1)
						local GuildID = guild.GetRoleGuildID(RoleID)
						for k, v in pairs(GuildTbl) do
							if GuildID == v then
								-- ����ð��ɵļ����
								x = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k].x
								y = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k].y
								z = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k].z
								break
							end
						end

						role.RoleGotoNewMap(MapID, InstanceID, RoleID, city_id[CityStruggle_ActID].MapID, x, y, z)
					end
				else
					-- ��������
					--local force = city.GetRoleForce(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, RoleID)
					local x, y, z
					local GuildID = guild.GetRoleGuildID(RoleID)
					local GuildTbl = city.GetConfirmGuild(CityID, force)
					for k, v in pairs(GuildTbl) do
						if GuildID == v then
							if force == 0 then
								-- ����ð��ɵļ����
								x = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k+5].x
								y = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k+5].y
								z = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k+5].z
							elseif force == 1 then
								x = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k].x
								y = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k].y
								z = Enter_Pos[city_id[CityStruggle_ActID].EnterIndex][k].z
							else

							end
							break
						end
					end

					role.RoleGotoNewMap(MapID, InstanceID, RoleID, city_id[CityStruggle_ActID].MapID, x, y, z)
				end
			end
		end

		stepindex = 1

	elseif stepindex == 3 then
		if TalkIndex ~= 4 then
			return
		end

		local GuildID = guild.GetRoleGuildID(RoleID)
		if GuildID ~= 4294967295 and GuildID == city.GetCityAppointedAtt(3, 2) then		-- ����ͳ�ΰ��ɳ�Ա�������
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3695619387, 812, 208, 503)

		else
			local Money = role.GetRoleSilver(MapID, InstanceID, RoleID)
			if Money < 10000 then
				--��ʾ��ҽ�Ǯ����
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	1004)
				msg.AddMsgEvent(MsgID, 21,	4)			    --ȷ��
				msg.AddMsgEvent(MsgID, 1,	21)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else
				-- �۳���Ǯ������
				role.DecRoleSilver(MapID, InstanceID, RoleID, 10000, 200)
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3695619387, 812, 208, 503)
			end

			stepindex = 1
		end
	end

	role.SetRoleScriptData(RoleID, 1, RoleDataType["CS_FX_TalkIndex"], stepindex)
end

-- ��ȡ���ɲμӳ�ս����
function CityStruggle_GetGuildMemNum(GuildID)
	local num = 0
	for k, v in pairs(Role_Alliance) do
		if GuildID == guild.GetRoleGuildID(k) then
			num = num + 1
		end
	end

	return num
end

-- ��ȡ����������
function CityStruggle_GetYinYangZhuIndex(TargetTypeID)
	for i = 1, 5 do
		if YinYangZhuTypeID[i] == TargetTypeID then
			return i-1;
		end
	end

	return nil
end

-- ��ӡ
function OnSealSkill(MapID, InstanceID, SkillID, OwnerID, TargetID, TargetTypeID)
	local GuildID = guild.GetRoleGuildID(OwnerID)

	if TargetTypeID == 4900029 then			-- ����
		unit.CancelBuff(MapID, InstanceID, LongZhu_State.LongZhuID, 9999101)		-- ȥ����ӡʱ��Ч
		unit.CancelBuff(MapID, InstanceID, OwnerID, 9999401)						-- ȥ����ӡ���޵�buff

		-- ȥ�����鵱ǰ��Ч���������Ч
		if LongZhu_State.Belong == 1 then
			unit.CancelBuff(MapID, InstanceID, LongZhu_State.LongZhuID, 9999201)
			unit.AddBuff(MapID, InstanceID, LongZhu_State.LongZhuID, 9999301, OwnerID)
		else
			unit.CancelBuff(MapID, InstanceID, LongZhu_State.LongZhuID, 9999301)
			unit.AddBuff(MapID, InstanceID, LongZhu_State.LongZhuID, 9999201, OwnerID)
		end

		LongZhu_State.ChangeMin = CityStruggle_CurMin
		LongZhu_State.GuildID = GuildID
		LongZhu_State.RoleID = nil			-- ��ӡ���ȥ����ӡ�߼�¼
		LongZhu_State.Belong = Role_Alliance[OwnerID]


		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	1018)				-- �ɹ���ӡ����
		msg.AddMsgEvent(MsgID, 19, GuildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		-- ��������ʿ��+5
		for k, v in pairs(Role_Alliance) do
			if v == Role_Alliance[OwnerID] then
				unit.ModAttValue(MapID, InstanceID, k, 63, 5)
				FX_ResetMorale(MapID, InstanceID, k)
			end
		end

		-- ͬ������״̬����ͼ�������
		SycAllRoleOneInfo(MapID, InstanceID, 3, 20, Role_Alliance[OwnerID])

		-- ���ӿ����������
		city.AddRoleData(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, OwnerID, 6, 1)

	elseif TargetTypeID == 4900030 or TargetTypeID == 4900043 or TargetTypeID == 4900044 or TargetTypeID == 4900045 or TargetTypeID == 4900046 then		-- ������

		-- ���м������ɳ�Ա��ӡ����
		if YinYangZhu_State[TargetTypeID] == Role_Alliance[OwnerID] then
			return 0
		end

		-- ȥ����������ǰ��Ч���������Ч
		unit.CancelBuff(MapID, InstanceID, TargetID, 9999102)					-- ȥ����ӡʱ����Ч

		if YinYangZhu_State[TargetTypeID] == 1 then
			unit.CancelBuff(MapID, InstanceID, TargetID, 9999202)
			unit.AddBuff(MapID, InstanceID, TargetID, 9999302, OwnerID)
		else
			unit.CancelBuff(MapID, InstanceID, TargetID, 9999302)
			unit.AddBuff(MapID, InstanceID, TargetID, 9999202, OwnerID)
		end

		YinYangZhu_State[TargetTypeID] = Role_Alliance[OwnerID]

		-- ͬ��������״̬����ͼ�������
		local YinYangZhuIndex = CityStruggle_GetYinYangZhuIndex(TargetTypeID)
		if YinYangZhuIndex ~= nil then
			SycAllRoleOneInfo(MapID, InstanceID, 4, YinYangZhuIndex, Role_Alliance[OwnerID])
		end

		-- ��ȡ˫��ռ������������
		local AtkSeize = 0
		local DefSeize = 0
		for k, v in pairs(YinYangZhu_State) do
			if v == 0 then
				AtkSeize = AtkSeize + 1
			else
				DefSeize = DefSeize + 1
			end
		end

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	1017)				-- �ɹ���ӡ������
		msg.AddMsgEvent(MsgID, 19, GuildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 9, AtkSeize)
		msg.AddMsgEvent(MsgID, 9, DefSeize)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		-- ��������ʿ��+3
		for k, v in pairs(Role_Alliance) do
			if v == Role_Alliance[OwnerID] then
				unit.ModAttValue(MapID, InstanceID, k, 63, 3)
				FX_ResetMorale(MapID, InstanceID, k)
			end
		end

		-- ����˫���ĸ���ʱ��
		if YinYangZhu_State[TargetTypeID] == 0 then
			Atk_Revive_Time = Atk_Revive_Time - 5
			Def_Revive_Time = Def_Revive_Time + 5
		else
			Atk_Revive_Time = Atk_Revive_Time + 5
			Def_Revive_Time = Def_Revive_Time - 5
		end

		city.SetReviveTime(city_id[CityStruggle_ActID].CityID, 0, Atk_Revive_Time)
		city.SetReviveTime(city_id[CityStruggle_ActID].CityID, 1, Def_Revive_Time)

		-- ���������������������3������ǿ�ƽ���������ӡ����Ĳ���
		if LongZhu_State.RoleID ~= nil then
			local SeizeNum = 0
			for k, v in pairs(YinYangZhu_State) do
				if v ~= nil and  v == Role_Alliance[LongZhu_State.RoleID] then
					SeizeNum = SeizeNum + 1
				end
			end

			if SeizeNum < 3 and LongZhu_State.RoleID ~= nil then
				role.RoleCancelSkill(MapID, InstanceID, LongZhu_State.RoleID, 9001501)
				local GuildID = guild.GetRoleGuildID(LongZhu_State.RoleID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101,	1016)				-- ǿ����ֹ��ӡ����
				msg.AddMsgEvent(MsgID, 19, GuildID)
				msg.AddMsgEvent(MsgID, 2, LongZhu_State.RoleID)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

				-- ͬ������״̬����ͼ�������
				--SycAllRoleOneInfo(MapID, InstanceID, 2, 0, Role_Alliance[OwnerID])

				--unit.CancelBuff(MapID, InstanceID, LongZhu_State.LongZhuID, 9999101)		-- ȥ����ӡʱ��Ч
				--unit.CancelBuff(MapID, InstanceID, LongZhu_State.RoleID, 9999401)						-- ȥ����ӡ���޵�buff
				--LongZhu_State.RoleID = nil						-- �����ӡ�߼�¼�ÿ�
			end
		end

		-- ���ӿ�������������
		city.AddRoleData(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, OwnerID, 5, 1)

	else

	end

	return 0
end

-- �Ƿ���ʹ�÷�ӡ����  ������999�����޷�ʹ�ü������޴������ԣ�
function OnSealCanCast(MapID, InstanceID, SkillID, OwnerID, TargetID)
	local GuildID = guild.GetRoleGuildID(OwnerID)
	local Pos = guild.GetRoleGuildPos(GuildID, OwnerID)
	local UnitTypeID
	UnitTypeID = unit.GetUnitTypeID(MapID, InstanceID, TargetID)

	-- ��ӡ����
	if UnitTypeID == 4900029 then
		if CityStruggle_IsStart ~= 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1010)			-- �ǳ�ս�ڼ��޷�ִ�з�ӡ�������
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end

		if Pos ~= 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1008)			-- ֻ�и����ɰ����ɶ�������з�ӡ��
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		else
			if Role_Alliance[OwnerID] == LongZhu_State.Belong then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1009)		-- ������Ŀǰ�ѱ������������˷�ӡ��
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)
				return 999

			else
				local SeizeNum = 0
				for k, v in pairs(YinYangZhu_State) do
					if v ~= nil and  v == Role_Alliance[OwnerID] then
						SeizeNum = SeizeNum + 1
					end
				end

				if SeizeNum < 3 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 1011)		-- ֻ�е���������ռ���������������������ſɽ��������ӡ��
					msg.DispatchRoleMsgEvent(OwnerID, MsgID)
					return 999
				else
					if LongZhu_State.RoleID ~= nil then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 1030)	-- ����ͬʱֻ����һ�˽��з�ӡ������
						msg.DispatchRoleMsgEvent(OwnerID, MsgID)

						return 999
					end

					-- ����ƹ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 1015)		-- �ѿ�ʼ��ӡ���飬������������ռ������������3��ʱ����ӡ����������ֹ��
					msg.AddMsgEvent(MsgID, 19, GuildID)
					msg.AddMsgEvent(MsgID, 2, OwnerID)
					msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

					LongZhu_State.RoleID = OwnerID		-- ��¼��ӡ��ҵ�ID

					unit.AddBuff(MapID, InstanceID, LongZhu_State.LongZhuID, 9999101, OwnerID)		-- ��ӷ�ӡ��Ч
					unit.AddBuff(MapID, InstanceID, OwnerID, 9999401, OwnerID)						-- ����ӡ������޵�buff

					-- ͬ������״̬����ͼ�������
					SycAllRoleOneInfo(MapID, InstanceID, 1, 0, Role_Alliance[OwnerID])

					return 0							-- �ɹ���ӡ
				end
			end
		end

	-- ��ӡ������
	elseif UnitTypeID == 4900030 or UnitTypeID == 4900043 or UnitTypeID == 4900044 or UnitTypeID == 4900045 or UnitTypeID == 4900046 then

		if Pos ~= 2 and Pos ~= 3 and Pos ~= 6 and Pos ~= 9 and Pos~= 12 and Pos ~= 15 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1012)			-- ֻ�и�������������ְ���߿ɶ����������з�ӡ��
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		else
			if YinYangZhu_State[UnitTypeID] ~= nil and Role_Alliance[OwnerID] == YinYangZhu_State[UnitTypeID] then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1013)		-- ��������Ŀǰ�ѱ������������˷�ӡ��
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)
				return 999
			else
				unit.AddBuff(MapID, InstanceID, TargetID, 9999102, OwnerID)		-- ��ӷ�ӡ��Ч
				return 0								-- �ɹ���ӡ
			end
		end

	-- �ػ�ʯ
	elseif UnitTypeID == 4900048 then
--[[ �ر��ػ����ٻ�����
		if Role_Alliance[OwnerID] == 1 then
			if Eudemon_State.Summoned ~= 1 then

				Eudemon_Stone[OwnerID] = 1
				-- ���㵱ǰ�ٻ�����
				local nPerson = 0
				for k,v in pairs(Eudemon_Stone) do
					if v == 1 then
						nPerson = nPerson + 1
					end
				end
				Eudemon_State.NowPerson = nPerson


				if Eudemon_State.NowPerson < Eudemon_State.NeedPerson and Eudemon_State.BeginSummon ~= 1 then	-- ��û��ʼ�ٻ�

					for k,v in pairs(Eudemon_Stone) do
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 1043)		-- ��ǰ�ٻ�������X/Y����������ƶ��������������㹻�󽫿�ʼ�ٻ���
						msg.AddMsgEvent(MsgID, 9, Eudemon_State.NowPerson)
						msg.AddMsgEvent(MsgID, 9, Eudemon_State.NeedPerson)
						msg.DispatchRoleMsgEvent(k, MsgID)
					end

--				elseif Eudemon_State.NowPerson < Eudemon_State.NeedPerson and Eudemon_State.BeginSummon == 1 then	-- �Ѿ���ʼ�ٻ�
					-- �����ٻ�����ֹͣ�ٻ�����
--					for k,v in pairs(Eudemon_Stone) do
--						role.RoleCancelSkill(MapID, InstanceID, k, SkillID)
--					end
--
					-- ����ٻ���
--					Eudemon_Stone = {}

				else	-- ��û�ٻ����Ѵﵽ�ٻ�����

					-- ��ʼ�ٻ��޵���ʱ
					city.SetSummonTime(city_id[CityStruggle_ActID].CityID, Eudemon_State.NeedSecond)

					-- �㲥
					for k,v in pairs(Eudemon_Stone) do
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 1044)		-- �ػ����ٻ��ѿ�ʼ����������ƶ�������
						msg.DispatchRoleMsgEvent(k, MsgID)

						--role.EudemonSummonCountDown(MapID, IstanceID, k, Eudemon_State.NeedSecond)
					end

					Eudemon_State.BeginSummon = 1
				end
				return 0

			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1045)			-- ���γ�ս���Ѿ����й��ػ����ٻ�
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)
				return 999
			end

		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1046)				-- ֻ�з������˵İ��ɳ�Ա�ɽ����ػ����ٻ�
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)
			return 999
		end
]] -- �ر��ػ����ٻ�����
		-- �ر��ػ����ٻ�
		return 999
	else

	end
end

-- ȡ����ӡ
function OnSealCancelSkill(MapID, InstanceID, SkillID, OwnerID, TargetID, TargetTypeID)
	if TargetTypeID == 4900048 then				-- ����ʯ
		Eudemon_Stone[OwnerID] = nil

		-- ���㵱ǰ�ٻ�����
		Eudemon_State.NowPerson = Eudemon_State.NowPerson - 1
		if Eudemon_State.BeginSummon == 1 and Eudemon_State.NowPerson < Eudemon_State.NeedPerson then	-- ����Ѿ���ʼ�ٻ�����������
			-- �����ٻ�����ֹͣ�ٻ�����
			for k,v in pairs(Eudemon_Stone) do
				role.RoleCancelSkill(MapID, InstanceID, k, SkillID)

				-- �㲥
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1048)			-- �ٻ��������㣬�ٻ��ػ���ǿ����ֹ
				msg.DispatchRoleMsgEvent(k, MsgID)
			end

			-- ����ٻ���
			Eudemon_Stone = {}
			-- ֹͣ�ٻ�
			Eudemon_State.BeginSummon = 0
			city.StopSummon(city_id[CityStruggle_ActID].CityID)
		end

	elseif TargetTypeID == 4900029 then			-- ����
		unit.CancelBuff(MapID, InstanceID, LongZhu_State.LongZhuID, 9999101)		-- ȥ����ӡʱ��Ч
		unit.CancelBuff(MapID, InstanceID, OwnerID, 9999401)						-- ȥ����ӡ���޵�buff
		if LongZhu_State.RoleID == OwnerID then
			LongZhu_State.RoleID = nil				-- ȥ����ǰ��ӡ��Ҽ�¼
		end

		-- ͬ������״̬����ͼ�������
		SycAllRoleOneInfo(MapID, InstanceID, 2, 0, Role_Alliance[OwnerID])

	elseif TargetTypeID == 4900030 or TargetTypeID == 4900043 or TargetTypeID == 4900044 or TargetTypeID == 4900045 or TargetTypeID == 4900046 then
		unit.CancelBuff(MapID, InstanceID, TargetID, 9999102)		-- ȥ����ӡʱ��Ч
	end
end

-- ���Ƴ���
function CDoor_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	-- �ҵ���Ӧ���� ��
	for k, v in pairs(Door_State) do
		if Door_State[k].CreatureID == CreatureID then
			Door_State[k].Opened = 1
			Door_State[k].Change_Min = CityStruggle_CurMin
			map.OpenCloseDoorByPos(MapID, InstanceID, Door_State[k].x, Door_State[k].y, Door_State[k].z, true)

			-- ͬ������״̬
			local CountDown = 20
			if Door_State[k].Repair_Time >= 3 then
				CountDown = -1
			end
			SycAllRoleOneInfo(MapID, InstanceID, 6, k, CountDown)

			-- �㲥
			local GuildID = guild.GetRoleGuildID(KillerID)
			local id_msg = 1031 + k - 1
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, id_msg)			-- ����/����/�����ѱ����������ɵġ��������ƣ�
			msg.AddMsgEvent(MsgID, 19, GuildID)
			msg.AddMsgEvent(MsgID, 2, KillerID)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

			break
		end
	end

	-- ���ӹ��ݳ��Ż���
	city.AddRoleData(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, KillerID, 2, 1)

	return true
end

-- ���ٷ����������ʿ�
function C4900027_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	Def_Revive_Time = Def_Revive_Time + 5
	city.SetReviveTime(city_id[CityStruggle_ActID].CityID, 1, Def_Revive_Time)

	for k, v in pairs(Material_State) do
		if Material_State[k].Destroied ~= 1 then
			local x, y, z = unit.GetPosition(MapID, InstanceID, CreatureID)
			Material_State[k].Destroied = 1
			Material_State[k].Change_Min = CityStruggle_CurMin
			Material_State[k].x = x
			Material_State[k].y = y
			Material_State[k].z = z
			break
		end
	end

	-- ��ȡ��ǰδ�����ٵ����ʿ�����
	local num = 0
	for k, v in pairs(Material_State) do
		if Material_State[k].Destroied ~= 1 then
			num = num + 1
		end
	end

	-- ͬ��
	SycAllRoleOneInfo(MapID, InstanceID, 5, 0, num)

	-- ���ӹ������ʿ����
	city.AddRoleData(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, KillerID, 3, 1)

	-- ���ǹ㲥
	local GuildID = guild.GetRoleGuildID(KillerID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1037)			-- �ݻ���һ���������˵����ʿ�
	msg.AddMsgEvent(MsgID, 19, GuildID)
	msg.AddMsgEvent(MsgID, 2, KillerID)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end

-- �����л��
function C4900028_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	if CreatureID == Atk_ZhaoHunFan.CreatureID then
		Atk_ZhaoHunFan.use = 0
	elseif CreatureID == Def_ZhaoHunFan.CreatureID then
		Def_ZhaoHunFan.ues = 0
	else

	end
end

--[[

-- ʹ����������
function I1350011_Use(MapID, InstanceID, TypeID, TargetID)
	-- ��ԭ��ˢ��һ����������
	local tbl = {CreatureID, x, y, z}
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	tbl.x = x
	tbl.y = y
	tbl.z = z
	tbl.CreatureID = map.MapCreateCreature(MapID, InstanceID, 1000893, x, y, z)

	for k, v in pairs(city_id) do
		if MapID == v.MapID then
			city_id[k].GuardNum = city_id[k].GuardNum + 1
			table.insert(city_guard[city_id[k].GuardIndex], tbl)
		end
	end
end

-- �ܷ�ʹ����������
function I1350011_CanUse(MapID, InstanceID, TypeID, TargetID)
	for k, v in pairs(city_id) do
		if MapID == v.MapID then
			if act.GetActIsStart(k) == true then	-- ��ս��ʼ���޷�ʹ��
				return 32, true
			elseif city_id[k].GuardNum >= 20 then	-- ���ڷ�20����������
				return 32, true
			end
		end
	end

	return 0, false
end

]]

-- �ܷ�ʹ���л��
function I3300006_CanUse(MapID, InstanceID, TypeID, TargetID)
	if MapID ~= 3695619387 then						-- �ǳ�ս��������ʹ��
		return 47, 0
	end

	local guildID = guild.GetRoleGuildID(TargetID)
	local bCanNotUse = map.IsInArea(MapID, InstanceID, TargetID, 1442)
	if bCanNotUse == true then
		return 43, 0
	elseif guildID ~= 4294967295 then
		if guild.GetRoleGuildPos(guildID, TargetID) == 2 then		-- �����ſ���ʹ��
			if Role_Alliance[TargetID] == 0 then
				if Atk_ZhaoHunFan.use == 1 then
					return 39, 0
				else
					return 0, 0
				end
			else
				if Def_ZhaoHunFan.use == 1 then
					return 39, 0
				else
					return 0, 0
				end
			end
		else
			return 32, 0
		end

	else
		return 32, 0
	end

end

-- ʹ���л��
function I3300006_Use(MapID, InstanceID, TypeID, TargetID)
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	if Role_Alliance[TargetID] == 0 then	-- ������
		Atk_ZhaoHunFan.CreatureID = map.MapCreateCreature(MapID, InstanceID, 4900028, x, y, z)
		Atk_ZhaoHunFan.use = 1
		Atk_ZhaoHunFan.x = x
		Atk_ZhaoHunFan.y = y
		Atk_ZhaoHunFan.z = z
		unit.SetState(MapID, InstanceID, Atk_ZhaoHunFan.CreatureID, 8)		-- ���ý�����
		--unit.AddBuff(MapID, InstanceID, Atk_ZhaoHunFan.CreatureID, 2028001, Atk_ZhaoHunFan.CreatureID)
	else									-- ���ط�
		Def_ZhaoHunFan.CreatureID = map.MapCreateCreature(MapID, InstanceID, 4900028, x, y, z)
		Def_ZhaoHunFan.use = 1
		Def_ZhaoHunFan.x = x
		Def_ZhaoHunFan.y = y
		Def_ZhaoHunFan.z = z
		unit.SetState(MapID, InstanceID, Def_ZhaoHunFan.CreatureID, 9)		-- ���÷�����
		--unit.AddBuff(MapID, InstanceID, Def_ZhaoHunFan.CreatureID, 2028001, Def_ZhaoHunFan.CreatureID)
	end
end

-- ���ǰ�����
function C4900051_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if CityStruggle_IsStart == 1 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 213, 232, 321)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1022)			-- Ŀǰ�ǳ�սʱ�䣬͵Ϯ�ص��ѹرգ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end

-- ���ǰ�����
function C4900052_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if CityStruggle_IsStart == 1 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 1287, 209, 173)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1022)			-- Ŀǰ�ǳ�սʱ�䣬͵Ϯ�ص��ѹرգ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end

-- ����ʯ��
function I3300004_CanUse(MapID, InstanceID, TypeID, TargetID)
	if CityStruggle_IsStart == 1 and MapID == city_id[CityStruggle_ActID].MapID then
		local bCanUse = map.IsInArea(MapID, InstanceID, TargetID, 1424)
		if bCanUse == true then
			return 0, false
		else
			return 43, false	-- �����ڸ�λ��ʹ����Ʒ
		end
	end
	return 43, false
end

function I3300004_Use(MapID, InstanceID, TypeID, TargetID)
	unit.ChangeHP(MapID, InstanceID, Door_State[1].CreatureID, 10000)
	unit.ChangeHP(MapID, InstanceID, Door_State[3].CreatureID, 10000)

	-- ����ʹ��ʯ�ϻ���
	city.AddRoleData(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, TargetID, 4, 1)
end

-- �׻�ʯ��
function I3300005_CanUse(MapID, InstanceID, TypeID, TargetID)
	if CityStruggle_IsStart == 1 and MapID == city_id[CityStruggle_ActID].MapID then
		local bCanUse = map.IsInArea(MapID, InstanceID, TargetID, 1425)
		if bCanUse == true then
			return 0, false
		else
			return 43, false	-- �����ڸ�λ��ʹ����Ʒ
		end
	end
	return 43, false
end

function I3300005_Use(MapID, InstanceID, TypeID, TargetID)
	unit.ChangeHP(MapID, InstanceID, Door_State[1].CreatureID, 10000)
	unit.ChangeHP(MapID, InstanceID, Door_State[2].CreatureID, 10000)

	-- ����ʹ��ʯ�ϻ���
	city.AddRoleData(MapID, InstanceID, city_id[CityStruggle_ActID].CityID, TargetID, 4, 1)
end

-- ��ͼ��ʼ��ʱ
function CityStruggle_OnMapInit(MapID, InstanceID)
	-- �򿪵�ͼ���е���
	map.OpenCloseAllDoors(MapID, InstanceID, true)

	-- ˢ���ǳ�սʱ����
	LongZhu_State.LongZhuID = map.MapCreateCreature(MapID, -1, 4900029, 731, 11180, 754)
	-- ������Ч

end

-- ���ű�����
function CDoor_OnBeAttack(MapID, InstanceID, CreatureID, SrcID)

	-- �㲥��������ֵ
	local MainDoorHpPercent = 0
	local WestDoorHpPercent = 0
	local EastDoorHpPercent = 0
	if Door_State[1].Opened ~= 1 then
		MainDoorHpPercent = unit.GetAttValue(city_id[CityStruggle_ActID].MapID, -1, Door_State[1].CreatureID, 9) * 100 / unit.GetAttValue(city_id[CityStruggle_ActID].MapID, -1, Door_State[1].CreatureID, 8)
		MainDoorHpPercent = math.floor(100 - MainDoorHpPercent)
	end

	if Door_State[2].Opened ~= 1 then
		WestDoorHpPercent = unit.GetAttValue(city_id[CityStruggle_ActID].MapID, -1, Door_State[2].CreatureID, 9) * 100 / unit.GetAttValue(city_id[CityStruggle_ActID].MapID, -1, Door_State[2].CreatureID, 8)
		WestDoorHpPercent = math.floor(100 - WestDoorHpPercent)
	end

	if Door_State[3].Opened ~= 1 then
		EastDoorHpPercent = unit.GetAttValue(city_id[CityStruggle_ActID].MapID, -1, Door_State[3].CreatureID, 9) * 100 / unit.GetAttValue(city_id[CityStruggle_ActID].MapID, -1, Door_State[3].CreatureID, 8)
		EastDoorHpPercent = math.floor(100 - EastDoorHpPercent)
	end

	if MainDoorHpPercent ~= 0 and MainDoorHpPercent ~= 100 and (100 - MainDoorHpPercent)/10 <= Door_State[1].HpNode then
		local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
		for k, v in pairs(RoleTbl) do
			local MsgID = msg.BeginMsgEvent()
			--msg.AddMsgEvent(MsgID, 13,	1)
			msg.AddMsgEvent(MsgID, 101, 1023)
			msg.AddMsgEvent(MsgID, 9, MainDoorHpPercent)
			--msg.DispatchMapMsgEvent(MsgID, city_id[CityStruggle_ActID].MapID, -1)
			msg.DispatchRoleMsgEvent(v, MsgID)
		end
		Door_State[1].HpNode = Door_State[1].HpNode - 1
	end
	if WestDoorHpPercent ~= 0 and WestDoorHpPercent ~= 100 and (100 - WestDoorHpPercent)/10 <= Door_State[2].HpNode then
		local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
		for k, v in pairs(RoleTbl) do
			local MsgID = msg.BeginMsgEvent()
			--msg.AddMsgEvent(MsgID, 13,	1)
			msg.AddMsgEvent(MsgID, 101, 1024)
			msg.AddMsgEvent(MsgID, 9, WestDoorHpPercent)
			--msg.DispatchMapMsgEvent(MsgID, city_id[CityStruggle_ActID].MapID, -1)
			msg.DispatchRoleMsgEvent(v, MsgID)
		end
		Door_State[2].HpNode = Door_State[2].HpNode - 1
	end
	if EastDoorHpPercent ~= 0 and EastDoorHpPercent ~= 100 and (100 - EastDoorHpPercent)/10 <= Door_State[3].HpNode then
		local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
		for k, v in pairs(RoleTbl) do
			local MsgID = msg.BeginMsgEvent()
			--msg.AddMsgEvent(MsgID, 13,	1)
			msg.AddMsgEvent(MsgID, 101,	1025)
			msg.AddMsgEvent(MsgID, 9, EastDoorHpPercent)
			--msg.DispatchMapMsgEvent(MsgID, city_id[CityStruggle_ActID].MapID, -1)
			msg.DispatchRoleMsgEvent(v, MsgID)
		end
		Door_State[3].HpNode = Door_State[3].HpNode - 1
	end

end

-- �ж����ʿ���Ƿ�Ϊ0-200
function FX_ResetMorale(MapID, InstanceID, RoleID)
	local Morale = unit.GetAttValue(MapID, InstanceID, RoleID, 63)
	if Morale ~= nil then
		if Morale < 0 then
			unit.SetAttValue(MapID, InstanceID, RoleID, 63, 0)
		elseif Morale > 200 then
			unit.SetAttValue(MapID, InstanceID, RoleID, 63, 200)
		else
		end
	end
end

-- �ػ����ٻ����
function CityStruggle_OnFinishEudemonSummon()
	-- ֹͣ���з�����ҵ��ٻ�����
	for k,v in pairs(Eudemon_Stone) do
		role.RoleCancelSkill(city_id[CityStruggle_ActID].MapID, -1, k, 9001801)

	end

	Eudemon_State.Summoned = 1
	Eudemon_State.BeginSummon = 0
	Eudemon_Stone = {}

	-- ˢ���ػ���

	-- �㲥
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1047)
	msg.DispatchMapMsgEvent(MsgID, city_id[CityStruggle_ActID].MapID, -1)

	-- ����ʿ��
	for k, v in pairs(Role_Alliance) do
		if v == 1 then
			unit.ModAttValue(city_id[CityStruggle_ActID].MapID, -1, k, 63, 10)
			FX_ResetMorale(city_id[CityStruggle_ActID].MapID, -1, k)
		end
	end
end

--���й㲥
function City_OnBroadCast(BroadType, Para1, Para2, Para3, Para4, Para5)

	--0//ȫ����Ϣ�㲥����###���������ƣ����###���������ƣ���ͳ��Ȩ����
	--1//���óɹ���ϵͳȫ����㲥����###���������ƣ���˰�ʱ��Ϊ###����
	--2//ϵͳȫ����Ϣ�㲥�������γ�ս�ĸ����뷽��ȷ�������λ���ڳ�ս��Ϣ�����в�ѯ����

	--3//��ȷ��ӵ�г�ս����Ȩ�ĸ������ɣ�ȫ���ɳ�Ա��ʾ�������ɻ���˶�###���������ƣ��Ĺ���Ȩ�����λ���ڻ�����ս����
	--4//��ȷ�ϵİ���ȫ���ɳ�Ա��ʾ���������ѱ�ͬ�����###���������ƣ��ĳ�ս������Ϊ������Э�����أ����λ��Ա������ս��
	--5//ȫ�������ʾ���������Ѷ�###���������ƣ����й�����������
	--6//ȫ�������ʾ���������Ѷ�###���������ƣ�����Э��������������
	--7//ͬʱȫ���ɳ�Ա��Ϣ����###����ɫ���ƣ���������####���������ƣ���###���������ƣ�����������###�㡣��
	--8//###���������ƣ����������������###���������ƣ���ͳ��Ȩ��ȫ����ɳ�Ա��Ӧ�ó����������͵���������֮�;����������������0����м��㣡��
	if BroadType == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1100)
		msg.AddMsgEvent(MsgID, 19, Para1)
		msg.AddMsgEvent(MsgID, 6, Para2)
		msg.DispatchWorldMsgEvent(MsgID)
	elseif BroadType == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1101)
		msg.AddMsgEvent(MsgID, 6, Para1)
		msg.AddMsgEvent(MsgID, 9, Para2)
		msg.DispatchWorldMsgEvent(MsgID)
	elseif BroadType == 2 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1102)
		msg.DispatchWorldMsgEvent(MsgID)
	elseif BroadType == 3 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 1103)
		msg.AddMsgEvent(MsgID, 6, Para1)
		msg.DispatchGuildMsgEvent(MsgID,Para2)
	elseif BroadType == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 1104)
		msg.AddMsgEvent(MsgID, 6, Para1)
		msg.DispatchGuildMsgEvent(MsgID,Para2)
	elseif BroadType == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 1105)
		msg.AddMsgEvent(MsgID, 6, Para1)
		msg.DispatchGuildMsgEvent(MsgID,Para2)
	elseif BroadType == 6 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 1106)
		msg.AddMsgEvent(MsgID, 6, Para1)
		msg.DispatchGuildMsgEvent(MsgID,Para2)
	elseif BroadType == 7 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 1107)
		msg.AddMsgEvent(MsgID, 2, Para1)
		msg.AddMsgEvent(MsgID, 4, Para2)
		msg.AddMsgEvent(MsgID, 6, Para3)
		msg.AddMsgEvent(MsgID, 9, Para4)
		msg.DispatchGuildMsgEvent(MsgID,Para5)
	elseif BroadType == 8 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 1108)
		msg.AddMsgEvent(MsgID, 2, Para1)
		msg.AddMsgEvent(MsgID, 6, Para2)
		msg.DispatchGuildMsgEvent(MsgID,Para3)
	end

end



aux.RegisterActEvent(70, 0, "CityStruggle_OnInit")
aux.RegisterActEvent(70, 1, "CityStruggle_OnTimer")
aux.RegisterActEvent(70, 2, "CityStruggle_OnStart")
aux.RegisterActEvent(70, 3, "CityStruggle_OnEnd")
aux.RegisterActEvent(70, 4, "CityStruggle_OnTimerMin")
aux.RegisterActEvent(70, 6, "CityStruggle_OnBroadCast")
aux.RegisterActEvent(70, 7, "CityStruggle_OnFinishEudemonSummon")

aux.RegisterMapEvent("c03", 0, "CityStruggle_OnMapInit")
aux.RegisterMapEvent("c03", 2, "CityStruggle_OnPlayerEnter")
aux.RegisterMapEvent("c03", 3, "CityStruggle_OnPlayerLeave")
aux.RegisterMapEvent("c03", 4, "CityStruggle_OnCreatureDie")
aux.RegisterMapEvent("c03", 5, "CityStruggle_OnRoleDie")
aux.RegisterMapEvent("c03", 11, "CityStruggle_RoleRevive")
aux.RegisterMapEvent("c03", 12, "CityStruggle_CanEnterWhenOnline")
aux.RegisterMapEvent("c03", 16, "CityStruggle_FriendEnemy")


aux.RegisterCreatureEvent(4900004, 7, "C4900004_OnTalk")		-- ������Ǵ�����
aux.RegisterCreatureEvent(4900005, 7, "C4900005_OnTalk")		-- �������Ǵ�����
aux.RegisterCreatureEvent(4900051, 7, "C4900051_OnTalk")		-- ���ǰ�����
aux.RegisterCreatureEvent(4900052, 7, "C4900052_OnTalk")		-- ���ǰ�����
aux.RegisterCreatureEvent(4900027, 4, "C4900027_OnDie")			-- ���ʿ�
aux.RegisterCreatureEvent(4900028, 4, "C4900028_OnDie")			-- �л��
aux.RegisterCreatureEvent(4900019, 4, "CDoor_OnDie")			-- ���ֳ���
aux.RegisterCreatureEvent(4900020, 4, "CDoor_OnDie")
aux.RegisterCreatureEvent(4900021, 4, "CDoor_OnDie")
aux.RegisterCreatureEvent(4900022, 4, "CDoor_OnDie")
aux.RegisterCreatureEvent(4900023, 4, "CDoor_OnDie")
aux.RegisterCreatureEvent(4900024, 4, "CDoor_OnDie")
aux.RegisterCreatureEvent(4900025, 4, "CDoor_OnDie")
aux.RegisterCreatureEvent(4900026, 4, "CDoor_OnDie")
aux.RegisterCreatureEvent(4900019, 14, "CDoor_OnBeAttack")
aux.RegisterCreatureEvent(4900020, 14, "CDoor_OnBeAttack")
aux.RegisterCreatureEvent(4900021, 14, "CDoor_OnBeAttack")
aux.RegisterCreatureEvent(4900022, 14, "CDoor_OnBeAttack")
aux.RegisterCreatureEvent(4900023, 14, "CDoor_OnBeAttack")
aux.RegisterCreatureEvent(4900024, 14, "CDoor_OnBeAttack")
aux.RegisterCreatureEvent(4900025, 14, "CDoor_OnBeAttack")
aux.RegisterCreatureEvent(4900026, 14, "CDoor_OnBeAttack")


aux.RegisterSkillEvent(9001501, 0, "OnSealCanCast")
aux.RegisterSkillEvent(9001501, 2, "OnSealSkill")
aux.RegisterSkillEvent(9001501, 3, "OnSealCancelSkill")
aux.RegisterSkillEvent(9001601, 0, "OnSealCanCast")
aux.RegisterSkillEvent(9001601, 2, "OnSealSkill")
aux.RegisterSkillEvent(9001601, 3, "OnSealCancelSkill")

aux.RegisterItemEvent(3300006, 0, "I3300006_CanUse")			-- �л��
--aux.RegisterItemEvent(1350012, 0, "I1350012_CanUse")
aux.RegisterItemEvent(3300004, 0, "I3300004_CanUse")
aux.RegisterItemEvent(3300005, 0, "I3300005_CanUse")

aux.RegisterItemEvent(3300006, 1, "I3300006_Use")				-- �л��
--aux.RegisterItemEvent(1350012, 1, "I1350012_Use")
aux.RegisterItemEvent(3300004, 1, "I3300004_Use")
aux.RegisterItemEvent(3300005, 1, "I3300005_Use")

aux.RegisterMapEvent("c03", 22, "City_OnBroadCast") --���й㲥
