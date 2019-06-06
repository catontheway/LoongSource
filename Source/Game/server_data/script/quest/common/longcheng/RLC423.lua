--Χ��֮�ѣ�10423
--����1�����������ˢ����ֻ�����۷�[1003081]��ԭ�أ�����Ϊ��
--[3401, 3642, 780]
--[3403, 3642, 784]
--[3402, 3642, 778]


--�����ȡʱ����
function q10423_OnAccept(MapID, InstanceID, QuestID, TargetID, AccepterID)
	--m02��ͼ����1��10423 �����۷�[1003081]��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)

	if a < 15 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1003081, 3401, 3643, 780)
		map.MapCreateCreature(MapID, InstanceID, 1003081, 3403, 3643, 784)
		map.MapCreateCreature(MapID, InstanceID, 1003081, 3402, 3643, 778)
		map.SetMapScriptData(MapID,InstanceID,1,1,a+3)
	end
end

--ע��
aux.RegisterQuestEvent(10423, 0, "q10423_OnAccept")

function q10423_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	if a == nil then
		a = 0
		map.SetMapScriptData(MapID,InstanceID,1,1,0)
	end
	if map.GetMapScriptData(MapID,InstanceID,1,1) >= 15 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 148)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end

aux.RegisterQuestEvent(10423, 4, "q10423_OnCheckAccept")

--����������ScriptData����1 ��ֵ��1

function c1003081_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,1) --����Data����1
	map.SetMapScriptData(MapID,InstanceID,1,1,a-1)
end
aux.RegisterCreatureEvent(1003081,4,"c1003081_OnDie")
