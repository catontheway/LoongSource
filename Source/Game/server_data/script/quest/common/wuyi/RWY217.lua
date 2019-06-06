--RWY217�����
--����1����ȡ����֮��ˢ��5�����ķ���1007107��һ�����ķ�������1007108��

function q14217_OnAccept(MapID, InstanceID, QuestID, TargetID, AccepterID)
	--m06��ͼ����1��14217 5�����ķ��� 1007107 ��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)--���ķ���,��ͼ����1
	if a == nil then
		a = 0
	end
	if a < 10 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1007107, x+4, y, z)
		map.MapCreateCreature(MapID, InstanceID, 1007107, x-4, y, z)
		map.MapCreateCreature(MapID, InstanceID, 1007107, x+2, y, z+2)
		map.MapCreateCreature(MapID, InstanceID, 1007107, x-2, y, z+2)
		map.MapCreateCreature(MapID, InstanceID, 1007107, x, y, z-2)
		map.SetMapScriptData(MapID,InstanceID,1,1,a+5)
	end

	--m06��ͼ����2��14217 һ�����ķ�������1007108 ��¼����
	local b = map.GetMapScriptData(MapID,InstanceID,1,2)--���ķ�������,��ͼ����2
	if b == nil then
		b = 0
	end
	if b < 3 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1007108, x, y, z+4)
		map.SetMapScriptData(MapID,InstanceID,1,2,b+1)
	end
end

aux.RegisterQuestEvent(14217, 0, "q14217_OnAccept")

function q14217_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if map.GetMapScriptData(MapID,InstanceID,1,1) >= 10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 148)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	elseif map.GetMapScriptData(MapID,InstanceID,1,2) >= 3 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 148)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end

aux.RegisterQuestEvent(14217, 4, "q14217_OnCheckAccept")

--����������ScriptData���� ��ֵ��1
function c1007107_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,1) --����Data����1
	map.SetMapScriptData(MapID,InstanceID,1,1,a-1)
end
aux.RegisterCreatureEvent(1007107,4,"c1007107_OnDie")

function c1007108_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,2) --����Data����2
	map.SetMapScriptData(MapID,InstanceID,1,2,a-1)
end
aux.RegisterCreatureEvent(1007108,4,"c1007108_OnDie")
