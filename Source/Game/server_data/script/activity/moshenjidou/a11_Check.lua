

function s9001901_CanCast(MapID, InstanceID, SkillID, RoleID, TargetID)
	if Jidou.RoomInfo[InstanceID].IsOver ~= 1 then
		local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, TargetID)
		if TypeID == 4902112 then --��
			if Jidou.RoomInfo[InstanceID].Role[RoleID].Side == "Red" then
				if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 999 then
					return 14
				else
					return 0
				end
			else
				if Jidou.RoomInfo[InstanceID].BlueFlag.State == "outside" then
				   return 0
				else
				   return 14
				end
			end
		elseif TypeID == 4902113 then --��
			if Jidou.RoomInfo[InstanceID].Role[RoleID].Side == "Blue" then
				if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 999 then
					return 14
				else
					return 0
				end
			else
				if Jidou.RoomInfo[InstanceID].RedFlag.State == "outside" then
				   return 0
				else
				   return 14
				end
			end
		elseif TypeID == 4902114 then --���������
			if Jidou.RoomInfo[InstanceID].Role[RoleID].Side == "Blue" and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11402) then --����������
				if Jidou.RoomInfo[InstanceID].BlueFlag.State == "outside" then

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 12921)   --�з����ı�����ڼ��������Ա�
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					return 999
				else
					return 0
				end
			else
			   return 14
			end
		elseif TypeID == 4902133 then --�췽����� ��Ҫ��ID
			if Jidou.RoomInfo[InstanceID].Role[RoleID].Side == "Red" and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11401) then
				if Jidou.RoomInfo[InstanceID].RedFlag.State == "outside" then

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 12921)   --�з����ı�����ڼ��������Ա�
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					return 999
				else
					return 0
				end
			else
			   return 14
			end
		end
	end
    return 14
end

--ע��
aux.RegisterSkillEvent(9001901, 0, "s9001901_CanCast")


function BlueFlag_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	if Jidou.RoomInfo[InstanceID].Role[RoleID].Side == "Blue" then
		Jidou.RoomInfo[InstanceID].BlueFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902112,97,5556,406)--��������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12922)			--����XXX�Ӻ췽���ж��������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		Jidou.RoomInfo[InstanceID].BlueFlag.State = "home"
		Jidou.RoomInfo[InstanceID].BlueFlag.Role = 0
		Jidou.RoomInfo[InstanceID].BlueFlag.Time = 0
		if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)  == 999 then     --�ڰ��ڱ��˼ҵĿ���˴�����ɫ����Ҫ�ڵ�һ��λ�����999,Ҫˢ�����
		    map.MapCreateCreature(MapID, InstanceID, 4902113,425,5483,109)
		end

	else
	    unit.AddBuff(MapID, InstanceID, RoleID, 1140101, RoleID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12923)			--�췽XXX�����˺췽������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		Jidou.RoomInfo[InstanceID].BlueFlag.State = "outside"
		Jidou.RoomInfo[InstanceID].BlueFlag.Role = RoleID
		Jidou.RoomInfo[InstanceID].BlueFlag.Time = 0
	end

end

aux.RegisterCreatureEvent(4902112, 4, "BlueFlag_OnDie")

function RedFlag_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	if Jidou.RoomInfo[InstanceID].Role[RoleID].Side == "Red" then
		Jidou.RoomInfo[InstanceID].RedFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902113,425,5483,109)--�췽����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12924)			--�췽XXX���������ж��������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		Jidou.RoomInfo[InstanceID].RedFlag.State = "home"
		Jidou.RoomInfo[InstanceID].RedFlag.Role = 0
		Jidou.RoomInfo[InstanceID].RedFlag.Time = 0
		if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)  == 999 then     --�ڰ��ڱ��˼ҵĿ���˴�����ɫ����Ҫ�ڵ�һ��λ�����999,Ҫˢ�����
		    map.MapCreateCreature(MapID, InstanceID, 4902112,64,5555,440)
		end
	else
	    unit.AddBuff(MapID, InstanceID, RoleID, 1140201, RoleID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12925)			--����XXX�����˺췽������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		Jidou.RoomInfo[InstanceID].RedFlag.State = "outside"
		Jidou.RoomInfo[InstanceID].RedFlag.Role = RoleID
		Jidou.RoomInfo[InstanceID].RedFlag.Time = 0
	end

end

aux.RegisterCreatureEvent(4902113, 4, "RedFlag_OnDie")

function hongFangKong_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	Jidou.RoomInfo[InstanceID].BlueFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902112, x, y, z)
	unit.CancelBuff(MapID, InstanceID, RoleID, 1140101)
	cre.SetCreatureScriptData(MapID, InstanceID, Jidou.RoomInfo[InstanceID].BlueFlag.FlagID, 1, 0, 999)       --��¼����Ƕ���������
	local ID = map.MapCreateCreature(MapID, InstanceID, 4902134, 1, 1, 1)                                       --��ʱ��,30���������Ļ���,���λ����֮ǰ�ļ�ʱ����ͬһ��ID
	cre.GetCreatureScriptData(MapID, InstanceID, ID, 1, 0, Jidou.RoomInfo[InstanceID].BlueFlag.FlagID)       --��¼��λ�ļ�ʱ��
	Jidou.RoomInfo[InstanceID].BlueFlag.Role = RoleID
	Jidou.RoomInfo[InstanceID].BlueFlag.Time = 0
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 12926)			--�췽��XXX��ȡ��һ�����������ģ�
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	for k,v in pairs(Jidou.RoomInfo[InstanceID].Role) do
	    if v.Leave == 0 then
            if  v.Side == "Red" then
			    v.GetFlag = v.GetFlag + 1
			else
			    v.MissFlag = v.MissFlag + 1
			end
		end
	end
	MSJD_SendAllNormalInformation(MapID, InstanceID)
    MSJD_CheckIsOver(MapID, InstanceID)

end

aux.RegisterCreatureEvent(4902133, 4, "hongFangKong_OnDie")

function LanFangKong_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	Jidou.RoomInfo[InstanceID].RedFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902113, x, y, z)
	unit.CancelBuff(MapID, InstanceID, RoleID, 1140201)
	cre.SetCreatureScriptData(MapID, InstanceID, Jidou.RoomInfo[InstanceID].RedFlag.FlagID, 1, 0, 999)                     --��¼����Ƕ���������
	local ID = map.MapCreateCreature(MapID, InstanceID, 4902134, 1, 1, 1)                                       --��ʱ��,30���������Ļ���,���λ����֮ǰ�ļ�ʱ����ͬһ��ID
	cre.GetCreatureScriptData(MapID, InstanceID, ID, 1, 0, Jidou.RoomInfo[InstanceID].RedFlag.FlagID)       --��¼��λ�ļ�ʱ��
	Jidou.RoomInfo[InstanceID].RedFlag.Role = RoleID
	Jidou.RoomInfo[InstanceID].RedFlag.Time = 0
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101,12927 )			--������XXX��ȡ��һ��췽�����ģ�
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	for k,v in pairs(Jidou.RoomInfo[InstanceID].Role) do
	    if v.Leave == 0 then
            if  v.Side == "Blue" then
			    v.GetFlag = v.GetFlag + 1
			else
			    v.MissFlag = v.MissFlag + 1
			end
		end
	end
	MSJD_SendAllNormalInformation(MapID, InstanceID)
    MSJD_CheckIsOver(MapID, InstanceID)

end

aux.RegisterCreatureEvent(4902114, 4, "LanFangKong_OnDie")
