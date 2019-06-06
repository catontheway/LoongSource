--����ʦ����13469
--����1����ȡ�����ˢ��3ֻ����ħ[1006151]

--�����ȡʱ����
function q13469_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)

	--m05��ͼ����2��13469 ����ħ[1006151]��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,2)
	if a == nil then
		a = 0
	end
	if a < 15 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
		map.MapCreateCreature(MapID, InstanceID, 1006151, x-2, y, z)
		map.MapCreateCreature(MapID, InstanceID, 1006151, x+2, y, z)
		map.MapCreateCreature(MapID, InstanceID, 1006151, x, y, z+2)
		map.SetMapScriptData(MapID,InstanceID,1,2,a+3)
	end
end

--ע��
aux.RegisterQuestEvent(13469, 0, "q13469_OnAccept")

function q13469_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if map.GetMapScriptData(MapID,InstanceID,1,2) >= 15 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 148)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end

aux.RegisterQuestEvent(13469, 4, "q13469_OnCheckAccept")

--����������ScriptData���� ��ֵ��1

function c1006151_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,2) --����Data����2
	map.SetMapScriptData(MapID,InstanceID,1,2,a-1)
end
aux.RegisterCreatureEvent(1006151,4,"c1006151_OnDie")
